#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int mask_img_r = 113;
int kener_size = 3;
int erase_max = 2000;//抹去连续区域像素点数（相当于去噪稳定）
int erase_min = 0;
int match_type = 4;
int m_center_x = 70;
int m_center_y = 102;
Point center(m_center_x, m_center_y);//中心点
Mat mask_img;//匹配模版图像

//CheckMode: 0代表去除黑区域，1代表去除白区域; NeihborMode：0代表4邻域，1代表8邻域;  
void RemoveRegion(Mat& Src, Mat& Dst, int AreaLimitMin, int AreaLimitMax, int CheckMode, int NeihborMode)
{
	int RemoveCount = 0;       //记录除去的个数  
							   //记录每个像素点检验状态的标签，0代表未检查，1代表正在检查,2代表检查不合格（需要反转颜色），3代表检查合格或不需检查  
	Mat Pointlabel = Mat::zeros(Src.size(), CV_8UC1);

	if (CheckMode == 1)
	{
		for (int i = 0; i < Src.rows; ++i)
		{
			uchar* iData = Src.ptr<uchar>(i);
			uchar* iLabel = Pointlabel.ptr<uchar>(i);
			for (int j = 0; j < Src.cols; ++j)
			{
				if (iData[j] < 10)
				{
					iLabel[j] = 3;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < Src.rows; ++i)
		{
			uchar* iData = Src.ptr<uchar>(i);
			uchar* iLabel = Pointlabel.ptr<uchar>(i);
			for (int j = 0; j < Src.cols; ++j)
			{
				if (iData[j] > 10)
				{
					iLabel[j] = 3;
				}
			}
		}
	}

	vector<Point2i> NeihborPos;  //记录邻域点位置  
	NeihborPos.push_back(Point2i(-1, 0));
	NeihborPos.push_back(Point2i(1, 0));
	NeihborPos.push_back(Point2i(0, -1));
	NeihborPos.push_back(Point2i(0, 1));
	if (NeihborMode == 1)
	{
		NeihborPos.push_back(Point2i(-1, -1));
		NeihborPos.push_back(Point2i(-1, 1));
		NeihborPos.push_back(Point2i(1, -1));
		NeihborPos.push_back(Point2i(1, 1));
	}
	int NeihborCount = 4 + 4 * NeihborMode;
	int CurrX = 0, CurrY = 0;
	//开始检测  
	for (int i = 0; i < Src.rows; ++i)
	{
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0; j < Src.cols; ++j)
		{
			if (iLabel[j] == 0)
			{
				//********开始该点处的检查**********  
				vector<Point2i> GrowBuffer;  //堆栈，用于存储生长点  
				GrowBuffer.push_back(Point2i(j, i));
				Pointlabel.at<uchar>(i, j) = 1;
				int CheckResult = 0;//用于判断结果（是否超出大小），0为未超出，1为超出  

				for (int z = 0; z<GrowBuffer.size(); z++)
				{

					for (int q = 0; q<NeihborCount; q++)//检查四个邻域点  
					{
						CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
						CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
						if (CurrX >= 0 && CurrX<Src.cols&&CurrY >= 0 && CurrY<Src.rows) //防止越界  
						{
							if (Pointlabel.at<uchar>(CurrY, CurrX) == 0)
							{
								GrowBuffer.push_back(Point2i(CurrX, CurrY)); //邻域点加入buffer  
								Pointlabel.at<uchar>(CurrY, CurrX) = 1; //更新邻域点的检查标签，避免重复检查  
							}
						}
					}
				}
				if (GrowBuffer.size()<AreaLimitMax && GrowBuffer.size() > AreaLimitMin)
				{
					CheckResult = 2; //判断结果（是否超出限定的大小），1为未超出，2为超出 
				}
				else { CheckResult = 1;   RemoveCount++; }
				for (int z = 0; z<GrowBuffer.size(); z++)//更新Label记录  
				{
					CurrX = GrowBuffer.at(z).x;
					CurrY = GrowBuffer.at(z).y;
					Pointlabel.at<uchar>(CurrY, CurrX) += CheckResult;
				}
			}
		}
	}

	CheckMode = 255 * (1 - CheckMode);
	//开始反转面积过小的区域  
	for (int i = 0; i < Src.rows; ++i)
	{
		uchar* iData = Src.ptr<uchar>(i);
		uchar* iDstData = Dst.ptr<uchar>(i);
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0; j < Src.cols; ++j)
		{
			if (iLabel[j] == 2)
			{
				iDstData[j] = CheckMode;
			}
			else if (iLabel[j] == 3)
			{
				iDstData[j] = iData[j];
			}
		}
	}

}


static void ContrastAndBright(int, void *)
{
	Mat m = Mat(600, 640, 0);
	imshow("【控制面板】", m);
}


int HistGet(Mat img)
{
	MatND hist;       // 在cv中用CvHistogram *hist = cvCreateHist  
	int dims = 1;
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };   //这里需要为const类型  
	int size = 256;
	int channels = 0;
	//计算图像的直方图  
	calcHist(&img, 1, &channels, Mat(), hist, dims, &size, ranges);    //cv中是cvCalcHist  
	int hpt = saturate_cast<int>(0.9 * size);

	double minVal = 0;
	double maxVal = 0;
	int scale = 1;
	Mat imageShow(size * scale, size, CV_8U, Scalar(0));

	minMaxLoc(hist, &minVal, &maxVal, 0, 0);    //cv中用的是cvGetMinMaxHistValue  
	int max_realValue = 0;
	int max_position;
	for (int i = 0; i < 256; i++)
	{
		float value = hist.at<float>(i);           // 注意hist中是float类型    cv中用cvQueryHistValue_1D  
		int realValue = saturate_cast<int>(value * hpt / maxVal);
		if (realValue  >  max_realValue)
		{
			max_realValue = realValue;
			max_position = i;
		}
		rectangle(imageShow, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue), Scalar(255));
	}
	imshow("直方图", imageShow);
	return max_position;
}

int HistGetMaxVal(Mat img)
{
	MatND hist;
	int dims = 1;
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };
	int size = 256;
	int channels = 0;
	calcHist(&img, 1, &channels, Mat(), hist, dims, &size, ranges);
	int hpt = saturate_cast<int>(0.9 * size);

	double minVal = 0;
	double maxVal = 0;

	minMaxLoc(hist, &minVal, &maxVal, 0, 0);
	return maxVal;
}


void GetBrightness(Mat InputImg, float& cast, float& f_dark_quantity)
{
	Mat GRAYimg;
	cvtColor(InputImg, GRAYimg, CV_BGR2GRAY);
	float a = 0;
	int Hist[256];
	for (int i = 0; i<256; i++)
		Hist[i] = 0;
	for (int i = 0; i<GRAYimg.rows; i++)
	{
		for (int j = 0; j<GRAYimg.cols; j++)
		{
			a += float(GRAYimg.at<uchar>(i, j) - 128);//在计算过程中，考虑128为亮度均值点  
			int x = GRAYimg.at<uchar>(i, j);
			Hist[x]++;
		}
	}
	f_dark_quantity = a / float(GRAYimg.rows*InputImg.cols);
	float D = abs(f_dark_quantity);
	float Ma = 0;
	for (int i = 0; i<256; i++)
	{
		Ma += abs(i - 128 - f_dark_quantity)*Hist[i];
	}
	Ma /= float((GRAYimg.rows*GRAYimg.cols));
	float M = abs(Ma);
	float K = D / M;
	cast = K;
	return;
}


struct Circlecenter
{
	Point2f center;
	int circleR;
};

//------------------------------------
bool sys_init()
{
	//初始化模板匹配模板图像
	Mat mask = Mat(Size(mask_img_r + 2, mask_img_r + 2), CV_8UC1, Scalar::all(0));
	circle(mask, Point(mask_img_r / 2 + 1, mask_img_r / 2 + 1), mask_img_r / 2, Scalar(255, 255, 255), 1);
	mask_img = mask.clone();

	return 0;
}
//---------------------------------
void match_temp(Mat bitwise_out, Mat templ)
{
	Mat g_resultImage;
	Mat srcImage;
	bitwise_out.copyTo(srcImage);
	int resultImage_cols = bitwise_out.cols - templ.cols + 1;
	int resultImage_rows = bitwise_out.rows - templ.rows + 1;
	g_resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);

	//进行匹配
	matchTemplate(bitwise_out, templ, g_resultImage, match_type);

	//标准化
	normalize(g_resultImage, g_resultImage, 0, 2, NORM_MINMAX, -1, Mat());
	double minValue, maxValue;
	Point minLocation, maxLocation, matchLocation;

	//定位最匹配的位置
	minMaxLoc(g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation);

	if (match_type == TM_SQDIFF || match_type == CV_TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	int x = matchLocation.x + mask_img_r / 2;
	int y = matchLocation.y + mask_img_r / 2;


	center.x = x;
	center.y = y;

	
}
//Point2f center_f;
Mat FindCirCen(Mat src_img, Mat color_img)
{
	Mat in_img = src_img.clone();
	vector<vector<Point> > contours;
	findContours(in_img, contours, RETR_LIST, CHAIN_APPROX_NONE);
	struct Circlecenter  cirpocenter[50];
	int len = 0;
	int have_circle = 0;
	if (contours.size() == 0)
	{
		return color_img;
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		size_t count = contours[i].size();
		if (count < 6)
			continue;

		Mat pointsf;
		Mat(contours[i]).convertTo(pointsf, CV_32F);
		RotatedRect box = fitEllipse(pointsf);

		if (MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height) * 1.02)
			continue;
		//if (MAX(box.size.width, box.size.height) < MIN(box.size.width, box.size.height) * 1.15)
		//	continue;
		if (box.size.width < 40 || box.size.height < 40 || box.size.width >  200 || box.size.height > 200)
			continue;

		//drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);

		//---------------------------------增加一个教研功能，如果该点是黑色的就去掉-----------------------
		//先判断点有没有越界
		if (box.center.x < 5 || box.center.x >(in_img.cols - 5))
		{
			printf("point get out of the picuter");
			continue;
		}
		if (box.center.y < 5 || box.center.y >(in_img.rows - 5))
		{
			printf("point get out of the picuter");
			continue;
		}

		cirpocenter[len].center.x = box.center.x;
		cirpocenter[len].center.y = box.center.y;
		//cirpocenter[len].circleR = (640 - cirpocenter[len].center.x) * (640 - cirpocenter[len].center.x) + (480 - cirpocenter[len].center.y) * (480 - cirpocenter[len].center.y);
		cirpocenter[len].circleR = cirpocenter[len].center.x * cirpocenter[len].center.x + cirpocenter[len].center.y * cirpocenter[len].center.y;
		//circle(color_img, cirpocenter[len].center, 2, Scalar(255, 0, 0), 2);
		have_circle++;
		if ((++len) >= 50)
		{
			printf("worring: len is big than array's len \n");
			break;
		}
		ellipse(color_img, box, Scalar(0, 255, 255), 2, LINE_AA);
		//Point2f vtx[4];
		//box.points(vtx);
		//for (int j = 0; j < 4; j++)
		//	line(color_img, vtx[j], vtx[(j + 1) % 4], Scalar(0, 255, 0), 1, LINE_AA);
	}

	if (len == 0) //
	{
		//printf("not foud \n");
		return color_img;
	}
	int sogolen = 0;
	int minddirect = cirpocenter[0].circleR;
	for (int mlen = 0; mlen < len; mlen++)
	{
		if (cirpocenter[mlen].circleR< minddirect)
		{
			sogolen = mlen;
		}
	}

	circle(color_img, cirpocenter[sogolen].center, 2, Scalar(255, 0, 0), 2);
	//x方向相反
	String showpoint = "(" + to_string(cirpocenter[sogolen].center.x) + "," + to_string(cirpocenter[sogolen].center.y) + ")";
	printf("%f     %f      \n", cirpocenter[sogolen].center.x, cirpocenter[sogolen].center.y);
	putText(color_img, showpoint, cirpocenter[sogolen].center, FONT_HERSHEY_PLAIN, 2, Scalar(128, 128, 128), 2);
	//串口发送数据，调整
	return color_img;
}



int thresh_mm = 20;
int canny_num = 200;

int i_rect_img_x = 250;
int i_rect_img_y = 60;
int i_rect_img_w = 140;
int i_rect_img_h = 204;

//-------------------------------------------------遍历像素-------------------------------
void Flip(Mat &img)
{
	int rows = img.rows;
	int cols = img.cols;
	for (int i = 0; i<rows; i++)
	{
		if (abs(i - center.x) > (mask_img_r/2+6))
		{
			for (int j = 0; j < cols; j++)
			{
				img.at<uchar>(i, j) = 0;
			}
			continue;
		}
		for (int j = 0; j<cols; j++)
		{
			if (abs(j - center.y) > (mask_img_r / 2+6))
			{
			
				img.at<uchar>(i, j) = 0;

			}	
		}
	}
}

int main()
{  
	namedWindow("【控制面板】", CV_WINDOW_NORMAL);
	//创建轨迹条  
	createTrackbar("thresh_mm：", "【控制面板】", &thresh_mm, 255, ContrastAndBright);
	createTrackbar("canny_num：", "【控制面板】", &canny_num, 600, ContrastAndBright);
	createTrackbar("i_rect_img_x：", "【控制面板】", &i_rect_img_x, 400, ContrastAndBright);
	createTrackbar("i_rect_img_y：", "【控制面板】", &i_rect_img_y, 400, ContrastAndBright);
	createTrackbar("i_rect_img_w：", "【控制面板】", &i_rect_img_w,200, ContrastAndBright);
	createTrackbar("i_rect_img_h：", "【控制面板】", &i_rect_img_h, 300, ContrastAndBright);
	createTrackbar("mask_img_r：", "【控制面板】", &mask_img_r, 200, ContrastAndBright);
	createTrackbar("kener_size：", "【控制面板】", &kener_size, 10, ContrastAndBright);
	//createTrackbar("m_method：", "【控制面板】", &m_method, 5, ContrastAndBright);
	ContrastAndBright(thresh_mm, 0);
	sys_init();
	VideoCapture capture(1);
	Mat img;
	//namedWindow("other");
	while (1)
	{
		capture >> img;
		if (!img.data)
		{
			printf("please sure you have connect the camera");
			break;
		}
	//	imshow("src",img);
		Mat big_scc = img.clone();
		imshow("SRCOURCE", big_scc);
		img = img(Rect(i_rect_img_x, i_rect_img_y, i_rect_img_w, i_rect_img_h));
		Mat src = img.clone();
		/*line(src, Point(0, src.rows / 2), Point(src.cols, src.rows / 2), Scalar(0, 0, 255));
		line(src, Point(src.cols / 2, 0), Point(src.cols / 2, src.rows), Scalar(0, 0, 255));*/
		// imshow("ghjkl",src);
		Mat imme = src.clone();
		int sizeData = kener_size;
		if (sizeData % 2 == 0)
		{
			sizeData += 1;
		}
		 
		//medianBlur(imme, imme, sizeData);

		cvtColor(imme, imme, CV_BGR2GRAY);
		threshold(imme, imme, thresh_mm, 255, 1);
		imshow("gray_img", imme); 
		medianBlur(imme, imme, sizeData);
		imshow("medianBlur", imme);
		match_temp(imme, mask_img);
		//circle(src, center, 3, Scalar(0, 255, 255), -1, 8, 0);
		//圆轮廓
		//circle(src, Point(center.x, center.y), mask_img_r / 2 - 2, Scalar(0, 255, 255), 4, 8, 0);
		line(src, Point(0, src.rows / 2), Point(src.cols, src.rows / 2), Scalar(0, 0, 255));
		line(src, Point(src.cols / 2, 0), Point(src.cols / 2, src.rows), Scalar(0, 0, 255));
		imshow("ghjkl", src);
		//int num = HistGet(src);
		//float f_dark_quantity;//图像亮度
		//float cast;
		//GetBrightness(src, cast, f_dark_quantity);
		//String strs = "ca:" + to_string(cast);
		//putText(src, strs, Point(0, 10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 255, 255), 1);
		//strs = "da:" + to_string(f_dark_quantity);
		//putText(src, strs, Point(0, 23), FONT_HERSHEY_PLAIN, 1, Scalar(0, 255, 255), 1);
		//imshow("roiphoto", src);
		/*printf("最大值坐标  %d", num);

		int num_value = HistGetMaxVal(src);
		printf("\t最大值的值  %d \n",num_value);*/

	/*	cvtColor(src, src, CV_BGR2GRAY);
		Canny(src,src, canny_num, canny_num,3);
		imshow("forcanny",src);*/
	/*	Mat imme = img.clone();
		cvtColor(imme, imme, CV_BGR2GRAY);
		threshold(imme, imme, thresh_mm, 255, 1);
		imshow("gray_img", imme);*/
	/*	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
		filter2D(src, src, CV_8UC3, kernel);
		imshow("LApuLAsi", src);*/
		//----------------------------------------------------------------------
		//Mat imageLog(src.size(), CV_32FC3);
		////	 printf("it is ok \n");
		//for (int i = 0; i < src.rows; i++)
		//{
		//	for (int j = 0; j < src.cols; j++)
		//	{
		//		imageLog.at<Vec3f>(i, j)[0] = log(1 + src.at<Vec3b>(i, j)[0]);
		//		imageLog.at<Vec3f>(i, j)[1] = log(1 + src.at<Vec3b>(i, j)[1]);
		//		imageLog.at<Vec3f>(i, j)[2] = log(1 + src.at<Vec3b>(i, j)[2]);
		//	}
		//}
		////归一化到0~255    
		//normalize(imageLog, imageLog, 0, 255, CV_MINMAX);
		//convertScaleAbs(imageLog, imageLog);
		//imshow("imageLog", imageLog);
		//------------------------
		/*Mat imageRGB[3];
		split(src, imageRGB);
		for (int i = 0; i < 3; i++)
		{
			equalizeHist(imageRGB[i], imageRGB[i]);
		}
		merge(imageRGB, 3, src);
		imshow("直方图均衡化图像增强效果", src);
		imshow("yuantu",img);

		Mat imme = src.clone();
		cvtColor(imme, imme, CV_BGR2GRAY);
		threshold(imme, imme, thresh_mm, 255, 1);
		imshow("gray_img", imme); */

		waitKey(20);
	}
	getchar();
	return 0;
}

int rect_img_x = 254;
int rect_img_y = 180;
int rect_img_w = 140;
int rect_img_h = 204;

int main34()
{
	namedWindow("【控制面板】", CV_WINDOW_NORMAL);
	//创建轨迹条  
	createTrackbar("thresh_mm：", "【控制面板】", &thresh_mm, 255, ContrastAndBright);
	createTrackbar("canny_num：", "【控制面板】", &canny_num, 600, ContrastAndBright);
	//createTrackbar("m_method：", "【控制面板】", &m_method, 5, ContrastAndBright);
	ContrastAndBright(thresh_mm, 0);
	sys_init();
	VideoCapture capture(1);
	Mat img;
	namedWindow("other");
	while (1)
	{
		capture >> img;
		if (!img.data)
		{
			printf("please sure you have connect the camera");
			break;
		}
		img = img(Rect(rect_img_x, rect_img_y, rect_img_w, rect_img_h));
		Mat src = img.clone();
		imshow("source",img);

	/*	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
		filter2D(img, img, CV_8UC3, kernel);
		imshow("LApuLAsi", img);*/

		//Mat imageLog(img.size(), CV_32FC3);
		////	 printf("it is ok \n");
		//for (int i = 0; i < img.rows; i++)
		//{
		//	for (int j = 0; j < img.cols; j++)
		//	{
		//		imageLog.at<Vec3f>(i, j)[0] = log(1 + img.at<Vec3b>(i, j)[0]);
		//		imageLog.at<Vec3f>(i, j)[1] = log(1 + img.at<Vec3b>(i, j)[1]);
		//		imageLog.at<Vec3f>(i, j)[2] = log(1 + img.at<Vec3b>(i, j)[2]);
		//	}
		//}
		////归一化到0~255    
		//normalize(imageLog, imageLog, 0, 255, CV_MINMAX);
		//convertScaleAbs(imageLog, imageLog);
		//imshow("imageLog", imageLog);

		Mat imme;
		cvtColor(img, imme, CV_BGR2GRAY);
		threshold(imme, imme, thresh_mm, 255, 1);
		imshow("gray_img", imme);
		match_temp(imme, mask_img);
		circle(src, center, 3, Scalar(0, 255, 255), -1, 8, 0);
		
		//圆轮廓
		circle(src, Point(center.x, center.y), mask_img_r / 2 - 2, Scalar(0, 255, 255), 4, 8, 0);
		
		String str = "[" + to_string(center.x) + "," + to_string(center.y) + "]";
		putText(src, str, center, FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1);
		imshow("ofcource",src);

		if (waitKey(20) == 'q')
		{
			break;
		}
	}
	return 0;
}
