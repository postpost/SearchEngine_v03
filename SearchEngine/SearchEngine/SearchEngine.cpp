#include "Crawler.h"
#include <Windows.h>

#include <iostream>
#include <boost/asio.hpp>
#include "Server.h"
#pragma execution_character_set("utf-8")

using namespace boost::asio;
using ip::tcp;

std::string ReadRequestFromClient(tcp::socket& socket) {
	boost::asio::streambuf buf;
	boost::asio::read_until(socket, buf, "\n");
	std::string dataRes = boost::asio::buffer_cast<const char*>(buf.data());
	return dataRes;
}

void SendReplyToClient(tcp::socket& socket, const std::string& message) {
	const std::string msg = message + "\n";
	boost::asio::write(socket, boost::asio::buffer(msg));
}


int main(int argc, char* argv[]) {

	//SetConsoleCP(CP_UTF8);
	//SetConsoleOutputCP(CP_UTF8);

	//std::string iniFileName = "config.ini";

	////Crawler
	//Crawler crawler(iniFileName);

	//Server
	std::cout << "what's up?" << std::endl;
	//boost::asio::io_service io_service;

	////listen for new connection, requested by a client
	//tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234)); //конечная точка соединения инициализируется на ipv4 и порт 1234

	////creates socket
	//tcp::socket socket(io_service); //Далее сервер создаст сокет и будет ждать соединения от клиента.

	////waits for connection
	//acceptor.accept(socket);

	////reads a message from the client
	//std::string msg = ReadRequestFromClient(socket);
	//std::cout << msg << std::endl;


	////Send reply to the client
	//SendReplyToClient(socket, "Yikes! Hello from Server!");
	//std::cout << "Server sent a reply to the client!\n";

	///*Server server;
	//std::string word = "analysis";
	//server.RequestToDB(word);*/

	return 0;
};