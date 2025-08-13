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
    public partial class ReturnBoxViewModel : ObservableObject
    {
        private readonly MainViewModel _mainViewModel;
        private IDialogCoordinator DIALOGCOORDINATOR;


        public ReturnBoxViewModel(IDialogCoordinator DIALOGCOORDINATOR, MainViewModel mainViewModel)
        {
            this._mainViewModel = mainViewModel;
            this.DIALOGCOORDINATOR = DIALOGCOORDINATOR;
        }

        [RelayCommand]
        public void Wait()
        {
            _mainViewModel.ShowWait();
        }
    }
}
