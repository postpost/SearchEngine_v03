#include "IniParser.h"
#include "IniParser.h"
#include "IniParser.h"
#include <string>

IniParser::IniParser(std::string iniFileName) : _iniFileName{ iniFileName } {};

IniParser::~IniParser()
{
	_fileIn.close();
}

void IniParser::ParseIniFile()
{
	std::cout << "Open IniFile..." << _iniFileName << std::endl;
	try {
		_fileIn.open(_iniFileName);
		
		if(_fileIn.is_open()) 
		{
			std::string line;
			while (!_fileIn.eof()) {
				std::getline(_fileIn, line);
				
				if (line[0] == ';' || line.empty()) {
					continue;
				}
				if (line.find(';') != line.npos) {
					RemoveComments(line);
				}
				if (line.find("DatabaseConnection") != line.npos) 
				{
					GetDataFromIni();
				}
				else if (line.find( "CrawlerData") != line.npos)
				{
					GetDataFromIni();
				}
				else if (line.find("HTTPServerData") != line.npos) {
					GetDataFromIni();
				}
			}
		}
		else {
			std::cout << "File is not opened\n";
		}

	}
	catch (std::exception& ex) {
		std::cout << "Error occurred in <<ParseIniFile()>>: " << ex.what() << std::endl;
	}
}

void IniParser::PrintConfigData()
{
	std::cout << "-----------[DataToConnectToDB]--------\n"
		<< "host: " << dataToConnect._host << '\n'
		<< "port: " << dataToConnect._port << '\n'
		<< "dbname: " << dataToConnect._dbname << '\n'
		<< "user: " << dataToConnect._user << '\n'
		<< "password: " << dataToConnect._password << '\n'
		<< "-----------[CrawlerData]--------\n"
		<< "start web: " << dataToCrawl._startWeb << '\n'
		<< "recursion depth: " << dataToCrawl._recursionDepth << '\n';
}

std::string IniParser::GetHost()
{
	return dataToConnect._host;
}

std::string IniParser::GetPort()
{
	return dataToConnect._port;
}

std::string IniParser::GetDBName()
{
	return dataToConnect._dbname;
}

std::string IniParser::GetUser()
{
	return dataToConnect._user;
}

std::string IniParser::GetPassword()
{
	return dataToConnect._password;
}

std::string IniParser::GetStartWebPage()
{
	return dataToCrawl._startWeb;
}

int IniParser::GetRecursionDepth()
{
	return dataToCrawl._recursionDepth;
}

void IniParser::RemoveComments(std::string& str)
{
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == ';') {
			while (str.find('\n') != str.npos) {
				str.erase(str[i], '\n');
			}
		}
	}
	std::cout << str << std::endl;
}

void IniParser::SaveDataFromFile(std::string& dataString)
{
	auto itr = dataString.begin();
	size_t start = 0, end = 0;
	const char delim = '=', newLine = '\n';
	
	for (itr; itr !=dataString.end() && !dataString.empty(); ++itr) {
			
		start = dataString.find(delim);
		end = dataString.find(newLine);

		std::string substring = dataString.substr(0, end);
		std::string fieldName = substring.substr(0, start);

		if (fieldName == "host") {
				dataToConnect._host = substring.substr(start+1,end);
		}
		
		else if (fieldName == "port") {
			dataToConnect._port = substring.substr(start + 1,end);
		}

		else if (fieldName == "dbname") {
			dataToConnect._dbname = substring.substr(start + 1, end);
		}

		else if (fieldName == "user") {
			dataToConnect._user = substring.substr(start + 1, end);
		}
		
		else if (fieldName == "password") {
			dataToConnect._password = substring.substr(start + 1, end);
		}

		else if (fieldName == "startWeb") {
			dataToCrawl._startWeb = substring.substr(start + 1, end);
		}

		else if (fieldName == "recursionDepth") {
			dataToCrawl._recursionDepth = std::stoi(substring.substr(start + 1, end));
		}

		else if (fieldName == "serverPort") {
			server._port = std::stoi(substring.substr(start + 1, end));
		}

		dataString.erase(0, (end +1));
		if (dataString.empty() || dataString == "\n")
			break;
	}
}

void IniParser::GetDataFromIni()
{
	const char delim = '[';
	std::string sectionStr;
	std::getline(_fileIn, sectionStr,delim);
	//std::cout << connectionDataStr << std::endl;
	SaveDataFromFile(sectionStr);
}
