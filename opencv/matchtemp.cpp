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
