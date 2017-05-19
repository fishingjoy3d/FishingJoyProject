using UnityEngine;
using System.Collections.Generic;



public class LauncherComponent : MonoBehaviour 
{
    public bool     m_PlaySpriteAnim;
    public bool     m_LightDot;
    public bool     m_MoveLight;
    public bool     m_MuzzleEft;
    public int      m_ComponentCount;
    public string   m_MuzzleNamePrefix;                 //炮口特效序列帧名字


    public LauncherComponent()
    {
        m_PlaySpriteAnim = false;
    }
	
}
