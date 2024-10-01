#include "Server.h"
#include <memory>


Server::Server(std::string& fileName)
{
	_parser = new IniParser(fileName);
	_port = _parser->GetServerPort();
	_dbManager = new DataBaseManager(_port);
}

Server::~Server()
{
	delete _parser;
	delete _dbManager;
}

void Server::RequestToDB(std::string& word)
{
	if (_dbManager) {
		_dbManager->RequestToDB(word);
	}
}
