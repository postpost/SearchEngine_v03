#pragma once

#include "IniParser.h"
#include "Indexer.h"
#include "DataBaseManager.h"
#include <boost/system/api_config.hpp>
#include <boost/asio/ssl.hpp>
#include <unordered_set>
#include <iostream>
#include <ctime> //to store HTML pages if they are more than 1

struct URLComponents {
	std::string hostname;
	std::string protocol;
	std::string path;
	int version;
};

class Crawler  {
public:
	Crawler(std::string& fileName);
	~Crawler();
	
	//Methods
	int GetRecursionDepth() { return _recursionDepth; }
	
	void CleanHTML(std::string& fileToClean);
	std::string& GetHTMLContentFileName();

	void SaveLowerCaseFile();
	void CountWords();

	void PrintCountedWords();

	//connection to DB via Indexer
	void AddToDB();

	int GetURLCount(std::unordered_set<std::string>& urls);

public:
	//Fields
	
private:
	//Fields
	std::string _iniFilePath;
	std::string _urlString;
	int _recursionDepth;
	int _recursionCount;

	std::shared_ptr<IniParser> _IniParser = nullptr;
	Indexer* _indexer = nullptr;

	std::string _file = "HTMLResponse.html";
	std::string _HTMLContentFile = "";
	std::unordered_set<std::string> _urls;
	unsigned int _statusCode;
	std::regex _self_regex;

	//to handle HTMLContent
	time_t _timestamp;

	//parse url
	URLComponents _urlFields;
	int count = 0;

	//threadpool
	std::vector<std::thread> _threadPool;


private:
	//Methods
	void ParseURL (const std::string& urlStr, URLComponents* urlFields);

	void DownloadStartWeb(const std::string url, boost::asio::io_context& io);
	std::unordered_set<std::string> GetHTMLLinks();
	
	void DownloadURLs(boost::asio::io_context& ioc);
	//void DownloadWebPage(URLComponents& urlFields, boost::asio::io_context& ioc);
	int GetHTTPS(URLComponents* urlFields, boost::asio::io_context &ioc);
	int GetHTTP(URLComponents* urlFields, boost::asio::io_context& ioc);

	void HandleThreadPoolTask(const std::string& urlStr, URLComponents* urlFields, boost::asio::io_context& ioc);
	
	void ShutdownHandler(const boost::system::error_code& err);
	void PrintCertificate(boost::asio::ssl::verify_context& ctx);
	void ReadStatusLineHandler(const boost::system::error_code& err);
	//---
	void ReadStatusLineHandlerHTTP(const std::stringstream& buffer, const boost::system::error_code& err); //test function
	//--
	void ReadHeaderHandler(const boost::system::error_code& err, std::ifstream& response_stream);
	
	void SaveContent(const boost::system::error_code& err, std::ifstream& response_stream);
	
	std::string DefineFileName(std::string& timeStr);
	std::string GetDate();

	int GetCoresCount();
};
