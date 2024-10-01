#include "Indexer.h"
#include "DataBaseManager.h"
#include <fstream>


//#include <boost/locale.hpp>

Indexer::Indexer(std::shared_ptr<IniParser> parser): _parser(parser)
{
	_sign_regex = std::regex("<.*?>|[^\\w\\s]"); 
}

Indexer::~Indexer()
{
	delete _dbManager;
}

void Indexer::CleanText(std::string& txtToClean)
{
	std::ofstream fout(_cleanedHTML);
	std::string newStr = std::regex_replace(txtToClean, _sign_regex, " ");
	fout << newStr;
	fout.close();

	//std::cout << "Result string: \"" << newStr << "\"\n";
}

void Indexer::ConvertToLowerCase()
{
	std::string buffer;
	std::ifstream fin(_cleanedHTML);

	if (fin.is_open()) {

		while (!fin.eof())
		{
			std::getline(fin, buffer);
		}
	}
	fin.close();

	//boost::locale::to_lower(buffer);

	std::string resLine;
	auto itr = buffer.begin();
	for (itr; itr != buffer.end(); ++itr) {
		resLine += std::tolower(*itr);
	}
	SaveToFile(resLine);
}

std::map<std::string, int> Indexer::CountWords()
{
	std::regex separator(R"([\t\s+/g,\.\!\-\"]+)"); //

	std::string line="";
	std::ifstream fin(_lowerCaseFile);
	if (fin.is_open()) {
		while (!fin.eof()) {
			std::getline(fin, line);
		}
	}
//	std::regex_replace(line, std::regex("[^a-zA-Z]"), ""); //удаляет ненужные цифры
	
	std::sregex_token_iterator itr(line.cbegin(), line.cend(), separator, -1), end;
	std::map <std::string, int> wordFreq;
	for (;itr != end; ++itr) {
		++wordFreq[*itr];
	}
	CleanMap(wordFreq);
	//PrintCountedWords();
	return _countedWords;
}

void Indexer::PrintCountedWords()
{
	if (!_countedWords.empty()) {
		for (const auto& word : _countedWords) {
			std::cerr <<"\"" << word.first << "\"" << " met " << word.second << " times." << std::endl;
		}
	}
}

void Indexer::AddToDataBase()
{
	if (_parser == nullptr) {
		std::cout << "Indexer::AddToDataBase(): _PARSER == NULLPTR\n";
		return;
	}
	std::string host = GetHost();
	std::string	port = GetPort();
	std::string	dbname = GetDBName();
	std::string	user = GetUser();
	std::string	password = GetPassword();
	_dbManager = new DataBaseManager(host, port, dbname, user, password);
	_dbManager->AddToDB(_countedWords, _lowerCaseFile);
}


std::string Indexer::DefineFileName()
{
	++_fileCount;
	return _lowerCaseFile = "HTMLDB\\LowerCaseHTML_" + std::to_string(_fileCount) + ".html";

}

void Indexer::SaveToFile(std::string& text)
{
	DefineFileName();
	std::ofstream fout(_lowerCaseFile);
	if (fout.is_open()) {
		fout << text;
	}

	fout.close();
}

void Indexer::CleanMap(std::map <std::string, int>& container)
{
	_countedWords.clear();
	auto itr = container.begin();
	for (; itr != container.end(); ++itr) {
		//std::cerr << itr->first.length() << std::endl;
		if (itr->first.length() >= 3 && itr->first.length() <= 32) {
			_countedWords.insert(std::pair(itr->first, itr->second));
		}
	}

}



