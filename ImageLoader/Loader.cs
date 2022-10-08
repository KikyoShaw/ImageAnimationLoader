using ImageParser;
using System;
using System.Collections.Generic;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ImageLoader
{
    public struct KeyFrame
    {
        /// <summary>
        /// ��ǰ֡��bitmap
        /// </summary>
        public BitmapSource AFrame;
        /// <summary>
        /// ��ǰ֡����ǰһ֡�ļ������λ-���룬��һ֡��ʱ���� 0
        /// </summary>
        public double Duration;
    }
    public class Loader
    {
        /// <summary>
        /// ��ȡwebp��ͼ�ļ���Bitmapsource
        /// �����Ⱥ͸߶�ֻ����һ����ʱ��ԭͼ�������ţ����������ô���ԭͼ����
        /// </summary>
        /// <param name="localPath">����·��</param>
        /// <param name="decodeWidth">������</param>
        /// <param name="decodeHeight">����߶�</param>
        /// <returns>�ɹ����� wpf BitmapSource��ʧ�ܷ��� null</returns>
        public static BitmapSource GetWebpFrameByLocalPath(string localPath, int decodeWidth = 0, int decodeHeight = 0)
        {
            using var webp = new ImageParser.Webp();
            var pr = webp.ParseWebpFile(localPath, decodeWidth, decodeHeight);
            if (!pr)
                return null;

            return GetFirstFrameInner(webp);
        }

        /// <summary>
        /// ��ȡwebp�ڴ����ݵ�Bitmapsource
        /// �����Ⱥ͸߶�ֻ����һ����ʱ��ԭͼ�������ţ����������ô���ԭͼ����
        /// </summary>
        /// <param name="memBytes">�ڴ�����</param>
        /// <param name="decodeWidth">������</param>
        /// <param name="decodeHeight">����߶�</param>
        /// <returns>�ɹ����� wpf BitmapSource��ʧ�ܷ��� null</returns>
        public static BitmapSource GetWebpFrameByMemBytes(byte[] memBytes, int decodeWidth = 0, int decodeHeight = 0)
        {
            using var webp = new ImageParser.Webp();
            var pr = webp.ParseWebpMemBytes(memBytes, decodeWidth, decodeHeight);
            if (!pr)
                return null;

            return GetFirstFrameInner(webp);
        }

        /// <summary>
        /// ��ȡwebp��gif����
        /// </summary>
        /// <param name="localPath">����·��</param>
        /// <returns>����������֡</returns>
        public static List<KeyFrame> GetKeyFramesByLocalPath(string localPath)
        {
            var frames = new List<KeyFrame>();
            using var ani = new ImageParser.Animation();
            var pr = ani.ParseAnimationFrames(localPath);
            if (!pr)
                return frames;

            GetKeyFramesInner(ani, ref frames);

            return frames;
        }

        /// <summary>
        /// ��ȡwebp��gif����
        /// </summary>
        /// <param name="localPath">�ڴ�����</param>
        /// <returns>����������֡</returns>
        public static List<KeyFrame> GetKeyFramesByMemBytes(byte[] memBytes)
        {
            var frames = new List<KeyFrame>();
            using var ani = new ImageParser.Animation();
            var pr = ani.ParseAnimationFrames(memBytes);
            if (!pr)
                return frames;

            GetKeyFramesInner(ani, ref frames);

            return frames;
        }

        private static void GetKeyFramesInner(ImageParser.Animation ani, ref List<KeyFrame> frames)
        {
            var index = 0;
            var count = ani.GetFrameCount();
            var height = ani.GetCavHeight();
            var width = ani.GetCavWidth();
            var pf = ani.HasAlpha() ? PixelFormats.Bgra32 : PixelFormats.Bgr32;
            var rawStride = (width * pf.BitsPerPixel + 7) / 8;
            for (;index < count; ++index)
            {
                if (!ani.GetFrameAt(index, out var dataPtr, out var dataSize, out var duration))
                    continue;

                var bitmap = BitmapSource.Create(width, height, 96, 96, pf, null, dataPtr, dataSize, rawStride);
                frames.Add(new KeyFrame
                {
                    AFrame = bitmap,
                    Duration = duration
                });
            }
        }

        private static BitmapSource GetFirstFrameInner(ImageParser.Webp webp)
        {
            if (!webp.GetOk())
                return null;

            var height = webp.GetCavHeight();
            var width = webp.GetCavWidth();
            var pf = webp.HasAlpha() ? PixelFormats.Bgra32 : PixelFormats.Bgr32;
            var rawStride = (width * pf.BitsPerPixel + 7) / 8;
            if (webp.GetFrameData(out var dataPtr, out var dataSize))
            {
                return BitmapSource.Create(width, height, 96, 96, pf, null, dataPtr, dataSize, rawStride);
            }

            return null;
        }
    }
}
