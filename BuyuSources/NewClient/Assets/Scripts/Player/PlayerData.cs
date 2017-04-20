using UnityEngine;
using System.Collections.Generic;

public class PlayerData
{
    public string   Name;
    public uint     ID;         //玩家ID
    public byte     Level;      //等级
    public uint     ImgCrc;     //头像CRC
    public int      GoldNum;    //金币数量
}
public class PlayerExtraData
{
    public PlayerData playerData = new PlayerData();
}



