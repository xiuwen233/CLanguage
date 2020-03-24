int  zbarTobar(Mat img)
{ 
	clock_t startTime, endTime;
	Mat imageSource;
//	ghy = imread("E://photo_opencv/chipSet/BC1.jpg");
	cvtColor(img, imageSource, CV_BGR2GRAY);
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	int width1 = imageSource.cols;
	int height1 = imageSource.rows;
	uchar *raw = (uchar *)imageSource.data;
	Image imageZbar(width1, height1, "Y800", raw, width1 * height1);
	scanner.scan(imageZbar); //扫描条码      
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "查询条码失败，请检查图片！" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
		cout << "条码：" << endl << symbol->get_data() << endl << endl;
	}
	endTime = clock();
	cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

	return 0;
}
