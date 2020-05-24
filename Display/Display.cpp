///////////////////////////////////////////////////////////////////////////
// Display.cpp : defines Display.h functions			                 //
// ver 1.0                                                               //
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Wanxiang Xie. Syracuse University                     //
// contact       : Wxie15@syr.edu                                        //
///////////////////////////////////////////////////////////////////////////

#include "Display.h"
using namespace Lexer;
using namespace Utilities;
using namespace FileSystem;
using namespace CodeAnalysis;


Display::Display()
{
}

Display::~Display()
{
}

// -----< function to hide comments >---------------------------------

 void Display::hideComments(std::string& line) {

	if (line.find("//") != std::string::npos) {
		line.insert(line.find("//"), "<div class = \"comments\">");
		line.insert(line.length(), "</div>");
	}
	if (line.find("/*") != std::string::npos)
		line.insert(line.find("/*"), "<div class = \"comments\">");
	if (line.find("*/") != std::string::npos)
		line.insert(line.find("*/") + 2, "</div>");
}


 // -----< function to hide classes >---------------------------------

 void Display::hideClasses(std::string& line, const unsigned int & count) {
	 for (auto lineCount : class_startLineCount_) {
		 if (count == lineCount)
			 line.insert(line.find("{"), "<div class = \"class\">");
	 }
	 for (auto lineCount : class_endLineCount_) {
		 if (count == lineCount)
			 line.insert(line.find("}") + 1, "</div>");
	 }
 }

 // -----< function to hide comments >---------------------------------


 void Display::hideFunctions(std::string& line, const unsigned int & count) {
	 for (auto lineCount : function_startLineCount_) {
		 if (count == lineCount)
			 line.insert(line.find("{"), "<div class = \"function\">");
	 }
	 for (auto lineCount : function_endLineCount_) {
		 if (count == lineCount)
			 line.insert(line.find("}") + 1, "</div>");
	 }
 }

 //----< modify html file with hide/show function >---------------------

 void Display::modifyHTMLfile(const std::string & path) {
	 FileSystem::File infile(path);
	 infile.open(FileSystem::File::in, FileSystem::File::text);
	 std::string temp;
	 unsigned int count = 100000;
	 while (infile.isGood()) {

		 try
		 {
			 std::string line = infile.getLine();
			 if (line == "<pre>")
			 {
				 count = 0;
			 }
			 //addButton(line);
			 hideComments(line);
			 hideClasses(line, count);
			 hideFunctions(line, count);

			 temp = temp + line + "\n";
			 count++;
		 }
		 catch (std::out_of_range & exc) {}
	 }

	 FileSystem::File outfile(path);
	 outfile.open(FileSystem::File::out, FileSystem::File::text);
	 outfile.putLine(temp);
	 outfile.close();
	 //std::cout << "\n Modify " << path << " in ConvertedWebpages folder success\n";
	 class_startLineCount_.clear();
	 function_startLineCount_.clear();
	 class_endLineCount_.clear();
	 function_endLineCount_.clear();

 }


 std::vector<std::string> Display::getFileOutPath() {
	 return  fileOutPath_;
 }

 void Display::modifyAllHTMLfiles(const std::vector<std::string>& paths) {
		 std::string dir("../ConvertedWebpages/");
		 if (!FileSystem::Directory::exists(dir))
			 std::cout << "Not found dir.";
		 for (auto path : paths) {
			 std::string fileName = Path::getName(path) + ".html";
			 std::string fileOutPath = "../ConvertedWebpages/" + fileName;
			 fileOutPath_.push_back(fileOutPath);
			 fileNames_.push_back(fileName);
			 codeAnalysis(path);
		/*	 std::cout << "\n Modify file in ConvertedWebpages folder: " << fileOutPath;*/
			 modifyHTMLfile(fileOutPath);						 //write content in .html file
		 }
		 std::cout << "\n Modify all the files in ConvertedWebpages folder success.\n";

 }

 // -----< function to add buttons on the website >---------------------------------

 void Display::addButton(std::string& line) {
		 std::string button;
		 button += "<div>\n";
		 button += "<button onclick = \"toggleVisibility('comments')\">CommentsHidden</button>\n";
		 button += "<button onclick = \"toggleVisibility('class')\">ClassesHidden</button>\n";
		 button += "<button onclick = \"toggleVisibility('function')\">FunctionsHidden</button>\n";
		 button += "</div>\n";
		 if (line == "<label>") {
			 line.insert(0, button);
		 }
 }

 // -----< function to show type startLineCount endLineCount for each file>---------------------------------

 void Display::codeAnalysis(const std::string & path) {
	 std::string fileSpec = path;
	 ConfigParseForCodeAnal configure;
	 Parser* pParser = configure.Build();
	 std::string name;
	 try
	 {
		 if (pParser)
		 {
			 name = FileSystem::Path::getName(fileSpec);
			 if (!configure.Attach(fileSpec))
			 {
				 std::cout << "\n  could not open file " << name << std::endl;
				 //return ;
			 }
		 }
		 else
		 {
			 std::cout << "\n\n  Parser not built\n\n";
			 //return ;
		 }

		 // save current package name

		 Repository* pRepo = Repository::getInstance();
		 pRepo->package() = name;

		 // parse the package

		 while (pParser->next())
		 {
			 pParser->parse();
		 }
	/*	 std::cout << "\n";*/

		 // final AST operations
		 ASTNode* pGlobalScope = pRepo->getGlobalScope();

		 for (auto temp : class_startLineCount_) {
			std::cout << temp << "***";
		}
		for (auto temp : class_endLineCount_) {
			std::cout << temp << "***";
		}
		for (auto temp : function_startLineCount_) {
			std::cout << temp << "***";
		}
		for (auto temp : function_endLineCount_) {
			std::cout << temp << "***";
		}

		 // Walk AST, displaying each element, indented
		 // by generation
		 CodeWalk(pGlobalScope, false);

	 }
	 catch (std::exception& ex)
	 {
		 std::cout << "\n\n    " << ex.what() << "\n\n";
		 std::cout << "\n  exception caught at line " << __LINE__ << " ";
		 std::cout << "\n  in package \"" << name << "\"";
	 }
	 //std::cout << "\n";

 }

 
 void Display::CodeWalk(ASTNode* pItem, bool details = false)
 {
	 //std::cout <<  pItem->type_ << ":" << pItem->startLineCount_ << "->" << pItem->endLineCount_ << "!\n";
	 if (pItem->type_ == "class") {
		 class_startLineCount_.push_back(pItem->startLineCount_);
		 class_endLineCount_.push_back(pItem->endLineCount_);
	 }
	 if (pItem->type_ == "function") {
		 function_startLineCount_.push_back(pItem->startLineCount_);
		 function_endLineCount_.push_back(pItem->endLineCount_);
	 }
	 static std::string path;
	 auto iter = pItem->children_.begin();
	 while (iter != pItem->children_.end())
	 {
		 CodeWalk(*iter);
		 ++iter;
	 }
 }





 //----< test stub >--------------------------------------------------------
 //
#ifdef TEST_DISPLAY

 int main(int argc, char* argv[])
 {

	 //std::vector<std::string> fileOutPaths;
	 Display dsp;

	 dsp.codeAnalysis("../UtilitiesN/TestUtilities/TestUtilities.h");
	 dsp.modifyHTMLfile("../ConvertedWebpages/TestUtilities.h.html");
	 //dsp.modifyAllHTMLfiles(fileOutPaths);
	 return 0;
 }


#endif
