Mat  MatchTempleImage(Mat src, Mat temple, int matchtype )
{
	Mat result;
	int result_rows = src.rows - temple.rows + 1;
	int result_cols = src.cols - temple.cols + 1;
	result.create(result_cols, result_rows, CV_32FC1);
	matchTemplate(src, temple, result, matchtype);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	double minVal = -1;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	if (matchtype == TM_SQDIFF || matchtype == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	rectangle(src, matchLoc, Point(matchLoc.x + temple.cols, matchLoc.y + temple.rows), Scalar(0, 255, 0), 2, 8, 0);
	imshow("src", src);
	return src;

}

//汉明距离0 相似度很高 <5较高 >5差异比较大
Mat calPHashCode(Mat image)
{
	Mat floatImage, imageDct;
	resize(image, image, Size(32, 32));
	image.convertTo(floatImage, CV_32FC1);
	dct(floatImage, imageDct);
	Rect roi(0, 0, 8, 8);
	Scalar imageMean = mean(imageDct(roi));
	return (imageDct(roi) > imageMean[0]);
}

int calHammingDistance(Mat modelHashCode, Mat testHashCode)
{
	return countNonZero(modelHashCode != testHashCode);
}

