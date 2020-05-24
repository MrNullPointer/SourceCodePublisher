#pragma once
/////////////////////////////////////////////////////////////////////
//  Dependency.h  - help to find file dependecies                  //
//  ver 1.0                                                        //
//  Language:      Visual C++, Visual Studio 2015                  //
//  Platform:      Dell XPS 8920, Windows 10                       //
//  Application:   Prototype for CSE687 - OOD Projects             //
//  Author:        Wanxiang Xie,  Syracuse University              //
/////////////////////////////////////////////////////////////////////
/*
*  Package Responsibilities:
* ---------------------------
*  Display package support wrapping selected parts of the web page in <div> blocks
*  that can be displayed or hidden. 
*  The parts to be selected are:
*  Comment blocks at the begining of each source code file, if they exist
*  Function bodies, including the opening and closing braces
*  Class bodies, including the opening and closing braces
* ---------------------------
*  Package Operations:
* ---------------------------
*  Dependencies package shall support insertion, in each analyzed file,
*  links to all the files in the directory tree, on which it depends.
* ---------------------------
*  Required files:
* - FileSystem.h, FileSystem.cpp,
* ----------------------
*  Maintenance History:
* ----------------------
* ver 1.0 : 12 Mar 2019
*  - first release
*
*/
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <conio.h>
#include "../FileSystem/FileSystem.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"
#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../UtilitiesN/StringUtilities/StringUtilities.h"
#include "../UtilitiesN/CodeUtilities/CodeUtilities.h"

class Display
{
public:
	Display();
	~Display();

	void hideComments(std::string & line);
	void hideClasses(std::string& line, const unsigned int & count);
	void hideFunctions(std::string& line, const unsigned int & count);
	void addButton(std::string & line);
	void codeAnalysis(const std::string & path);

	void CodeWalk(CodeAnalysis::ASTNode* pItem, bool details);

	//void doDisplay(std::string appPath, std::vector<std::string> fileOutPaths);

	void doDisplay(std::string appPath);

	void modifyHTMLfile(const std::string & path);

	std::vector<std::string> getFileOutPath();

	void modifyAllHTMLfiles(const std::vector<std::string>& paths);

	

private:
	std::vector <size_t> class_startLineCount_;
	std::vector <size_t> function_startLineCount_;
	std::vector <size_t> class_endLineCount_;
	std::vector <size_t> function_endLineCount_;
	std::vector<std::string>  fileOutPath_;
	std::vector<std::string> fileNames_;
};

