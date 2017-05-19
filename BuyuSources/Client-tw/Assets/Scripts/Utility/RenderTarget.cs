using UnityEngine;
using System.Collections.Generic;

public class RenderTarget
{
    const int WIDTH = 512;
    const int HEIGHT = 512;
    public Camera m_RTCam;
    public GameObject m_CurObj;
    private RenderTexture m_SingleRenderTex;
    public GameObject m_BackObj;
    bool m_bMouseDown = false;
    bool m_bTouchDown = false;
    float m_FirstX;
    float m_RotX;
    float m_CurX;
    public RenderTexture SingleRenderTex
    {
        get { return m_SingleRenderTex; }
        set { m_SingleRenderTex = value; }
    }
    public RenderTarget()
    {
        GameObject obj = GameObject.Find("SceneBackground");
        m_BackObj = GameObject.Instantiate(obj);
        GameObject.Destroy(m_BackObj.GetComponent<Animator>());
        m_BackObj.transform.position = new Vector3(10000, 0, 630);
        Vector3 ss = m_BackObj.transform.localScale;
        ss.z *= 0.96f;
        m_BackObj.transform.localScale = ss;
        Texture2D tex = (Texture2D)ResManager.Instance.LoadObject("BackImg0", "SceneRes/BackImages/", ResType.SceneRes, typeof(Texture2D), false);
        m_BackObj.GetComponent<Renderer>().material.mainTexture = tex;
        GameObject go = new GameObject();
        m_RTCam = go.AddComponent<Camera>();
        m_RTCam.transform.position = new Vector3(10000, 0, 0);
        m_RTCam.backgroundColor = new Color32(49, 77, 121, 255);
        m_RTCam.aspect = Resolution.DEFAULT_ASPECT;
        m_RTCam.enabled = false;
    }
    public void Shutdown()
    {
        GameObject.Destroy(m_RTCam.gameObject);
        GameObject.Destroy(m_BackObj);
        m_RTCam = null;
        m_SingleRenderTex = null;
        DestroyOldObj();
    }
    void DestroyOldObj()
    {
        if (m_CurObj != null)
        {
            GameObject.Destroy(m_CurObj);
            m_CurObj = null;
        }
    }
    public void RenderSingleFish(byte idx)
    {
        m_RotX = 0;
        m_CurX = 0;
        if (m_SingleRenderTex == null)
        {
            m_SingleRenderTex = new RenderTexture(WIDTH, HEIGHT, 1);
            SceneMain.Instance.StartInnerCoroutine(ThreadUpdate(null));
        }
        DestroyOldObj();
        RenderObj(idx, m_SingleRenderTex);
    }
    void RotateObj(float xx)
    {
        m_CurX = xx;
        Quaternion quat = Quaternion.AngleAxis(m_RotX + m_CurX, Vector3.up);
        m_CurObj.transform.rotation = quat * Quaternion.AngleAxis(-10, Vector3.forward) * Quaternion.AngleAxis(180, Vector3.up);
    }
    public System.Collections.IEnumerator ThreadUpdate(Object obj)
    {
        yield return new WaitForEndOfFrame();
        while(m_SingleRenderTex != null)
        {
            if(Input.GetMouseButton(0))
            {
                if(m_bMouseDown == false)
                {
                    m_FirstX = Input.mousePosition.x;
                    m_bMouseDown = true;
                }
                else
                {
                    RotateObj(m_FirstX - Input.mousePosition.x);
                }
            }
            else if(m_bMouseDown)
            {
                m_RotX += m_CurX;
                m_CurX = 0;
                m_bMouseDown = false;
            }
            m_RTCam.Render();
            yield return new WaitForEndOfFrame();
        }
        yield break;
    }
    public void RenderObj(byte idx, RenderTexture rt)
    {
        Object obj = FishResManager.Instance.GetFishObj(idx);
        if (obj != null)
        {
            ResFishData fd = FishResManager.Instance.GetFishData(idx);
            float scl = fd.Size.x / 20;
            m_RTCam.targetTexture = rt;
            GameObject go = (GameObject)GameObject.Instantiate(obj);
            if(go.transform.childCount >= 3)
            {
                go.transform.GetChild(2).gameObject.SetActive(false);
            }
            if (idx == 22)
                go.transform.position = new Vector3(10000, 0, 33 * scl);
            else if (idx == 23)
                go.transform.position = new Vector3(10000, 0, 25 * scl);
            else if (idx == 0)
                go.transform.position = new Vector3(10000, 0, 30 * scl);
            else
                go.transform.position = new Vector3(10000, 0, 25 * scl);
            m_CurObj = go;
            RotateObj(0);
            m_RTCam.Render();
        }
        else
        {
            m_RTCam.targetTexture = null;
        }
    }
}
