using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace GUI
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static bool AutoDemo1 = false;
        public static bool AutoDemo2 = false;
        public static string[] Args;

        // -----------< receive cmd to run WPF >-------------
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            if (e.Args.Length > 0)
            {
                Args = e.Args;
                if (e.Args[3] == "AutoDemo1") {
                    AutoDemo1 = true;
                };
                if (e.Args[3] == "AutoDemo2")
                {
                    AutoDemo2 = true;
                };
                Console.WriteLine("Following commendLine received: ");
                foreach (var arg in Args)
                {
                    Console.Write(" " + arg);
                }
                Console.WriteLine();
            }
        }
    }
}
