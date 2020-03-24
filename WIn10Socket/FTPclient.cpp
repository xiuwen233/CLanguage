void main()
{
	BOOL dRes, pRes;
	HINTERNET hInternet;
	HINTERNET hConnect;
	hInternet = InternetOpen(_T("A3GS Sample"), INTERNET_OPEN_TYPE_DIRECT,
		NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE);
	if (NULL == hInternet)
	{
		printf("InternetOpen Error:%d\n", GetLastError());
	}

	printf("nfc");
	hConnect = InternetConnect(hInternet, _T("127.0.0.1")/*FTP服务器地址*/, INTERNET_DEFAULT_FTP_PORT/*FTP端口号，此为默认值---21*/,
		_T("guanliyuan")/*用户名*/, _T("123456")/*密码*/, INTERNET_SERVICE_FTP,
		INTERNET_FLAG_EXISTING_CONNECT || INTERNET_FLAG_PASSIVE, 0);
	if (NULL == hInternet)
	{
		printf("InternetConnect Error:%d\n", GetLastError());
		InternetCloseHandle(hInternet);
	}

	//dRes = FtpGetFile(hConnect, "./wwyy/download/test1.txt", "D:\\BT\\test1.txt", FALSE,
	//	FILE_ATTRIBUTE_ARCHIVE, FTP_TRANSFER_TYPE_UNKNOWN, 0);
	//if (dRes == 0)
	//{
	//	printf("FtpGetFile Error:\n", GetLastError());
	//}
	//else {
	//	printf("下载文件成功！\n");
	//}

	pRes = FtpPutFile(hConnect, _T("E://Apche/ReadMe.txt"), _T("ReadMe.txt"), FTP_TRANSFER_TYPE_ASCII, 0);
	if (pRes == 0)
	{
		printf("上传文件失败！\n");
	}
	else {
		printf("上传文件成功！\n");
	}
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	if (pRes)
	{
		getchar();
		return;
	}
	
	else
	{
		getchar();
		return;
	}
}
