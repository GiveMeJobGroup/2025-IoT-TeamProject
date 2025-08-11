using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;

namespace QRTestWpf.ViewModels
{
    public partial class MenuViewModel : ObservableObject
    {
        private IDialogCoordinator DIALOGCOORDINATOR;
        
        private readonly MainViewModel _mainViewModel;
        public MenuViewModel(IDialogCoordinator DIALOGCOORDINATOR, MainViewModel mainViewModel)
        {
            this._mainViewModel = mainViewModel;
            this.DIALOGCOORDINATOR = DIALOGCOORDINATOR;
        }
        
        // 메뉴 -> 검색
        [RelayCommand]
        public void Search()
        {
            _mainViewModel.ShowSearch();
        }

        // 메뉴 -> 대출
        [RelayCommand]
        public void QRScan()
        {
            _mainViewModel.ShowQRScan();
        }

        // 메뉴 -> 반납
        [RelayCommand]
        public void Return()
        {
            _mainViewModel.ShowReturn();

        }

    }
}
