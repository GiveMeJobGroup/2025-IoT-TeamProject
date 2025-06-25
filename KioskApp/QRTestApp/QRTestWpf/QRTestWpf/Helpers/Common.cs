using MahApps.Metro.Controls.Dialogs;

namespace QRTestWpf.Helpers
{
    public static class Common
    {
        public static readonly string CONNSTR = "Server=localhost;Database=logintest;Uid=root;Password=12345;Charset=utf8";

        // MahApps.Metro 다이얼로그 코디네이터(MVVM에서 사용하기 위해서)
        public static IDialogCoordinator DIALOGCOORDINATOR;
    }
}
