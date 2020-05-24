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
*  Demonstrate how to call all the function together as a glue
* ---------------------------
*  Package Operations:
* ---------------------------
*  Dependencies package shall support insertion, in each analyzed file,
*  links to all the files in the directory tree, on which it depends.
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
* ----------------------
*  Maintenance History:
* ----------------------
* ver 1.0 : 12 Mar 2019
*  - first release
*
*/




#include <string>
#include <vector>
#include <iostream>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"
#include "../Convert/Convert.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../UtilitiesN/StringUtilities/StringUtilities.h"
#include "../UtilitiesN/CodeUtilities/CodeUtilities.h"
#include "../DependencyTable/DependencyTable.h"
#include "../FileSystem/FileSystem.h"
#include <queue>



class Dependencies
{
public:
	using file = std::string;
	using files = std::vector<file>;
	using dependencies = std::vector<file>;
	Dependencies();
	Dependencies(const files& list);
	files getDependencies(const file & file);
	void addDependencies(DependencyTable & dt, const std::string & fileSpec);
	~Dependencies();

private:
	files list_;


};
