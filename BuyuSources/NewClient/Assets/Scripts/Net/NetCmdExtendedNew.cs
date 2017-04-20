using System;
using System.Collections.Generic;

public class NetCmdRegisterLogon : NetCmdBase
{
    [TypeInfo(0)]
    public uint CRC1;
    [TypeInfo(1)]
    public uint CRC2;
    [TypeInfo(2)]
    public uint CRC3;
    [TypeInfo(3, 16)]
    public string UID;
}
public class NetCmdFastLogon : NetCmdBase
{
    
}
public class NetCmdNormalLogon : NetCmdBase
{
    [TypeInfo(0)]
    public uint CRC1;
    [TypeInfo(1)]
    public uint CRC2;
    [TypeInfo(2)]
    public uint CRC3;
    [TypeInfo(3, 16)]
    public string UID;
}
public class NetCmdLogonResultSimple : NetCmdBase
{
    [TypeInfo(0)]
    public byte     Result;
    [TypeInfo(1)]
    public uint     ServerIP;
    [TypeInfo(2)]
    public ushort   ServerPort;
    [TypeInfo(3)]
    public uint     PlayerID;
}

public class NetCmdLogonResult : NetCmdBase
{
    [TypeInfo(0)]
    public uint CRC1;
    [TypeInfo(1)]
    public uint CRC2;
    [TypeInfo(2)]
    public uint CRC3;
    [TypeInfo(3)]
    public byte Result;
    [TypeInfo(4)]
    public uint ServerIP;
    [TypeInfo(5)]
    public ushort ServerPort;
    [TypeInfo(6)]
    public uint PlayerID;
    [TypeInfo(7, 16)]
    public string UID;
}
public class NetCmdLogonHall : NetCmdBase
{
    [TypeInfo(0)]
    public uint CRC1;
    [TypeInfo(1)]
    public uint CRC2;
    [TypeInfo(2)]
    public uint CRC3;
    [TypeInfo(3, 16)]
    public string UID;
}
public class NetCmdLogonHallResult : NetCmdBase
{
    [TypeInfo(0)]
    public byte Result;
    [TypeInfo(1)]
    public uint PlayerID;
}
public class NetCmdJoinRoomResult:NetCmdBase
{
    [TypeInfo(0)]
    public byte Result;
    [TypeInfo(1)]
    public byte RoomID;
    [TypeInfo(2)]
    public byte Seat;
    [TypeInfo(3)]
    public byte LauncherType;
    [TypeInfo(4)]
    public byte BackgroundImage;
}
public class NetCmdUseSkill:NetCmdBase
{
    [TypeInfo(0)]
    public byte SkillID;
}