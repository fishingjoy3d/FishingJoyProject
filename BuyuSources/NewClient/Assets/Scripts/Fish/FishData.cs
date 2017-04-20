using UnityEngine;
using System.Collections.Generic;

public class GroupData
{
    public Vector3[] PosList = null;
    public float SpeedScaling;
    public ushort FishNum;
    public byte FishIndex;
    public float FishScaling;
    public float ActionSpeed = 1;
    public bool ActionUnite;
}

public class FishPathGroupData
{
    public ushort PathGroupIndex;
    public float Speed;
    public byte FishIndex;
    public float FishScaling;
    public float ActionSpeed = 1;
    public bool ActionUnite;
}
public class GroupDataList
{
    public FishPathGroupData PathGroupData; //路径群数据
    public GroupData[] GroupDataArray;
    public Vector3 FrontPosition;
    public ushort[] PathList;
}
public enum FishClipType
{
    CLIP_YOUYONG = 0,
    CLIP_SIWANG,
    CLIP_CHAOFENG,
    CLIP_GONGJI,
    CLIP_MAX
};
public class ResFishData
{
    public byte FishIndex;
    public Vector3 Size;
    public float[] ClipLength = new float[(int)FishClipType.CLIP_MAX];
};
