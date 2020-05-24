///////////////////////////////////////////////////////////////////////////
// Dependencies.cpp : defines Dependencies.h functions                   //
// ver 1.0                                                               //
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Wanxiang Xie. Syracuse University                     //
// contact       : Wxie15@syr.edu                                        //
///////////////////////////////////////////////////////////////////////////

#include "Dependencies.h"
using namespace Lexer;
using namespace Utilities;
using namespace FileSystem;
using namespace CodeAnalysis;

using Demo = Logging::StaticLogger<1>;
#define Util StringHelper

// -----< default ctor >---------------------------------------------------
Dependencies::Dependencies() {
	std::cout<<"\n  Created instance of Dependencies using default ctor\n";
}

// -----< files initializer ctor >-----------------------------------------
Dependencies::Dependencies(const files & list)
{
	std::cout << "\n  Created instance of Dependencies using ctor\n";
	list_ = list;
}
Dependencies::~Dependencies() {

}

Dependencies::files Dependencies::getDependencies(const file & file) {
		files depList_;
		std::string fileSpec = file;
		std::string msg = "Processing file" + fileSpec;
		Util::title(msg);
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
					return depList_;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return depList_;
			}

			// save current package name

			Repository* pRepo = Repository::getInstance();
			pRepo->package() = name;

			// parse the package

			while (pParser->next())
			{
				pParser->parse();
			}
			std::cout << "\n";

			// final AST operations
			ASTNode* pGlobalScope = pRepo->getGlobalScope();

			// walk AST, computing complexity for each node
			// and record in AST node's element

		  //!to get all of dependency 
			for (auto temp : pGlobalScope->statements_) {
				if (temp->show().find(".h") != std::string::npos) {
					std::string dep = temp->show().erase(0, 14);
					dep = dep.substr(1, dep.length() - 3);
					std::cout << dep << "\n";
					depList_.push_back(dep);
				}
			}
			//complexityEval(pGlobalScope);
			// Walk AST, displaying each element, indented
			// by generation
			//TreeWalk(pGlobalScope);
			
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
			std::cout << "\n  exception caught at line " << __LINE__ << " ";
			std::cout << "\n  in package \"" << name << "\"";
		}
		std::cout << "\n";
		return depList_;
}

void Dependencies::addDependencies(DependencyTable &dt,const std::string& fileSpec) {

		for (auto dep : getDependencies(fileSpec)) {
			std::string dname = FileSystem::Path::getName(dep);
			for (auto fpath : list_) {
				std::string fname = FileSystem::Path::getName(fpath);
				if (dname == fname)
					dt.addDependency(fileSpec, dep);
			}
		}
}


//----< test stub >--------------------------------------------------------
//
#ifdef TEST_DEPENDENCIES

int main(int argc, char* argv[]) {

	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}
	std::string fileSpec;

	for (int i = 1; i < argc; ++i)  // iterate over files
	{
		fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);    // fileSpec is an absolute path
		std::string msg = "Processing file" + fileSpec;
		Util::title(msg);
		Util::Title("Testing Dependencies");
		std::cout << "=======================================================" << std::endl;
		Dependencies dep;
		dep.addDependencies(fileSpec);
	}



}

#endif
