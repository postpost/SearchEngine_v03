#pragma once

#include "IniParser.h"
#include "Indexer.h"
#include <pqxx/pqxx>
#include <map>
#include <pqxx/internal/gates/connection-errorhandler.hxx>

enum class TableType {
	words = 1,
	docs = 2,
	frequency = 3
};

class DataBaseManager {
public:
	DataBaseManager(std::string& host, std::string& port, std::string& dbname, std::string& user, std::string& password);
	DataBaseManager(int serverPort);
	~DataBaseManager();

	void AddToDB(std::map <std::string, int>& words, const std::string& filePath);
	//get data
	bool RequestToDB(std::string& findWord);
	void SendRequest(pqxx::connection& conn, std::string& findWord);
	std::string GetConnectionString() const { return _connectionStr; };

private:
	bool _connectionStatus = false;
	std::string _connectionStr="";

	std::string _wordsTableQuery = "";
	std::string _docsTableQuery = "";
	std::string _wordFrequency = "";
	std::string _insertQuery = "";

	pqxx::result _wordId;
	pqxx::result _docId;

	std::string _docPath="";
	int _serverPort;
	
	//methods
	bool InsertRow(pqxx::connection& connection, TableType table, const std::string& field);
	bool CreateTable(pqxx::connection& connection, std::string& query);

};