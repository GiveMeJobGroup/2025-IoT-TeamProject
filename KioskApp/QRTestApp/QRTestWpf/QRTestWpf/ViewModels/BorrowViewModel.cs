using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;
using QRTestWpf.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QRTestWpf.ViewModels
{
    public partial class BorrowViewModel : ObservableObject
    {

        private IDialogCoordinator DIALOGCOORDINATOR;

        private readonly MainViewModel _mainViewModel;

        public BorrowViewModel(IDialogCoordinator DIALOGCOORDINATOR, MainViewModel mainViewModel)
        {

            this._mainViewModel = mainViewModel;
            this.DIALOGCOORDINATOR = DIALOGCOORDINATOR;

            // 확인용 데이터
            RetrunDay = "2025.05.02";
        }

        #region 필드
        private string _retrunDay;

        public string RetrunDay
        {
            get => _retrunDay;
            set => SetProperty(ref _retrunDay, value);
        }
        #endregion

        #region 메소드
        [RelayCommand]
        public void Thx()
        {
            _mainViewModel.ShowMenu();  // 메뉴 화면으로 복귀
        }

        #endregion

    }

}

