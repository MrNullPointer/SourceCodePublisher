/////////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages      //
// ver 1.3                                                                 //
// Application    : CSE-687 C++/CLI Shim demostration                      //
// Platform       : Visual Studio 2017 Community Edition                   //
//                  Windows 10 Professional 64-bit, DELL                   //
// Author:         Wanxiang Xie, Syracuse Univeristy                       //
// Email:          wxie15@syr.edu                                          //
// Source:          Jim Fawcett, CSE687 - OOD, Spring 2018                 //
/////////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem/FileSystem.h"
#include "../Publisher/Publisher.h"
#include <chrono>
#include <sstream>
#include <algorithm>
#include <string>
#include <iterator>

namespace MsgPassComm = MsgPassingCommunication;

using namespace ServerRepository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

//----< return name of every file on path >----------------------------

Files Server::getFiles(const ServerRepository::SearchPath& path)
{
	return Directory::getFiles(path);
}
//----< return name of every subdirectory on path >--------------------

Dirs Server::getDirs(const ServerRepository::SearchPath& path)
{
	return Directory::getDirectories(path);
}

namespace MsgPassingCommunication
{
	// These paths, global to MsgPassingCommunication, are needed by 
	// several of the ServerProcs, below.
	// - should make them const and make copies for ServerProc usage

	std::string sendFilePath;
	std::string saveFilePath;

	//----< show message contents >--------------------------------------

	template<typename T>
	void show(const T& t, const std::string& msg)
	{
		std::cout << "\n  " << msg.c_str();
		for (auto item : t)
		{
			std::cout << "\n    " << item.c_str();
		}
	}
	//----< test ServerProc simply echos message back to sender >--------

	std::function<std::vector<Msg>(Msg)> echo = [](Msg msg) {
		std::vector<Msg> replies;
		Msg reply = msg;
		reply.to(msg.from());
		reply.from(msg.to());
		replies.push_back(reply);
		return replies;
	};
	//----< getFiles ServerProc returns list of files on path >----------
	std::function<std::vector<Msg>(Msg)> getFiles = [](Msg msg) {
		std::vector<Msg> replies;
		Msg reply;
		reply.to(msg.from());
		reply.from(msg.to());
		reply.command("getFiles");
		std::string path = msg.value("path");
		if (path != "")
		{
			std::string searchPath = path;
			Files files = Server::getFiles(searchPath);
			size_t count = 0;
			for (auto item : files)
			{
				std::string countStr = Utilities::Converter<size_t>::toString(++count);
				reply.attribute("file" + countStr, item);
			}
		}
		else
		{
			std::cout << "\n  getFiles message did not define a path attribute";
		}
		replies.push_back(reply);
		return replies;
	};
	//----< getDirs ServerProc returns list of directories on path >-----
	std::function<std::vector<Msg>(Msg)> getDirs = [](Msg msg) {
		std::vector<Msg> replies;
		Msg reply;
		reply.to(msg.from());
		reply.from(msg.to());
		reply.command("getDirs");
		std::string path = msg.value("path");
		if (path != "")
		{
			std::string searchPath = path;
			Files dirs = Server::getDirs(searchPath);
			size_t count = 0;
			for (auto item : dirs)
			{
				if (item != ".." && item != ".")
				{
					std::string countStr = Utilities::Converter<size_t>::toString(++count);
					//std::cout << "============================" << std::endl;

					std::cout << item << std::endl;
					reply.attribute("dir" + countStr, item);
				}
			}
		}
		else
		{
			std::cout << "\n  getDirs message did not define a path attribute";
		}
		replies.push_back(reply);
		return replies;
	};

	//----< getPublish ServerProc returns list of convertedfiles on path  >----------------
	std::function<std::vector<Msg>(Msg)> getPublish = [](Msg msg) {
		std::vector<Msg> replies;
		std::string directory = msg.value("path");
		bool hasS = false;
		if (msg.containsKey("SubDir")) {
			hasS = true;
		}
		std::string pattern = msg.value("Pattern");
		std::vector<std::string> patterns;
		std::istringstream iss1(pattern);
		std::copy(std::istream_iterator<std::string>(iss1),
			std::istream_iterator<std::string>(),
			std::back_inserter(patterns));

		std::string regex = msg.value("Regex");
		std::vector<std::string> regexes;
		std::istringstream iss2(regex);
		std::copy(std::istream_iterator<std::string>(iss2),
			std::istream_iterator<std::string>(),
			std::back_inserter(regexes));
		Publisher Publisher;
		std::vector<std::string> convertedFiles = Publisher.getConvertedFiles(directory, patterns, regexes, hasS);
		for (size_t i = 0; i < convertedFiles.size(); i++)
		{
			Msg reply;
			reply.to(msg.from());
			reply.from(msg.to());
			reply.command("getPublish");
			reply.attribute("sendingFile", convertedFiles[i]);
			replies.push_back(reply);
		}

		return replies;
	};
}

using namespace MsgPassingCommunication;

int main()
{
	SetConsoleTitleA("Project4Sample Server Console");
	std::cout << "\n  Testing Server Prototype";
	std::cout << "\n ==========================";
	std::cout << "\n";

	//StaticLogger<1>::attach(&std::cout);
	//StaticLogger<1>::start();

	sendFilePath = FileSystem::Directory::createOnPath("../SendFiles");
	saveFilePath = FileSystem::Directory::createOnPath("../SaveFiles");
	Server server(serverEndPoint, "ServerPrototype");

	MsgPassingCommunication::Context* pCtx = server.getContext();
	pCtx->saveFilePath = saveFilePath;
	pCtx->sendFilePath = sendFilePath;

	server.start();

	std::cout << "\n  testing getFiles and getDirs methods";
	std::cout << "\n --------------------------------------";
	Files files = server.getFiles();
	show(files, "Files:");
	Dirs dirs = server.getDirs();
	show(dirs, "Dirs:");
	std::cout << "\n";

	std::cout << "\n  testing message processing";
	std::cout << "\n ----------------------------";
	server.addMsgProc("echo", echo);
	server.addMsgProc("getFiles", getFiles);
	server.addMsgProc("getDirs", getDirs);
	server.addMsgProc("Publish", getPublish);
	server.addMsgProc("serverQuit", echo);
	server.processMessages();

	Msg msg(serverEndPoint, serverEndPoint);  // send to self
	msg.name("msgToSelf");

	std::cout << "\n  press enter to exit\n";
	std::cin.get();
	std::cout << "\n";

	msg.command("serverQuit");
	server.postMessage(msg);
	server.stop();
	return 0;
}

