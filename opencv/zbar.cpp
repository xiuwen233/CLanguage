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


//other demo
int ZbarTobar(Mat img)
{
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	//图片转换
	Mat imageGray;
	cvtColor(img, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;

	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //扫描条码    
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	//扫描结果打印
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "查询条码失败，请检查图片！" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
		cout << "条码：" << endl << symbol->get_data() << endl << endl;
	}
	//显示二维码
	imageZbar.set_data(NULL, 0);
	return  0;
}


int DMdemo()
{
	int length = DataMatrix_decode("tyu.jpg"); //解码并返回码字长度(解码失败则返回-1)
	if (length>0)
	{
		unsigned char* message = (unsigned char*)malloc(sizeof(char)*(length + 1));
		DataMatrix_output(message);//将解码码字保存到数组中
		message[length] = 0;		
		printf(" %s \n", message);
		free(message);
	}
	return 0;
}
