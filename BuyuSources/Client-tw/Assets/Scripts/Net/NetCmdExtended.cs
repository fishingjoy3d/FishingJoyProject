using System;
using UnityEngine;
using System.Collections.Generic;
//====================================================
//           添加自定义网络命令
//====================================================

//0.子弹发射与接收命令
public class NetCmdVector2
{
    public NetCmdVector2()
    {

    }
    public NetCmdVector2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
    public NetCmdVector2(Vector2 pos)
    {
        x = pos.x;
        y = pos.y;
    }
    [TypeInfo(0)]
    public float x;
    [TypeInfo(1)]
    public float y;
}
public class NetCmdVector3
{
    public NetCmdVector3()
    {

    }
    public NetCmdVector3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    public NetCmdVector3(Vector3 pos)
    {
        x = pos.x;
        y = pos.y;
        z = pos.z;
    }
    [TypeInfo(0)]
    public float x;
    [TypeInfo(1)]
    public float y;
    [TypeInfo(2)]
    public float z;
}
public class NetCmdVector4
{
    public NetCmdVector4()
    {

    }
    public NetCmdVector4(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    public NetCmdVector4(Vector4 pos)
    {
        x = pos.x;
        y = pos.y;
        z = pos.z;
        w = pos.w;
    }
    public NetCmdVector4(Quaternion pos)
    {
        x = pos.x;
        y = pos.y;
        z = pos.z;
        w = pos.w;
    }
    [TypeInfo(0)]
    public float x;
    [TypeInfo(1)]
    public float y;
    [TypeInfo(2)]
    public float z;
    [TypeInfo(3)]
    public float w;
}
public class PlayerBaseInfo
{
    [TypeInfo(0)]
	public uint	ID;
    [TypeInfo(1)]
	public byte	Lvl;
    [TypeInfo(2)]
	public uint	ImgCrc;
    [TypeInfo(3)]
	public bool	Sex;
    [TypeInfo(4)]
	public int	GoldNum;
    [TypeInfo(5, 7)]
	public string	Name;
};

public class NetReductionData
{
    [TypeInfo(0)]
    public byte Speed;
    [TypeInfo(1)]
    public byte Reduction1;
    [TypeInfo(2)]
    public byte Reduction2;
    [TypeInfo(3)]
    public byte Reduction3;
}
public class NetCmdBullet : NetCmdBase
{
    [TypeInfo(0)]
    public ushort   BulletID;
    [TypeInfo(1)]
    public short    Degree;
    [TypeInfo(2)]
    public byte     LauncherType;
    [TypeInfo(3)]
    public uint     Energy;
    [TypeInfo(4)]
    public byte     ReboundCount;
    [TypeInfo(5)]
    public ushort   LockFishID;
}

//1.鱼发射命令
public class NetCmdFish : NetCmdBase
{
    [TypeInfo(0)]
    public ushort GroupID;
    [TypeInfo(1)]
    public ushort PathID;
    [TypeInfo(2)]
    public ushort StartID;
}
public enum FishCatchEventType
{
    // 1 捕获， 2，受击，3.播放特效
    CATCH_EVENT_CATCHED = 1,
    CATCH_EVENT_ATTACK,
    CATCH_EVENT_EFFECT,
    CATCH_EVENT_CATCHED_LIGHTING,
    CATCH_EVENT_CATCHED_LIGHTING_MAIN,
};
public  class NetFishCatched
{
    [TypeInfo(0)]
    public byte CatchEvent;
    [TypeInfo(1)]
    public ushort FishID;
    [TypeInfo(2)]
    public ushort nReward;
    [TypeInfo(3)]
    public ushort LightingFishID;
}
public class NetFishDeadTime
{
    [TypeInfo(0)]
    public ushort   FishID;
    [TypeInfo(1)]
    public byte     DeadTime;
    [TypeInfo(2)]
    public ushort nReward;
    [TypeInfo(3)]
    public ushort LightingFishID;
}
public class NetCmdCatched : NetCmdBase
{
    [TypeInfo(0)]
    public ushort BulletID;
    [TypeInfo(1)]
    public ushort Combo;
    [TypeInfo(2)]
    public uint GoldNum;
    [TypeInfo(3)]
    public uint TotalNum;
    [TypeInfo(4, true)]
    public NetFishCatched[] Fishs;
}
//4.加入
public class NetCmdPlayerJoin : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat; 
    [TypeInfo(1)]
    public byte LauncherType;   //最高位是否有效,后七位是炮台索引
    [TypeInfo(2)]
    public byte rateIndex; 
    [TypeInfo(3)]
    public PlayerBaseInfo PlayerInfo;
}

//5.离开
public class NetCmdPlayerLeave : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
}
public class NetCmdChangeLauncher : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
    [TypeInfo(1)]
    public byte LauncherType;   //发送时：1表示向上加，0表示向下减。接收时：最高位表示是否有效。
}
public class NetCmdChangeLauncherType : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
    [TypeInfo(1)]
    public byte LauncherType; 
}
public class NetCmdClientChangeRate : NetCmdBase
{
    [TypeInfo(0)]
    public bool UpRoDownRate;
}
public class NetCmdChangeRate : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
    [TypeInfo(1)]
    public byte RateIndex;
    [TypeInfo(2)]
    public bool IsCanUseRate;//当前的倍率玩家是否可以使用 发送的时候 无须携带
} 
public class NetCmdChangeRateType : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
    [TypeInfo(1)]
    public byte RateIndex;
}
public class NetCmdSyncFish : NetCmdBase
{
    [TypeInfo(0)]
    public ushort FishID;
    [TypeInfo(1)]
    public ushort GroupID;
    [TypeInfo(2)]
	public float  FishTime;
    [TypeInfo(3)]
	public ushort PathGroup;
    [TypeInfo(4)]
	public ushort PathIdx;
    [TypeInfo(5)]
    public bool  IsActiveEvent;
    [TypeInfo(6)]
    public ushort ElapsedTime;
    [TypeInfo(7)]
    public byte DelayType;
    [TypeInfo(8)]
    public byte DelayScaling;	//减速百分比
    [TypeInfo(9)]
    public byte DelayDuration1;
    [TypeInfo(10)]
    public byte DelayDuration2;
    [TypeInfo(11)]
    public byte DelayDuration3;
    [TypeInfo(12)]
    public ushort DelayCurrentTime;
    [TypeInfo(13)]
    public byte Package;
}

public class NetCmdRoomInfo : NetCmdBase
{
    [TypeInfo(0)]
    public byte BackgroundImage;
    [TypeInfo(1, 10)]
    public string RoomName;
}

public class SyncBulletData
{
    [TypeInfo(0)]
	public ushort   BulletID;
    [TypeInfo(1)]
    public short    Degree;
    [TypeInfo(2)]
    public ushort   Time;
    [TypeInfo(3)]
    public byte     BulletType;
    [TypeInfo(4)]
    public byte     RateIdx;
    [TypeInfo(5)]
    public byte     ReboundCount;
    [TypeInfo(6)]
    public ushort   LockFishID;
    /*
    [TypeInfo(5)]
    public byte     CollideCount;
    [TypeInfo(6)]
    public ushort   PauseTime;
    [TypeInfo(7)]
    public ushort   SpeedScaling;
    */
}
public class NetCmdSyncBullet :  NetCmdBase
{
    [TypeInfo(0, true)]
    public SyncBulletData[] Bullets;
}
public class NetCmdSkillTornado : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
    [TypeInfo(1)]
    public uint GoldNum;
    [TypeInfo(2)]
    public uint TotalNum;
    [TypeInfo(3, true)]
    public NetFishDeadTime[] fishID;
}
public class NetCmdSkillFreeze:NetCmdBase
{
    [TypeInfo(0)]
    public byte	    Seat;
    [TypeInfo(1)]
    public uint GoldNum;
    [TypeInfo(2)]
    public uint TotalNum;
    [TypeInfo(3, true)]
    public NetFishDeadTime[] FishID;
}
public class NetCmdSkillLighting : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
    [TypeInfo(1)]
    public uint GoldNum;
    [TypeInfo(2)]
    public uint TotalNum;
    [TypeInfo(3, true)]
    public NetFishDeadTime[] FishID;
}
public class NetCmdSkillLaser: NetCmdBase
{
    [TypeInfo(0)]
    public short Degree;
}
public class NetCmdSkillLaserResponse : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
    [TypeInfo(1)]
    public byte LaserType;
    [TypeInfo(2)]
    public short Degree;
    [TypeInfo(3)]
    public uint GoldNum;
    [TypeInfo(4)]
    public uint TotalNum;
    [TypeInfo(5, true)]
    public NetFishDeadTime[] FishID;
}
public class NetCmdReduction: NetCmdBase
{
    [TypeInfo(0)]
    public byte ReductionType;// 1: Laser,
    [TypeInfo(1)]
    public byte LaserType;
    [TypeInfo(2, true)]
    public ushort[] FishID;
}
public class NetCmdClearScene : NetCmdBase
{
    [TypeInfo(0)]
    public byte ClearType;      //0从左到右，1从右到左，2重连清场。
}
public class NetCmdSkillDisaster : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
    [TypeInfo(1)]
    public uint GoldNum;
    [TypeInfo(2)]
    public uint TotalNum;
    [TypeInfo(3, true)]
    public NetFishDeadTime[] FishID;
}
public class NetCmdSkillLock : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
}
public class NetCmdBulletReduction : NetCmdBase
{
    [TypeInfo(0)]
    public ushort BulletID;
    [TypeInfo(1)]
    public ushort Combo;
    [TypeInfo(2)]
    public uint GoldNum;
    [TypeInfo(3)]
    public uint TotalNum;
    [TypeInfo(4, true)]
    public NetFishCatched[] FishID;
}
public class NetCmdUploadImgRequest:NetCmdBase
{
    [TypeInfo(0)]
    public ushort Size;
}
public class NetCmdUploadImgResponse: NetCmdBase
{
    [TypeInfo(0)]
    public byte AllowUpload;
}
public class NetCmdUploadImgChunk : NetCmdBase
{
    [TypeInfo(0)]
    public ushort StartIndex;
    [TypeInfo(1)]
    public ushort Size;
    [TypeInfo(2, true)]
    public byte[] ImgData;
}
public class NetCmdUploadImgChunkResponse : NetCmdBase
{
    [TypeInfo(0)]
    public byte UploadResult;
}
public class NetCmdUploadImgComplete : NetCmdBase
{
    [TypeInfo(0)]
    public byte Result;
    [TypeInfo(1)]
    public uint ImgCrc;
}
public class NetCmdImgChange : NetCmdBase
{
    [TypeInfo(0)]
    public byte Seat;
    [TypeInfo(1)]
    public uint PlayerID;
    [TypeInfo(2)]
	public uint ImgCrcValue;
};
public class NetCmdFishPackage : NetCmdBase
{
    [TypeInfo(0)]
    public ushort FishID;
    [TypeInfo(1)]
    public byte PackageType;
};
public class NetCmdJoinRoom : NetCmdBase
{
    [TypeInfo(0)]
    public byte RoomType;
}
public class CheckFishPos
{
    [TypeInfo(0)]
    public ushort FishID;
    [TypeInfo(1)]
    public float Time;
    [TypeInfo(2)]
    public float Speed;
    [TypeInfo(3)]
    public NetCmdVector3 Pos;
    [TypeInfo(4)]
    public NetCmdVector4 Rot;
}
public class NetCmdCheckFishPos:NetCmdBase
{
    [TypeInfo(0)]
    public ushort FishNum;
    [TypeInfo(1, true)]
    public CheckFishPos[] Fish;
}
public class CheckBulletPos
{
    [TypeInfo(0)]
    public ushort ID;
    [TypeInfo(1)]
    public NetCmdVector3 Pos;
}
public class NetCmdCheckBulletPos : NetCmdBase
{
    [TypeInfo(0)]
    public ushort Count;
    [TypeInfo(1, true)]
    public CheckBulletPos[] Bullets;
}
public class NetCmdLaunchFailed : NetCmdBase
{
    [TypeInfo(0)]
    public byte FailedType;
    [TypeInfo(1)]
    public int Energy;
}
public class NetCmdSkillFailed : NetCmdBase
{
    [TypeInfo(0)]
    public byte FailedType;
    [TypeInfo(1)]
    public byte bySkillType;
    [TypeInfo(2)]
    public ushort uTimes;
}