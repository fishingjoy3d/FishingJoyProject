using UnityEngine;
using System.Collections.Generic;

//只存放不会改变的常量
public class ConstValue
{
    public const byte   ACCOUNT_NUM         = 16;
    public const float  INV255              = 1.0f / 255;
    public const float  MIN_REDUCTION       = 0.1f;

    //近裁剪面的宽和高的一半
    public const float  NEAR_Z              = 100;
    public const float  FAR_Z               = 2000;
    public const float  NEAR_HALF_WIDTH     = 31.3470f;
    public const float  NEAR_HALF_HEIGHT    = 17.6327f;

    //屏幕的宽高比,16:9
    public const float  ASPECT              = 1.777777f;

    //鱼模型的最大数量
    public const ushort FISH_MAX_NUM        = 255;
    public const float  InvShortMaxValue    = 1.0f / short.MaxValue;
    public const float  InvUShortMaxValue   = 1.0f / ushort.MaxValue;

    //文件结束标识符
    public const uint   FILE_END_MAGIC      = 732425123;
    //文件版本号
    public const byte   FILE_VER = 1;
    //场景中玩家的数量
    public const byte PLAYER_MAX_NUM        = 4;

    //场景中炮的种类
    public const byte MAX_LAUNCHER_NUM      = 5;

    //好友聊天记录保存条数
    public const byte MAX_CHATLOG_NUM       = 50;

    public const float INV_SECOND           = 0.01666666f;
    public const byte CUSTOM_HEADER         = 5;
    public const float CLEAR_TIME           = 6.5f;
    public const float START_POS            = -3000;
    public const uint  FISH_OVER_TIME       = 3000;
    public const byte INVALID_FISH_TYPE     = 255;
    public const uint HEARBEAT_ID           = 0xffffffff;
    public const uint PING_ID               = 0x8fffffff;


}
