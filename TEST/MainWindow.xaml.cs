/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Code Publisher Client                      //
// ver 1.0                                                         //
// Author                                                          //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class MainWindow that provides tabs:
 * - Find Libs: Navigate through local directory to find files for testing
 *   - Shows local directories and files
 *   - Can navigate by double clicking directories
 *   - Can select files by selecting.  That displays selections in a popup window.
 * - Request Tests: left for students
 * - Show Results:  left for students
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * SelectionWindow.xaml, SelectionWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 4 Apr 2019
 * 
 */


using System;
using System.Collections.Generic;
using System.IO;
using System.Windows;
using System.Windows.Input;

namespace TEST
{
    public partial class MainWindow : Window
    {

        private string WDirectory { get; set; }
        private string patterns { get; set; }
        private string regex { get; set; }


        public MainWindow()
        {
            InitializeComponent();
        }


        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            //WDirectory = Directory.GetCurrentDirectory();
            //WDirectory = getAncestorPath(3, WDirectory);

            WDirectory = "../../";
            WDirectory = Path.GetFullPath(WDirectory);
            txtPath.Text = WDirectory;
            //patterns = "*.h *.cpp";
            //regex = "[A-B](.*)";
            //txtPatterns.Text = patterns;
            //txtRegex.Text = regex;
            //LoadNavTab(WDirectory);

        }
   
   


    }
}
