#include<iostream>
#include<json\json.h>

using namespace std;


void testchar()
{
	const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";
	cout << str << endl;
}


void whathelp(Json::Value value)
{
	Json::Value::Members men = value.getMemberNames();
	for (Json::Value::Members::iterator iterMember = men.begin(); iterMember != men.end(); iterMember++)
	{
		string strKey = *iterMember;
		cout << strKey.c_str() << "	";


	}// 遍历每个key
	cout << endl;
}

string setwindows()
{
	
	Json::Value root;
	Json::Value data;
	Json::Value hjk;

	root["action"] = "run";
	data["number"] = 1;
	data["dudoyou"] = 3;
	data["fuck"] = "";
	data["valu"].append(1);
	data["valu"].append(2);
	data["valu"].append(3);
	root["data"] = data;

	/*hjk["name"] = "wang";
	hjk["age"] = "23";
	root.append(hjk);
	hjk["name"] = "liu";
	hjk["age"] = "18";
	root.append(hjk)*/;

	Json::StreamWriterBuilder builder;
	const std::string json_file = Json::writeString(builder, root);
	std::cout << json_file << std::endl;
	return json_file;
}

int main()
{

	string strValue = "{\"MsgCode\":1002,\"MsgBody\":{\"bkid\":\"\",\"socid\":1}}";
	//string strValue = setwindows();
	Json::Value value;
	Json::CharReaderBuilder breadvuilder;
	const std::unique_ptr<Json::CharReader> reader(breadvuilder.newCharReader());
	JSONCPP_STRING err;

	if (reader->parse(strValue.c_str(), strValue.c_str() + strValue.length(), &value, &err))
	{
	//	whathelp(value);
		if (value["MsgBody"].type() == Json::objectValue)
		{
			string str = value["MsgBody"].toStyledString();
			cout << str << endl;
			reader->parse(str.c_str(), str.c_str() + str.length(), &value, &err);
			int name = value["socid"].asInt();
			cout << name << endl;
			//whathelp(value);
		}
			//value["data"].asString();
		
		//std::cout << out << std::endl;
	}

	//testchar();
	
	getchar();
	return 0;
}




