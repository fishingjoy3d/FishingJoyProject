using UnityEngine;
using System.Collections.Generic;

class WndManager:Singleton<WndManager>
{
    List<Transform> m_WndList = new List<Transform>();
    int m_TopIndex = 1;
    public void Push(GameObject obj, bool sort = true)
    {
        Push(obj.transform, sort);
    }
    public void SetPanelSort(GameObject go, int idx)
    {
        UIPanel panel =go.GetComponent<UIPanel>();
        if (panel != null)
        {
            panel.sortingOrder = idx;
        }
        for (int i = 0; i < go.transform.childCount; ++i)
            SetPanelSort(go.transform.GetChild(i).gameObject, idx);
    }
    public void Push(Transform obj, bool sort = true)
    {
        if(sort)
            SetPanelSort(obj.gameObject, m_TopIndex++);
        m_WndList.Add(obj);
    }
    public bool HasTopWnd
    {
        get
        {
            return m_WndList.Count > 0;
        }
    }
    public void Pop(GameObject go)
    {
        if(go == null || go.transform == null)
        {
           // LogMgr.Log("对象为null:" + go);
        }
        else
            Pop(go.transform);
    }
    public void Pop(Transform fo)
    {
        bool bFind = false;
        for (int i = 0; i < m_WndList.Count; ++i)
        {
            if(m_WndList[i] == fo)
            {
                SetPanelSort(fo.gameObject, 0);
                m_WndList.RemoveAt(i);
                bFind = true;
                break;
            }
        }
        if(bFind == false)
        {
           // LogMgr.Log("未找到Pop对象:" + fo.gameObject);
        }
    }
    public Transform TopWnd
    {
        get
        {
            return m_WndList[m_WndList.Count - 1];
        }
    }
    public void Check()
    {
        if(HasTopWnd)
        {
            Transform topWnd = TopWnd;
            if(topWnd.gameObject.activeSelf == false)
            {
                LogMgr.Log("弹出窗口闭关却没有Pop:" + topWnd.gameObject.ToString());
            }
        }
    }
    public void Clear()
    {
        m_WndList.Clear();
    }
    public bool CanClick(GameObject go)
    {
        return CanClick(go.transform);
    }
    public bool CanClick(Transform go)
    {
        if (HasTopWnd)
        {
            Transform topWnd = TopWnd;
            Transform t = go.transform;
            while (t.parent != null && t.parent != SceneObjMgr.Instance.UIPanelTransform)
            {
                t = t.parent;
                if (t == topWnd)
                    return true;

            }
            return (t == topWnd);
        }
        else
            return true;
    }
}
