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
    public partial class MenuViewModel : ObservableObject
    {
        private IDialogCoordinator DIALOGCOORDINATOR;
        
        private readonly MainViewModel _mainViewModel;
        public MenuViewModel(IDialogCoordinator DIALOGCOORDINATOR)
        {
            this.DIALOGCOORDINATOR = DIALOGCOORDINATOR;
        }
        [RelayCommand]
        public void QRScan()
        {
            _mainViewModel.ShowQRScan();
        }
    }
}
