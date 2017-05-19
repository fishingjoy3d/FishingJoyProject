using UnityEngine;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
public class PathManager: Singleton<PathManager>
{
    List<PathLinearInterpolator>    m_PathInterpList = new List<PathLinearInterpolator>();
    List<PathLinearInterpolator>    m_PathInterpListInv = new List<PathLinearInterpolator>();
    List<PathLinearInterpolator[]>  m_PathGroupList = new List<PathLinearInterpolator[]>();
    List<PathLinearInterpolator[]>  m_PathGroupListInv = new List<PathLinearInterpolator[]>();
    PathLinearInterpolator      m_DouDongPath;
    PathLinearInterpolator[]    m_LongJuanFeng;
    PathLinearInterpolator      m_BoLang;

    int             m_NodeCount = 0;
    volatile bool   m_bInitOK = false;
    public uint     m_PathCRC = 0;
    TextAsset       m_TextAsset = null;
    public bool IsInitOK
    {
        get
        {
            if(m_bInitOK)
            {
                if(m_TextAsset != null)
                {
                    ResManager.Instance.UnloadObject(m_TextAsset);
                    m_TextAsset = null;
                }
            }
            return m_bInitOK;
        }
    }
    public uint PathCRC
    {
        get
        {
            return m_PathCRC;
        }
    }
    public PathLinearInterpolator BoLang
    {
        get { return m_BoLang; }
    }
    
    public PathLinearInterpolator GetPath(int idx, bool bInverse = false)
    {
        if(bInverse)
            return m_PathInterpListInv[idx];
        else
            return m_PathInterpList[idx];
    }

    public PathLinearInterpolator[] GetPathGroup(int idx, bool bInverse = false)
    {
        if (bInverse)
            return m_PathGroupListInv[idx];
        else
            return m_PathGroupList[idx];
    }
    public int PathCount
    {
        get { return m_PathInterpList.Count; }
    }

    public int PathGroupCount
    {
        get { return m_PathGroupList.Count; }
    }
    public PathLinearInterpolator DouDongPath
    {
        get { return m_DouDongPath; }
    }

    public PathLinearInterpolator[] LongJuanFeng
    {
        get { return m_LongJuanFeng; }
    }
    PathLinearInterpolator ReadPathData(BinaryReader br)
    {
        PathLinearInterpolator pi = new PathLinearInterpolator();
        pi.m_WorldMatrix        = Utility.ReadMatrix4x4(br);
        pi.m_WorldRotation      = Utility.ReadQuaternion(br);

	    pi.m_MaxDistance        = br.ReadSingle();
	    pi.m_SampleMaxDistance  = br.ReadSingle();
	    pi.m_HasPathEvent       = br.ReadBoolean();
        int sampleCount         = br.ReadInt32();
        int nodeCount           = br.ReadInt32();
	    pi.m_NodeList           = new LinearNodeData[nodeCount];
        pi.m_SplineDataList     = new SplineSampleData[sampleCount];
	    for (int i = 0; i < nodeCount; ++i)
	    {
            LinearNodeData node = new LinearNodeData();
            node.EventType = br.ReadByte();
            node.Times = br.ReadUInt16();
            pi.m_NodeList[i] = node;
	    }
        m_NodeCount += sampleCount;
	    for (int j = 0; j < sampleCount; ++j)
	    {
            SplineSampleData node = new SplineSampleData();
            node.pos    = Utility.ReadVec3(br);
            node.rot = Utility.ReadQuaternion(br);
            node.timeScaling = br.ReadSingle();
            node.nodeIdx = br.ReadInt16();
            pi.m_SplineDataList[j] = node;
	    }
        return pi;
    }
    PathLinearInterpolator[] ReadPathGroupData(BinaryReader br,int pathCount)
    {
        PathLinearInterpolator[] piList = new PathLinearInterpolator[pathCount];
        for (int n = 0; n < pathCount; ++n)
        {
            piList[n] = ReadPathData(br);
        }
        return piList;
    }
    public bool Init()
    {
        m_TextAsset = (TextAsset)ResManager.Instance.LoadObject("NewFishData", "PathRes/", ResType.PathRes, typeof(TextAsset));
        if (m_TextAsset == null) 
			return false;
        MemoryStream ms = new MemoryStream(m_TextAsset.bytes);
		BinaryReader br = new BinaryReader(ms);
		m_PathCRC = br.ReadUInt32();
		System.Threading.Thread t = new System.Threading.Thread(new System.Threading.ParameterizedThreadStart(NewInitProcedure));
		t.Start(br);
		br = null;
        return true;
    }
    void NewInitProcedure(object od)
    {
        BinaryReader br = (BinaryReader)od;
        ushort pathCount = br.ReadUInt16();
        ushort pathGroupCount = br.ReadUInt16();
        ushort fishGroupCount = br.ReadUInt16();
        ushort fishDataCount = br.ReadUInt16();

        for (ushort i = 0; i < pathCount; ++i)
        {
            PathLinearInterpolator pi = ReadPathData(br);
            PathLinearInterpolator piInv = ReadPathData(br);
            m_PathInterpList.Add(pi);
            m_PathInterpListInv.Add(piInv);
        }

        for (ushort i = 0; i < pathGroupCount; ++i)
        {
            short subPathCount = br.ReadInt16();
            PathLinearInterpolator[] pi = ReadPathGroupData(br, subPathCount);
            PathLinearInterpolator[] piInv = ReadPathGroupData(br, subPathCount);
            m_PathGroupList.Add(pi);
            m_PathGroupListInv.Add(piInv);
        }
        for (ushort i = 0; i < fishDataCount; ++i)
        {
            ResFishData fd = new ResFishData();
            fd.Size = Utility.ReadVec3(br);
            Utility.ReadQuaternion(br);
            for (int j = 0; j < (int)FishClipType.CLIP_MAX; ++j)
                fd.ClipLength[j] = br.ReadSingle();
            FishResManager.Instance.AddFishData(fd);
        }
        for (ushort i = 0; i < fishGroupCount; ++i)
        {
            GroupDataList gdl = new GroupDataList();
            FishResManager.Instance.AddGroupData(gdl);
            bool bFishPathGroup = br.ReadBoolean();
            if (bFishPathGroup)
            {
                gdl.PathGroupData = new FishPathGroupData();
                gdl.PathGroupData.PathGroupIndex = br.ReadUInt16();
                gdl.PathGroupData.Speed = br.ReadSingle();
                gdl.PathGroupData.FishIndex = br.ReadByte();
                gdl.PathGroupData.FishScaling = br.ReadSingle();
                gdl.PathGroupData.ActionSpeed = br.ReadSingle();
                gdl.PathGroupData.ActionUnite = br.ReadBoolean();
                //重复次数和间隔
                br.ReadUInt16();
                br.ReadUInt16();
            }
            else
            {
                gdl.FrontPosition = Utility.ReadVec3(br);
                ushort subPathCount = br.ReadUInt16();
                gdl.PathList = new ushort[subPathCount];
                for (ushort j = 0; j < subPathCount; ++j)
                {
                    gdl.PathList[j] = br.ReadUInt16();
                }
                ushort groupDataCount = br.ReadUInt16();
                gdl.GroupDataArray = new GroupData[groupDataCount];
                for (ushort j = 0; j < groupDataCount; ++j)
                {
                    GroupData gd = new GroupData();
                    gd.FishIndex = br.ReadByte();
                    gd.FishNum = br.ReadUInt16();
                    gd.FishScaling = br.ReadSingle();
                    gd.SpeedScaling = br.ReadSingle();
                    gd.ActionSpeed = br.ReadSingle();
                    gd.ActionUnite = br.ReadBoolean();
                    gd.PosList = new Vector3[gd.FishNum];
                    for (int n = 0; n < gd.FishNum; ++n)
                        gd.PosList[n] = Utility.ReadVec3(br);
                    gdl.GroupDataArray[j] = gd;

                }
            }// end if (pd.FishGroupByPathGroup)
        }

        m_BoLang = ReadPathData(br);
        m_DouDongPath = ReadPathData(br);
        short subPathCount2 = br.ReadInt16();
        m_LongJuanFeng = ReadPathGroupData(br, subPathCount2);

        m_BoLang.SetWorldPosition(Vector3.zero);
        m_DouDongPath.SetWorldPosition(Vector3.zero);

        for (int i = 0; i < m_LongJuanFeng.Length; ++i)
            m_LongJuanFeng[i].SetWorldPosition(Vector3.zero);

        uint endmagic = br.ReadUInt32();
        if (endmagic != ConstValue.FILE_END_MAGIC)
        {
            LogMgr.Log("路径文件结束不正确.");
        }
		br = null;
		od = null;
        m_bInitOK = true;
    }
}
