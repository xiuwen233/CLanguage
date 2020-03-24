Mat justforGrenn(Mat img)
{
	for (int i = 0; i < img.rows; i++)
	{
		uchar* p = img.ptr(i);//获取Mat某一行的首地址  

		for (int j = 0; j < img.cols; j++)
		{
			//顺序是BGR,不是RGB
			int B = *(p + j * 3);
			int G = *(p + j * 3 + 1);
			int R = *(p + j * 3 + 2);
			//float Gray = R*0.299 + G*0.587 + B*0.114;
			if ( abs(R-G)<20 && (R-B) >30 &&(G - B)>30 && R>140&&R<220) // R > num && G>num && B>num  Gray >  num  || ( R>200 && G>200 && B>200  && abs(R-B)<20 && abs(R - G)<20)
			{
				*(p + j * 3) = 255;
				*(p + j * 3 + 1) = 255;
				*(p + j * 3 + 2) = 255;
			}
			else {
				*(p + j * 3) = 0;
				*(p + j * 3 + 1) = 0;
				*(p + j * 3 + 2) = 0;
			}
		}
	}
	//imwrite("JustGreen.jpg", img);
	return img;
}
