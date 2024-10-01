#pragma once

#include "IniParser.h"
#include "DataBaseManager.h"


class Server {
public:
	Server(std::string& fileName);
	~Server();

private:
	IniParser* _parser = nullptr;
	DataBaseManager* _dbManager = nullptr;
	int _port = 0;

public:
	void RequestToDB(std::string& word);
};