///////////////////////////////////////////////////////////////////////////////////
// Convert.cpp  - convert files matching patterns and regexes into .HTML file    //
// ver 1.0                                                                       //
// Language:    C++, Visual Studio 2017					        				 //
// Application: Most Projects, CSE687 - Object Oriented Design                   //
// Author:      Wanxiang Xie, Syracuse University,						         //
//              wxie15@syr.edu											         //
///////////////////////////////////////////////////////////////////////////////////

#include "Convert.h"
#include <iostream>
#include <vector>


//----< test stub >--------------------------------------------------------

#ifdef TEST_CONVERT

int main(int argc, char* argv[])
{
	Utilities::Title("Demonstrate DirExplorer-Naive, " + DirExplorerN::version());

	std::cout << "\n Accept a path and a regular expression from the command line(meet the Requirements 4)" << std::endl;
	std::cout << "=======================================================" << std::endl;
	Utilities::ProcessCmdLine pcl(argc, argv);
	/*pcl.usage(customUsage());*/
	Utilities::preface("Command Line: ");
	pcl.showCmdLine();
	pcl.showRegexes();
	//Utilities::putline();
	if (pcl.parseError())
	{
		pcl.usage();
		std::cout << "\n\n";
		return 1;
	}
	DirExplorerN de(pcl.path());
	for (auto patt : pcl.patterns())    //add patterns
	{
		de.addPattern(patt);
	}
	for (auto patt : pcl.regexes())  //add regexes
	{
		de.addRegex(patt);
	}
	if (pcl.hasOption('s'))
	{
		de.recurse();
	}

	std::cout << "\n Show directories and files which have been found. " << std::endl;
	std::cout << "=======================================================" << std::endl;
	de.search();                  //search files matching patterns and regexes

	std::cout << "\n Show Stats: ";
	de.showStats();
	Convert cvt(pcl.path());     // put in dir:\DemoPackageConstruction

	std::cout << "\n Convert matching file text to a valid html file, and save to a folder named 'convertedPages'.\n  (meet the Requirements 6)" << std::endl;
	std::cout << "=======================================================" << std::endl;


	cvt.createHTMLFiles(de.getFilePaths());
	return 0;

}


#endif
