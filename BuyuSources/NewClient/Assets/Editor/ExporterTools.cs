using UnityEngine;
using System.Collections.Generic;
using UnityEditor;
using System.IO;
using System.Runtime.InteropServices;
public class FileFlagData
{
    public string path;
    public bool Prefab;
    public float wscaling;
    public float hscaling;
}
public class FileExtData
{
    public string Path;
    public string Ext;
}
public class FileNameData
{
    public string ID;
    public List<FileExtData> ExtList = new List<FileExtData>();
    public FileExtData FindExt(string ext)
    {
        foreach(FileExtData fe in ExtList)
        {
            if(fe.Ext == ext || fe.Ext == ".bytes" || ext == ".bytes")
            {
                return fe;
            }
        }
        return null;
    }

}
public class ExporterTools
{
    static List<string> ImgConvertList = new List<string>();
    static List<string> AtlasConvertList = new List<string>();
    static bool CHECK_IMG_SIZE = true;
    static int SIZE_ALIGN = 4;
    public static bool IsImg(string pathx)
    {
        string path = pathx.ToLower();
        if (path.LastIndexOf(".jpg") != -1 ||
            path.LastIndexOf(".png") != -1 ||
            path.LastIndexOf(".tga") != -1)
            return true;
        
        if (path.LastIndexOf(".bmp") != -1 || path.LastIndexOf(".psd") != -1 || path.LastIndexOf(".dds") != -1 || path.LastIndexOf(".tif") != -1 || path.LastIndexOf(".tiff") != -1)
        {
            EditorUtility.DisplayDialog("错误", "图片的扩展名只能是.jpg, .png,.tga:" + pathx, "确定");
            return true;
        }
        return false;
    }
    public static bool IsEven(int s)
    {
        return (s & 1) == 0;
    }
    public static bool Is2N(int s)
    {

        return (s & (s - 1)) == 0;
    }
    public static int GetMaxSize(int width, int hegiht)
    {
        int maxsize = Mathf.Max(width, hegiht);
        return maxsize;
    }
    public static bool IsPrefab(string path)
    {
        return path.LastIndexOf(".prefab") != -1;
    }
    public static bool CheckFont(string path)
    {
        Object obj = AssetDatabase.LoadAssetAtPath(path, typeof(GameObject));
        if (obj == null)
        {
            EditorUtility.DisplayDialog("错误", "打开Prefab失败:" + path, "确定");
            return false;
        }
        GameObject go = obj as GameObject;
        UIFont font = go.GetComponent<UIFont>();
        if (font != null)
        {
            if (font.atlas != null)
            {
                EditorUtility.DisplayDialog("错误", "字体只能使用材质，不能使用图集:" + path, "OK");
                return false;
            }
            if (font.material == null)
            {
               // EditorUtility.DisplayDialog("错误", "字体未找到材质属性:" + path, "OK");
                //return false;
            }
        }
        return true;
    }
    public enum ImgUpdateType
    {
        IMG_HD,
        IMG_SD,
        IMG_KEEP
    }
    public static bool UpdateImgAttr(string path, ImgUpdateType opt)
    {
        TextureImporter txtImporter = AssetImporter.GetAtPath(path) as TextureImporter;
        txtImporter.maxTextureSize = 2048;
        txtImporter.SaveAndReimport();
        txtImporter = null;
        Texture2D tx = AssetDatabase.LoadAssetAtPath(path, typeof(Texture2D)) as Texture2D;
        if (tx == null)
        {
            EditorUtility.DisplayDialog("错误", "打开图片失败:" + path, "确定");
            return false;
        }
        
        int width = tx.width;
        int height = tx.height;
        if (width > 32 && height > 32)
        {
            if ((width % SIZE_ALIGN != 0) || (height % SIZE_ALIGN != 0))
            {
                string errPath = "图片的大小不是4的整数倍，无法用于硬件压缩格式:" + path + ", width:" + width + ", height:" + height;
                //Debug.Log(errPath);
               // EditorUtility.DisplayDialog("错误", errPath, "确定");
                //return false;
            }
            if (width > 2048 || height > 2048)
            {
                string imgPath = "图片大小大于2048:" + path + ", width:" + width + ", height:" + height;
                Debug.Log(imgPath);
                EditorUtility.DisplayDialog("错误",imgPath , "确定");
                //return false;
            }
        }
        bool bFishImg = path.IndexOf("FishRes") != -1;
        txtImporter = AssetImporter.GetAtPath(path) as TextureImporter;
        txtImporter.textureFormat = bFishImg ? TextureImporterFormat.AutomaticCompressed : TextureImporterFormat.AutomaticTruecolor; //;//
        txtImporter.textureType = TextureImporterType.Default;
        txtImporter.npotScale = TextureImporterNPOTScale.None;
        txtImporter.generateMipsInLinearSpace = false;
        txtImporter.anisoLevel = 0;
        txtImporter.filterMode = FilterMode.Bilinear;
        txtImporter.mipmapEnabled = bFishImg;//必须是POT的大小才能开始压缩和mipmap;
        txtImporter.isReadable = false;
        txtImporter.SaveAndReimport();
        
        return true;
    }
    public static bool ConvertImgKeep(List<string> objList)
    {
        bool bRet = true;
        try
        {
            foreach (string path in objList)
            {
                if (IsImg(path))
                {
                    if (!UpdateImgAttr(path, ImgUpdateType.IMG_KEEP))
                    {
                        bRet = false;
                        break;
                    }
                }
            }
        }
        catch (System.Exception)
        {
            bRet = false;
        }
        return bRet;
    }
   
    public static bool CheckImgAndAtlas(List<string> pathList)
    {
        bool bRet = true;
        try
        {
            Dictionary<string, FileNameData> strMap = new Dictionary<string, FileNameData>();
            foreach (string path in pathList)
            {
                int k = path.LastIndexOf('/');
                if(k != -1)
                {
                    ++k;
                    string id = path.Substring(k, path.Length - k);
                    int j = id.LastIndexOf(".");
                    string ext = "";
                    if (j != -1)
                    {
                        ext = id.Substring(j, id.Length - j).ToLower();
                        id = id.Remove(j, id.Length - j);
                    }
                   
                    FileNameData fnd;
                    if(strMap.TryGetValue(id, out fnd))
                    {
                        FileExtData fed = fnd.FindExt(ext);
                        if(fed != null)
                        {
                            string err = ("重复的ID名称:" + fed.Path + " <-> " + path);
                            Debug.Log(err);
                            bRet = false;
                            //throw new System.Exception(err);
                        }
                        else
                        {
                            FileExtData det = new FileExtData();
                            det.Ext = ext;
                            det.Path = path;
                            fnd.ExtList.Add(det);
                        }
                    }
                    else
                    {
                        fnd = new FileNameData();
                        fnd.ID = id;
                        FileExtData det = new FileExtData();
                        det.Ext = ext;
                        det.Path = path;
                        fnd.ExtList.Add(det);
                        strMap.Add(id, fnd);
                    }
                }
                if (IsImg(path))
                {
                    if (!UpdateImgAttr(path, ImgUpdateType.IMG_SD))
                    {
                        bRet = false;
                        break;
                    }
                }
                else if(IsPrefab(path))
                {
                    if (!CheckFont(path))
                    {
                        bRet = false;
                        break;
                    }
                }
            }
        }
        catch (System.Exception)
        {
            bRet = false;
        }
        return bRet;
    }
    static public Rect ConvertToPixels(Rect rect, int width, int height, bool round)
    {
        Rect final = rect;
        {
            final.xMin = Mathf.RoundToInt(rect.xMin * width);
            final.xMax = Mathf.RoundToInt(rect.xMax * width);
            final.yMin = Mathf.RoundToInt((rect.yMax) * height);
            final.yMax = Mathf.RoundToInt((rect.yMin) * height);
        }
        
        return final;
    }
    static List<Texture2D> SpliteTexture(UIAtlas atlas, float scaling, out Texture2D tex, List<UISpriteData> splist)
    {
        List<Texture2D> texList = new List<Texture2D>();
        List<UIAtlasMaker.SpriteEntry> entryList = new List<UIAtlasMaker.SpriteEntry>();
        UIAtlasMaker.ExtractSprites(atlas, entryList);
        int ij = 0;
        foreach (UIAtlasMaker.SpriteEntry sd in entryList)
        {
            //Debug.Log(sd.paddingBottom + "," + sd.paddingLeft + "," + sd.paddingRight + "," + sd.paddingTop);
            //byte[] att = sd.tex.EncodeToPNG();
            //FileStream fs = new FileStream(Application.dataPath + "/Resources/vata"+ ij +".png", FileMode.Create);
            //fs.Write(att, 0, att.Length);
            //fs.Close();
            texList.Add(sd.tex);
            ++ij;
        }
        {
            const int SIZE = 380;
            tex = new Texture2D(SIZE, SIZE, TextureFormat.ARGB32, false);
            Rect[] rectList = tex.PackTextures(texList.ToArray(), 2, tex.width);
            int i = 0;
            foreach(Rect r in rectList)
            {
                UISpriteData sd = new UISpriteData();
                Rect finalRect = NGUIMath.ConvertToPixels(r, tex.width, tex.height, true);
                sd.x = Mathf.RoundToInt(finalRect.x);
                sd.y = Mathf.RoundToInt(finalRect.y);
                sd.width = Mathf.RoundToInt(finalRect.width);
                sd.height = Mathf.RoundToInt(finalRect.height);
                
                sd.name = entryList[i++].name;
                Debug.Log("sprite:" + sd.name + ": " + finalRect.x + "," + finalRect.y + "," + finalRect.width + "," + finalRect.height);
                splist.Add(sd);
            }
            byte[] att = tex.EncodeToPNG();
            FileStream fs = new FileStream(Application.dataPath + "/Resources/vata.png", FileMode.Create);
            fs.Write(att, 0, att.Length);
            fs.Close();
        }
        EditorUtility.ClearProgressBar();
        return texList;
    }
    public static void TestAtlas()
    {
        string path = "Assets/Resources/HallRes/Prefab/UI/PrefabImage/Hall_RoomImage.prefab";
        string matPath = "Assets/Resources/HallRes/Image/UI/Materials/Hall_RoomImage.mat";
        Object obj = AssetDatabase.LoadAssetAtPath(path, typeof(GameObject));
        Material objMat = (Material)AssetDatabase.LoadAssetAtPath(matPath, typeof(Material));
        if (objMat == null)
            return;
        GameObject go = (GameObject)obj;
        UIAtlas atl = go.GetComponent<UIAtlas>();
        Texture2D tex;
        List<UISpriteData> splist = new List<UISpriteData>();
        SpliteTexture(atl, 0.5f, out tex, splist);
        TextureImporter ti = AssetImporter.GetAtPath("Assets/Resources/vata.png") as TextureImporter;
        ti.npotScale = TextureImporterNPOTScale.None;
        ti.SaveAndReimport();

        Texture2D matTex = (Texture2D)AssetDatabase.LoadAssetAtPath("Assets/Resources/vata.png", typeof(Texture2D));
        objMat.mainTexture = matTex;
        SerializedObject so = new SerializedObject(atl);
        SerializedProperty spp = so.FindProperty("mSprites");

        for (int i = 0; i < splist.Count; ++i )
        {
            UISpriteData sd = splist[i];
            SerializedProperty sdd = spp.GetArrayElementAtIndex(i);
            sdd.FindPropertyRelative("x").intValue = sd.x;
            sdd.FindPropertyRelative("y").intValue = sd.y;
            sdd.FindPropertyRelative("width").intValue = sd.width;
            sdd.FindPropertyRelative("height").intValue = sd.height;
            sdd.FindPropertyRelative("name").stringValue = sd.name;

            sdd.FindPropertyRelative("borderLeft").intValue = sd.borderLeft;
            sdd.FindPropertyRelative("borderRight").intValue = sd.borderRight;
            sdd.FindPropertyRelative("borderTop").intValue = sd.borderTop;
            sdd.FindPropertyRelative("borderBottom").intValue = sd.borderBottom;

            sdd.FindPropertyRelative("paddingLeft").intValue = sd.paddingLeft;
            sdd.FindPropertyRelative("paddingRight").intValue = sd.paddingRight;
            sdd.FindPropertyRelative("paddingTop").intValue = sd.paddingTop;
            sdd.FindPropertyRelative("paddingBottom").intValue = sd.paddingBottom;

        }
        so.ApplyModifiedProperties();
        AssetDatabase.SaveAssets();
    }
    public static void ScaleTextureBilinear(Texture2D originalTexture, float scaleFactor, out float ws, out float hs)
    {
        int orgWidth = originalTexture.width;
        int orgHeight = originalTexture.height;

        int width = Mathf.RoundToInt(orgWidth * scaleFactor);
        int height = Mathf.RoundToInt(orgHeight * scaleFactor);

        int wf = (width % SIZE_ALIGN);
        int hf = (height % SIZE_ALIGN);
        if (wf != 0)
            width += SIZE_ALIGN - wf;
        if (hf != 0)
            height += SIZE_ALIGN - hf;
       
        ws = width / (float)orgWidth;
        hs = height / (float)orgHeight;

        int newWidth = Mathf.RoundToInt(ws * orgWidth);
        int newHeight = Mathf.RoundToInt(hs * orgHeight);

        if (newWidth != width || newHeight != height)
        {
            Debug.Log("贴图的缩放不相等: " + newWidth + ", " + newHeight + ", " + width + ", " + height);
        }
    }  
    static void ScalingAtlas(string path, float wscaling, float hscaling)
    {
        Object obj = AssetDatabase.LoadAssetAtPath(path, typeof(GameObject));
        if(obj == null)
        {
            Debug.Log("加载图集失败:" + path);
            return;
        }
        GameObject go = (GameObject)obj;
        UIAtlas atl = go.GetComponent<UIAtlas>();
        if (atl == null)
            return;
        SerializedObject so = new SerializedObject(atl);
        SerializedProperty spp = so.FindProperty("mSprites");
        for (int i = 0; i < spp.arraySize; ++i)
        {
            SerializedProperty sdd = spp.GetArrayElementAtIndex(i);
            sdd.FindPropertyRelative("x").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("x").intValue * wscaling);
            sdd.FindPropertyRelative("y").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("y").intValue * hscaling);
            sdd.FindPropertyRelative("width").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("width").intValue * wscaling);
            sdd.FindPropertyRelative("height").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("height").intValue * hscaling);

            sdd.FindPropertyRelative("borderLeft").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("borderLeft").intValue * wscaling);
            sdd.FindPropertyRelative("borderRight").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("borderRight").intValue * wscaling);
            sdd.FindPropertyRelative("borderTop").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("borderTop").intValue * hscaling);
            sdd.FindPropertyRelative("borderBottom").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("borderBottom").intValue * hscaling);

            sdd.FindPropertyRelative("paddingLeft").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("paddingLeft").intValue * wscaling);
            sdd.FindPropertyRelative("paddingRight").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("paddingRight").intValue * wscaling);
            sdd.FindPropertyRelative("paddingTop").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("paddingTop").intValue * hscaling);
            sdd.FindPropertyRelative("paddingBottom").intValue = Mathf.RoundToInt(sdd.FindPropertyRelative("paddingBottom").intValue * hscaling);

        }
        so.ApplyModifiedProperties();
        AssetDatabase.SaveAssets();
    }
    static void ScalingFont(UIFont font, float wscaling, float hscaling)
    {
        SerializedObject so = new SerializedObject(font);
        SerializedProperty spp = so.FindProperty("mFont");
        spp.FindPropertyRelative("mWidth").intValue = Mathf.RoundToInt(spp.FindPropertyRelative("mWidth").intValue * wscaling);
        spp.FindPropertyRelative("mHeight").intValue = Mathf.RoundToInt(spp.FindPropertyRelative("mHeight").intValue * hscaling);
        SerializedProperty gylist = spp.FindPropertyRelative("mSaved");
        for(int i = 0; i < gylist.arraySize; ++i)
        {
            SerializedProperty s = gylist.GetArrayElementAtIndex(i);

            s.FindPropertyRelative("x").intValue = Mathf.RoundToInt(s.FindPropertyRelative("x").intValue * wscaling);
            s.FindPropertyRelative("y").intValue = Mathf.RoundToInt(s.FindPropertyRelative("y").intValue * hscaling);

            s.FindPropertyRelative("width").intValue = Mathf.RoundToInt(s.FindPropertyRelative("width").intValue * wscaling);
            s.FindPropertyRelative("height").intValue = Mathf.RoundToInt(s.FindPropertyRelative("height").intValue * hscaling);

            s.FindPropertyRelative("offsetX").intValue = Mathf.RoundToInt(s.FindPropertyRelative("offsetX").intValue * wscaling);
            s.FindPropertyRelative("offsetY").intValue = Mathf.RoundToInt(s.FindPropertyRelative("offsetY").intValue * hscaling);
        }
        so.ApplyModifiedProperties();
        AssetDatabase.SaveAssets();
    }

    static void ScalingTex(string path, float scaling, out float ws, out float hs)
    {
        TextureImporter ti =AssetImporter.GetAtPath(path) as TextureImporter;
        if(ti == null)
        {
            Debug.Log("图片转换失败:" + path);
            ws = scaling;
            hs = scaling;
            return;
        }
        ti.isReadable = true;
        ti.npotScale = TextureImporterNPOTScale.None;
        ti.maxTextureSize = 2048;
        ti.SaveAndReimport();
        ti = null;
        Texture2D tex = (Texture2D)AssetDatabase.LoadAssetAtPath(path, typeof(Texture2D));
        if (tex.width <= 32 || tex.height <= 32)
        {
            ws = 1.0f;
            hs = 1.0f;
        }
        else
        {
            if (path.IndexOf("FishRes/") != -1)
            {
                ws = 0.5f;
                hs = 0.5f;
            }
            else
                ScaleTextureBilinear(tex, scaling, out ws, out hs);
            if (ws * tex.width < 1 || hs * tex.height < 1)
            {
                Debug.Log("错误的缩放:" + tex.width + "," + tex.height + "," + ws + "," + hs + ",(" + ws * tex.width + ", " + hs * tex.height + ")");
            }
            else
            {
                bool bRet = VataTex.TexResize.Resize(path, ws, hs, VataTex.IMAGE_FILTER.FILTER_CATMULLROM);
                if (!bRet)
                {
                    Debug.Log("转换图片失败:" + path);
                }
            }
        }
        ti = AssetImporter.GetAtPath(path) as TextureImporter;
        ti.isReadable = false;
        ti.npotScale = TextureImporterNPOTScale.None;
        ti.SaveAndReimport();
        AssetDatabase.Refresh();
    }
    static bool IsCopy(string dirStr)
    {

        if (dirStr.LastIndexOf(".svn") != -1 ||
               dirStr.LastIndexOf("/library") != -1 ||
               dirStr.LastIndexOf("/obj") != -1 ||
               dirStr.LastIndexOf("/temp") != -1 ||
               dirStr.LastIndexOf("/assets/data") != -1 ||
               dirStr.LastIndexOf("/assets/streamingassets") != -1)
            return false;
        else
            return true;
    }
    static void CopyDirectory(string srcDir, string tgtDir)
    {
        DirectoryInfo source = new DirectoryInfo(srcDir);
        DirectoryInfo target = new DirectoryInfo(tgtDir);

        if (!source.Exists)
        {
            return;
        }

        if (!target.Exists)
        {
            target.Create();
        }

        FileInfo[] files = source.GetFiles();

        for (int i = 0; i < files.Length; i++)
        {
            string dirStr = files[i].FullName.ToLower().Replace('\\', '/');
            if (IsCopy(dirStr) == false)
                continue;
            string targetPath = target.FullName + @"\" + files[i].Name;
            try
            {
                string sourceFile = files[i].FullName.Replace('\\', '/');
                targetPath = targetPath.Replace('\\', '/');
                File.Copy(sourceFile, targetPath, true);
            }
            catch
            {
                Debug.Log("文件Copy失败:" + targetPath);
            }
        }

        DirectoryInfo[] dirs = source.GetDirectories();

        for (int j = 0; j < dirs.Length; j++)
        {
            string dirStr = dirs[j].FullName.ToLower().Replace('\\', '/');
            if (IsCopy(dirStr) == false)
                continue;
            CopyDirectory(dirs[j].FullName, target.FullName + @"/" + dirs[j].Name);
        }
    }
    static void CopyDirectoryOnlyInnerRes(string srcDir, string tgtDir)
    {
        DirectoryInfo source = new DirectoryInfo(srcDir);
        DirectoryInfo target = new DirectoryInfo(tgtDir);

        if (!source.Exists)
        {
            return;
        }

        if (!target.Exists)
        {
            target.Create();
        }

        FileInfo[] files = source.GetFiles();

        for (int i = 0; i < files.Length; i++)
        {
            string dirStr = files[i].FullName.ToLower().Replace('\\', '/');
            if (IsCopy(dirStr) == false)
                continue;

            if (dirStr.IndexOf("/resources/fishres") != -1 ||
                dirStr.IndexOf("/resources/pathres") != -1 ||
                dirStr.IndexOf("/resources/extrares") != -1 ||
                dirStr.IndexOf("/resources/sceneres") != -1 ||
                dirStr.IndexOf("/resources/hallres") != -1 ||
                dirStr.IndexOf("/resources/logonres") != -1 ||
                dirStr.IndexOf("/resources/globalres") != -1)
                continue;
            string targetPath = target.FullName + @"\" + files[i].Name;
            try
            {
                string sourceFile = files[i].FullName.Replace('\\', '/');
                targetPath = targetPath.Replace('\\', '/');
                File.Copy(sourceFile, targetPath, true);
            }
            catch
            {
                Debug.Log("文件Copy失败:" + targetPath);
            }
        }

        DirectoryInfo[] dirs = source.GetDirectories();

        for (int i = 0; i < dirs.Length; i++)
        {
            string dirStr = dirs[i].FullName.ToLower().Replace('\\', '/');
            if (IsCopy(dirStr) == false)
                continue;

            if (dirStr.IndexOf("/resources/fishres") != -1 ||
               dirStr.IndexOf("/resources/pathres") != -1 ||
               dirStr.IndexOf("/resources/extrares") != -1 ||
               dirStr.IndexOf("/resources/sceneres") != -1 ||
               dirStr.IndexOf("/resources/hallres") != -1 ||
               dirStr.IndexOf("/resources/logonres") != -1 ||
               dirStr.IndexOf("/resources/globalres") != -1 ||
               dirStr.IndexOf("/resources/gameres") != -1)
                continue;
            CopyDirectoryOnlyInnerRes(dirs[i].FullName, target.FullName + @"/" + dirs[i].Name);
        }
    }
    public static void CreateSDRes(float scaling)
    {
        string sdfile = Application.dataPath + "/sd.dat";
        if (File.Exists(sdfile))
        {
            EditorUtility.DisplayDialog("错误", "当前SD目录已经是低清版本", "OK");
            return;
        }

        if (Application.dataPath.LastIndexOf("NewClient_SD") == -1)
        {
            EditorUtility.DisplayDialog("错误", "当前目录不是SD目录", "OK");
            return;
        }

        EditorUtility.DisplayProgressBar("稍候", "正在转换资源", 0.4f);
        List<FileFlagData> ffdList = new List<FileFlagData>();
        string dir = Application.dataPath + "/Resources";
        

        for (int i = 0; i < ResManager.RES_NUM; ++i)
        {
            ResType rt = (ResType)i;
            string[] fileEntries = Directory.GetFiles(dir + "/" + rt.ToString(), "*", SearchOption.AllDirectories);
            foreach (string s in fileEntries)
            {
                string lower = s.ToLower();
                if (lower.LastIndexOf(".meta") != -1 ||
                    lower.LastIndexOf(".svn") != -1)
                {
                    continue;
                }
                if (lower.LastIndexOf(".png") != -1 ||
                    lower.LastIndexOf(".jpg") != -1 ||
                    lower.LastIndexOf(".tga") != -1)
                {
                    FileFlagData ffd = new FileFlagData();
                    ffd.path = s.Replace('\\', '/');
                    ffd.Prefab = false;
                    ffdList.Add(ffd);
                }
                else if (lower.LastIndexOf(".prefab") != -1)
                {
                    FileFlagData ffd = new FileFlagData();
                    ffd.path = s.Replace('\\', '/');
                    ffd.Prefab = true;
                    ffdList.Add(ffd);
                }
            }
        }

        //字体不缩放
        Dictionary<string, bool> fontMap = new Dictionary<string, bool>();
        foreach (FileFlagData ffd in ffdList)
        {
            string filePath = "Assets" + ffd.path.Replace(Application.dataPath, "");
            if (ffd.Prefab)
            {
                Object obj = AssetDatabase.LoadAssetAtPath(filePath, typeof(GameObject));
                if (obj == null)
                {
                    Debug.Log("加载图集失败:" + filePath);
                    continue;
                }
                GameObject go = (GameObject)obj;
                UIFont font = go.GetComponent<UIFont>();
                if (font != null)
                {
                    if (font.atlas != null)
                    {
                        EditorUtility.DisplayDialog("错误", "字体只能使用材质，不能使用图集:" + filePath, "OK");
                        EditorUtility.ClearProgressBar();
                        Debug.Log("转换SD资源失败!");
                        return;
                    }
                    if (font.material == null)
                    {
                        continue;
                        EditorUtility.DisplayDialog("错误", "字体未找到材质属性:" + filePath, "OK");
                        EditorUtility.ClearProgressBar();
                        Debug.Log("转换SD资源失败!");
                        return;
                    }
                    //保存字体使用的图片，不做缩放
                    if (font.material.mainTexture != null)
                    {
                        string picPath = AssetDatabase.GetAssetPath(font.material.mainTexture.GetInstanceID());
                        if (fontMap.ContainsKey(picPath) == false)
                            fontMap.Add(picPath, true);
                        else
                            Debug.Log("相同的路径:" + picPath);
                    }
                }
            }
        }

        EditorUtility.DisplayProgressBar("稍候", "正在转换贴图", 0.45f);
        Dictionary<string, FileFlagData> picList = new Dictionary<string, FileFlagData>();
        foreach (FileFlagData ffd in ffdList)
        {
            string filePath = "Assets" + ffd.path.Replace(Application.dataPath, "");
            if (!ffd.Prefab)
            {
                if (fontMap.ContainsKey(filePath))
                {
                    Debug.Log("字体不转换:" + filePath);
                    continue;
                }
                //系统设置弹出图片不缩放
                if (filePath.LastIndexOf("SceneBtnWindImage") != -1 || filePath.LastIndexOf("BackImages/BackImg") != -1 || filePath.LastIndexOf("FishShowImage") != -1)
                {
                    ffd.wscaling = 1.0f;
                    ffd.hscaling = 1.0f;
                }
                else
                {
                    ScalingTex(filePath, scaling, out ffd.wscaling, out ffd.hscaling);
                }
                picList.Add(filePath, ffd);
            }
        }
        EditorUtility.DisplayProgressBar("稍候", "正在转换图集", 0.7f);
        foreach (FileFlagData ffd in ffdList)
        {
            string filePath = "Assets" + ffd.path.Replace(Application.dataPath, "");
            if (ffd.Prefab)
            {
                Object obj = AssetDatabase.LoadAssetAtPath(filePath, typeof(GameObject));
                if (obj == null)
                {
                    Debug.Log("加载图集失败:" + filePath);
                    continue;
                }
                GameObject go = (GameObject)obj;
                UIAtlas atl = go.GetComponent<UIAtlas>();
                if (atl != null)
                {
                    if (atl.spriteMaterial == null || atl.spriteMaterial.mainTexture == null)
                    {
                        Debug.Log("不存在的图集的贴图:" + filePath);
                        continue;
                    }
                    string picPath = AssetDatabase.GetAssetPath(atl.spriteMaterial.mainTexture.GetInstanceID());
                    FileFlagData picffd;
                    if (picList.TryGetValue(picPath, out picffd))
                    {
                        ScalingAtlas(filePath, picffd.wscaling, picffd.hscaling);
                    }
                    else
                    {
                        Debug.Log("不存在图集的贴图,atlas:" + filePath + ", pic:" + picPath);
                    }
                }
            }
        }
        
        EditorUtility.ClearProgressBar();
        FileStream fs = File.Create(sdfile);
        if (fs != null)
            fs.Close();
        Debug.Log("转换SD资源成功!");
    }
    [MenuItem("Buyu/转换SD资源(0.1)")]
    public static void CreateSD111()
    {
        CreateSDRes(0.1f);
    }
    [MenuItem("Buyu/转换SD资源(0.5)")]
    public static void CreateSD11()
    {
        CreateSDRes(0.5f);
    }
    [MenuItem("Buyu/转换SD资源(0.55)")]
    public static void CreateSD12()
    {
        CreateSDRes(0.55f);
    }
    [MenuItem("Buyu/转换SD资源(0.6)")]
    public static void CreateSD123()
    {
        CreateSDRes(0.6f);
    }
    [MenuItem("Buyu/转换SD资源(0.65)")]
    public static void CreateSD1()
    {
        CreateSDRes(0.65f);
    }
    [MenuItem("Buyu/转换SD资源(0.7)")]
    public static void CreateSD2()
    {
        CreateSDRes(0.7f);
    }
    [MenuItem("Buyu/转换SD资源(0.75)")]
    public static void CreateSD3()
    {
        CreateSDRes(0.75f);
    }
    [MenuItem("Buyu/转换SD资源(1.0)")]
    public static void CreateSD133()
    {
        CreateSDRes(1.0f);
    }
    [MenuItem("Buyu/创建标清目录")]
    public static void CopyToSD()
    {
        //copy to sd
        if(Application.dataPath.LastIndexOf(Exporter.SD_DIR) != -1)
        {
            EditorUtility.DisplayDialog("错误", "当前目录已经是SD目录","OK");
            return;
        }
        EditorUtility.DisplayProgressBar("稍候", "正在创建标清目录", 0.2f);
        string pathDir = Application.dataPath;
        int idx = pathDir.LastIndexOf('/');
        if(idx != -1)
            pathDir = pathDir.Substring(0, idx);
        idx = pathDir.LastIndexOf('/');
        if (idx != -1)
            pathDir = pathDir.Substring(0, idx);
        pathDir += "/" + Exporter.SD_DIR;

        if (Directory.Exists(pathDir) == false)
            Directory.CreateDirectory(pathDir);

        string pathStr = Application.dataPath;
        idx = pathStr.LastIndexOf("/");
        pathStr = pathStr.Substring(0, idx);

        DeleteDirAndFile(pathDir);

        CopyDirectory(pathStr, pathDir);

        EditorUtility.ClearProgressBar();
        Debug.Log("创建标清目录成功!");
    }
    static void DeleteDirAndFile(string path)
    {
        string[] fileEntries = Directory.GetFiles(path, "*", SearchOption.TopDirectoryOnly);
        foreach (string s in fileEntries)
        {
            try
            {
                File.Delete(s);
            }
            catch
            {
                Debug.Log("文件删除失败:" + s);
            }
        }
        string[] fileEntries2 = Directory.GetDirectories(path, "*", SearchOption.TopDirectoryOnly);
        foreach (string s in fileEntries2)
        {
            string filePath = s.ToLower().Replace('\\','/');
            if (filePath.LastIndexOf("/temp") != -1 ||
                filePath.LastIndexOf("/library") != -1 ||
                filePath.LastIndexOf("/obj") != -1)
                continue;

            DeleteDirAndFile(filePath);
            if (Directory.Exists(filePath))
            {
                try
                {
                    Directory.Delete(filePath);
                }
                catch
                {
                    Debug.Log("文件夹删除失败:" + filePath);
                }
            }
        }
    }
    [MenuItem("Buyu/创建发布目录")]
    public static void CopyToPublish()
    {
        //copy to sd
        if (Application.dataPath.LastIndexOf(Exporter.PUBLISH_DIR) != -1)
        {
            EditorUtility.DisplayDialog("错误", "当前目录已经是发布目录", "OK");
            return;
        }
        if (Application.dataPath.LastIndexOf(Exporter.SD_DIR) != -1)
        {
            EditorUtility.DisplayDialog("错误", "当前目录不是原始目录", "OK");
            return;
        }
        EditorUtility.DisplayProgressBar("稍候", "正在创建发布目录", 0.2f);
        string pathDir = Application.dataPath;
        int idx = pathDir.LastIndexOf('/');
        if (idx != -1)
            pathDir = pathDir.Substring(0, idx);
        idx = pathDir.LastIndexOf('/');
        if (idx != -1)
            pathDir = pathDir.Substring(0, idx);
        pathDir += "/" + Exporter.PUBLISH_DIR;

        if (Directory.Exists(pathDir) == false)
            Directory.CreateDirectory(pathDir);

        string pathStr = Application.dataPath;
        idx = pathStr.LastIndexOf("/");
        pathStr = pathStr.Substring(0, idx);

        DeleteDirAndFile(pathDir);

        CopyDirectoryOnlyInnerRes(pathStr, pathDir);

        EditorUtility.ClearProgressBar();
        Debug.Log("创建发布目录成功!");
    }
    [MenuItem("Buyu/生成非SVN目录")]
    public static void CopyToNewClient()
    {
        if (Application.dataPath.LastIndexOf("NewClient_SD") != -1)
        {
            EditorUtility.DisplayDialog("错误", "只有非SD目录才能操作", "OK");
            return;
        }
        EditorUtility.DisplayProgressBar("稍候", "正在生成非SVN目录", 0.4f);

        string dir = ExporterTools.GetRootDir("NewClient_NoneSVN");
        if (Directory.Exists(dir) == false)
            Directory.CreateDirectory(dir);

        DeleteDirAndFile(dir);

        string path = Application.dataPath;
        int idx = path.LastIndexOf("/");
        path = path.Substring(0, idx);
        CopyDirectory(path, dir);

        EditorUtility.ClearProgressBar();
        Debug.Log("生成非SVN目录成功!");
    }
    public static string GetRootDir(string dir)
    {
        string pathDir = Application.dataPath;
        int idx = pathDir.LastIndexOf('/');
        if (idx != -1)
            pathDir = pathDir.Substring(0, idx);
        idx = pathDir.LastIndexOf('/');
        if (idx != -1)
            pathDir = pathDir.Substring(0, idx);
        pathDir += "/" + dir;
        if (Directory.Exists(pathDir) == false)
            Directory.CreateDirectory(pathDir);
        return pathDir;
    }
    static void ShowMsg(string str)
    {
        EditorUtility.DisplayDialog("错误", str, "OK");
    }
    static string ReplaceCRCAndSize(string xml, uint crc, uint size)
    {
        int idx  = xml.IndexOf(Exporter.CLIENT_CRC_TEMP);
        if (idx == -1)
        {
            ShowMsg("XML中未找到client_crc节点");
            return null;
        }
        idx += Exporter.CLIENT_CRC_TEMP.Length + 1;
        int idx2 = xml.IndexOf('"', idx);
        xml = xml.Remove(idx, idx2 - idx);
        xml = xml.Insert(idx, crc.ToString());

        idx = xml.IndexOf(Exporter.CLIENT_SIZE_TEMP);
        if (idx == -1)
        {
            ShowMsg("XML中未找到client_size节点");
            return null;
        }
        idx += Exporter.CLIENT_SIZE_TEMP.Length + 1;
        idx2 = xml.IndexOf('"', idx);
        xml = xml.Remove(idx, idx2 - idx);
        xml = xml.Insert(idx, size.ToString());
        return xml;
    }
    static bool ReplaceFile(string path, uint crc, uint size )
    {
        if (File.Exists(path) == false)
            return false;
        StreamReader sr = new StreamReader(path, new System.Text.UTF8Encoding(false));
        if (sr == null)
            return false;
        string xml = sr.ReadToEnd();
        sr.Close();
        xml = ReplaceCRCAndSize(xml, crc, size);
        if (xml == null)
            return false;
        File.Delete(path);
        StreamWriter sw = new StreamWriter(path, false, new System.Text.UTF8Encoding(false));
        if (sw == null)
            return false;
        sw.Write(xml);
        sw.Close();
        return true;
    }
   
    static void ModifyLabel(UILabel label)
    {
        Object obj = AssetDatabase.LoadAssetAtPath("assets/resources/innerres/myfontprefab.prefab", typeof(GameObject));
        SerializedObject so = new SerializedObject(label);
        SerializedProperty spp = so.FindProperty("mFont");
        if (spp == null)
            return;
        if (spp.objectReferenceValue == null)
        {
            spp.objectReferenceValue = obj;
            so.ApplyModifiedProperties();
            AssetDatabase.SaveAssets();
        }
        else
        {
            //Debug.Log("已经使用了自定义字体:" + spp.objectReferenceValue.ToString());
        }
    }
    static void FindLabel(GameObject go, ref int updateCount)
    {
        UILabel[] ll = go.GetComponents<UILabel>();
        updateCount += ll.Length;
        foreach(UILabel i in ll)
        {
            ModifyLabel(i);
        }
        for(int i = 0; i < go.transform.childCount; ++i)
        {
            FindLabel(go.transform.GetChild(i).gameObject, ref updateCount);
        }
    }
    public static bool ModifyFont(List<string> pathList)
    {
        bool bRet = true;
        int updateCount = 0;
        try
        {
            foreach (string path in pathList)
            {
                if (!IsPrefab(path))
                    continue;

                Object obj = AssetDatabase.LoadAssetAtPath(path, typeof(GameObject));
                if (obj == null)
                {
                    EditorUtility.DisplayDialog("错误", "打开Prefab失败:" + path, "确定");
                    continue;
                }
                GameObject go = obj as GameObject;
                FindLabel(go, ref updateCount);
            }
        }
        catch (System.Exception)
        {
            bRet = false;
        }
        Debug.Log("转换字体:" + updateCount + "个.");
        return bRet;
    }
}