  WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	int ret = 0;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return;
	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(atoi(socketinfo.host_port));
	inet_pton(AF_INET, socketinfo.host_ip, &serAddr.sin_addr);

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{

		return;
	}
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{  //连接失败 
		closesocket(sclient);
		return;
	}
	host_ip.socket_cpu = sclient;

	char senddata[256] = { 0 };
	memset(senddata, 0, sizeof(senddata));

	ret = send(sclient, senddata, lengh, 0);
	if (ret == SOCKET_ERROR)
	{
	
		return;
	}
	
	char recData[255];
	
	while (true)
	{	
		ret = recv(sclient, recData, 255, 0);
    ....................
  }
