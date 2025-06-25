using MahApps.Metro.Controls.Dialogs;
using QRTestWpf.Helpers;
using QRTestWpf.ViewModels;
using QRTestWpf.Views;
using System.Configuration;
using System.Data;
using System.Windows;

namespace QRTestWpf
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            Common.DIALOGCOORDINATOR = DialogCoordinator.Instance;

            var viewModel = new MainViewModel(Common.DIALOGCOORDINATOR);
            var view = new MainView
            {
                DataContext = viewModel,
            };

            view.ShowDialog();
        }

    }

}
