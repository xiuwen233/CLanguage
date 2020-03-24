	//Mat imageLog(imme.size(), CV_32FC3);
		////	 printf("it is ok \n");
		//for (int i = 0; i < imme.rows; i++)
		//{
		//	for (int j = 0; j < imme.cols; j++)
		//	{
		//		imageLog.at<Vec3f>(i, j)[0] = log(1 + imme.at<Vec3b>(i, j)[0]);
		//		imageLog.at<Vec3f>(i, j)[1] = log(1 + imme.at<Vec3b>(i, j)[1]);
		//		imageLog.at<Vec3f>(i, j)[2] = log(1 + imme.at<Vec3b>(i, j)[2]);
		//	}
		//}
		////归一化到0~255    
		//normalize(imageLog, imageLog, 0, 255, CV_MINMAX);
		//convertScaleAbs(imageLog, imageLog);
		//imshow("imageLog", imageLog);
		//----------------------------------------------------------------------------------------------------------------------------------
			Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
		filter2D(imme, imme, CV_8UC3, kernel);
		imshow("LApuLAsi", imme);
