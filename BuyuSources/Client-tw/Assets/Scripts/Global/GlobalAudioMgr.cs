using UnityEngine;
using System.IO;
using System.Collections.Generic;
public class Audio
{
    EffectBGType m_BGType;

    // EffectAudioClip m_MyAudioClipthing;
    AudioClip m_MyAudioClip;
    public string m_clipname;
    // AudioInf m_AudioInf;
    string path = "ExtraRes/Audio/";
    public void CreateAudioClip()
    {
        m_MyAudioClip = (AudioClip)ResManager.Instance.LoadObject(m_clipname, path, ResType.ExtraRes, typeof(AudioClip), false);
    }
    //public bool Update(float dTime)
    //{
    //    m_AudioInf.m_LifeTime -= dTime;
    //    if (m_AudioInf.m_LifeTime < 0)
    //        return false;
    //    return true;
    //}
    //public bool EffectAudioIsPlaying
    //{
    //    get { return m_MyAudioClipthing.isPlaying; }
    //    set { m_MyAudioClipthing.isPlaying = value; }
    //}
    //internal EffectAudioClip MyAudioClipthing
    //{
    //    get { return m_MyAudioClipthing; }
    //    set { m_MyAudioClipthing = value; }
    //}
    public AudioClip MyAudioClip
    {
        get { return m_MyAudioClip; }
        set { m_MyAudioClip = value; }
    }
    public string AudioName
    {
        get { return m_clipname; }
        set { m_clipname = value; }
    }
    public EffectBGType BGType
    {
        get { return m_BGType; }
        set { m_BGType = value; }
    }
    //struct AudioInf
    //{
    //    public bool m_IsPlayer;
    //    public float m_LifeTime;
    //}
    //public struct EffectAudioClip
    //{
    //    public string name;
    //    public bool isPlaying;
    //}
    public enum MusicType
    {
        m_SkillMusic = 0,
        m_OrdianryMusic,
        m_BGMusic,
        m_FishMusic,
    }
    public enum SkillMusic
    {
        m_Skill_Harvest_1 = 0,//技能结束                                                  0
        m_Skill_Harvest_2,//金币滚动计分 可循环 与_1 叠加使用                           1
        m_Skill_0,//第1个位置技能                                                     2
        m_skill_1,//第2个位置                                                         3
        m_Skill_2,//第3个位置                                                           4
        m_Skill_3,//第4个位置                                                             5
        m_SuperSkill_1_2,//1-2级炮大招                                                  6
        m_SuperSkill_3, //                                                              7
        m_SuperSkill_4,//                                                               8
        m_SuperSkill_5,//                                                               9
    }
    public enum OrdianryMusic//普通音效
    {
        m_Contest_Voer = 0,//比赛结束弹窗                                           0
        m_CloseUI,//关闭UI                                                          1
        m_BtnMusic,//点击按钮（打开界面）                                           2
        m_UserGetGold,//金币收入囊中的音效                                          3
        m_GoldJump,//领取奖励                                                       4
        m_Hit_1_2,//受击1~2                                                         5
        m_Hit_3,//受击3                                                             6
        m_Hit_4,//受击4                                                             7
        m_Hit_5,//受击5                                                             8
        m_Shot_1_2,//发射1~2                                                        9
        m_Shot_3,//发射3                                                            10
        m_Shot_4,//发射4                                                            11
        m_Shot_5,//发射5                                                            12
        m_KillBuff,//下击必杀                                                       13 
        m_Rechage,//充值成功                                                        14
        m_Reward,//获得奖励                                                         15
        m_FullEnergy,//满能量                                                       16
        m_ChangeGun,//切换武器                                                      17
        m_ClickSkill,//点击技能                                                     18
        m_ClearFish,//清场                                                          19   
        m_Gift0,//第一个l礼物声音                                                   20
        m_Gift1, //                                                                 21
        m_Gift2,//                                                                  22         
        m_Gift3,//                                                                  23
        m_Gift4,//                                                                  24
        m_bosscoming,//boss警告                                                     25
        m_lottery1,//开始寻宝                                                       26
        m_lottery2,//展开牌                                                         27
        m_lottery3,//点击翻卡                                                       28
        m_lottery4,//最后奖励                                                       29
        catch_bossfish,//捕获boss鱼                                                 30

        m_Niuniu1,//牛1                                                             31
        m_Niuniu2,//牛2                                                             32
        m_Niuniu3,//牛3                                                             33
        m_Niuniu4,//牛4                                                             34
        m_Niuniu5,//牛5                                                             35
        m_Niuniu6,//牛6                                                             36
        m_Niuniu7,//牛7                                                             37
        m_Niuniu8,//牛8                                                             38
        m_Niuniu9,//牛9                                                             39
        m_Niuniu10,//牛牛                                                           40
        m_Niuniu11,//五花牛                                                         41
        m_Niuniu12,//炸弹                                                           42
        m_Niuniu13,//五小牛                                                         43
        m_Niuniu14,//背景                                                           44
        m_Niuniu15,//下注                                                           45
        m_Niuniu16,//发牌                                                           46
        m_Niuniu17,//金币飞行                                                       47
        m_Niuniu18,//胜利                                                           48
        m_Niuniu19,//失败                                                           49
        m_Niuniu20,//无牛                                                           50
       
        m_redpanda, //开奖红色熊猫 51                                              
        m_redmonkey,
        m_redrabbit,
        m_redlion,

        m_yellowpanda,
        m_yellowmonkey,
        m_yellowrabbit,
        m_yellowlion,

        m_greenpanda,
        m_greenmonkey,
        m_greenrabbit,
        m_greenlion,        //开奖绿色狮子
        m_panda,            //开奖熊猫
        m_monkey,           
        m_rabbit,
        m_lion,              //开奖狮子
        m_onchip,
        m_cd,  //时间
        m_cdover,//到ji
    }                                                             

    public enum EffectBGType
    {
        EffectBGSound1 = 0,
        EffectBGSound2,
        EffectBGSound3,//boss1
        EffectBGSound4,//boss2
        EffectBGSound5,
        EffectBGSound6,//舞会1
        EffectBGSound7, //舞会2
        EffectBGSound8,//车音乐1
        EffectBGSound9,//车2
    }
}
class GlobalAudioMgr : Singleton<GlobalAudioMgr>
{
    AudioSource LoopAudio = null;
    AudioSource LoopAudio_1 = null;//bg
    AudioSource LoopAudio_Gift = null;
    List<Audio> m_listOrdianryAudio = new List<Audio>();
    List<Audio> m_listSkillAudio = new List<Audio>();
    List<Audio> m_listBGAudio = new List<Audio>(); 
    float m_BgValue = 0.25f;//主的
    float m_Value = 0.25f;  //背景的
    float m_fPause = 0;
    const float m_fsoundrelax = 0.15f;

    public void GlobalInit()
    { }
    public bool Init()
    {
        Camera cam = Camera.main;
        if (cam == null)
            cam = GameObject.FindObjectOfType(typeof(Camera)) as Camera;
        AudioListener mListener = cam.gameObject.GetComponent<AudioListener>();
        if (mListener == null)
        {
            if (cam != null)
                mListener = cam.gameObject.AddComponent<AudioListener>();
        }
        //#if UNITY_4_3 || UNITY_4_5 || UNITY_4_6
        //        BGAudio = mListener.audio;
        //#else
        LoopAudio = mListener.GetComponent<AudioSource>();
        //#endif
        if (LoopAudio == null)
            LoopAudio = mListener.gameObject.AddComponent<AudioSource>();
        LoopAudio_1 = mListener.gameObject.AddComponent<AudioSource>();
        LoopAudio_Gift = mListener.gameObject.AddComponent<AudioSource>();
        InitLogic.StartInit();
        SceneMain.Instance.StartInnerCoroutine(Initaudio(null));
        LoadAudioValue();
        SetBGMusicVolue(m_BgValue);
        SetMusicVolue(m_Value);
        //int bg = Random.Range(4, 7);
        int bg = 1;
        GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound2);

        //GlobalAudioMgr.Instance.PlayerBGMusic(GlobalAudioMgr.BGMusic.BgMusic0);
        return true;
    }
    System.Collections.IEnumerator Initaudio(object dd)
    {
        for (byte i = 0; i < 9; i++)
        {
            Audio effectAudio = new Audio();
            effectAudio.AudioName = string.Format("bgm{0}", i);
            effectAudio.CreateAudioClip();
            m_listBGAudio.Add(effectAudio);
        }
        yield return new WaitForEndOfFrame();
        for (byte o = 0; o <=69; ++o)
        {
            Audio effectAudio = new Audio();
            effectAudio.AudioName = string.Format("0{0}", o);
            effectAudio.CreateAudioClip();
            if (effectAudio.MyAudioClip != null)
                m_listOrdianryAudio.Add(effectAudio);
            else
                break;
        }
        yield return new WaitForEndOfFrame();
        for (byte s = 0; s < 10; ++s)
        {
            Audio effectAudio = new Audio();
            effectAudio.AudioName = string.Format("S{0}", s);
            effectAudio.CreateAudioClip();
            if (effectAudio.MyAudioClip != null)
                m_listSkillAudio.Add(effectAudio);
            else
                break;
        }
        InitLogic.EndInit();
        yield break;

    }
    public void StopBgMusic()
    {
        LoopAudio_1.Stop();
    }
    public void PlayerBGMusic(Audio.EffectBGType type)
    {
        LoopAudio_1.Stop();
        LoopAudio_1.loop = true;
        LoopAudio_1.clip = m_listBGAudio[(int)type].MyAudioClip;
        LoopAudio_1.Play();      
    }
    public void PlaySkillMusic(Audio.SkillMusic SkillMusicIndex/*音效的类型*/, bool bLoop = false/*是否循环*/, bool bStopLoop = false/*是否停止循环主要是用来听上个音效*/, float volume = 1)
    {
        if (bStopLoop)
        {
            if (LoopAudio == null)
                return;
            if (LoopAudio.isPlaying)
                LoopAudio.Stop();
            LoopAudio.loop = false;
        }

        if (bLoop)
        {
            if (LoopAudio == null)
                return;
            LoopAudio.Stop();
            LoopAudio.loop = true;
            LoopAudio.clip = m_listSkillAudio[(int)SkillMusicIndex].MyAudioClip;
            LoopAudio.Play();
            LoopAudio.volume = m_BgValue * volume;           
        }
        else
        {    
            NGUITools.PlaySound(m_listSkillAudio[(int)SkillMusicIndex].MyAudioClip, volume);
        }
    }
    public void PlayOrdianryMusic(Audio.OrdianryMusic OrdianryMuysicIndex/*音效的类型*/, bool bLoop = false/*是否循环*/, bool bStopLoop = false/*是否停止循环主要是用来听上个音效*/, float volume = 1)
    {
        if (bStopLoop)
        {
            if (LoopAudio == null)
                return;
            LoopAudio.Stop();
            LoopAudio.loop = false;
        }

        if (bLoop)
        {
            if (LoopAudio == null)
                return;
            LoopAudio.Stop();
            LoopAudio.loop = true;
            LoopAudio.clip = m_listOrdianryAudio[(int)OrdianryMuysicIndex].MyAudioClip;
            LoopAudio.Play();
            LoopAudio.volume = m_BgValue * volume;
        }
        else
        {         
            NGUITools.PlaySound(m_listOrdianryAudio[(int)OrdianryMuysicIndex].MyAudioClip, volume);
        }
    }
    public void PlayCharmMusic(Audio.OrdianryMusic OrdianryMuysicIndex)
    {
        m_fPause = 1.8f;
        LoopAudio.volume = m_BgValue * m_fsoundrelax;
        LoopAudio_1.volume = m_Value * m_fsoundrelax;

        LoopAudio_Gift.loop = false;       
        LoopAudio_Gift.clip = m_listOrdianryAudio[(int)OrdianryMuysicIndex].MyAudioClip;
        LoopAudio_Gift.Play();
        LoopAudio_Gift.volume = m_BgValue;      
    }
    //
    //第一
    public void SetBGMusicVolue(float volue)
    {
       // NGUITools.soundVolume = volue;
        if (LoopAudio != null)
            LoopAudio.volume = volue;
        m_BgValue = volue;
        Recover();
    }
    public void SetMusicVolue(float volue)
    {
        if (LoopAudio_1 != null)
        {
            LoopAudio_1.volume = volue;
        }
        m_Value = volue;
        Recover();
    }
    void Recover()
    {
        m_fPause = 0;
        LoopAudio.volume = m_BgValue;
        LoopAudio_1.volume = m_Value;    
    }
    public bool LoadAudioValue()
    {
        //读取SD卡的版本
        byte[] data = RuntimeInfo.GetLocalFile(RuntimeInfo.GetAudioFileName());
        if (data == null || data.Length != 2 * 4)
            return false;

        for (int i = 0, idx = 0; i < 2; ++i, idx += 4)
        {
            float value = System.BitConverter.ToSingle(data, idx);
            if (i == 0)
                m_BgValue = value;
            else
                m_Value = value;
        }
        return true;
    }
    //音效保存到SDK里
    public void SaveAudioValue()
    {
        byte[] data;
        MemoryStream ms = new MemoryStream(2 * 4);
        for (int i = 0; i < 2; ++i)
        {
            if (i == 0)
                data = System.BitConverter.GetBytes(m_BgValue);
            else
                data = System.BitConverter.GetBytes(m_Value);
            ms.Write(data, 0, data.Length);
        }
        bool bRet = RuntimeInfo.SaveLocalFile(RuntimeInfo.GetAudioFileName(), ms.ToArray());
        ms.Close();
    }
    public float BgValue
    {
        get { return m_BgValue; }
    }
    public float Value
    {
        get { return m_Value; }
    }
    public void Update(float delta)
    {
        if (m_fPause > 0)
        {
            m_fPause -= delta;
            if (m_fPause < 0)
            {
                Recover();                         
            }
        }
    }
    public void Shutdown()
    {

    }
    public void Clear()
    {

    }
    public enum BGMusic
    {
        BgMusic0 = 0,
        BgMusic1,
        BgMusic2,
        BgMusic3,
        BgMusic4,
    }
}
