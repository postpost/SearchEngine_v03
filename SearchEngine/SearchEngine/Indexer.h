#pragma once

#include <iostream>
#include <regex>
#include <map>
#include "DataBaseManager.h"
#include "IniParser.h"

class Indexer {
public:
	Indexer(std::shared_ptr<IniParser> parser);
	~Indexer();
	void CleanText(std::string& txtToClean);
	void ConvertToLowerCase();


	std::map<std::string, int> CountWords();
	void PrintCountedWords();

	//connect to DB
	void AddToDataBase();
	std::string GetHost() const { return _parser->GetHost(); };
	std::string GetPort() const { return _parser->GetPort(); };
	std::string GetDBName() const { return _parser->GetDBName(); };
	std::string GetUser() const { return _parser->GetUser(); };
	std::string GetPassword() const { return _parser->GetPassword(); };

private:
	std::shared_ptr<IniParser> _parser;
	DataBaseManager* _dbManager = nullptr;

	std::regex _sign_regex;
	std::string _cleanedHTML = "CleanedHTML.html"; //будет сохранено в отдельной папке
	std::string _lowerCaseFile = " ";
	uint32_t _fileCount;

	std::map<std::string, int> _countedWords;

	std::string DefineFileName();
	void SaveToFile(std::string& text);

	void CleanMap(std::map <std::string, int>& container);

};