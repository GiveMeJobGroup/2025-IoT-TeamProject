using CommunityToolkit.Mvvm.ComponentModel;
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
        }


    }
}
