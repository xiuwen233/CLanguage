vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	CvBox2D rect;
	CvPoint2D32f Corners[4];
  
  	findContours(getrch, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point()); //CV_RETR_EXTERNAL
		for (int i = 0; i < contours.size(); i++)
		{
			rect = minAreaRect((contours[i]));

			cvBoxPoints(rect, Corners);
			if (rect.size.width  * rect.size.height > findcut_size &&
				rect.size.width > fincout_x &&
				rect.size.height > findcut_y) {

				if (is_drawresult)
				{
					line(frame, Point((int)Corners[0].x, (int)Corners[0].y), Point((int)Corners[1].x, (int)Corners[1].y), Scalar(255, 0, 255), 2, 8, 0);
					line(frame, Point((int)Corners[1].x, (int)Corners[1].y), Point((int)Corners[2].x, (int)Corners[2].y), Scalar(255, 0, 255), 2, 8, 0);
					line(frame, Point((int)Corners[2].x, (int)Corners[2].y), Point((int)Corners[3].x, (int)Corners[3].y), Scalar(255, 0, 255), 2, 8, 0);
					line(frame, Point((int)Corners[0].x, (int)Corners[0].y), Point((int)Corners[3].x, (int)Corners[3].y), Scalar(255, 0, 255), 2, 8, 0);
				}
			    
				if (is_getchippostion)
				{
					findchippostion.angel = rect.angle;
					findchippostion.center_x = rect.center.x;
					findchippostion.center_y = rect.center.y;
					is_getchippostion = false;
				}
	

				for (int j = 0; j < 4; j++)
				{
					corner_point[j].x = (int)Corners[j].x;
					corner_point[j].y = (int)Corners[j].y;
				}

				ResultRect = GetTheRectRangle();

				if (is_writematchtemp)
				{	
					imwrite("matchtemp.jpg", src(Rect(ResultRect.rect_x, ResultRect.rect_y, ResultRect.rect_weight, ResultRect.rect_hight)));
					is_writematchtemp = false;
					is_fileexist = true;
				}

				is_findcouter = true;
			}
		}


findContours(getrch, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			rect = minAreaRect((contours[i]));
			
			cvBoxPoints(rect, Corners);
			boundRect[i] = boundingRect(Mat(contours[i]));
			if (rect.size.width  * rect.size.height > 10000 &&
				rect.size.width > 100 &&
				rect.size.height > 100) {

				line(frame, Point((int)Corners[0].x, (int)Corners[0].y), Point((int)Corners[1].x, (int)Corners[1].y), Scalar(255, 0, 255), 2, 8, 0);
				line(frame, Point((int)Corners[1].x, (int)Corners[1].y), Point((int)Corners[2].x, (int)Corners[2].y), Scalar(255, 0, 255), 2, 8, 0);
				line(frame, Point((int)Corners[2].x, (int)Corners[2].y), Point((int)Corners[3].x, (int)Corners[3].y), Scalar(255, 0, 255), 2, 8, 0);
				line(frame, Point((int)Corners[0].x, (int)Corners[0].y), Point((int)Corners[3].x, (int)Corners[3].y), Scalar(255, 0, 255), 2, 8, 0);

				for (int j = 0; j < 4; j++)
				{
					corner_point[j].x = (int)Corners[j].x;
					corner_point[j].y = (int)Corners[j].y;
				}

				rectangle(frame, Point(boundRect[i].x , boundRect[i].y ),
					Point(boundRect[i].x + boundRect[i].width*0.9 , boundRect[i].y + boundRect[i].height*0.9), Scalar(255), 2, 8);
				ResultRect = SmallGetTheRectRangle();
				findcouter_number++;
			}
		}
