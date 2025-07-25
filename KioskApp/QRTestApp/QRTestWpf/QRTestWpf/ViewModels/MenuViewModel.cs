﻿using CommunityToolkit.Mvvm.ComponentModel;
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
        [RelayCommand]
        public void QRScan()
        {
            _mainViewModel.ShowQRScan();
        }


    }
}
