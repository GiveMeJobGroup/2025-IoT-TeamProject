using QRTestWpf.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace QRTestWpf.Views
{
    /// <summary>
    /// QRScanView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class QRScanView : UserControl
    {
        private StringBuilder _qrBuffer = new StringBuilder();
        private static readonly Regex _uuidRegex = new Regex(
            @"^[a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12}$",
            RegexOptions.IgnoreCase
        );
        public QRScanView()
        {
            InitializeComponent();
            this.Loaded += QRScanView_Loaded;
            this.PreviewTextInput += QRScanView_PreviewTextInput;
            this.PreviewKeyDown += QRScanView_PreviewKeyDown;
        }

        private void QRScanView_Loaded(object sender, RoutedEventArgs e)
        {
            this.Focusable = true;
            this.Focus(); // 포커스를 받아야 키 입력을 감지함
        }

        private void QRScanView_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            _qrBuffer.Append(e.Text); // 문자 하나씩 누적
        }

        private void QRScanView_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                string scanned = _qrBuffer.ToString();
                _qrBuffer.Clear(); // 초기화는 무조건 해줘야 중복 방지

                if (_uuidRegex.IsMatch(scanned))
                {
                    if (DataContext is QRScanViewModel viewModel)
                    {
                        viewModel.OnQRScanned(scanned);
                    }
                }
                else
                {
                    // 잘못된 입력일 경우 무시하거나 로그 출력 가능
                    Console.WriteLine($"유효하지 않은 입력: {scanned}");
                }

                e.Handled = true; // Enter 키 입력 처리 완료 표시
            }
        }
    }
}

