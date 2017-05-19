using System;
using System.Collections.Generic;
using UnityEngine;

/*
 *  变长命令发送流程参考
    NetCmdCollide ncc = (NetCmdCollide)NetCmdHelper.CreateCmdData(NetCmdType.CMD_COLLIDE);
    ncc.BulletID = 111;
    ncc.BulletTime = 1212;
    ncc.FishList.FishID = 321;
    ncc.FishList.FishTime = 432;
    object[] fishs = new object[5];
    for(ushort i = 0; i < fishs.Length; ++i)
    {
        NetFishList nfl = new NetFishList();
        nfl.FishID = (ushort)(i + 100);
        nfl.FishTime = (ushort)(i * 100);
        fishs[i] = nfl;
    }
    NetCmdObjEx objEx = new NetCmdObjEx(ncc);
    objEx.VarArray = fishs;
    TCPClient.Instance.Send(objEx);
*/

public class NetCmdHandler
{
//    static public bool HandleCmd(NetCmdObj obj)
//    {
//        switch (obj.Cmd.GetCmdType())
//        {
//            case NetCmdType.CMD_HEARTBEAT:
//                {
//                    //临时：在线人数存储于心跳的字节大小中。
//                    //TCPClient.Instance.ServerOnlineCount = obj.Cmd.GetCmdSize();
//                }
//                break;
//            case NetCmdType.CMD_BULLET:
//                {
//                    NetCmdBullet bullet = (NetCmdBullet)obj.Cmd;
//                    //LauncherManager.Instance.PlayerReceiveBullet(bullet);
//                }
//                break;
//            case NetCmdType.CMD_FISH:
//                {
//                    NetCmdFish fish = (NetCmdFish)obj.Cmd;
//                    FishManager.Instance.FishLauncher.LaunchFish(FishManager.Instance.FishList, fish.GroupID, fish.PathID, fish.StartID);
//                }
//                break;
//            case NetCmdType.CMD_CATCHED:
//                {
//                    NetCmdCatched fish = (NetCmdCatched)obj.Cmd;
//                    //LauncherManager.Instance.PlayerReceiveFishColliderMsg(fish);
//                }
//                break;
//            case NetCmdType.CMD_PLAYER_JOIN:
//                {
//                    NetCmdPlayerJoin player = (NetCmdPlayerJoin)obj.Cmd;
//                    //int count = Utility.GetByteCharCount(player.Name);
//                    string playerName = "Player";// System.Text.Encoding.Unicode.GetString(player.Name, 0, count);
//                    LauncherManager.Instance.PlayerJoin(player, playerName);
//                    string str = "[" + playerName + "]坐在了" + player.Seat.ToString() + "号位置!";
//                    CameraTex.Instance.ChangePlayerImg(player.PlayerID, player.ImgCrcValue);
//                    if (player.Self == 1)
//                    {
                        
//                        PlayerInF inf = FishSetting.GetMyPlayerInF();
//                        RuntimeInfo.SelfPlayerID = player.PlayerID;
//                        inf.m_PlayerID = player.PlayerID;
//                        FishSetting.SetMyPlayerInf(inf);
//                    }
                    
//                    NoticeManager.Instance.PlayNotice(str);
//                }
//                break;
//            case NetCmdType.CMD_PLAYER_LEAVE:
//                {
//                    NetCmdPlayerLeave player = (NetCmdPlayerLeave)obj.Cmd;
//                    string playerName = LauncherManager.Instance.GetPlayerNameBySeat(player.Seat);
//                    NoticeManager.Instance.PlayNotice("[" + playerName + "]离开了!");
//                    LauncherManager.Instance.PlayerLeave(player);
//                }
//                break;
//            case NetCmdType.CMD_CHANGE_SEAT:
//                {
//                    NetCmdChangeSeat player = (NetCmdChangeSeat)obj.Cmd;
//                    LauncherManager.Instance.PlayerChangeSeat(player);
//                }
//                break;
            
//            case NetCmdType.CMD_CHANGE_LAUNCHER:
//                {
//                    NetCmdChangeLauncher changeLauncher = (NetCmdChangeLauncher)obj.Cmd;
                    
//                    LauncherManager.Instance.PalyerChangeLauncher(changeLauncher.Seat, changeLauncher.LauncherType);
//                }
//                break;
//            case NetCmdType.CMD_CHANGE_RATE:
//                {
//                    NetCmdChangeRate changeRate = (NetCmdChangeRate)obj.Cmd;
//                    LauncherManager.Instance.PlayerChageRate(changeRate.Rate);
//                }
//                break;
//            case NetCmdType.CMD_ROOM_INFO:
//                {
//                    NetCmdRoomInfo room = (NetCmdRoomInfo)obj.Cmd;
//                    SceneMain.m_RoomName = room.RoomName;
//                    SceneObjManager.Instance.SwapBackground(room.BackgroundImage);
//                }
//                break;
//            case NetCmdType.CMD_GLOBAL_SETTING:
//                {
//                    NetCmdGlobalSetting set = (NetCmdGlobalSetting)obj.Cmd;
//                    if (GlobalSetting.GameSetting.ClientVersion != set.Version)
//                    {
//                        //版本号不匹配
//                        SceneMain.ShowInfoAndExit(StringTable.Instance.GetString("version_error"));
//                        return false;
//                    }

//                    GlobalSetting.GameSetting.Bullet_MaxNum = set.Bullet_MaxNum;
//                    GlobalSetting.GameSetting.Fish_MaxNum = set.Fish_MaxNum;
//                    GlobalSetting.GameSetting.BulletSpeed = set.Bullet_Speed;
//                    GlobalSetting.GameSetting.MultipleEffect = set.Multiple_Effect;
//                    for (int i = 0; i < RuntimeInfo.MAX_LAUNCHER_NUM; ++i)
//                    {
//                        GlobalSetting.GameSetting.LauncherInterval[i] = set.Launcher_Interval[i] * RuntimeInfo.MinDurationTime;
//                        GlobalSetting.GameSetting.LaserCDTime[i] = set.Laser_CDTime[i] * RuntimeInfo.MinDurationTime;
//                        GlobalSetting.GameSetting.EnergyThreshold[i] = set.Energy_Threshold[i];
//                        GlobalSetting.GameSetting.BulletConsume[i] = set.Bullet_Consume[i];
//                        Utility.ReductionToFloat(set.LaserReduction[i].Speed,
//                        set.LaserReduction[i].Reduction1,
//                        set.LaserReduction[i].Reduction2,
//                        set.LaserReduction[i].Reduction3,
//                        out GlobalSetting.GameSetting.LaserReduction[i].Speed,
//                        GlobalSetting.GameSetting.LaserReduction[i].Reduction);
//                    }
//                    for (int i = 0; i < RuntimeInfo.MAX_SKILL_NUM; ++i)
//                    {
//                        GlobalSetting.GameSetting.SkillCDTime[i] = set.Skill_CDTime[i] * RuntimeInfo.MinDurationTime;
//                    }
//                    for (byte i = 0; i < RuntimeInfo.BULLET_RATES; ++i )
//                    {
//                        GlobalSetting.GameSetting.BulletRate[i] = set.Bullet_Rate[i];
//                    }
//                    for (byte i = 0; i < RuntimeInfo.FISH_TYPES; ++i )
//                    {
//                        GlobalSetting.GameSetting.FishValue[i] = set.Fish_Value[i];
//                    }

//                        Utility.ReductionToFloat(set.SkillFreezeReduction.Speed,
//                            set.SkillFreezeReduction.Reduction1,
//                            set.SkillFreezeReduction.Reduction2,
//                            set.SkillFreezeReduction.Reduction3,
//                            out GlobalSetting.GameSetting.SkillFreezeReduction.Speed,
//                            GlobalSetting.GameSetting.SkillFreezeReduction.Reduction);

//                    Utility.ReductionToFloat(set.BulletFreezeReduction.Speed,
//                        set.BulletFreezeReduction.Reduction1,
//                        set.BulletFreezeReduction.Reduction2,
//                        set.BulletFreezeReduction.Reduction3,
//                        out GlobalSetting.GameSetting.BulletFreezeReduction.Speed,
//                        GlobalSetting.GameSetting.BulletFreezeReduction.Reduction);
//                }
//                break;
//            case NetCmdType.CMD_CHANGE_IMG:
//                {
//                    NetCmdImgChange cmd = (NetCmdImgChange)obj.Cmd;
//                    CameraTex.Instance.ChangePlayerImg(cmd.PlayerID, cmd.ImgCrcValue);
//                }
//                break;
//            case NetCmdType.CMD_UPLOADING_RESPONSE:
//            case NetCmdType.CMD_UPLOADING_CHUNK_RESPONSE:
//            case NetCmdType.CMD_UPLOADING_COMPLETE:
//                HeadPhotoUpload.Instance.m_UploadCmdList.Add(obj.Cmd);
//                break;
//            default:
//                Debug.Log("未知的网络命令:" + obj.Cmd.GetCmdType().ToString());
//                return false;
//        }
//        return true;
//    }
}

