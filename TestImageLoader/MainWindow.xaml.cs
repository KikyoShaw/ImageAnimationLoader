using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
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

namespace TesTImageLoader
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            TestAnimationMemData();
            TestAnimationLocalFile();
            TestWebpMemData();
            TestWebpLocalFile();
        }

        public void TestAnimationMemData()
        {
            var uri = new Uri(@"/Resource/test_animation.webp"/*or test_animation.webp*/, UriKind.Relative);
            var info = Application.GetResourceStream(uri);
            using var r = new MemoryStream();
            info.Stream.CopyTo(r);

            var s = ImageLoader.Loader.GetKeyFramesByMemBytes(r.ToArray());
            var l = new List<BitmapSource>();
            foreach (var frame in s)
            {
                l.Add(frame.AFrame);
            }
            MemAni.ItemsSource = l;
        }

        public void TestAnimationLocalFile()
        {
            var s = ImageLoader.Loader.GetKeyFramesByLocalPath(@"D:/test_animation.gif"/*or test_animation.webp*/);
            var l = new List<BitmapSource>();
            foreach (var frame in s)
            {
                l.Add(frame.AFrame);
            }
            LocalFileAni.ItemsSource = l;
        }

        public void TestWebpMemData()
        {
            var uri = new Uri(@"/Resource/test.webp", UriKind.Relative);
            var info = Application.GetResourceStream(uri);
            using var r = new MemoryStream();
            info.Stream.CopyTo(r);
            var s = ImageLoader.Loader.GetWebpFrameByMemBytes(r.ToArray(), 20, 100);
            MemWebp.Source = s;
        }

        public void TestWebpLocalFile()
        {
            var s = ImageLoader.Loader.GetWebpFrameByLocalPath(@"D:/test.webp");
            FileWebp.Source = s;
        }
    }
}
