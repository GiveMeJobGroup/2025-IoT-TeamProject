using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;
using QRTestWpf.Helpers;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QRTestWpf.ViewModels
{
    public partial class SearchResultViewModel : ObservableObject
    {
        private IDialogCoordinator DIALOGCOORDINATOR;
        private readonly MainViewModel _mainViewModel;
        public SearchResultViewModel(IDialogCoordinator DIALOGCOORDINATOR, MainViewModel mainViewModel)
        {
            this._mainViewModel = mainViewModel;
            this.DIALOGCOORDINATOR = DIALOGCOORDINATOR;
        }

        // 이 밑에 그냥 아무생각없이 타이밍 했었는데 나중에 DB 만들고나서 데이터 들고오고 나서 하는게 나을 것 같음
        // 나중에 DB 만들고 나서 할것.

        //private string _bookItem;

        //public string BookItem
        //{
        //    get => _bookItem;
        //    set => SetProperty(ref _bookItem, value);
        //}

        //private ObservableCollection<BookItem> _bookItems;
        //public ObservableCollection<BookItem> BookItems
        //{
        //    get => _bookItems;
        //    set => SetProperty(ref _bookItems, value);
        //}

        //private BookItem _selectedBookItem;
        //public BookItem SelectedBookItem
        //{
        //    get => _selectedBookItem;
        //    set
        //    {

        //    }
        //}


        [RelayCommand]
        public void BookItemDoubleClick()
        {
            _mainViewModel.ShowDirectionOX();
        }

        [RelayCommand]
        public void check()
        {
            _mainViewModel.ShowDirectionOX();
        }

    }

}
