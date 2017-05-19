using UnityEngine;
using System.Collections.Generic;
using System.IO;
public class Crc
{
    public static uint Crc32(byte[] data, int offset, int len)
    {
        SevenZip.CRC crc = new SevenZip.CRC();
        crc.Update(data, (uint)offset, (uint)len);
        return crc.GetDigest();
    }
    public static uint Crc32FromFile(string path)
    {
        if(!File.Exists(path))
            return 0;
        FileStream fs = new FileStream(path, FileMode.Open);
        byte[] data = new byte[fs.Length];
        fs.Read(data, 0, data.Length);
        fs.Close();
        return Crc32(data, 0, data.Length);
    }
}
