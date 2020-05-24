#pragma once
///////////////////////////////////////////////////////////////////////////////////
// Display.h  -  Display the Modified .HTML pages  pop-up in browser             //
// ver 1.0                                                                       //
// Language:    C++, Visual Studio 2017					        				 //
// Application: Most Projects, CSE687 - Object Oriented Design                   //
// Author:      Wanxiang Xie, Syracuse University,						         //
//              wxie15@syr.edu											         //
///////////////////////////////////////////////////////////////////////////////////
//
//* Package Operations :
//*------------------ -
//* Display the converted .HTML pages  pop-up in browser
//*  Required Files :
//*---------------- -
//* Process.h
//* 
//*  Maintenance History :
//*----------------------
//*  ver 1.0 : 12 Mar 2019
//* -first release

#include "../FileSystem/FileSystem.h"
#include "../Process/Process.h"
#include <iostream>
#include <conio.h>
#include <vector>


class DisplayProcess
{
public:
	DisplayProcess(std::vector<std::string> path);
	~DisplayProcess();
	void doDisplay(std::string appPath);
private:
	std::vector<std::string> filePaths_;
	int count = 0;
};

DisplayProcess::DisplayProcess(std::vector<std::string> path)
{
	filePaths_ = path;
}

DisplayProcess::~DisplayProcess()
{
}

// -----< Display files you found by browser and pop-up one by one  >---------------

inline void DisplayProcess::doDisplay(std::string appPath) {

	std::cout << "\n Demonstrating the modified .html file in the browser!" << std::endl;
	std::cout << "=======================================================" << std::endl;
	Process p;
	p.title("test application");
	//appPath = "C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe";  // path to application to start
	p.application(appPath);

	std::cout << "\n  Will start browser 5 times and each time wait for termination.";
	std::cout << "\n  You need to kill each window (upper right button) to continue.";
	std::cout << "\n  Press key to start";
	_getche();
	if (filePaths_.size() != 0)
		for (auto temp : filePaths_) {
			std::string path = FileSystem::Path::getFullFileSpec(temp);
			
					std::string cmdLine = "/A " + path;
					std::cout << cmdLine << std::endl;
					p.commandLine("--new-window" + cmdLine);				// asking browser to display Process.h
					std::cout << "\n  starting process: \"" << appPath << "\"";
					std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";

					p.create();
					CBP callback = []() { std::cout << "\n  --- child process exited with this message ---"; };
					p.setCallBackProcessing(callback);
					p.registerCallback();
					WaitForSingleObject(p.getProcessHandle(), INFINITE);  // wait for created process to terminate
					count++;
					if (count == 5)			 //only allow to show 5 pages one by one
						break;
		}

	else
		std::cout << "You have no files to pop-up in browser, Please check on it! " << std::endl;
	std::cout << "\n  after OnExit";
	std::cout.flush();
	char ch;
	std::cin.read(&ch, 1);
}