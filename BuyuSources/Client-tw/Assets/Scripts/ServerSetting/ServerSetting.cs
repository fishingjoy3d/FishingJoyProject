using UnityEngine;
using System.Collections.Generic;
using System.Xml;
using System.IO;

class LauncherInfo
{
    public byte nmincatch;
    public byte nmaxcatch;
    public ushort nrevise;
    public ushort nincome;
    public float denergyratio;
}
public enum ISPType
{
    ISP_DX = 0,
    ISP_LT,
    ISP_YD,
    ISP_MAX,
}
public class ServerIPData
{
    public ServerIPData()
    {
        IP = "";
        Port = 0;
        Connected = true;
    }
    public byte     ISP;
    public string   IP;
    public ushort   Port;
    public bool     Connected;
}
public class ServerSetting
{
    public static List<ServerIPData> ServerList = new List<ServerIPData>();
    public static string    ResFtp;
    public static ushort    ResFtpPort = 45045;
    public static string    RunFtp;
    public static string    HallServerIP;
    public static ushort    HallServerPort;
    public static uint      NewIP;
    public static ushort    NewPort;
    public static string    RT_IMAGE_DIR;
    public static string    RT_XML_DIR;
    public static string    CALLBACK_URL;
    public static string    SERVICES_URL;
    public static bool      IS_TEST = true;
    public static string    VERSION_DIR;
    public static uint      RES_VERSION;
    public static bool      SHOW_PING;
    public static string    ReporterIP = null;
    public static ushort    ReporterPort;
    public static int       ConnectServerIdx = -1;
    public static uint      ClientVer;
    public static bool      ShowExchange = false;    //对换
    public static bool      ShowExtraBtn = true;
    public static bool      ShowGame = false;
    public static bool      ShowHallThirdBtn = true;
    public static bool      ShowJBP = true;
    public static string    ShareWebUrl = "", ShareTxt = "", ShareImgUrl = "";
    public static bool      ShowMatch = false;//显示比赛按钮
    public static bool      ShowThirdLoginBtn = false;//显示第三方登录按钮
    public static bool      ShowShare = false;//显示分享按钮
    public static bool      SendOperatorLogon = true;//统一的登录接口

    public static void GlobalInit()
    {
        if (SDKMgr.PACKAGE_NAME == "com.leduo.buyu.uc")
            ShowExtraBtn = false;
    }
    public static void SetLogonServerState( bool bok)
    {
        int idx = ConnectServerIdx % ServerList.Count;
        ServerList[idx].Connected = bok;
    }
    public static ServerIPData GetLogonServer()
    {
        for(int i = 0; i < ServerList.Count;++i)
        {
            int idx = ++ConnectServerIdx % ServerList.Count;
            if (ServerList[idx].Connected)
            {
                ServerList[idx].Connected = false;
                return ServerList[idx];
            }
        }
        ConnectServerIdx = new System.Random().Next(0, ServerList.Count);
        return ServerList[ConnectServerIdx];
    }
    public static  void SetResAndRunFTP(string resFTP, string runFTP)
    {
        ResFtp          = resFTP;
        RunFtp          = runFTP;
        RT_IMAGE_DIR    = RunFtp + "/Images/";
        RT_XML_DIR      = RunFtp + "/XML/";
    }
    public static void SetRunFTP(string runFTP)
    {
        RunFtp          = runFTP;
        RT_IMAGE_DIR    = RunFtp + "/Images/";
        RT_XML_DIR      = RunFtp + "/XML/";
    }
    public static void SetCallbckUrl(string ip)
    {
        if(SDKMgr.SDK_TYPE == SdkType.SDK_SELF)
        {
            CALLBACK_URL = "http://" + ip + ":1680/pay_callback_self.clkj";
        }
        else if(SDKMgr.SDK_TYPE == SdkType.SDK_CHANNEL)
        {
            CALLBACK_URL = "http://" + ip + ":1680/pay_callback_lj.clkj?";
        }
        else if (SDKMgr.SDK_TYPE == SdkType.SDK_DERIVED)
        {
            CALLBACK_URL = "http://" + ip + ":1680/pay_callback_derived.clkj?";
        }
    }
    static void Attribute(XmlNode pNode, string attribute, ref float fValue)
    {
        if (pNode.Attributes[attribute] == null)
        {
            fValue = 0.0f;
            return;
        }
        fValue = float.Parse(pNode.Attributes[attribute].Value);
    }
    static  void Attribute(XmlNode pNode, string attribute, ref byte nValue)
    {
        if (pNode.Attributes[attribute] == null)
        {
            nValue = 0;
            return;
        }
        nValue = System.Convert.ToByte(pNode.Attributes[attribute].Value);
    }
    static void Attribute(XmlNode pNode, string attribute, ref ushort nValue)
    {
        if (pNode.Attributes[attribute] == null)
        {
            nValue = 0;
            return;
        }
        nValue = System.Convert.ToUInt16(pNode.Attributes[attribute].Value);
    }
    static void Attribute(XmlNode pNode, string attribute, ref uint nValue)
    {
        if (pNode.Attributes[attribute] == null)
        {
            nValue = 0;
            return;
        }
        nValue = System.Convert.ToUInt32(pNode.Attributes[attribute].Value);
    }
    static void Attribute(XmlNode pNode, string attribute, ref string szValue)
    {
        if (pNode.Attributes[attribute] == null)
        {
            szValue ="";
            return;
        }
        szValue = pNode.Attributes[attribute].Value;        
    }

   
    public static bool Init()
    {
        ConnectServerIdx = Utility.Range(0, ServerList.Count);
        //Object obj = ResManager.Instance.LoadObject("ServerSetting", "GlobalRes/ServerSetting/", ResType.GlobalRes, typeof(TextAsset));
        //if (obj == null)
        //    return false;
        //TextAsset ta = obj as TextAsset;
        //XmlDocument doc = new XmlDocument();
        //doc.LoadXml(ta.text);

        //XmlElement ele = doc.DocumentElement;

        //XmlNode fishNode = ele.SelectSingleNode("FishInfo");
        //XmlNode launcherNode = ele.SelectSingleNode("LauncherInfo");
        //XmlNode bulletNode = ele.SelectSingleNode("BulletInfo");
        //XmlNode skillNode = ele.SelectSingleNode("SkillInfo");
        //XmlNode otherNode = ele.SelectSingleNode("Other");
        //XmlNode fishDeadNode = ele.SelectSingleNode("FishDeadTime");

        //if (fishNode == null ||
        //    launcherNode == null ||
        //    bulletNode == null ||
        //    skillNode == null ||
        //    otherNode == null ||
        //    fishDeadNode == null)
        //{
        //    LogMgr.Log("ServerSetting load failed.");
        //    return false;
        //}
        //XmlNode childNode;
        ////fish
        //FishSetting.FISH_MAX_NUM = ushort.Parse(fishNode.Attributes["scene_max_num"].Value);
        //int fishCount = fishNode.ChildNodes.Count;
        //FishSetting.FishDataList = new FishData[fishCount];

        //for (int i = 0; i < fishCount; ++i)
        //{
        //    XmlNode node = fishNode.ChildNodes[i];
        //    FishData fd = new FishData();
        //    fd.ID = byte.Parse(node.Attributes["id"].Value);
        //    fd.Gold = ushort.Parse(node.Attributes["gold"].Value);
        //    if (FishSetting.FishDataList[fd.ID] != null)
        //    {
        //        LogMgr.Log("Exist the same fish id:" + fd.ID);
        //        return false;
        //    }
        //    FishSetting.FishDataList[fd.ID] = fd;
        //}
        ////launcher 
        //int launcherCount = launcherNode.ChildNodes.Count;
        //LauncherSetting.LauncherDataList = new LauncherData[launcherCount];
        //for (int i = 0; i < launcherCount; ++i)
        //{
        //    LauncherData ld = new LauncherData();
        //    XmlNode node = launcherNode.ChildNodes[i];
        //    ld.ID = byte.Parse(node.Attributes["id"].Value);
        //    ld.Interval = float.Parse(node.Attributes["interval"].Value);
        //    ld.Energy = ushort.Parse(node.Attributes["energy"].Value);
        //    ld.Consume = byte.Parse(node.Attributes["consume"].Value);
        //    ld.Speed = float.Parse(node.Attributes["bullet_speed"].Value);
        //    ld.LaserCDTime = float.Parse(node.FirstChild.Attributes["cdtime"].Value);
        //    ld.LaserReduction.Speed = float.Parse(node.FirstChild.Attributes["speed"].Value);
        //    ld.LaserReduction.Duration1 = float.Parse(node.FirstChild.Attributes["duration1"].Value);
        //    ld.LaserReduction.Duration2 = float.Parse(node.FirstChild.Attributes["duration2"].Value);
        //    ld.LaserReduction.Duration3 = float.Parse(node.FirstChild.Attributes["duration3"].Value);

        //    if (LauncherSetting.LauncherDataList[ld.ID] != null)
        //    {
        //        LogMgr.Log("Exist the same launcher id.");
        //        return false;
        //    }
        //    LauncherSetting.LauncherDataList[ld.ID] = ld;
        //}

        ////bullet
        //BulletSetting.BULLET_MAX_NUM = byte.Parse(bulletNode.Attributes["scene_max_num"].Value);
        //childNode = bulletNode.SelectSingleNode("FreezeReduction");
        //if (childNode == null)
        //{
        //    LogMgr.Log("FreezeReduction not found.");
        //    return false;
        //}
        //BulletSetting.FreezeBulletReduction.Speed = float.Parse(childNode.Attributes["speed"].Value);
        //BulletSetting.FreezeBulletReduction.Duration1 = float.Parse(childNode.Attributes["duration1"].Value);
        //BulletSetting.FreezeBulletReduction.Duration2 = float.Parse(childNode.Attributes["duration2"].Value);
        //BulletSetting.FreezeBulletReduction.Duration3 = float.Parse(childNode.Attributes["duration3"].Value);
        //childNode = bulletNode.SelectSingleNode("BulletRate");
        //if (childNode == null)
        //{
        //    LogMgr.Log("BulletRate not found.");
        //    return false;
        //}
        //int rateCount = childNode.ChildNodes.Count;
        //BulletSetting.BulletRate = new ushort[rateCount];
        //for (int i = 0; i < rateCount; ++i)
        //{
        //    BulletSetting.BulletRate[i] = ushort.Parse(childNode.ChildNodes[i].FirstChild.Value);
        //}

        ////SkillInfo
        //int skillCount = skillNode.ChildNodes.Count;
        //SkillSetting.SkillDataList = new SkillData[skillCount];
        //for (int i = 0; i < skillCount; ++i)
        //{
        //    SkillData sd = new SkillData();
        //    sd.ID = byte.Parse(skillNode.ChildNodes[i].Attributes["id"].Value);
        //    sd.CDTime = float.Parse(skillNode.ChildNodes[i].Attributes["cdtime"].Value);
        //    sd.Reduction.Speed = float.Parse(skillNode.ChildNodes[i].Attributes["speed"].Value);
        //    sd.Reduction.Duration1 = float.Parse(skillNode.ChildNodes[i].Attributes["duration1"].Value);
        //    sd.Reduction.Duration2 = float.Parse(skillNode.ChildNodes[i].Attributes["duration2"].Value);
        //    sd.Reduction.Duration3 = float.Parse(skillNode.ChildNodes[i].Attributes["duration3"].Value);
        //    if (SkillSetting.SkillDataList[sd.ID] != null)
        //    {
        //        LogMgr.Log("Exist the same skill id.");
        //        return false;
        //    }
        //    SkillSetting.SkillDataList[sd.ID] = sd;
        //}

        ////Other
        //childNode = otherNode.SelectSingleNode("MultipleEffect");
        //if (childNode == null)
        //{
        //    LogMgr.Log("MultipleEffect not found.");
        //    return false;
        //}
        //ExtraSetting.MultipleEffect = byte.Parse(childNode.FirstChild.Value);

        //childNode = otherNode.SelectSingleNode("DoubleHit");
        //if (childNode == null)
        //{
        //    LogMgr.Log("DoubleHit not found.");
        //    return false;
        //}
        //DoubleHit.IntervalTime = float.Parse(childNode.Attributes["interval_time"].Value);
        //DoubleHit.MaxTimes = ushort.Parse(childNode.Attributes["max_times"].Value);

        //childNode = otherNode.SelectSingleNode("RoomInfo");
        //if (childNode == null)
        //{
        //    LogMgr.Log("RoomInfo not found.");
        //    return false;
        //}
        //int roomCount = childNode.ChildNodes.Count;
        //ExtraSetting.RoomDataList = new RoomData[roomCount];
        //for (int i = 0; i < roomCount; ++i)
        //{
        //    RoomData rd = new RoomData();
        //    rd.RoomID = byte.Parse(childNode.ChildNodes[i].Attributes["id"].Value);
        //    rd.RoomRateIdx = byte.Parse(childNode.ChildNodes[i].FirstChild.Value);
        //    if (ExtraSetting.RoomDataList[rd.RoomID] != null)
        //    {
        //        LogMgr.Log("Exist the same room id.");
        //        return false;
        //    }
        //    ExtraSetting.RoomDataList[rd.RoomID] = rd;
        //}

        ////FishDeadTime
        //FishDeadTimeData ftd = new FishDeadTimeData();
        //ftd.TianZai_DouDong_Time = float.Parse(fishDeadNode.ChildNodes[0].FirstChild.Value);

        //ftd.TianZai_Stay_Time1 = float.Parse(fishDeadNode.ChildNodes[1].FirstChild.Value);
        //ftd.TianZai_Stay_Time2 = float.Parse(fishDeadNode.ChildNodes[2].FirstChild.Value);

        //ftd.TianZai_Dead_Time1 = float.Parse(fishDeadNode.ChildNodes[3].FirstChild.Value);
        //ftd.TianZai_Dead_Time2 = float.Parse(fishDeadNode.ChildNodes[4].FirstChild.Value);

        //ftd.Bullet_BingDong_Dead_Time1 = float.Parse(fishDeadNode.ChildNodes[5].FirstChild.Value);
        //ftd.Bullet_BingDong_Dead_Time2 = float.Parse(fishDeadNode.ChildNodes[6].FirstChild.Value);

        //ftd.BingDong_Dead_Time1 = float.Parse(fishDeadNode.ChildNodes[7].FirstChild.Value);
        //ftd.BingDong_Dead_Time2 = float.Parse(fishDeadNode.ChildNodes[8].FirstChild.Value);

        //ftd.ShanDian_Dead_Time1 = float.Parse(fishDeadNode.ChildNodes[9].FirstChild.Value);
        //ftd.ShanDian_Dead_Time2 = float.Parse(fishDeadNode.ChildNodes[10].FirstChild.Value);

        //ftd.LongJuanFeng_Dead_Time1 = float.Parse(fishDeadNode.ChildNodes[11].FirstChild.Value);
        //ftd.LongJuanFeng_Dead_Time2 = float.Parse(fishDeadNode.ChildNodes[12].FirstChild.Value);

        //ftd.JiGuang_Stay_Time = float.Parse(fishDeadNode.ChildNodes[13].FirstChild.Value);

        //ftd.JiGuang_Dead_Time1 = float.Parse(fishDeadNode.ChildNodes[14].FirstChild.Value);
        //ftd.JiGuang_Dead_Time2 = float.Parse(fishDeadNode.ChildNodes[15].FirstChild.Value);

        //ftd.ShandDian_Speed = float.Parse(fishDeadNode.ChildNodes[16].FirstChild.Value);
        //ftd.JiGuang_Speed = float.Parse(fishDeadNode.ChildNodes[17].FirstChild.Value);
        //ftd.JiGuang_BingDong_Speed = float.Parse(fishDeadNode.ChildNodes[18].FirstChild.Value);
        //ftd.TianZai_Speed = float.Parse(fishDeadNode.ChildNodes[19].FirstChild.Value);
        //ftd.BingDong_Speed = float.Parse(fishDeadNode.ChildNodes[20].FirstChild.Value);

        //FishSetting.FishDeadTime = ftd;
        ////end

        //ResManager.Instance.UnloadObject(obj);
        InitLogic.StartInit();
        SceneMain.Instance.StartInnerCoroutine(OnNewInit(null));

        Object obj = ResManager.Instance.LoadObject("FishConfig", "GlobalRes/ServerSetting/", ResType.GlobalRes, typeof(TextAsset));
        if (obj == null)
            return false;

        Object objErrorStr = ResManager.Instance.LoadObject("ErrorString", "GlobalRes/ServerSetting/", ResType.GlobalRes, typeof(TextAsset));
        if (objErrorStr == null)
            return false;

        InitLogic.StartInit();
        SceneMain.Instance.StartInnerCoroutine(FishConfig.Instance.LoadFishConfig(obj, objErrorStr));
        return true;
    }
    private static System.Collections.IEnumerator OnNewInit(object objd)
    {
        Object obj = ResManager.Instance.LoadObject("ServerSetting", "GlobalRes/ServerSetting/", ResType.GlobalRes, typeof(TextAsset));
        if (obj == null)
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        TextAsset ta = obj as TextAsset;
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(ta.text);
        XmlNode pConfig = doc.SelectSingleNode("Config");

        XmlNode verNode = pConfig.SelectSingleNode("ServerVersion");
        XmlNode comboNode = pConfig.SelectSingleNode("Combo");
        XmlNode energyNode = pConfig.SelectSingleNode("Energy");
        XmlNode cannonNode = pConfig.SelectSingleNode("Cannon");
        XmlNode skillNode = pConfig.SelectSingleNode("Skill");
        XmlNode rateNode = pConfig.SelectSingleNode("Rate");
        XmlNode fishNode = pConfig.SelectSingleNode("Fish");
        XmlNode rankNode = pConfig.SelectSingleNode("Rank");
        XmlNode dstNode = pConfig.SelectSingleNode("FishDeadTime");

        if (verNode == null ||
           comboNode == null ||
           energyNode == null ||
           skillNode == null ||
           rateNode == null ||
           cannonNode == null ||
           fishNode == null ||
            rankNode == null||
            dstNode==null)
        {
            LogMgr.Log("ServerSetting load failed.");
            InitLogic.SetInitFailed();
            yield break;
        }
        if (energyNode.ChildNodes.Count != cannonNode.ChildNodes.Count)
        {
            LogMgr.Log("energyNode<>cannonNode");
            InitLogic.SetInitFailed();
            yield break;
        }
        int nFishs = 0;
        int nFishValues = 0;
        LauncherInfo []plauncherinfo;


        yield return new WaitForEndOfFrame();
        Attribute(verNode, "wonderful_base",ref ExtraSetting.WonderFulBase);
        /////Ver
        //Attribute(verNode, "maxfish", ref FishSetting.FISH_MAX_NUM);
        //Attribute(verNode, "maxbullet", ref BulletSetting.BULLET_MAX_NUM);

        /////Combo
        Attribute(comboNode, "sustain_time", ref DoubleHit.IntervalTime);
        Attribute(comboNode, "buff_cycle", ref DoubleHit.MaxTimes); 

        ////Cannon
        int launcherCount = energyNode.ChildNodes.Count;
        plauncherinfo=new LauncherInfo[launcherCount];
        LauncherSetting.LauncherDataList = new LauncherData[launcherCount];
        for (int i = 0; i < launcherCount; ++i)//from energy
        {
            LauncherData ld = new LauncherData();
            
            XmlNode node = energyNode.ChildNodes[i];
             Attribute(node,"id",ref ld.ID);
            Attribute(node,"cditme",ref ld.LaserCDTime);
            Attribute(node,"speed", ref ld.LaserReduction.Speed);
            Attribute(node,"time1",ref ld.LaserReduction.Duration1);
            Attribute(node,"time2",ref ld.LaserReduction.Duration2);
            Attribute(node,"time3",ref ld.LaserReduction.Duration3);

            LauncherInfo Item = new LauncherInfo();          
            Attribute(node, "mincatch", ref Item.nmincatch);
            Attribute(node, "maxcatch", ref Item.nmaxcatch);
            Attribute(node, "revise", ref Item.nrevise);
            Attribute(node, "energyratio", ref Item.denergyratio);
 
          

            if (LauncherSetting.LauncherDataList[ld.ID] != null)
            {
                LogMgr.Log("Exist the same launcher id.");
                InitLogic.SetInitFailed();
                yield break;
            }
            LauncherSetting.LauncherDataList[ld.ID] = ld;
            plauncherinfo[ld.ID] = Item;
        }
        yield return new WaitForEndOfFrame();
        for (int i = 0; i < launcherCount; ++i)//from cannon
        {
            XmlNode node = cannonNode.ChildNodes[i];
            byte byid = 0;
            Attribute(node, "id", ref byid);
            LauncherData ld = LauncherSetting.LauncherDataList[byid];

            ld.name = node.Attributes["name"].Value;
            Attribute(node, "launcherinterval", ref ld.Interval);
            Attribute(node, "consume", ref ld.Consume);
            Attribute(node, "bulletspeed", ref ld.Speed);
            Attribute(node, "itemid", ref ld.nItemid);
            Attribute(node, "itemcount", ref ld.nItemcount);
            Attribute(node, "skill", ref ld.nSkillBind);
            
            if (byid == (byte)LauncherType.LAUNCHER_4)//4号炮特有
            {
                Attribute(node,"speed",ref BulletSetting.FreezeBulletReduction.Speed);
                Attribute(node,"time1",ref BulletSetting.FreezeBulletReduction.Duration1);
                Attribute(node,"time2",ref BulletSetting.FreezeBulletReduction.Duration2);
                Attribute(node,"time3",ref BulletSetting.FreezeBulletReduction.Duration3);                             
            }
        }
        yield return new WaitForEndOfFrame();
        /////SkillInfo
        int skillCount = skillNode.ChildNodes.Count;
        SkillSetting.SkillDataList = new SkillData[skillCount];
        for (int i = 0; i < skillCount; ++i)
        {
            SkillData sd = new SkillData();
            XmlNode node = skillNode.ChildNodes[i];
            Attribute(node, "id", ref sd.ID);
            sd.name = node.Attributes["name"].Value;
            Attribute(node, "cditme", ref sd.CDTime);
            Attribute(node, "goodsid", ref sd.ItemId);
            Attribute(node, "multiple", ref sd.multiple);
            Attribute(node, "speed", ref sd.Reduction.Speed);
            Attribute(node, "time1", ref sd.Reduction.Duration1);
            Attribute(node, "time2", ref sd.Reduction.Duration2);
            Attribute(node, "time3", ref sd.Reduction.Duration3);

            string szInfo="";
            Attribute(node, "goodsconsume", ref szInfo);
            if (szInfo.Length>0)
            {
                string[] pConsume = szInfo.Split(' ');
                for (int index = 0; index < pConsume.Length; index+=2)
                {
                    SkillConsume pskill = new SkillConsume();
                    pskill.byorder = System.Convert.ToByte(pConsume[index]);
                    pskill.byCount = System.Convert.ToByte(pConsume[index + 1]);
                    sd.NumRequire.Add(pskill);
                }               
            }
            if (SkillSetting.SkillDataList[sd.ID] != null)
            {
                LogMgr.Log("Exist the same skill id.");
                InitLogic.SetInitFailed();
                yield break;
            }
            SkillSetting.SkillDataList[sd.ID] = sd;
        }
        yield return new WaitForEndOfFrame();
        //Rate
        int rateCount = rateNode.ChildNodes.Count;
        BulletSetting.BulletRate = new ushort[rateCount];
        BulletSetting.RateUnlock = new ushort[rateCount];
        BulletSetting.RateReward = new ushort[rateCount];
        for (int i = 0; i < rateCount; ++i)
        {
            XmlNode node = rateNode.ChildNodes[i];
            byte byid=0;
            Attribute(node, "id", ref byid);
            Attribute(node, "value", ref BulletSetting.BulletRate[byid]);
            Attribute(node, "unlock", ref BulletSetting.RateUnlock[byid]);
            Attribute(node, "unlockreward", ref BulletSetting.RateReward[byid]);
        }

        //Fish
        int fishCount = fishNode.ChildNodes.Count;
         FishSetting.FishDataList = new FishData[fishCount];
        for (int i = 0; i < fishCount; ++i)
        {
            XmlNode node = fishNode.ChildNodes[i];
            FishData fd = new FishData();
            Attribute(node, "id", ref  fd.ID);
            Attribute(node, "value", ref fd.Gold);
            fd.Name = node.Attributes["name"].Value;
            fd.Decl = node.Attributes["declare"].Value;
            if (FishSetting.FishDataList[fd.ID] != null)
            {
                LogMgr.Log("Exist the same fish id:" + fd.ID);
                InitLogic.SetInitFailed();
                yield break;
            }
            FishSetting.FishDataList[fd.ID] = fd;

            byte maxcount = 0;
            Attribute(node, "maxcount", ref  maxcount);
            nFishs += maxcount;
            nFishValues += maxcount * fd.Gold;
        }
        FishSetting.SortByGold();
        yield return new WaitForEndOfFrame();
        //Rank
        int nRank = rankNode.ChildNodes.Count;
        Experience.Exp = new uint[nRank];
        for(int i=0;i<nRank;i++)
        {
            XmlNode node = rankNode.ChildNodes[i];
            byte byid = 0;
            Attribute(node, "id", ref  byid);
            Attribute(node, "experience", ref Experience.Exp[byid]);
        }

  
        ///
        //FishDeadTime
        FishDeadTimeData ftd = new FishDeadTimeData();

        Attribute(dstNode, "TianZai_DouDong", ref ftd.TianZai_DouDong_Time);//float.Parse(fishDeadNode.ChildNodes[0].FirstChild.Value);
        Attribute(dstNode, "TianZai_Stay_Min", ref ftd.TianZai_Stay_Time1);
        Attribute(dstNode, "TianZai_Stay_Max", ref ftd.TianZai_Stay_Time2);
        Attribute(dstNode, "TianZai_Dead_Min", ref ftd.TianZai_Dead_Time1);
        Attribute(dstNode, "TianZai_Dead_Max", ref ftd.TianZai_Dead_Time2);
        Attribute(dstNode, "Bullet_BingDong_Dead_Min", ref ftd.Bullet_BingDong_Dead_Time1);
        Attribute(dstNode, "Bullet_BingDong_Dead_Max", ref ftd.Bullet_BingDong_Dead_Time2);
        Attribute(dstNode, "BingDong_Dead_Min", ref ftd.BingDong_Dead_Time1);
        Attribute(dstNode, "BingDong_Dead_Max", ref ftd.BingDong_Dead_Time2);
        Attribute(dstNode, "ShanDian_Dead_Min", ref ftd.ShanDian_Dead_Time1);
        Attribute(dstNode, "ShanDian_Dead_Max", ref ftd.ShanDian_Dead_Time2);
        Attribute(dstNode, "LongJuanFeng_Dead_Min", ref ftd.LongJuanFeng_Dead_Time1);
        Attribute(dstNode, "LongJuanFeng_Dead_Max", ref ftd.LongJuanFeng_Dead_Time2);
        Attribute(dstNode, "JiGuang_Stay_Time", ref ftd.JiGuang_Stay_Time);
        Attribute(dstNode, "JiGuang_Dead_Min", ref ftd.JiGuang_Dead_Time1);
        Attribute(dstNode, "JiGuang_Dead_Max", ref ftd.JiGuang_Dead_Time2);
        Attribute(dstNode, "ShandDian_Speed", ref ftd.ShandDian_Speed);
        Attribute(dstNode, "JiGuang_Speed", ref ftd.JiGuang_Speed);
        Attribute(dstNode, "JiGuang_BingDong_Speed", ref ftd.JiGuang_BingDong_Speed);
        Attribute(dstNode, "TianZai_Speed", ref ftd.TianZai_Speed);
        Attribute(dstNode, "BingDong_Speed", ref ftd.BingDong_Speed);
        FishSetting.FishDeadTime = ftd;
        //cal
        for (int i = 0; i < launcherCount; i++)
        {
            LauncherInfo item = plauncherinfo[i];
            item.nincome = (ushort)((item.nmincatch + item.nmaxcatch) / 2.0f / nFishs * nFishValues + item.nrevise);
            LauncherSetting.LauncherDataList[i].Energy = (ushort)(item.nincome * item.denergyratio);//大招
        }
        yield return new WaitForEndOfFrame();

        int roomCount = 4;
        ExtraSetting.RoomDataList = new RoomData[roomCount];
        for (int i = 0; i < roomCount; ++i)
        {
            RoomData rd = new RoomData();
            rd.RoomID = (byte)i;
            rd.RoomRateIdx = (byte)i;
            ExtraSetting.RoomDataList[i] = rd;
        }


        //XmlNode childNode = pConfig.SelectSingleNode("RoomInfo");
        //if (childNode == null)
        //{
        //    LogMgr.Log("RoomInfo not found.");
        //    InitLogic.SetInitFailed();
        //    yield break;
        //}
        //int roomCount = childNode.ChildNodes.Count;
        //ExtraSetting.RoomDataList = new RoomData[roomCount];
        //for (int i = 0; i < roomCount; ++i)
        //{
        //    RoomData rd = new RoomData();
        //    rd.RoomID = byte.Parse(childNode.ChildNodes[i].Attributes["id"].Value);
        //    rd.RoomRateIdx = byte.Parse(childNode.ChildNodes[i].FirstChild.Value);
        //    if (ExtraSetting.RoomDataList[rd.RoomID] != null)
        //    {
        //        LogMgr.Log("Exist the same room id.");
        //        InitLogic.SetInitFailed();
        //        yield break;
        //    }
        //    ExtraSetting.RoomDataList[rd.RoomID] = rd;
        //}
        ResManager.Instance.UnloadObject(obj);
        InitLogic.EndInit();
    }
}
