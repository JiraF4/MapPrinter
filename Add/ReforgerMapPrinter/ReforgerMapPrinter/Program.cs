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
            var path1 = @"F:\MapPrinter\profile\MapPrint";
            var path2 = @"F:\MapPrinter\profile\MapPrint2";
            // Resize tiles and move it to other folder
            // DELETE original images
            // CompressImages(path1, path2, 256);
            
            // If something broke, I yse this for getting las render position
            // LastImagePos(path2);
            
            // Size of ortho camera
            var WorldTileSize = 8.0f;
            
            // concatenate 60x60 tiles to one image in original resolution with center in x: 9358.0 y: 1162.0
            var outPath = @"F:\MapPrinter\profile\MapPrintOut\Map.png";
            var scale = 1.0f;
            var size = 30;
            var WorldPosX = 9358.0f;
            var WorldPosY = 1162.0f;
            var PosX = (int) (WorldPosX / WorldTileSize);
            var PosY = (int) (WorldPosY / WorldTileSize);
            ConcatToImage(path2, outPath, new Rectangle(PosX-size, PosY-size, size*2, size*2), scale);
        }
        
        static void CompressImages(string path, string pathOut, int partSize)
        {
            var partSizeX = partSize;
            var partSizeY = partSize;
            
            // Get all bmp files in folder
            var directory = new DirectoryInfo(path);
            var rect = new Rectangle(0, 0, partSizeX, partSizeY);
            Parallel.ForEach(directory.GetFiles("*.bmp"), file =>
            {
                // Compress resize image and save to png
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
                
                // !!!Delete!!! original
                File.Delete(file.FullName);
            });
        }

        static void LastImagePos(string path)
        {
            var directory = new DirectoryInfo(path);

            var maxX = 0;
            var maxY = 0;
            
            // Scan tiles in folder
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
            
            // Write last position to console
            Console.WriteLine("X: " + maxX + " Y: " + maxY);
        }

        static void ConcatToImage(string path, string outPath, Rectangle rectPrint, float outScale)
        {
            var partSizeX = 64;
            var partSizeY = 64;
            
            // Get original tile size by (0, 0)
            using (var partImage = Image.FromFile(path + @"\0000-0000.png"))
            {
                partSizeX = (int) (partImage.Width * outScale);
                partSizeY = (int) (partImage.Height * outScale);
            }
            
            // Create new bitmap
            var bitmap = new Bitmap(rectPrint.Width * partSizeX, rectPrint.Height * partSizeY);
            var graphics = Graphics.FromImage(bitmap);
            
            // Draw each tile on our bitmap
            for (var posX = 0; posX < rectPrint.Width; posX++)
            {
                for (var posY = 0; posY < rectPrint.Height; posY++)
                {
                    var posInRectX = rectPrint.X + posX;
                    var posInRectY = rectPrint.Y + posY;
                    var partPath = path + "/" + posInRectX.ToString("0000") + "-" + posInRectY.ToString("0000") + ".png";
                    
                    // If there is no tile just skip.
                    if (!File.Exists(partPath)) continue;
                    
                    // Bad practice, but some time tile broken
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
            
            // Save our new image
            bitmap.Save(outPath, ImageFormat.Png);
            bitmap.Dispose();
        }
    }
}