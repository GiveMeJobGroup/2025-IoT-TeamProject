using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;
using QRTestWpf.Helpers;
using QRTestWpf.Views;
using System.Windows.Controls;

namespace QRTestWpf.ViewModels
{
    public partial class MainViewModel : ObservableObject
    {
        #region 필드
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
        #endregion

        #region 메서드
        [RelayCommand]
        public void ShowMenu()
        {
            var viewModel = new MenuViewModel(Common.DIALOGCOORDINATOR, this);

            var view = new MenuView
            {
                DataContext = viewModel,
            };
            CurrentView = view;
        }

        [RelayCommand]
        public void ShowQRScan()
        {
            var viewModel = new QRScanViewModel(Common.DIALOGCOORDINATOR, this);
            
            var view = new QRScanView
            {
                DataContext = viewModel,
            };
            CurrentView = view;
        }
        #endregion
    }
}
