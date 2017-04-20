using UnityEngine;
using System.Collections.Generic;
using System.Text;
using System.Xml;

public struct NormalAccountInfo
{
    public string UID ;
    public uint CRC1;
    public uint CRC2;
    public uint CRC3;

    //public NormalAccountInfo()
    //{
    //    UID = "";
    //    CRC1 = 0;
    //    CRC2 = 0;
    //    CRC3 = 0;
    //}
    public void Clear()
    {
        UID = "";
        CRC1 = 0;
        CRC2 = 0;
        CRC3 = 0;
    }
}
public struct PhoneAccountInfo
{
    public System.UInt64 PhoneNumber;   //手机号码
    public uint phoneCRC1 ;          //手机密码
    public uint phoneCRC2 ;
    public uint phoneCRC3 ;

    //public PhoneAccountInfo()
    //{
    //    PhoneNumber = 0;
    //    phoneCRC1 = 0;
    //    phoneCRC2 = 0;
    //    phoneCRC3 = 0;
    //}    
    public void Clear()
    {
        PhoneNumber = 0;
        phoneCRC1 = 0;
        phoneCRC2 = 0;
        phoneCRC3 = 0;
    }
}
public class AccountData
{
    public uint KEY;        //文件识别码
    public byte VER;        //版本号
    public byte TAG;        //游客账号标识  
    //账号密码
    public bool bIsSavePassword;//是否保存账号密码
    public NormalAccountInfo AccountInfo = new NormalAccountInfo();
    public NormalAccountInfo TempAccountInfo = new NormalAccountInfo();
    //手机密码
    public bool bIsSavePhonePassword;//是否保存手机密码
    public PhoneAccountInfo PhoneInfo = new PhoneAccountInfo();
    public PhoneAccountInfo TempPhoneInfo = new PhoneAccountInfo();

    public AccountData()
    {
        Init();
    }
    void Init()
    {
        KEY = ConstValue.FILE_END_MAGIC;
        VER = ConstValue.FILE_VER;
        TAG = 0;
        bIsSavePassword = false;
        bIsSavePhonePassword = false;
        AccountInfo.Clear();
        PhoneInfo.Clear();
        TempAccountInfo.Clear();
        TempPhoneInfo.Clear();
    }
    public void ClearAccountInfo()
    {
        AccountInfo.Clear();
    }
    public void ClearPhoneInfo()
    {
        PhoneInfo.Clear();
    }
    public void LoadTempAccountInfo()
    {
        if (TempAccountInfo.UID != null && TempAccountInfo.UID != "")
        {
            AccountInfo = TempAccountInfo;
            TempAccountInfo.Clear();
        }
    }
    public void LoadTempPhoneInfo()
    {
        if (TempPhoneInfo.PhoneNumber != 0)
        {
            PhoneInfo = TempPhoneInfo;
            TempPhoneInfo.Clear();
        }
    }
    public void SaveAccountData()
    {
        //GlobalLogon.Instance.AccountData = this;

        //因为有可能是临时的AccountData 所以只保存部分修改后的数据

        //将当前数据保存起来
        System.IO.MemoryStream ms = new System.IO.MemoryStream();
        System.IO.BinaryWriter bw = new System.IO.BinaryWriter(ms);

        KEY = ConstValue.FILE_END_MAGIC + 1;
        VER = ConstValue.FILE_VER;

        bw.Write(KEY);
        bw.Write(VER);
        bw.Write(TAG);
        bw.Write(AccountInfo.UID);
        bw.Write(bIsSavePassword);
        if (bIsSavePassword)
        {
            bw.Write(AccountInfo.CRC1);
            bw.Write(AccountInfo.CRC2);
            bw.Write(AccountInfo.CRC3);
        }
        else
        {
            bw.Write(0);
            bw.Write(0);
            bw.Write(0);
        }
        bw.Write(PhoneInfo.PhoneNumber);
        bw.Write(bIsSavePhonePassword);
        if (bIsSavePhonePassword)
        {
            bw.Write(PhoneInfo.phoneCRC1);
            bw.Write(PhoneInfo.phoneCRC2);
            bw.Write(PhoneInfo.phoneCRC3);
        }
        else
        {
            bw.Write(0);
            bw.Write(0);
            bw.Write(0);
        }
        RuntimeInfo.SaveLocalFile(RuntimeInfo.GetAccountFileName(), ms.ToArray());
        bw.Close();

        //string str = string.Format("账号:{0} 密码是是否为空 {1} 是否保存账号:{2} 手机账号:{3} 密码是否为空:{4} 是否保存手机账号:{5} ", AccountInfo.UID, AccountInfo.CRC1 == 0 && AccountInfo.CRC2 == 0 && AccountInfo.CRC3 == 0, bIsSavePassword, PhoneInfo.PhoneNumber, PhoneInfo.phoneCRC1 == 0 && PhoneInfo.phoneCRC2 == 0 && PhoneInfo.phoneCRC3 == 0, bIsSavePhonePassword);
        //LogMgr.Log(str);
    }
    public void LoadAccountData()
    {
        byte[] data = RuntimeInfo.GetLocalFile(RuntimeInfo.GetAccountFileName());
        if (data == null)
        {
            Init();
            return;
        }
        System.IO.MemoryStream ms = new System.IO.MemoryStream(data);
        System.IO.BinaryReader br = new System.IO.BinaryReader(ms);
        this.KEY = br.ReadUInt32();
        if (this.KEY == ConstValue.FILE_END_MAGIC)
        {
            this.VER = br.ReadByte();
            this.TAG = br.ReadByte();

            this.AccountInfo.UID = br.ReadString();
            this.bIsSavePassword = true;
            this.AccountInfo.CRC1 = br.ReadUInt32();
            this.AccountInfo.CRC2 = br.ReadUInt32();
            this.AccountInfo.CRC3 = br.ReadUInt32();
            this.PhoneInfo.PhoneNumber = 0;
            this.bIsSavePhonePassword = true;
            this.PhoneInfo.phoneCRC1 = 0;
            this.PhoneInfo.phoneCRC2 = 0;
            this.PhoneInfo.phoneCRC3 = 0;
        }
        else  if (this.KEY == ConstValue.FILE_END_MAGIC +1)
        {
            this.VER = br.ReadByte();
            this.TAG = br.ReadByte();

            this.AccountInfo.UID = br.ReadString();
            this.bIsSavePassword = br.ReadBoolean();
            this.AccountInfo.CRC1 = br.ReadUInt32();
            this.AccountInfo.CRC2 = br.ReadUInt32();
            this.AccountInfo.CRC3 = br.ReadUInt32();
            this.PhoneInfo.PhoneNumber = br.ReadUInt64();
            this.bIsSavePhonePassword = br.ReadBoolean();
            this.PhoneInfo.phoneCRC1 = br.ReadUInt32();
            this.PhoneInfo.phoneCRC2 = br.ReadUInt32();
            this.PhoneInfo.phoneCRC3 = br.ReadUInt32();
        }
        ms.Close();
        br.Close();
    }

    public bool IsSavePassword(bool IsNormalOrPhone)
    {
        if (IsNormalOrPhone)
        {
            return bIsSavePassword;
        }
        else
        {
            return bIsSavePhonePassword;
        }
    }
}
public class PhoneLogonData
{
    public System.UInt64 PhoneNumber;
    public System.UInt32 Password;
    public string NewPassword;
    public bool IsFirOrSec;
}
public class LocalSetting
{
    static Dictionary<uint, string> m_MatchTitle = new Dictionary<uint, string>();
    public static string m_DefaultTitle;
    public static string GetMatchTitle(uint crc)
    {
        string str;
        if (m_MatchTitle.TryGetValue(crc, out str))
        {
            return str;
        }
        else
            return m_DefaultTitle;
    }
    public static bool Init()
    {
        Object obj = ResManager.Instance.LoadObject("channel_setting", "GlobalRes/LocalSetting/", ResType.GlobalRes, typeof(TextAsset));
        if (obj == null)
        {
            InitLogic.SetInitFailed();
            return false;
        }
        TextAsset ta = obj as TextAsset;
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(ta.text);
        
        //比赛的标题
        //====================================
        XmlNode node = doc.DocumentElement.SelectSingleNode("match_title");
        byte[] data= System.Text.Encoding.ASCII.GetBytes("com.leduo.buyu");
        uint defCrc = Crc.Crc32(data, 0, data.Length);
        foreach(XmlNode n in node.ChildNodes)
        {
            string str = n.Attributes["title"].Value;
            string str2 = n.Attributes["package"].Value;

            data = System.Text.Encoding.ASCII.GetBytes(str2);
            uint crc = Crc.Crc32(data, 0, data.Length);
            if(crc == defCrc)
                m_DefaultTitle = str;
            m_MatchTitle.Add(crc, str);
        }
        //====================================
        ResManager.Instance.UnloadObject(obj);
        return true;
    }
}
