/////////////////////////////////////////////////////////////////////
//  Publisher.cpp - Use as lib for Project #3                      //
//  ver 1.2                                                        //
//  Language:      Visual C++, Visual Studio 2015                  //
//  Platform:      Dell XPS 8920, Windows 10                       //
//  Application:   Prototype for CSE687 - OOD Projects             //
//  Author:        Wanxiang Xie,  Syracuse University              //
//  Purpose:       CSE687 demo, Spring 2019                        //
/////////////////////////////////////////////////////////////////////
/*
*  Package Responsibilities:
* ---------------------------
*  Demonstrate how to call all the function together as a glue
* ---------------------------
   Required files
	- Parser.h, Parser.cpp,
	- ITokenCollection.h,
	- Semi.h, Semi.cpp,
	- toker.h, toker.cpp,
	- ActionsAndRules.h, ActionsAndRules.cpp,
	- GrammarHelpers.h, GrammarHelpers.cpp,
	- AbstrSynTree.h, AbstrSynTree.cpp,
	- ScopeStack.h, ScopeStack.cpp
	- ConfigureParser.h, ConfigureParser.cpp,
	- FileSystem.h, FileSystem.cpp,
	- Logger.h, Logger.cpp,
	- Utilities.h, Utilities.cpp
	- Display.h, Display.cpp
	- Dependecies.h, Dependecies.cpp
	- DependencyTable.h, DependencyTable.cpp
	- IPublisher.h
*  Maintenance History:
* ----------------------
* ver 1.2 : 14 Apr 2019
*  - add "IPublisher.h"
*  - add all the function to call in the Shim
* ver 1.1 : 12 Mar 2019
*  - add all the function to show demo
*  ver 1.0 : 8 Feb 2019
*  - first release
*
*/

#include "Publisher.h"


//
//std::vector<std::string> Publisher::GetConvertFiles() {
//	std::cout << "GetConvertFiles()....\n"<< std::endl;
//	std::cout << "Please check in the GUI." << std::endl;
//	return modifiedfiles_;
//}
//



#ifdef TEST_PUBLISHER


int main(int argc, char* argv[])
{
	Publisher publisher;
	std::vector<std::string> Patterns_;
	Patterns_.push_back("*.h");
	Patterns_.push_back("*.cpp");
	std::vector<std::string> Regex_;
	Regex_.push_back("[C](.*)");
	DisplayProcess dispPc(publisher.getConvertedFiles("../TestFiles", Patterns_, Regex_,true));


	std::string appPath;
	FileSystem::File app("../AppPath.txt");
	app.open(FileSystem::File::in, FileSystem::File::text);
	if (app.isGood()) {
		appPath = app.getLine();
	}
	else std::cout << "\nPlease add application path in AppPath.txt file!" << std::endl;
	app.close();

	if (publisher.getConvertedFiles("../TestFiles", Patterns_, Regex_, true).size() != 0) {
		dispPc.doDisplay(appPath);		     //display files by browser
	}
	else
		std::cout << "\n\nHave no matching files to display!" << std::endl;
	return 0;

}

#endif

