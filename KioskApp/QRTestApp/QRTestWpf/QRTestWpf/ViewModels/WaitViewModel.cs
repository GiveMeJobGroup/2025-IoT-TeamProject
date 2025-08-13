using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QRTestWpf.ViewModels
{
    public partial class WaitViewModel : ObservableObject
    {
        private readonly MainViewModel _mainViewModel;
        private IDialogCoordinator DIALOGCOORDINATOR;

        public WaitViewModel(IDialogCoordinator DIALOGCOORDINATOR, MainViewModel mainViewModel)
        {
            this._mainViewModel = mainViewModel;
            this.DIALOGCOORDINATOR = DIALOGCOORDINATOR;
        }

        [RelayCommand]
        public void ReturnO()
        {
            //_mainViewModel.ShowReturnO();
        }
    }
}
