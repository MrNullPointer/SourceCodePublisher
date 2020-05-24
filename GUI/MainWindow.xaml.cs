/////////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs : WPF Client that uses Shim to call native C++ code  //
// ver 2.2                                                                 //
// Application    : CSE-687 C++/CLI Shim demostration                      //
// Platform       : Visual Studio 2017 Community Edition                   //
//                  Windows 10 Professional 64-bit, Dell                   //
// Author         : Wanxiang Xie, Syracuse Univeristy                      //
// Email          : wxie15@syr.edu                                         //
/////////////////////////////////////////////////////////////////////////////
/*
 *  Package Decription:
 * =====================
 * This package uses the compiled MainWindow.xaml file to define the GUI
 * Using the work GUI eliminates the need to programmatically create GUIs.
 * Different from the previously dependent Shim project, now it will depend on the translater
 *  Required Files:
 * =================
 *  MainWindow.xaml MainWindow.xaml.cs Translater.dll PopupWindow.xaml
 * Package Operations:
 * ===================
 * Window_Loaded: start GUI and initialize the parameters.
 * TxtPatterns_KeyUp: change pattern through input.
 * TxtPath_TextChanged: change Directory.
 * BtnBrowse_Click: direclty choose the Directory.
 * LstFoldersAndFiles_MouseDoubleClick: goes into or goes back.
 * BtnConverter_Click: send CommandLine arguments into server.
 * BtnDisplayinBrowser_Click: receive the converted files and display the selected one
 * BtnDisplayinWindow_Click:receive the converted files and display the selected one
 *  
 *  Build Process:
 * ================
 *  msbuild WPF.csproj
 *  
 *  Maintainence History:
 * =======================
 *   ver 2.1 : 29 Apr 2019
 * - construct message and add auto test
 * 
 *   ver 2.0 : 25 Apr 2019
 * - construct message
 * - Supporting new format of communicating with remote server
 * 
 *   ver 1.0 : 2 April 2019
 * - modify the structure of GUI
 * - 
 *   ver 1.1 : 4 April 2019
 * - finish related click functions
 * - send
 *   ver 1.0 : 2 April 2019
 * - finish basic structure of GUI
 * - create related functions
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.IO;
using MsgPassingCommunication;
using System.Threading;



namespace GUI
{

    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        //path; patterns; regex;
        private Stack<string> pathStack_ = new Stack<string>();
        private string WDirectory { get; set; }
        private string Patterns { get; set; }
        private string Regexes { get; set; }
        private string BrowserPath { get; set; }
        private CsEndPoint clientEndPoint_;
        private CsEndPoint serverEndPoint_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();
        Translater translater;
        internal string saveFilesPath;

  
        private void setTxtPath(string path)
        {
            WDirectory = path;
            txtPath.Text = displayTxtPath(WDirectory);
            PostMsg("getDirs");
            PostMsg("getFiles");
        }
        /*------------< Double click actions to enter a directory >--------------*/
        private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (DirList.SelectedIndex == -1) return;
            string selectedItem = DirList.SelectedItem.ToString();
            if (selectedItem == null)
                return;
            string path;
            if (selectedItem == "..")
            {
                if (pathStack_.Count > 1)  // don't pop off "TestFiles"
                    pathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = pathStack_.Peek() + "/" + selectedItem;
                pathStack_.Push(path);
            }
     
            txtPath.Text = displayTxtPath(pathStack_.Peek());
            WDirectory = pathStack_.Peek();

            PostMsg("getDirs");
            PostMsg("getFiles");
        }
        /*---------------------------< change patterns >-------------------------*/
        private void TxtPatterns_TextChanged(object sender, TextChangedEventArgs e)
        {
            Patterns = txtPatterns.Text;
        }
        /*---------------------------< change Regex >-------------------------*/
        private void TxtRegex_TextChanged(object sender, TextChangedEventArgs e)
        {
            Regexes = txtRegexes.Text;
        }
        // ------------< post converting information to server (C++ Code Publisher in this solution) >--------------
        private void PostMsg(string command)
        {
            CsMessage msg = AssembleMsg();
            msg.add("to", CsEndPoint.toString(serverEndPoint_));
            msg.add("from", CsEndPoint.toString(clientEndPoint_));
            msg.add("command", command);
            Dispatcher.Invoke(() =>
            {
                txtProgress.Text = "Sent a Message: " + command + " to the server. Waiting server to reply.";
            });
            translater.postMessage(msg);
        }

        // -----------< Assemble message for posting >------------
        private CsMessage AssembleMsg()
        {
            CsMessage msg = new CsMessage();
            if (cbRecursion.IsChecked.HasValue)
            {
                msg.add("SubDir", "/s");
            }
            msg.add("Directory", WDirectory);
            msg.add("path", WDirectory);
            msg.add("BrowserPath", BrowserPath);
            msg.add("Pattern", Patterns);
            msg.add("Regex", Regexes);
            return msg;
        }
        //----< strip off name of first part of path >---------------------
        private string displayTxtPath(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }
        /*------< click button to send CommandLine & receive file path then display converted filenames in listbox >----*/
        private void BtnConverter_Click(object sender, RoutedEventArgs e)
        {
            lstConverted.Items.Clear();
            PostMsg("Publish");
        }


        //----< process incoming messages on child thread >----------------
        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    try
                    {
                        string msgId = msg.value("command");
                        Console.Write("\n  client getting message \"{0}\"", msgId);
                        Dispatcher.Invoke(() =>
                        {
                            txtProgress.Text = "Message from server got!";
                            if (dispatcher_.ContainsKey(msgId))
                                dispatcher_[msgId].Invoke(msg);
                        });
                    }
                    catch (Exception ex)
                    {
                        Console.Write("\n  {0}", ex.Message);
                        msg.show();
                    }
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }
        //----< load all dispatcher processing >---------------------------
        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherLoadGetConvertFiles();
        }


        //----< start Comm, fill window display with dirs and files >------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string[] args = Environment.GetCommandLineArgs();
            string[] clientURLArray = args[1].Split(':');
            string[] serverURLArray = args[2].Split(':');
            string clientURL = clientURLArray[0];
            string serverURL = serverURLArray[0];
            int clientPort = int.Parse(clientURLArray[1]);
            int serverPort = int.Parse(serverURLArray[1]);
            string remotePath;
            remotePath = args[3]; // the address of testfiles folder
            Console.WriteLine(remotePath);
            BrowserPath = " C:/Program Files(x86)/Google/Chrome/Application/chrome.exe";

            // start Comm
            clientEndPoint_ = new CsEndPoint();
            clientEndPoint_.machineAddress = clientURL;
            clientEndPoint_.port = clientPort;
            Console.WriteLine("Shwo client url: " + clientEndPoint_.machineAddress + clientEndPoint_.port);
            serverEndPoint_ = new CsEndPoint();
            serverEndPoint_.machineAddress = serverURL;
            serverEndPoint_.port = serverPort;
            Console.WriteLine("Show server url: " + serverEndPoint_.machineAddress + serverEndPoint_.port);

            //set variables with initial value
            pathStack_.Push(remotePath);
            WDirectory = pathStack_.Peek();
            Patterns = "*.h *.cpp";
            Regexes = "[A-C](.*)";
            txtPatterns.Text = Patterns;
            txtRegexes.Text = Regexes;

            translater = new Translater();
            translater.listen(clientEndPoint_);
            setTxtPath(WDirectory);

            // start processing messages
            processMessages();

            // load dispatcher
            loadDispatcher();

            // client files saving position
            saveFilesPath = translater.setSaveFilePath("../../../SaveFiles");

            //auto demo
            if (App.AutoDemo1 == true)
            {
                lstConverted.Items.Clear();
                PostMsg("Publish");
            }
            else if (App.AutoDemo2 == true)
            {
                txtRegexes.Text = "[P](.*)";
                lstConverted.Items.Clear();
                PostMsg("Publish");
            }
        }

        //----< add client processing for message with key >---------------
        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }
        //----< load getDirs processing into directory block >-------
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    DirList.Items.Clear();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            DirList.Items.Add(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action updateStatusBar = () =>
                {
                    DirList.Items.Insert(0, "..");
                    txtProgress.Text = "\"getDirs\" reply received!";
                };
                Dispatcher.Invoke(updateStatusBar, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }
        //----< load getFiles processing into files block >------
        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    FileList.Items.Clear();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            FileList.Items.Add(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
                Action updateStatusBar = () =>
                {
                    txtProgress.Text = "\"getFiles\" reply received!";
                };
                Dispatcher.Invoke(updateStatusBar, new Object[] { });
            };
            addClientProc("getFiles", getFiles);
        }
        //----< load converted files for displaying >------
        private void DispatcherLoadGetConvertFiles()
        {
            Action<CsMessage> convertFiles = (CsMessage rcvMsg) =>
            {
                Console.Write("\n  converted files received");
                var enumer = rcvMsg.attributes.GetEnumerator();
                int filesCount = 0;
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    Console.WriteLine(key);
                    Console.WriteLine(enumer.Current.Value);

                    if (key.Contains("sendingFile"))
                    {
                        Console.WriteLine(enumer.Current.Value);
                        filesCount++;
                        Action<string> doConverted = (string file) =>
                        {
                            string fileName = System.IO.Path.GetFileName(file);
                            lstConverted.Items.Add(fileName);
                            tabCtrl.SelectedIndex = 1;
                        };
                        Dispatcher.Invoke(doConverted, new Object[] { enumer.Current.Value });
                    }
                }
                Action updateStatusBar = () =>
                {
                    txtProgress.Text = filesCount + " files are converted and received!";
                };
                Dispatcher.Invoke(updateStatusBar, new Object[] { });
            };
            addClientProc("getPublish", convertFiles);
        }

        /*----------< display the converted html files in browser >---------*/
        private void BtnDisplayinBrowser_Click(object sender, RoutedEventArgs e)
        {
            if (lstConverted.SelectedIndex == -1) return;
            string selectedItem = lstConverted.SelectedItem.ToString();
            selectedItem = System.IO.Path.Combine(saveFilesPath, selectedItem);
            selectedItem = System.IO.Path.GetFullPath(selectedItem);
            Console.WriteLine(selectedItem);
            System.Diagnostics.Process.Start(selectedItem);
        }
        /*---------< display the converted html files in window >----------*/
        private void BtnDisplayinWindow_Click(object sender, RoutedEventArgs e)
        {
            if (lstConverted.SelectedIndex == -1) return;
            string selectedItem = lstConverted.SelectedItem.ToString();
            selectedItem = System.IO.Path.Combine(saveFilesPath, selectedItem);
            selectedItem = System.IO.Path.GetFullPath(selectedItem);
            string content = File.ReadAllText(selectedItem);
            PopupWindow popWin = new PopupWindow();
            popWin.codeView.Text = content;
            popWin.Show();
        }

        private void TabCtrl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void DirList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void FileList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
