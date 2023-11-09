using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Threading.Tasks;

namespace ReforgerMapPrinter
{
    internal class Program
    {
        public static void Main(string[] args)
        {
            CompressImages();
            
            var WorldTileSize = 8.0f;
            var WorldPosX = 9358.0f;
            var WorldPosY = 1162.0f;

            var PosX = (int) (WorldPosX / WorldTileSize);
            var PosY = (int) (WorldPosY / WorldTileSize);
            var size = 30;
            
            ConcatToImage(new Rectangle(PosX-size, PosY-size, size*2, size*2), 1.0f);

            /*
            CompressImages();
            ConcatToImage(new Rectangle(0, 0, 1792, 1612), 0.05f);
            */
        }
        
        
        
        static void CompressImages()
        {
            var path = @"F:\MapPrinter\profile\MapPrint";
            var pathOut = @"F:\MapPrinter\profile\MapPrint2";
            
            var partSizeX = 256;
            var partSizeY = 256;
            
            var directory = new DirectoryInfo(path);
            var rect = new Rectangle(0, 0, partSizeX, partSizeY);
            Parallel.ForEach(directory.GetFiles("*.bmp"), file =>
            {
                try
                {
                    using (var bitmap = new Bitmap(partSizeX, partSizeY))
                    {
                        var graphics = Graphics.FromImage(bitmap);
                        using (var partImage = Image.FromFile(file.FullName))
                        {
                            graphics.DrawImage(partImage, rect);
                        }

                        bitmap.Save(file.FullName.Replace(path, pathOut).Replace(".bmp", ".png"), ImageFormat.Png);
                    }
                }
                catch { }

                File.Delete(file.FullName);
            });
        }
        
        static void Crop()
        {
            var path = @"F:\MapPrinter\profile\MapPrint2";
            var directory = new DirectoryInfo(path);

            foreach (var file in directory.GetFiles("*.png"))
            {
                var x = int.Parse(file.Name.Substring(0, 4));
                var y = int.Parse(file.Name.Substring(5, 4));
                if (y < 155)
                {
                    //File.Delete(file.FullName);
                }
            }
            
        }

        static void LastImagePos()
        {
            var path = @"F:\MapPrinter\profile\MapPrint2";
            var directory = new DirectoryInfo(path);

            var maxX = 0;
            var maxY = 0;
            
            foreach (var file in directory.GetFiles("*.png"))
            {
                var x = int.Parse(file.Name.Substring(0, 4));
                var y = int.Parse(file.Name.Substring(5, 4));
                if (maxY < y)
                {
                    maxX = 0;
                    maxY = y;
                }
                if (maxY <= y && maxX < x) maxX = x;
            }
            
            Console.WriteLine("X: " + maxX + " Y: " + maxY);
        }

        static void ConcatToImage(Rectangle rectPrint, float outScale)
        {
            var path = @"F:\MapPrinter\profile\MapPrint2";
            var maxX = 1792;
            var maxY = 1612;
            var partSizeX = 64;
            var partSizeY = 64;

            using (var partImage = Image.FromFile(path + @"\0200-0200.png"))
            {
                partSizeX = (int) (partImage.Width * outScale);
                partSizeY = (int) (partImage.Height * outScale);
            }
            
            var bitmap = new Bitmap(rectPrint.Width * partSizeX, rectPrint.Height * partSizeY);
            var graphics = Graphics.FromImage(bitmap);
            
            for (var posX = 0; posX < rectPrint.Width; posX++)
            {
                for (var posY = 0; posY < rectPrint.Height; posY++)
                {
                    var posInRectX = rectPrint.X + posX;
                    var posInRectY = rectPrint.Y + posY;
                    var partPath = path + "/" + posInRectX.ToString("0000") + "-" + posInRectY.ToString("0000") + ".png";
                    if (!File.Exists(partPath)) continue;

                    try
                    {
                        using (var partImage = Image.FromFile(partPath))
                        {
                            var rect = new Rectangle(posX * partSizeX, posY * partSizeY + partSizeY, partSizeX, -partSizeY - 1);
                            graphics.DrawImage(partImage, rect);
                        }
                    }
                    catch {}
                }
            }
            
            bitmap.Save(@"F:\MapPrinter\profile\MapPrintOut\Map.png", ImageFormat.Png);
            bitmap.Dispose();
        }
    }
}