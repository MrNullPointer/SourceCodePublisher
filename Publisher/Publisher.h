#pragma once
/////////////////////////////////////////////////////////////////////
//  Publisher.h  - Use as lib for Project #3                       //
//  ver 1.0                                                        //
//  Language:      Visual C++, Visual Studio 2017                  //
//  Application:   Prototype for CSE687 - OOD Projects             //
//  Author:        Wanxiang Xie,  Syracuse University              //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../Convert/Convert.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../UtilitiesN/StringUtilities/StringUtilities.h"
#include "../UtilitiesN/CodeUtilities/CodeUtilities.h"
#include "../DependencyTable/DependencyTable.h"
#include "../Dependencies/Dependencies.h"
#include "../Display/Display.h"
#include "../DisplayProcess/DisplayProcess.h"
#include "../FileSystem/FileSystem.h"


class Publisher 
{
public:
	Publisher();
	~Publisher();
	std::vector<std::string> getConvertedFiles(std::string Directory_, std::vector<std::string> Patterns_, std::vector<std::string> Regex_, bool hasS_);
	//virtual int SetCmd(int argc, char** argv) override;
	std::vector<std::string> GetConvertFiles();

private:

	std::vector<std::string> modifiedfiles_;
	
};


#include <exception>
#include <conio.h>
#include <queue>
#include <string>
#define Util StringHelper

using namespace CodeAnalysis;
using namespace FileSystem;
using namespace Lexer;
using namespace Utilities;
using Demo = Logging::StaticLogger<1>;

inline Publisher::Publisher()
{
}


Publisher::~Publisher() {};

std::vector<std::string> Publisher::getConvertedFiles(std::string Directory_, std::vector<std::string> Patterns_, std::vector<std::string> Regex_, bool hasS_) {

	DirExplorerN de(Directory_);
	for (auto patt : Patterns_)    //add patterns
	{
		de.addPattern(patt);
	}
	for (auto patt : Regex_)  //add regexes
	{
		de.addRegex(patt);
	}
	if (hasS_)
	{
		de.recurse();
	}
	de.search();
	de.showStats();
	Convert cvt(Directory_);
	Dependencies dep(de.getFilePaths());
	DependencyTable dt;
	for (auto fileSpec : de.getFilePaths()) {
		dep.addDependencies(dt, fileSpec);
	}
	std::cout << "\n\n===============================================================================";
	dt.display();
	std::cout << "\n\n===============================================================================";
	cvt.setDepTable(dt);
	cvt.createHTMLFiles(de.getFilePaths());
	std::cout << "\n The Display package support hide & show Comments, Functions, Classes " << std::endl;
	std::cout << "===============================================================================" << std::endl;
	Display dsp;
	dsp.modifyAllHTMLfiles(de.getFilePaths());
	modifiedfiles_ = dsp.getFileOutPath();

	return modifiedfiles_;
}