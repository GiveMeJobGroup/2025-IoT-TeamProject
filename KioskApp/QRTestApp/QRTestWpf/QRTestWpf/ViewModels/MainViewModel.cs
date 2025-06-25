using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;
using QRTestWpf.Helpers;
using QRTestWpf.Views;
using System.Windows.Controls;
using System.Windows;

namespace QRTestWpf.ViewModels
{
    public partial class MainViewModel : ObservableObject
    {
        private readonly IDialogCoordinator dialogCoordinator;

        private UserControl _currentView;

        public MainViewModel(IDialogCoordinator coordinator)
        {
            this.dialogCoordinator = coordinator;

            ShowMenu();
        }

        public UserControl CurrentView
        {
            get => _currentView;
            set => SetProperty(ref _currentView, value);
        }

        [RelayCommand]
        public void ShowMenu()
        {
            var viewModel = new MenuViewModel(Common.DIALOGCOORDINATOR);

            var view = new MenuView
            {
                DataContext = viewModel,
            };
            CurrentView = view;
        }
        [RelayCommand]
        public void ShowQRScan()
        {
            MessageBox.Show("대출 열기");
            var viewModel = new QRScanViewModel(Common.DIALOGCOORDINATOR);
            
            var view = new QRScanView
            {
                DataContext = viewModel,
            };
            CurrentView = view;
        }
    }
}
