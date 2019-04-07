using Astc;
using System;
using System.Diagnostics;
using System.Drawing.Imaging;
using System.IO;

namespace AstcTest
{
	public static class Program
	{
		public static void Main(string[] args)
		{
			if (args.Length < 4)
			{
				Console.WriteLine("Format: {path} {width} {height} {blockXSize} {blockYSize}");
				Console.ReadKey();
			}
			else
			{
				string path = args[0];
				int width = int.Parse(args[1]);
				int height = int.Parse(args[2]);
				int blockXSize = int.Parse(args[3]);
				int blockYSize = int.Parse(args[4]);
				byte[] data = File.ReadAllBytes(path);

				Stopwatch stopwatch = new Stopwatch();
				using (DirectBitmap bitmap = new DirectBitmap(width, height))
				{
					ConsoleKeyInfo key;
					do
					{
						stopwatch.Start();
						AstcDecoder.DecodeASTC(data, width, height, blockXSize, blockYSize, bitmap.Bits);
						stopwatch.Stop();
						Console.WriteLine("Processed " + stopwatch.ElapsedMilliseconds);
						stopwatch.Reset();
						key = Console.ReadKey();
					}
					while (key.Key == ConsoleKey.Spacebar);


					string dirPath = Path.GetDirectoryName(path);
					string name = Path.GetFileNameWithoutExtension(path);
					string newPath = Path.Combine(dirPath, name + "_decoded.png");
					//bitmap.Bitmap.Save(newPath, ImageFormat.Png);
				}

				Console.WriteLine("Finished!");
			}

		}
	}
}
