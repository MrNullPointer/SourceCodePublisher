///////////////////////////////////////////////////////////////////////////////////
// DisplayProcess.h  -  Display the converted .HTML pages  pop-up in browser     //
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
//* Display.h
//* Convert.h
//*  Maintenance History :
//*----------------------
//*  ver 1.0 : 12 Mar 2019
//* -first release


#include "DisplayProcess.h"
#include "../Convert/Convert.h"


//----< test stub >--------------------------------------------------------

#ifdef TEST_DISPLAYPROCESS

int main(int argc, char* argv[])
{

	std::cout << "\n Accept a path and a regular expression from the command line" << std::endl;
	std::cout << "=======================================================" << std::endl;
	Utilities::ProcessCmdLine pcl(argc, argv);
	Utilities::preface("Command Line: ");
	pcl.showCmdLine();
	pcl.showRegexes();
	Utilities::putline();
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

	de.search();                  //search files matching patterns and regexes
	Convert cvt(pcl.path());     // put in dir:\DemoPackageConstruction
	cvt.createHTMLFiles(de.getFilePaths());

	std::cout << " display each converted file using browser,waiting for the child to exit before displaying the next file.\n  (meet the Requirements 7)" << std::endl;
	std::cout << "=======================================================" << std::endl;
	Display dsp(de.getFilePaths());
	std::string appPath;
	FileSystem::File app("../AppPath.txt");
	app.open(FileSystem::File::in, FileSystem::File::text);

	if (app.isGood()) {
		appPath = app.getLine();
	}
	else std::cout << "\nPlease add application path in AppPath.txt file!" << std::endl;

	app.close();

	if (de.getFilePaths().size() != 0) {
		dsp.doDisplay(appPath);		     //display files by browser
	}
	else
		std::cout << "\n\nHave no matching files to display!" << std::endl;
	return 0;
}

#endif