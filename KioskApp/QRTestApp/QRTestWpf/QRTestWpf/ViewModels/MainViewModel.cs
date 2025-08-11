using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;
using QRTestWpf.Helpers;
using QRTestWpf.Views;
using System.DirectoryServices;
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

        // 빈화면에서 Menu화면이 뜨게 하는 부분
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

        // 메뉴 -> 검색
        [RelayCommand]
        public void ShowSearch()
        {
            var viewModel = new SearchViewModel(Common.DIALOGCOORDINATOR, this);

            var view = new SearchView
            {
                DataContext = viewModel,
            };
            CurrentView = view;
        }

        // 검색화면에서 검색 클릭
        //[RelayCommand]
        //public void SearchBook()
        //{
        //    var viewModel = new SearchResultViewModel(Common.DIALOGCOORDINATOR, this);

        //    var view = new SearchResultView
        //    {
        //        DataContext = viewModel,
        //    };
        //    CurrentView = view;
        //}

        // Menu -> 대출
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

        // Menu -> 반남
        [RelayCommand]
        public void ShowReturn()
        {



        }
        #endregion
    }
}
