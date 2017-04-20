using UnityEngine;
using System.Collections.Generic;
using System.IO;
public class LZMA
{
    public static void CompressMemory(MemoryStream input, out MemoryStream output)
    {
        SevenZip.Compression.LZMA.Encoder coder = new SevenZip.Compression.LZMA.Encoder();
        output = new MemoryStream();
        coder.WriteCoderProperties(output);
        output.Write(System.BitConverter.GetBytes((int)input.Length), 0, 4);
        coder.Code(input, output, input.Length, -1, null);
        output.Flush();
        input.Close();
    }
    public static void CompressFile(string inFile, string outFile)
    {
        FileStream output = new FileStream(outFile, FileMode.Create);
        MemoryStream ms;
        
        byte[] data = File.ReadAllBytes(inFile);
        MemoryStream inputms = new MemoryStream(data);

        CompressMemory(inputms, out ms);

        output.Write(ms.GetBuffer(), 0, (int)ms.Position);
        output.Flush();
        output.Close();
        ms.Close();
        inputms.Close();
    }
    public static void DecompressMemory(MemoryStream input, out MemoryStream output)
    {
        byte[] properties = new byte[5];
        input.Read(properties, 0, 5);

        byte[] fileLengthBytes = new byte[4];
        input.Read(fileLengthBytes, 0, 4);
        int fileLength = System.BitConverter.ToInt32(fileLengthBytes, 0);

        output = new MemoryStream();
       
        SevenZip.Compression.LZMA.Decoder coder = new SevenZip.Compression.LZMA.Decoder();
        coder.SetDecoderProperties(properties);
        coder.Code(input, output, input.Length, fileLength, null);
        output.Flush();
    }
    public static void DecompressFile(string inFile, string outFile)
    {
        FileStream input = new FileStream(inFile, FileMode.Open);
        FileStream output = new FileStream(outFile, FileMode.Create);

        byte[] properties = new byte[5];
        byte[] fileLengthBytes = new byte[4];
        input.Read(properties, 0, 5);
        input.Read(fileLengthBytes, 0, 4);
        int fileLength = System.BitConverter.ToInt32(fileLengthBytes, 0);

        SevenZip.Compression.LZMA.Decoder coder = new SevenZip.Compression.LZMA.Decoder();
        coder.SetDecoderProperties(properties);
        coder.Code(input, output, input.Length, fileLength, null);

        output.Flush();
        input.Close();
        output.Close();
    }
}
