using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using MahApps.Metro.Controls.Dialogs;
using MySql.Data.MySqlClient;

namespace QRTestWpf.ViewModels
{
    public partial class QRScanViewModel : ObservableObject
    {
        private IDialogCoordinator DIALOGCOORDINATOR;

        private readonly MainViewModel _mainViewModel;

        private string _qrResult;

        public string QrResult
        {
            get => _qrResult;
            set => SetProperty(ref _qrResult, value);
        }

        public QRScanViewModel(IDialogCoordinator DIALOGCOORDINATOR, MainViewModel mainViewModel)
        {
            this._mainViewModel = mainViewModel;
            this.DIALOGCOORDINATOR = DIALOGCOORDINATOR;
        }

        [RelayCommand]
        public void CloseQR()
        {
            _mainViewModel.ShowMenu();
        }

        // QR 스캔 후 호출
        public async void OnQRScanned(string text)
        {
            QrResult = text;
            Console.WriteLine($"스캔된 QR 코드: {text}");

            string email = await GetEmailByUuidAsync(text);

            if (!string.IsNullOrEmpty(email))
            {
                await DIALOGCOORDINATOR.ShowMessageAsync(this, "인증 성공", $"이메일: {email}");
                // TODO: 필요 시 화면 전환 or 자동 로그인 처리
                // 여기서 다음 화면으로 전환
                _mainViewModel.ShowAfterScan();
            }
            else
            {
                await DIALOGCOORDINATOR.ShowMessageAsync(this, "실패", "QR 코드에 해당하는 사용자가 없습니다.");
            }
        }

        /// <summary>
        /// QR 코드(UUID)를 기반으로 사용자 email 조회
        /// </summary>
        private async Task<string> GetEmailByUuidAsync(string uuid)
        {
            try
            {
                using var conn = new MySqlConnection(Helpers.Common.CONNSTR);
                await conn.OpenAsync();

                string query = "SELECT email FROM member WHERE qrcode = @qrcode LIMIT 1";

                using var cmd = new MySqlCommand(query, conn);
                cmd.Parameters.AddWithValue("@qrcode", uuid);

                var result = await cmd.ExecuteScalarAsync();
                return result?.ToString();
            }
            catch (Exception ex)
            {
                await DIALOGCOORDINATOR.ShowMessageAsync(this, "DB 오류", ex.Message);
                return null;
            }
        }

        // 화면 전환 확인용
        [RelayCommand]
        public void AfterView()
        {
            _mainViewModel.ShowAfterScan();
        }
    }
}