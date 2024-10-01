#include "DataBaseManager.h"
#include "IniParser.h"
#include <iostream>
#include <string>

DataBaseManager::DataBaseManager(std::string& host, std::string& port, std::string& dbname, std::string& user, std::string& password)
{
	_connectionStr = host + " " + port + " " + dbname + " " + user + " " + password;
	_wordsTableQuery = "CREATE TABLE if not exists public.words "
		"(id SERIAL primary key, "
		"word text NOT NULL)";

	_docsTableQuery = "CREATE TABLE if not exists public.docs "
		"(id SERIAL primary key, "
		"doc_path text NOT NULL)";

	_wordFrequency = "CREATE TABLE if not exists word_frequency "
		"(word_id INTEGER references public.words(id), "
		"doc_id INTEGER references public.docs(id), "
		"word_frequency int NOT NULL, "
		"constraint pk primary key (word_id, doc_id))";
		
	_insertQuery = "";
}

DataBaseManager::DataBaseManager(int serverPort): _serverPort(serverPort) 
{
}

DataBaseManager::~DataBaseManager()
{
}

void DataBaseManager::AddToDB(std::map<std::string, int>& words, const std::string& filePath)
{
	try 
	{
		pqxx::connection conn(_connectionStr);
		std::cout << "Connection to DB... SUCCESS" << std::endl;
		_connectionStatus = true;
		
		CreateTable(conn, _wordsTableQuery);
		CreateTable(conn, _docsTableQuery);
		CreateTable(conn, _wordFrequency);
		
		for (const auto& rows : words) {
			InsertRow(conn, TableType::words, rows.first);
			InsertRow(conn, TableType::docs, filePath);
			InsertRow(conn, TableType::frequency, std::to_string(rows.second)); //тут access violation, возможно нужен lock_guards
		}
	}

	catch (pqxx::sql_error ex) {
		std::cout << "ERROR (CONNECTION TO DB): " << ex.what() << std::endl;
		_connectionStatus = false;
	}
}

bool DataBaseManager::RequestToDB(std::string& findWord)
{
	GetConnectionString();
	try {
		pqxx::connection conn(_connectionStr);
		std::cout << "Connection to DB... SUCCESS" << std::endl;
		_connectionStatus = true;

		SendRequest(conn, findWord);
	}
	catch (std::exception& ex) {
		std::cout << "Connection to DB failed. " << ex.what() << std::endl;
	}

	return _connectionStatus;
}

void DataBaseManager::SendRequest(pqxx::connection& conn, std::string& findWord)
{
	std::string query_word = "SELECT id, word from public.words WHERE word LIKE '" + findWord + "'";
	pqxx::transaction tx(conn);
	auto result = tx.query<int, std::string>(query_word);

	for (auto& row : result) {
		std::cout	<< "id: " << std::get<0>(row)
					<< "word " << std::get<1>(row) << std::endl;
	}

	//std::string query = "SELECT word_id, doc_id, word_frequency from public.word_frequency WHERE firstName LIKE '" + findWord + "'";
	//
	//
	//
	tx.commit();
}

bool DataBaseManager::InsertRow(pqxx::connection& connection, TableType table, const std::string& field)
{
	try {

		pqxx::transaction tx(connection);
		switch (table) {
			case TableType::words:
			{
				_wordId.clear();
				_insertQuery = "INSERT INTO public.words(word) VALUES ('" + tx.esc(field) + "') returning id";
				_wordId = tx.exec(_insertQuery);
			} break;
			case TableType::docs:
			{
				if (_docPath != field) {
					_docId.clear();
					_insertQuery = "INSERT INTO public.docs(doc_path) VALUES ('" + tx.esc(field) + "') returning id";
					_docId = tx.exec(_insertQuery);
					_docPath = field;
				}
			} break;
			case TableType::frequency:
			{
				std::tuple<std::string> word_id = _wordId[0].as<std::string>();
				std::tuple<std::string> doc_id = _docId[0].as<std::string>();

				//auto ids = std::make_tuple(_wordId[0].as<std::string>(), _docId[0].as<std::string>());
				_insertQuery = "INSERT INTO word_frequency(word_id, doc_id, word_frequency) VALUES ('" + tx.esc(std::get<0>(word_id)) + "','" + tx.esc(std::get<0>(doc_id)) + "','" + tx.esc(field) + "')";
				tx.exec(_insertQuery);
			} break;
			default:
				break;
		}
		tx.commit();
		return true;
	}
	catch (std::exception& ex) {
		std::cout << "ERROR (INSERT INTO DB): " << ex.what() << std::endl;
		return false;
	}
}

bool DataBaseManager::CreateTable(pqxx::connection& connection, std::string& query)
{
	if (_connectionStatus) {
		pqxx::transaction trx(connection);
		trx.exec(query);
		trx.commit();
		return true;
	}
	return false;
}
