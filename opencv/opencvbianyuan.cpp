int Firstmain()
{  
	Mat videoimage;
	Mat videoimage2;
	Mat filter, canny,wavefilter;
	Mat blue, out;
	Point center;

	Mat soble_x, soble_y;
	Mat abs_x, abs_y, dst;

	InitControlBoard();
	VideoCapture  capture(1);
	VideoCapture  capture2(2);
	vector<Vec4i> lines;

	int true_threshold = 0;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);

	//First_circle_mask_made();

	while (1)
	{
		capture >> videoimage;
		capture2 >> videoimage2;
		videoimage2 = videoimage2(Rect(110, 50, 360, 360));
		imshow("videoimage2", videoimage2);
	//	imshow("videoimage", videoimage);
		videoimage = videoimage(Rect(100,100,260,260));
		out = videoimage.clone();

		//filter2D(videoimage, videoimage, CV_8UC3, kernel);
		//videoimage = Gammaimg(videoimage); //LogTurn Gammaimg
		imshow("videoimage", videoimage);

		cvtColor(videoimage, videoimage, CV_RGB2GRAY);

		Sobel(videoimage, soble_x, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
		convertScaleAbs(soble_x, abs_x);
		imshow("x方向soble", abs_x);

		Sobel(videoimage, soble_y, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
		convertScaleAbs(soble_y, abs_y);
		imshow("y向soble", abs_y);
       
		addWeighted(abs_x, 0.5, abs_y, 0.5, 0, dst);
		imshow("整体方向soble", dst);

		//GaussianBlur(videoimage, videoimage, Size(5,5), 0, 0);
	   //   medianBlur(abs_y, abs_y, 3);
        true_threshold = i_threshold;
		if (true_threshold <= 0 || true_threshold >= 255)
		{
			true_threshold = 180;
		}
		//adaptiveThreshold(videoimage, blue, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,3,5);
	   threshold(dst, blue, true_threshold, 255, CV_THRESH_BINARY);
	    imshow("blue", blue);
		//center = MatchCircleTemplate(blue, First_circle_mask);
		//circle(out, Point(center.x, center.y), first_circle_mask_d / 2, Scalar(255, 0, 255), 1);
		//imshow("out", out);
		morphologyEx(blue, wavefilter, MORPH_CLOSE, element);
		imshow("wavefilter", wavefilter);

        Canny(wavefilter, filter,50,200, 5);
       imshow("filter", filter);

		HoughLinesP(filter, lines, 1, CV_PI / 360, 100, 80, 18);
		center = Findercenter(lines);
		if ( 0 == center.y)
		{
			circle(out, Point(center.x,130), 3, Scalar(255, 0, 255), -1);
		}
		else if (-1 != center.y)
		{
			circle(out, Point(center.x, 130), 3, Scalar(255, 0, 255), -1);
			circle(out, Point(center.y, 130), 3, Scalar(255, 0, 255), -1);
		}
		printf(" lines.size() is %d  \n", lines.size());
		for (int i = 0; i < lines.size(); i++)
		{
			line(out, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(255, 0, 255), 1);
		}
		imshow("out", out);
		waitKey(20);
	}
	
	return 0;
}


//伽马变换对于图像对比度偏低，并且整体亮度值偏高（对于于相机过曝）情况下的图像增强效果明显。
Mat  Gammaimg(Mat img)
{
	Mat image = img.clone();
	Mat imageGamma(image.size(), CV_32FC3);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			imageGamma.at<Vec3f>(i, j)[0] = (image.at<Vec3b>(i, j)[0])*(image.at<Vec3b>(i, j)[0])*(image.at<Vec3b>(i, j)[0]);
			imageGamma.at<Vec3f>(i, j)[1] = (image.at<Vec3b>(i, j)[1])*(image.at<Vec3b>(i, j)[1])*(image.at<Vec3b>(i, j)[1]);
			imageGamma.at<Vec3f>(i, j)[2] = (image.at<Vec3b>(i, j)[2])*(image.at<Vec3b>(i, j)[2])*(image.at<Vec3b>(i, j)[2]);
		}
	}
	//归一化到0~255    
	normalize(imageGamma, imageGamma, 0, 255, CV_MINMAX);
	//转换成8bit图像显示    
	convertScaleAbs(imageGamma, imageGamma);
	return imageGamma;
	//imwrite("Gammaimg.jpg", imageGamma);
}
