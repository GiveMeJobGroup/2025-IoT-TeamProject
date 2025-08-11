using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;
using MySql.Data.MySqlClient;

namespace QRTestWpf.ViewModels
{
    public partial class SearchViewModel :ObservableObject
    {
        private IDialogCoordinator DIALOGCOORDINATOR;

        private readonly MainViewModel _mainViewModel;

        public SearchViewModel(IDialogCoordinator DIALOGCOORDINATOR, MainViewModel mainViewModel)
        {
            this._mainViewModel = mainViewModel;
            this.DIALOGCOORDINATOR = DIALOGCOORDINATOR;
        }

        [RelayCommand]
        public void SearchBook()
        {
            //_mainViewModel.SearchBook();
        }
    }
}
