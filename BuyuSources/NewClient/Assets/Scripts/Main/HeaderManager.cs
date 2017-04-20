using UnityEngine;
using System.Collections.Generic;

public enum HeaderOptType
{
    HEADER_UPLOAD,          //玩家上传头像。
    HEADER_ME,              //打开自己的头像
    HEADER_SCENE,           //场景中获取玩家头像
    HEADER_FRIEND,          //好友列表头像。
    HEADER_FIND,            //查找好友头像.
    HEADER_BLACK,           //黑名单头像
    HEADER_RANK,            //排行榜头像
    HEADER_MATCH,           //比赛排行榜头像
    HEADER_CARDS,           //名片头像
    HEADER_NIUNIU,          //牛牛自己头像
    HEADER_NIUNIUHOST,      //牛牛庄家头像
    HEADER_NIUNIUVIP,       //牛牛vip头像
    HEADER_NIUNIUNORMAL,    //牛牛大众席头像
    HEADER_MATCHRANK,       //场景中比赛排行榜头像
    HEADER_FORESTDANCE,     //森林舞会玩家头像
    HEADER_FORESTDANCEVIP,  //森林舞会vip头像
    HEADER_FORESTDANCEHOST, //森林舞会庄家头像
    HEADER_FORESTDANCENORMAL,//森林舞会大众席头像
    HEADER_CAR_SELF, //自己
    HEADER_CAR_HOST,  //庄家
    HEADER_CAR_PlAYER_LIST,//大众席
    HEADER_GAME_SHARE,//分享
}
class UploadData
{
    public Texture2D Tex;
    public WWW W;
    public byte[] TexData;
    public ushort UploadSize;
}
class DownPlayerHeader
{
    public byte Opt;
    public uint PlayerID;
    public uint Crc;
    public bool Local;
    public WWW  W;
    public DownloadData DownData;
    public object extraData;
}
public class HeaderManager:Singleton<HeaderManager>
{
    const int UPLOAD_TIMEOUT = 10;
    UploadData m_Upload = null;
    List<DownPlayerHeader> m_DownList = new List<DownPlayerHeader>();
    DownPlayerHeader m_ActiveDown = null;
    float   m_fTime = 0;
    bool    m_bBeginUpload = false;
    public void BeginUpload()
    {
        NetCmdUploadImgRequest upload = new NetCmdUploadImgRequest();
        upload.SetCmdType(NetCmdType.CMD_UPLOADING_REQUEST);
        upload.Size = (ushort)m_Upload.TexData.Length;
        NetServices.Instance.Send<NetCmdUploadImgRequest>(upload);
    }
    void SendChunk()
    {
        //上传
        NetCmdUploadImgChunk chunk = new NetCmdUploadImgChunk();
        chunk.SetCmdType(NetCmdType.CMD_UPLOADING_CHUNK);
        chunk.StartIndex = m_Upload.UploadSize;
        chunk.Size = (ushort)Mathf.Min(64, m_Upload.TexData.Length - m_Upload.UploadSize);
        chunk.ImgData = new byte[chunk.Size];
        System.Array.Copy(m_Upload.TexData, m_Upload.UploadSize, chunk.ImgData, 0, chunk.Size);
        m_Upload.UploadSize += chunk.Size;
        NetServices.Instance.Send<NetCmdUploadImgChunk>(chunk);
        m_fTime = 0;
        int progress = (int)(m_Upload.UploadSize / (float)m_Upload.TexData.Length * 100);

   
        GlobalHallUIMgr.Instance.ShowSystemTipsUI(string.Format(StringTable.GetString("uploading"), progress), UPLOAD_TIMEOUT * 2, false);
    }
    void ClearUpload()
    {
        m_bBeginUpload = false;
        m_Upload = null;
    }
    public bool HandleCmd(NetCmdPack pack)
    {
        switch(pack.cmd.GetCmdType())
        {
            case NetCmdType.CMD_UPLOADING_RESPONSE:
                NetCmdUploadImgResponse rep = (NetCmdUploadImgResponse)pack.cmd;
                if(rep.AllowUpload == 0)
                {
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("uploadfailed"), 2.0f, false);
                    ClearUpload();
                }
                else if(m_Upload != null)
                {
                    m_bBeginUpload = true;
                    SendChunk();
                }
                break;
            case NetCmdType.CMD_UPLOADING_CHUNK_RESPONSE:
                NetCmdUploadImgChunkResponse rep2 = (NetCmdUploadImgChunkResponse)pack.cmd;
                if ((rep2.UploadResult & 1) == 0)
                {
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("uploadfailed"), 2.0f, false);
                    ClearUpload();
                }
                else if((rep2.UploadResult & 128) != 0)
                {
                    //LogMgr.Log("上传完成1.");
                }
                else if (m_Upload != null)
                {
                    SendChunk();
                }
                break;
            case NetCmdType.CMD_UPLOADING_COMPLETE:
                {
                    if (m_Upload != null)
                    {
                        NetCmdUploadImgComplete rep3 = (NetCmdUploadImgComplete)pack.cmd;
                        if(rep3.Result == 1)
                        {
                            SaveTex(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), rep3.ImgCrc, m_Upload.TexData);
                            //PlayerRole.Instance.RoleInfo.ChangeRoleCustFaceID(rep3.ImgCrc);
                            GetPlayerHeaderResult(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), m_Upload.Tex, HeaderOptType.HEADER_UPLOAD, null);
                            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("uploadok"), 2.0f, false);
                        }
                        else
                        {
                            //上传失败。
                            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("uploadfailed"), 2.0f, false);
                        }
                    }
                    ClearUpload();
                }
                break;
            default:
                return false;
        }
        return true;
    }
    bool SaveTex(uint id, uint crc, byte[] data)
    {
        try
        {
            string path = RuntimeInfo.GetLocalDirectory() + "/images/" + id + "_" + crc + ".jpg";
            System.IO.File.WriteAllBytes(path, data);
            return true;
        }
        catch
        {
        }
        return false;
    }
    void CheckDownList()
    {
        if(m_ActiveDown != null)
        {
            if (m_ActiveDown.DownData != null)
            {
                if(m_ActiveDown.DownData.IsCompletion)
                {
                    if(m_ActiveDown.DownData.IsOK)
                    {
                        bool bret = SaveTex(m_ActiveDown.PlayerID, m_ActiveDown.Crc, m_ActiveDown.DownData.Bytes);
                        if (bret)
                        {
                            m_ActiveDown.DownData = null;
                            SceneMain.Instance.StartInnerCoroutine(GetHeader(m_ActiveDown));
                        }
                        else
                        {
                            GetPlayerHeaderResult(m_ActiveDown.PlayerID, null, (HeaderOptType)m_ActiveDown.Opt, m_ActiveDown.extraData);
                            m_ActiveDown.DownData = null;
                        }
                        
                    }
                    else
                    {
                        GetPlayerHeaderResult(m_ActiveDown.PlayerID, null, (HeaderOptType)m_ActiveDown.Opt, m_ActiveDown.extraData);
                        m_ActiveDown = null;
                    }
                }
            }
            else if(m_ActiveDown.W.error != null)
            {
                GetPlayerHeaderResult(m_ActiveDown.PlayerID, null, (HeaderOptType)m_ActiveDown.Opt, m_ActiveDown.extraData);
                m_ActiveDown = null;
            }
            else if(m_ActiveDown.W.isDone)
            {
                if (!m_ActiveDown.Local)
                    SaveTex(m_ActiveDown.PlayerID, m_ActiveDown.Crc, m_ActiveDown.W.texture.EncodeToJPG());
                GetPlayerHeaderResult(m_ActiveDown.PlayerID, m_ActiveDown.W.texture, (HeaderOptType)m_ActiveDown.Opt, m_ActiveDown.extraData);
                m_ActiveDown = null;
            }
        }
        if(m_ActiveDown == null)
        {
            if(m_DownList.Count > 0)
            {
                m_ActiveDown = m_DownList[0];
                Utility.ListRemoveAt(m_DownList, 0);
                SceneMain.Instance.StartInnerCoroutine(GetHeader(m_ActiveDown));
            }
        }
    }
    public void Update(float delta)
    {
        CheckDownList();

        if (m_bBeginUpload)
        {
            m_fTime += delta;
            if (m_fTime > UPLOAD_TIMEOUT)
            {
                ClearUpload();
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("uploadfailed"), 2.0f, false);
                return;
            }
        }
        if (m_Upload == null || m_Upload.W == null)
            return;
        
        if (m_Upload.W.error != null)
        {
            m_Upload = null;
            LogMgr.Log("从本地加载图片失败.");

            return;
        }
        if(m_Upload.W.isDone)
        {
            m_Upload.Tex = m_Upload.W.texture;
            m_Upload.TexData = m_Upload.Tex.EncodeToJPG();
            if(m_Upload.TexData.Length > short.MaxValue)
            {
                LogMgr.Log("图片大小超出限制.");
                m_Upload = null;
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Change_Header_Field"), 3);
                return;
            }
            m_Upload.W = null;
            BeginUpload();
        }
    }
    public void UploadHeader(string path)
    {
        if (m_Upload != null)
            return;
        string url;
        if (!System.IO.File.Exists(path))
        {
            LogMgr.Log("图片不存在.");
            return;
        }
        url = "file:///" + path;
        m_Upload = new UploadData();
        m_Upload.W = new WWW(url);
    }
    //获取头像
    System.Collections.IEnumerator GetHeader(object obj)
    {
        DownPlayerHeader dph = (DownPlayerHeader)obj;
        string pic = dph.PlayerID + "_" + dph.Crc + ".jpg";
        string path = RuntimeInfo.GetLocalDirectory() + "/images/" + pic;
        if (System.IO.File.Exists(path))
        {
            dph.Local = true;
            dph.W = new WWW("file:///" + path);
        }
        else
        {
            dph.Local = false;
            dph.DownData = FTPClient.Instance.AddDownload(DownloadType.DOWNLOAD_DEFAULT, ServerSetting.RT_IMAGE_DIR + pic, null);
        }
        yield break;
    }
    public void GetPlayerHeader(uint playerid, uint crc, HeaderOptType opt, object extraData)
    {
        DownPlayerHeader dph = new DownPlayerHeader();
        dph.PlayerID = playerid;
        dph.Crc = crc;
        dph.Opt = (byte)opt;
        dph.extraData = extraData;
        m_DownList.Add(dph);
        //SceneMain.Instance.StartInnerCoroutine(GetHeader(dph));
    }
    //玩家的头像获取结果。
    public void GetPlayerHeaderResult(uint id, Texture2D tex, HeaderOptType opt, object extraData)
    {
        switch(opt)
        {
            case HeaderOptType.HEADER_ME:
                GlobalHallUIMgr.Instance.SetMyHeader(tex);
                if (SceneRuntime.Instance != null)
                    SceneRuntime.LogicUI.SetMySelfHeader(tex);
                break;
            case HeaderOptType.HEADER_UPLOAD:   //上传
                GlobalHallUIMgr.Instance.SetMyHeader(tex);
                if (HallRunTimeInfo.Instance_UI != null)
                    HallRunTimeInfo.Instance_UI.m_loginUi.SetMyHeader(tex);
                break;
            case HeaderOptType.HEADER_SCENE:    //场景中玩家头像
                if(SceneRuntime.Instance != null)
                    SceneRuntime.PlayerMgr.SetPlayerHeader(id, (byte)extraData, tex);
                break;
            case HeaderOptType.HEADER_FRIEND:   //好友
                if (FriendRuntime.Instance != null)
                {
                    FriendRuntime.FriendSysMgr.SetHeader(id, (byte)extraData, tex);
                }
                break;
            case HeaderOptType.HEADER_FIND:     //查找好友中玩家头像
                if (FriendRuntime.Instance != null)
                {
                    FriendRuntime.SearchUI.SetHeader(id, tex);
                }
                break;  
            case HeaderOptType.HEADER_BLACK:    //黑名单中玩家头像
                if (FriendRuntime.Instance != null)
                {
                    FriendRuntime.BlacklistUI.SetHeader(id, tex);
                }
                break;
            case HeaderOptType.HEADER_RANK:     //排行榜
                if (HallRunTimeInfo.Instance_UI != null)
                {
                    HallRunTimeInfo.Instance_UI.m_loginUi.SetInformationHeader(id, (byte)extraData, tex);
                }
                break;
            case HeaderOptType.HEADER_MATCH:    //比赛排行榜数据
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.SetHeader(id, tex);
                    }
                }
                break;
            case HeaderOptType.HEADER_CARDS:    //名片头像
                {
                    GlobalHallUIMgr.Instance.SetCardsHeader(id, tex);
                }
                break;
            case HeaderOptType.HEADER_NIUNIU:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.SetPlayerHeader(tex);
                    }
                }
                break;
            case HeaderOptType.HEADER_NIUNIUHOST:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.SetNiuniuHostHeader(tex);
                    }
                }
                break;
            case HeaderOptType.HEADER_NIUNIUVIP:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.NiuniuSetVipHead((byte)extraData, tex);
                    }
                }
                break;
            case HeaderOptType.HEADER_NIUNIUNORMAL:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.NiuniuSetNormalHead(tex, id, (byte)extraData);
                    }
                }
                break;
            case HeaderOptType.HEADER_MATCHRANK:
                {
                    if (SceneRuntime.Instance != null)
                        SceneRuntime.LogicUI.SetSceneMatchRankHeader(tex, id);
                }
                break;
            case HeaderOptType.HEADER_FORESTDANCE:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.ForestDanceSetPlayerHead(tex);
                    }
                }
                break;
            case HeaderOptType.HEADER_FORESTDANCEVIP:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.ForestDanceSetVipHead((byte)extraData, tex);
                    }
                }
                break;
            case HeaderOptType.HEADER_FORESTDANCEHOST:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.ForestDanceSetHostHead(tex);
                    }
                }
                break;
            case HeaderOptType.HEADER_FORESTDANCENORMAL:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.ForestDanceSetNormalHead(tex, id, (byte)extraData);
                    }
                    break;
                }
            case HeaderOptType.HEADER_CAR_SELF:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.SetCarAvetarSelf(tex);
                    }

                    break;
                }
            case HeaderOptType.HEADER_CAR_HOST:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.SetCarAvetarHost(tex);
                    }
                    break;
                }
            case HeaderOptType.HEADER_CAR_PlAYER_LIST:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.SetCarAvetarPlayList(tex, id, (byte)extraData);
                    }
                    break;
                }
            case HeaderOptType.HEADER_GAME_SHARE:
                {
                    if (GlobalHallUIMgr.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.GameShare.SetAvatar(tex);
                    }
                    break;
                }
        }
    }
}
