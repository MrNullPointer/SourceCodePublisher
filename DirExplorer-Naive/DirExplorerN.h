#pragma once
/////////////////////////////////////////////////////////////////////
// DirExplorerN.h - Naive directory explorer                       //
// ver 1.5                                                         //
// Author: Wanxiang Xie, Syracuse University                       //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018 //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * DirExplorerN provides a class, of the same name, that executes a
 * depth first search of a directory tree rooted at a specified path.
 * Each time it enters a directory, it invokes its member function
 * doDir, and for every file in that directory, it invokes doFile.
 *
 * We call this a "Naive Directory Explorer" because, while it carries
 * out its assigned responsibilities well, there is no way to change
 * what its doDir and doFile functions do, without changing the class
 * itself.  It would be much better to provide a mechanism to allow
 * a using application to supply this processing without changing
 * the Directory Navigator. 
 *
 * Other projects in the DirectoryNavigator folder in Repository/Cpp 
   do just that, in different ways.  

 * - DirExplorer-Naive (this project):
 *     Implements basic processing well, but applications have to
 *     change its code to affect how files and directories are
 *     handled.
 * - DirExplorer-Template:
 *     Applications provide template class parameters to define file
 *     and directory processing.
 * - DirExplorer-Inheritance:
 *     Applications subclass DirExplorerI, overriding virtual functions
 *     doFile, doDir, and doQuit to define how files and directories
 *     are handled, and whether processing should terminate before
 *     visiting all directories.
 * - DirExplorer-Events:
 *     Applications use Event Interfaces, published by DirExplorerE,
 *     by subscribing event-handler functions, and control terminate
 *      processing using a plug-in interface.
 * - DirExplorer-Provider:
 *     Applications implement a file system provider that implements
 *     a Provider interface published by DirExplorerP.
 *     
 * We'll be using the Repository/Cpp solution to illustrate techniques 
 * for building flexible software.
 *
 * Required Files:
 * ---------------
 * DirExplorerN.h, DirExplorerN.cpp
 * FileSystem.h, FileSystem.cpp      // Directory and Path classes
 * StringUtilities.h                 // Title function
 * CodeUtilities.h                   // ProcessCmdLine class
 *
 * Maintenance History:
 * --------------------
 * ver 1.5 : 6 Feb 2019
 * - Add function to use specified regexs for selecting file names.
 * ver 1.4 : 24 Jan 2019
 * - Removed all projects except those needed for DirExplorerN.
 * ver 1.3 : 19 Aug 2018
 * - Removed some options to make this version simple.  Those are
 *   implemented in the more advanced navigators, presented here.
 * ver 1.2 : 17 Aug 2018
 * - Moved method definitions to inlines, below the class declaration.
 * ver 1.1 : 16 Aug 2018
 * - Made no recursion default, added option /s for recursion.
 * ver 1.0 : 11 Aug 2018
 * - first release
 *
*/
#include <vector>
#include <iostream>
#include <regex>
#include "../FileSystem/FileSystem.h"

namespace FileSystem
{
	class DirExplorerN
	{
	public:
		using patterns = std::vector<std::string>;
		using regexs = std::vector<std::string>;
		static std::string version() { return "ver 1.5"; }

		DirExplorerN(const std::string& path);

		// set options for analysis

		void addPattern(const std::string& patt);
		void addRegex(const std::string& patt);
		void recurse(bool doRecurse = true);

		// conduct depth first search

		void search();
		void find(const std::string& path);

		// define what happens when a file or dir is encountered

		void doFile(const std::string& filename);
		void doDir(const std::string& dirname);

		// extract traversal statistics 

		size_t fileCount();
		size_t dirCount();
		void showStats();
		std::vector<std::string> getFileNames();
		std::vector<std::string> getFilePaths();

	private:

		std::string path_;
		patterns patterns_;
		regexs regexes_;
		std::vector<std::string> fileNames_;
		std::vector<std::string> filePaths_;
		size_t dirCount_ = 0;
		size_t fileCount_ = 0;
		bool recurse_ = false;
		patterns filterFiles_;

	};

	//----< construct DirExplorerN instance with default pattern >-----

	inline DirExplorerN::DirExplorerN(const std::string& path) : path_(path)
	{
		patterns_.push_back("*.*");
	}
	//----< add specified patterns for selecting file names >----------

	inline void DirExplorerN::addPattern(const std::string& patt)
	{
		if (patterns_.size() == 1 && patterns_[0] == "*.*")
			patterns_.pop_back();
		patterns_.push_back(patt);
	}
	//----< add specified regexs for selecting file names >----------

	inline void DirExplorerN::addRegex(const std::string& patt)
	{
		if (regexes_.size() == 1 && regexes_[0] == "*.*")
			regexes_.pop_back();
		regexes_.push_back(patt);
	}
	//----< set option to recusively walk dir tree >-------------------

	inline void DirExplorerN::recurse(bool doRecurse)
	{
		recurse_ = doRecurse;
	}
	//----< start Depth First Search at path held in path_ >-----------

	inline void DirExplorerN::search()
	{
		find(path_);

		if (regexes_.size() == 0) {
			std::cout << "\n\n have no regexes, found Files matching patterns above!" << std::endl;
		}
		else {
			if (filterFiles_.size() == 0)
				std::cout << "\n\n Files matching Regexes are not Found above!\n (meet the Requirements 2)" << std::endl;
			else
				std::cout << "\n\n Files matching Regexes are Found above!" << std::endl;
		}
	}
	//----< search for directories and their files >-------------------
	/*
	  Recursively finds all the dirs and files on the specified path,
	  executing doDir when entering a directory and doFile when finding a file
	*/
	inline void DirExplorerN::find(const std::string& path)
	{
		// show current directory

		std::string fpath = FileSystem::Path::getFullFileSpec(path);
		doDir(fpath);

		for (auto patt : patterns_)
		{
			std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt); //!files: names of all the files matching pattern

			for (auto f : files)
			{
				if (regexes_.size() == 0) {             //if have no regexes, match patterns
					doFile(f);
					filePaths_.push_back(fpath +"\\"+ f);		
				}
				else {									//if have regexes, match regexes
					for (auto regexes : regexes_) {        
						if (std::regex_match(f, std::regex(regexes))) {
							filterFiles_.push_back(f);
							doFile(f);  // show each file in current directory
							filePaths_.push_back(fpath +"\\"+f);
							}
				
						}

					}
			}
			
		}

		std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
		for (auto d : dirs)
		{
			if (d == "." || d == "..")
				continue;
			std::string dpath = fpath + "\\" + d;
			if (recurse_)
			{
				find(dpath);   // recurse into subdirectories
			}
			else
			{
				doDir(dpath);  // show subdirectories
			}
		}
	}
	//----< an application changes to enable specific file ops >-------

	inline void DirExplorerN::doFile(const std::string& filename)
	{
		++fileCount_;
		std::cout << "\n  --   " << filename;

		fileNames_.push_back(filename);
		
	}
	//----< an application changes to enable specific dir ops >--------

	inline void DirExplorerN::doDir(const std::string& dirname)
	{
		++dirCount_;
		std::cout << "\n  ++ " << dirname;
	}
	//----< return number of files processed >-------------------------

	inline size_t DirExplorerN::fileCount()
	{
		return fileCount_;
	}
	//----< return number of directories processed >-------------------

	inline size_t DirExplorerN::dirCount()
	{
		return dirCount_;
	}
	//----< show final counts for files and dirs >---------------------

	inline void DirExplorerN::showStats()
	{
		std::cout << " processed " << fileCount_ << " files in " << dirCount_ << " directories" << std::endl;
	}

	//----< get file names matching patterns and regexes >---------------------

	inline std::vector<std::string> DirExplorerN::getFileNames() {
		
		return fileNames_;
	}

	inline std::vector<std::string> DirExplorerN::getFilePaths() {

		return filePaths_;
	}

}

