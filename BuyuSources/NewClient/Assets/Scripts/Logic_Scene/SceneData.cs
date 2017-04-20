using UnityEngine;
using System.Collections.Generic;

public enum SkillType
{
    SKILL_LIGHTING,
    SKILL_FREEZE,
    SKILL_TORNADO,
    SKILL_DISASTER,
    SKILL_LOCK,
    SKILL_MAX
}
//捕获金币的类型
public enum CatchedType
{
    CATCHED_BULLET, //子弹捕获
    CATCHED_LASER,  //大招捕获
    CATCHED_SKILL,  //技能捕获
}
public class CatchedData
{
    public byte     ClientSeat;     //客户端座位
    public byte     CatchType;      //捕获的类型
    public byte     SubType;        //子类型：子弹的类型，技能的类型，大招的类型。
    public int      GoldNum;        //此次获取的总金币数量
    public byte     RateIndex;      //当前的倍率
    public List<CatchFishData> FishList;     //捕获的鱼列表。
    public byte     DeadNum;        //当前捕获列表中已经播放完死亡特效的数量
    public byte     GoldFinished;   //金币到达终点的数量。
}

public class FishNormalMaterial
{
    public Shader shaer;
    public Texture2D baseTex;
}
public class FishSkillMaterial
{
    public Shader shader;
    public Texture2D baseTex;
    public Texture2D extraTex;
}
public enum BlendType
{
    BLEND_COLOR,
    BLEND_ADD_TEX,
    BLEND_LERP_TEX,
    BLEND_DISSOLVE_TEX,
}
public class BlendData
{
    public BlendData(Color blendColor, float factor, float d1, float d2, float d3)
    {
        Blend_Type = (byte)BlendType.BLEND_COLOR;
        BlendColor = blendColor;
        Factor = factor;
        Duration1 = d1;
        Duration2 = d2;
        Duration3 = d3;
    }
    public BlendData(BlendType bt, float factor, float d1, float d2, float d3, Texture2D effect, Texture2D baseTex = null)
    {
        Blend_Type = (byte)bt;
        Factor = factor;
        Duration1 = d1;
        Duration2 = d2;
        Duration3 = d3;
        EffectTex = effect;
        BaseTex = baseTex;
    }
    public BlendData()
    {

    }
    public float    Factor;
    public float    Duration1;
    public float    Duration2;
    public float    Duration3;
    public byte     Blend_Type;
    public Color     BlendColor;
    public Texture2D EffectTex;
    public Texture2D BaseTex;
}
public struct CatchFishData
{
    public byte     FishType;
    public Fish     FishObj;
    public bool IsValidFishType()
    {
        return FishType != ConstValue.INVALID_FISH_TYPE;
    }
}
public struct CatchBulletData
{
    public byte     LauncherType;
    public byte     RateIndex;
    public Bullet   BulletObj;
    public bool IsValid()
    {
        return BulletObj != null || LauncherType != 255;
    }
}