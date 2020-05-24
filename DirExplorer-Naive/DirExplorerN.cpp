/////////////////////////////////////////////////////////////////////
// DirExplorerN.cpp - Naive directory explorer                     //
// ver 1.2                                                         //
// Author: Wanxiang Xie, Syracuse University                       //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018 //
/////////////////////////////////////////////////////////////////////


#ifdef TEST_DIREXPLORERN  // onNly compile the following when defined
#include "DirExplorerN.h"
#include "../UtilitiesN/StringUtilities/StringUtilities.h"
#include "../UtilitiesN/CodeUtilities/CodeUtilities.h"

using namespace Utilities;
using namespace FileSystem;

int main(int argc, char *argv[])
{
  Title("Demonstrate DirExplorer-Naive, " + DirExplorerN::version());

  ProcessCmdLine pcl(argc, argv);
  preface("Command Line: ");
  pcl.showCmdLine();
  putline();

  if (pcl.parseError())
  {
    return 1;
  }

  DirExplorerN de(pcl.path());
  for (auto patt : pcl.patterns())
  {
    de.addPattern(patt);
  }

  if (pcl.hasOption('s'))
  {
    de.recurse();
  }

  de.search();
  de.showStats();
  std::cout << "\n\n";
  return 0;
}

#endif