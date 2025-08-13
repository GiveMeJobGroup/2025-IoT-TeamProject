using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;


namespace QRTestWpf.ViewModels
{
    public partial class AfterScanViewModel : ObservableObject
    {
        private IDialogCoordinator DIALOGCOORDINATOR;

        private readonly MainViewModel _mainViewModel;
        public AfterScanViewModel(IDialogCoordinator DIALOGCOORDINATOR, MainViewModel mainViewModel)
        {
            this._mainViewModel = mainViewModel;
            this.DIALOGCOORDINATOR = DIALOGCOORDINATOR;

            // 데이터 확인용으로 사용 
            // TODO : DB 로 가져와야함
            UserName = "홍길동";
            StatusMessage = "책을 인식해주세요";
        }

        #region 필드
        private string _userName;

        public string UserName
        {
            get => _userName;
            set => SetProperty(ref _userName, value);
        }

        private string _statusMessage;

        public string StatusMessage
        {
            get => _statusMessage;
            set => SetProperty(ref _statusMessage, value);
        }

        #endregion

        [RelayCommand]
        public void BorrowOX()
        {
            _mainViewModel.ShowBorrowOX();
        }




    }
}


