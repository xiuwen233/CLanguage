  WSACleanup();
	state_record = "";
	theApp.GetMainWnd()->GetDlgItem(IDC_DATA_RECEIVE)->SetWindowText(state_record);
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsdata;
	SOCKET serverSocket;
	CString  str;

	if (WSAStartup(sockVersion, &wsdata) != 0)
	{
		//待处理
		//printf("WSAStartup failed \n");
		str = "WSAStartup failed ";
		SendMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_MyMessage, 0, LPARAM(&str));
		return;
	}

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		//待处理
		str = "ServerSocket  Establish failed ";
		SendMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_MyMessage, 0, LPARAM(&str));
		return;
	}
	//绑定套接字
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(666);
	sockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.201.250");   //inet_addr("127.0.0.1")  INADDR_ANY  inet_addr("192.168.1.0");

	if ( ::bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		//待处理
		str = "ServerSocket  Bind  failed ";
		SendMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_MyMessage, 0, LPARAM(&str));
		return;
	}

	//开始监听
	if (listen(serverSocket, 10) == SOCKET_ERROR)
	{
		//待处理
		str = "ServerSocket  Listen failed ";
		SendMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_MyMessage, 0, LPARAM(&str));
		return;
	}

	SOCKET clientSocket;
	sockaddr_in client_sin;
	int len = sizeof(client_sin);
	int socketnum = 0;
	int num_acceprt = 0;

	//printf("ready to accept\n");
	globalstate.socket_connect = true;
	while (true)
	{
		clientSocket = accept(serverSocket, (sockaddr*)&client_sin, &len);
    .................
  }
