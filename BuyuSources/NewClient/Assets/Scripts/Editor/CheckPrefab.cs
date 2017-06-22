using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class CheckPrefab : MonoBehaviour
{


    // 在菜单来创建 选项 ， 点击该选项执行搜索代码  
    [MenuItem("Tools/遍历项目所有文件夹")]
    static void CheckSceneSetting()
    {
        List<string> dirs = new List<string>();
        GetDirs(Application.dataPath, ref dirs);
    }
    //参数1 为要查找的总路径， 参数2 保存路径  
    private static void GetDirs(string dirPath, ref List<string> dirs)
    {
        foreach (string path in Directory.GetFiles(dirPath))
        {
            //获取所有文件夹中包含后缀为 .prefab 的路径  
            if (System.IO.Path.GetExtension(path) == ".prefab")
            {
                dirs.Add(path.Substring(path.IndexOf("Assets")));
                string newPath = path.Substring(path.IndexOf("Assets"));

                GameObject _prefab = AssetDatabase.LoadAssetAtPath(newPath, typeof(GameObject)) as GameObject;
                if (_prefab != null)
                {
                    //UILabel[] labels = _prefab.GetComponentsInChildren<UILabel>();
                    //for (int i = 0; i < labels.Length; i++)
                    //{
                    //    string fontName = labels[i].bitmapFont.name;
                    //    if (fontName == "Score_Font")
                    //    {
                    //        Debug.Log(newPath + "  " + labels[i].bitmapFont.name);
                    //    }
                    //}
                    CheckTransform(_prefab.transform);
                }

            }
        }

        if (Directory.GetDirectories(dirPath).Length > 0)  //遍历所有文件夹  
        {
            foreach (string path in Directory.GetDirectories(dirPath))
            {
                GetDirs(path, ref dirs);
            }
        }
    }

    static void CheckTransform(Transform t)
    {
        UILabel comp = t.GetComponent<UILabel>();
        if (comp != null && comp.bitmapFont != null)
        {
            string fontName = comp.bitmapFont.name;
            if (fontName == "ShopNum_Font")
            {
                string path = comp.name;
                Transform parent = comp.transform.parent;
                while (parent != null)
                {
                    path = parent.name + "/" + path;
                    parent = parent.parent;
                }
                Debug.Log(path);
            }
        }

        int count = t.childCount;
        for (int i = 0; i < count; i++)
        {
            CheckTransform(t.GetChild(i));
        }
    }
}
