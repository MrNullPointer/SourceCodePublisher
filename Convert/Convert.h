#pragma once
///////////////////////////////////////////////////////////////////////////////////
// Convert.h  - convert files matching patterns and regexes into .HTML file      //
// ver 1.1                                                                       //
// Language:    C++, Visual Studio 2017					        		         //
// Application: Most Projects, CSE687 - Object Oriented Design                   //
// Author:      Wanxiang Xie, Syracuse University,						         //
//              wxie15@syr.edu											         //
///////////////////////////////////////////////////////////////////////////////////

// Package Operations :
// --------------------
// This file including function to create HTML files with names matching patterns and regexes.dui
// Convert html fragment into a valid html document by insert Webpage Template.
// Replace specific markeup sign to which HTML could distinguish.
// Required Files :
// ===============
// DirExplorerN.h
// StringUtilities.h
// CodeUtilities.h
//  Maintenance History :
// ----------------------
// ver 1.1 : add function to addDependencyLinks




#include <iostream>
#include <vector>
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../UtilitiesN/StringUtilities/StringUtilities.h"
#include "../UtilitiesN/CodeUtilities/CodeUtilities.h"
#include <fstream>
#include "../DependencyTable/DependencyTable.h"



using namespace FileSystem;

namespace FileSystem {
	class Convert {
	public:
		using patterns = std::vector<std::string>;
		Convert(std::string);
		~Convert();
		void replaceSpecMarkup(std::string & str);
		void TransToHTML(std::string path, FileSystem::File & outfile);
		void createHTMLFiles(const std::vector<std::string>& fpath);
		void setDepTable(const DependencyTable & dt);
		void addDependencyLinks(std::string file, FileSystem::File & outfile);
		std::vector<std::string> getFileOutPaths();
		std::vector<std::string> getFileNames();

	private:
		std::string directory_;
		std::vector<std::string> fileOutPath_;
		std::vector<std::string> fileNames_;

		DependencyTable dt_;
		
	};

	inline Convert::Convert(std::string directory) {
		directory = directory_;
	}

	inline Convert::~Convert() {
	}


	//----< Replace specific markeup sign to which HTML could distinguish >---------------------

	inline void Convert::replaceSpecMarkup(std::string& str) {
		for (size_t i = 0; i < str.length(); i++) {
			if (str[i] == '<') {
				str.replace(i, 1, "&lt;");
				i += 1;
			}
			if (str[i] == '>') {
				str.replace(i, 1, "&gt;");
				i += 1;
			}
			if (str[i] == '&') {
				str.replace(i, 1, "&amp;");
				i += 1;
			}
			if (str[i] == '"') {
				str.replace(i, 1, "&quot;");
				i += 1;
			}
		}
	}


	// -----< set dependency table function >---------------------------------

	inline void Convert::setDepTable(const DependencyTable & dt)
	{
		dt_ = dt;
	}

	// -----< add dependency links to the html file >---------------------------------

	inline void Convert::addDependencyLinks(std::string file, FileSystem::File &outfile)
	{
		std::string filename = Path::getName(file);
		if (!dt_.has(file)) { // in case of single file conversion
			std::cout << "\n    No entry found in DependencyTable for [" + filename + "]. Skipping dependency links..";
			return;
		}

		if (dt_[file].size() == 0) { // in case the file has no dependencies
			std::cout << "\n    No dependencies found for [" + filename + "]. Skipping dependency links..";
			return;
		}

		for (auto dep : dt_[file]) {
					outfile.putLine("<a href=\"", false);
					outfile.putLine(Path::getName(dep), false);
					outfile.putLine(".html\">", false);
					outfile.putLine(Path::getName(dep), false);
					outfile.putLine("</a>", true);
				}
	}


	//----< convert html fragment into a valid html document >---------------------

	inline void Convert::TransToHTML(std::string path, FileSystem::File &outfile) {
			FileSystem::File infile(path);
			infile.open(FileSystem::File::in, FileSystem::File::text);

			FileSystem::File ftmp("../WebpageTemplate.txt");
			ftmp.open(FileSystem::File::in, FileSystem::File::text);
			while (ftmp.isGood()) {
				std::string line = ftmp.getLine();
				if (line == "<pre>") {
					addDependencyLinks(path, outfile);
				}
	
				if (line == "</pre>") {
					while (infile.isGood()) {
						std::string line = infile.getLine();
						replaceSpecMarkup(line);
						outfile.putLine(line);
					}
					infile.close();
				}
				outfile.putLine(line);  
			}
			ftmp.close();
	}


	//----< create HTML files with names matching patterns and regexes >---------------------

	inline void Convert::createHTMLFiles(const std::vector<std::string>& fpath) {

		std::string dir("../ConvertedWebpages/");
		if (!FileSystem::Directory::exists(dir)) 
			FileSystem::Directory::create(dir);				 //generate .htmls file in ../ConvertedWebpages/ dirctory	
		for (auto path : fpath) {
			std::string fileName = Path::getName(path) + ".html";
			std::string fileOutPath = dir + "/" + fileName;  
			fileOutPath_.push_back(fileOutPath);
			fileNames_.push_back(fileName);
			FileSystem::File outfile(fileOutPath);    			//create empty .html files by path
			outfile.open(FileSystem::File::out, FileSystem::File::text);
			//std::cout << "\n Create file in ConvertedWebpages folder: " << fileName;
			TransToHTML(path, outfile);						 //write content in .html file
			outfile.close();
			}
		/*std::cout << "\n Use the standard streams libraries for all I/O and operators new and delete for memory management.\n (meet the Requirements 2)" << std::endl;
		*/
		}	


	inline std::vector<std::string> Convert::getFileOutPaths() {
		return fileOutPath_;
	}

	inline std::vector<std::string> Convert::getFileNames() {
		return fileNames_;
	}
}
