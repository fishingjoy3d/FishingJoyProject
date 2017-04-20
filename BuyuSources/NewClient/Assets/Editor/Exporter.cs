using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;
using System.Text;
using System.Xml;

 public class Exporter
 {
    public static string RES_EXT = ".dat";
    //客户端下载地址
    public static string IOS_CLIENT_URL_IMG = "https://{0}/FishPublish/{1}/{2}/icon.png";
    public static string CLIENT_VER;
    public static string[] RES_VERSION = new string[ResManager.RES_NUM];
    public static string SD_DIR = "NewClient_SD";
    public static string PUBLISH_DIR = "NewClient_Publish";
    public static string PACKAGE_DIR = "Packages";
    public static string TEMP_DIR = "Temp";
    public static string VERSION_DIR = "Publish";
    public static string CLIENT_CRC = " client_crc=\"0\" ";
    public static string CLIENT_SIZE = " client_size=\"0\" ";
    public static string CLIENT_CRC_TEMP = " client_crc=";
    public static string CLIENT_SIZE_TEMP = " client_size=";
    public static string CUR_RELEASE_DIR;
    struct ResData
    {
        public uint fileSize;
        public uint unzipFileSize;
        public uint fileCrc;
    };
    class ResDataList
    {
        public ResData[] ResList = new ResData[ResManager.RES_NUM];
    }
    public enum ExportType
    {
        Publish,
        Test,
        Editor,
        MAX
    }
    static string GetClientFilePath(string str, string ext, string dir)
    {
        string[] fileEntries = Directory.GetFiles(dir, ext, SearchOption.AllDirectories);
        foreach(string p in fileEntries)
        {
            if (p.IndexOf(str) != -1)
                return p;
        }
        return null;
    }
    public static void GetPlatformAppID(string packagname, out string appid, out string secret)
    {
        appid = "";
        secret = "";
        if (packagname == "_com.leduo.buyu3d")
        {
            appid = "wxd981f909976781d0";
            secret = "51c781291cc0cc7c55f8ef210497e43e";
        }
        else if (packagname == "_com.leduo.buyu.three")
        {
            appid = "wx38eb36a0f6904795";
            secret = "02c3e8484c2eef60e32f7a8aa5214b9b";
        }
        else if (packagname == "_com.leduo.buyu.uc")
        {
            appid = "wx674601e7a6625146";
            secret = "f3ea1a7ddc86df60839065dce8b06bd7";
        }
        else if (packagname == "_com.leduo.buyu.baidu")
        {
            appid = "wx8d20a7b3fe2091d6";
            secret = "e54efbca5e371e9291357cf1eb291680";
        }
        else if (packagname == "_com.leduo.buyu.huawei")
        {
            appid = "wx57d29ca51b4021ed";
            secret = "0206c470853ca921e8190cd6c5ec465a";
        }
        else if (packagname == "_com.leduo.buyu.leshi")
        {
            appid = "wx5251a2a70deb165d";
            secret = "512c633ea48a8912f2bf8ec167f2a42b";
        }
        else if (packagname == "_com.leduo.buyu.egame")
        {
            appid = "wxbde863e81d3d036a";
            secret = "3fba06ebcd0ac40c244c849c8a3c50ea";
        }
        else if (packagname == "_com.leduo.buyu.nearme.gamecenter")
        {
            appid = "wxccb136e5f5b48593";
            secret = "82c97160fbe939030d34b4da254475c8";
        }
        else// if(packagname == "com.leduo.buyu.self")
        {
            appid = "wxc347ead5686ce7e1";
            secret = "c82089f40722b2b799674d0c03a34179";
        }
    }
    public static void SaveVersionFile(PlatformType pt, ExportType export)
    {
        EditorUtility.DisplayProgressBar("打包中...", "正在生成Version文件!", 0.9f);
        string verPath = Application.dataPath + "/Resources/InnerRes/version.bytes";
        string expPath;
        if(export == ExportType.Publish)
            expPath = Application.dataPath + "/ExportSetting/Setting_Publish.xml";
        else
            expPath = Application.dataPath + "/ExportSetting/Setting_Test.xml";
        XmlDocument doc1 = new XmlDocument();
        XmlDocument doc2 = new XmlDocument();
        XmlReaderSettings settings = new XmlReaderSettings();
        settings.IgnoreComments = true;
        XmlReader reader1 = XmlReader.Create(verPath, settings);
        XmlReader reader2 = XmlReader.Create(expPath, settings);
        
        doc1.Load(reader1);
        doc2.Load(reader2);
        
        XmlElement xe1 = doc1.DocumentElement;
        XmlElement xe2 = doc2.DocumentElement;
        XmlNode curServer = xe2.SelectSingleNode("Server/" + export.ToString());
        XmlNode clientList = xe2.SelectSingleNode("ClientList");
        XmlNode services = xe2.SelectSingleNode("Services");
        XmlNode share = xe2.SelectSingleNode("Share");
        XmlNode reporter = xe1.SelectSingleNode("Reporter");
        string clientdir_str = curServer.Attributes["clientdir"].Value;
        string resdir_str = curServer.Attributes["resdir"].Value;
        string ClientVer = xe1.SelectSingleNode("Params/Version").FirstChild.Value;
        string ResVer = xe1.SelectSingleNode("Params/Resource").FirstChild.Value;
        string showPing = xe1.SelectSingleNode("Params/Ping").FirstChild.Value;
        string OrgVer = ClientVer;
        string DatStr = ResVer;// "_" + System.DateTime.Now.Year + "_" + System.DateTime.Now.Month + "_" + System.DateTime.Now.Day;
        string RootDir = ExporterTools.GetRootDir("FishPublish");
        //ClientVer += DatStr;
        if (!Directory.Exists(RootDir))
            Directory.CreateDirectory(RootDir);
        string ResDir = RootDir + "/" + resdir_str + "/" + ResVer + "/";
        string ClientDir = RootDir + "/" + clientdir_str + "/" + ClientVer + "/";
        if (!Directory.Exists(ResDir))
            Directory.CreateDirectory(ResDir);
        if (!Directory.Exists(ClientDir))
            Directory.CreateDirectory(ClientDir);
        //1.Copy Res To DestDir
        string[] resList = null;
        if(true)
            resList= new string[] { "HD", "SD" };
        else
            resList = new string[] { "HD"};

        ResDataList[] resDataList = new ResDataList[resList.Length];
        for (int i = 0; i < resList.Length; ++i)
        {
            resDataList[i] = new ResDataList();
            string srcDir = ExporterTools.GetRootDir(PACKAGE_DIR) + "/FishRes_" + pt.ToString() + "_Zip/" +resList[0]+"/";
            string destDir = ResDir  + pt.ToString() + "_" + resList[i] + "/";

            if (!Directory.Exists(destDir))
                Directory.CreateDirectory(destDir);
            for (int j = 0; j < ResManager.RES_NUM; ++j)
            {
                string path =  pt.ToString() + "_" + ((ResType)j).ToString() + RES_EXT;
                string filePath = srcDir + path;
                string copyfile = destDir + path;
                resDataList[i].ResList[j].fileCrc = Crc.Crc32FromFile(filePath);
                if (File.Exists(filePath) == false)
                {
                    Debug.Log("不存在的文件:" + filePath);
                    EditorUtility.ClearProgressBar();
                    break;
                }
                File.Delete(copyfile);
                File.Copy(filePath, copyfile);
                resDataList[i].ResList[j].fileSize = (uint)new FileInfo(filePath).Length;
                string tempfile = filePath + ".temp";
                LZMA.DecompressFile(filePath, tempfile);
                resDataList[i].ResList[j].unzipFileSize = (uint)new FileInfo(tempfile).Length;
                File.Delete(tempfile);
            }
        }
        //2.Create XML

        int[] winSizeList = null;
        //3. Copy client Software
        ResData[] clientData = new ResData[clientList.ChildNodes.Count];
        string publishDir = ExporterTools.GetRootDir(Exporter.VERSION_DIR) +"/";
        if (export == ExportType.Test)
            publishDir += "Test/";
        else if (export == ExportType.Publish)
            publishDir += "Publish/";
        XmlNode serverNode = curServer;
        for(int i = 0; i < clientList.ChildNodes.Count; ++i)
        {
            clientData[i] = new ResData();
            XmlNode clientNode = clientList.ChildNodes[i];
            string type = clientNode.Attributes["type"].Value;
            string extname = clientNode.Attributes["xmlext"].Value;
            string findext = "";
            if (clientNode.FirstChild != null)
                findext = clientNode.FirstChild.Value;
            if (type != pt.ToString())
                continue;
            if(type == "Android")
            {
                string srcFile = GetClientFilePath(extname + findext, "*.apk", publishDir);
                if (extname != "")
                    extname = "_" + extname;
                string clientName = "LeDuoBuYu3D" + extname + ".apk";
                
                if(srcFile == null)
                {
                    EditorUtility.DisplayDialog("错误", "未找到文件:" + (extname + findext), "确定");
                    EditorUtility.ClearProgressBar();
                    return;
                }
                string destFile = ClientDir + clientName;
                File.Delete(destFile);
                File.Copy(srcFile, destFile);
                clientData[i].fileCrc = Crc.Crc32FromFile(destFile);
                clientData[i].fileSize = (uint)new FileInfo(destFile).Length;
            }
            else if(type == "IOS")
            {
                string findex = extname + findext;
                if (findex == "com.leduo.buyu3d")
                    findex = "com.leduo.buyu.self";
                string srcFile = GetClientFilePath(findex, "*.ipa", publishDir);
                if (extname != "")
                    extname = "_" + extname;
                
                string clientName = "LeDuoBuYu3D"+ extname + ".ipa";
                
                if (File.Exists(srcFile) == false)
                {
                    EditorUtility.DisplayDialog("错误", "未找到文件:" + (extname + findext), "确定");
                    EditorUtility.ClearProgressBar();
                    return;
                }
                string destFile = ClientDir + clientName;
                File.Delete(destFile);
                File.Copy(srcFile, destFile);
                clientData[i].fileCrc = Crc.Crc32FromFile(destFile);
                clientData[i].fileSize = (uint)new FileInfo(destFile).Length;

                //生成XML
                //-----------------------------------------------------
                string IOS_CLIENT_URL_IMG = string.Format("https://ios.52leduo.com/FishPublish/" + clientdir_str + "/{0}/icon.png", ClientVer);
                string icon = Application.dataPath + "/PList/icon.png";
                string plist = Application.dataPath + "/PList/LeDuoBuYu3D.plist";

                string newicon = ClientDir + "icon.png";
                string newplist = ClientDir + clientName.Replace(".ipa", ".plist");
                File.Delete(newicon);
                File.Delete(newplist);
                StreamReader sr = new StreamReader(plist, new UTF8Encoding(false));
                string str = sr.ReadToEnd();
                string ipaclientName = clientName;
                string ipaurl = "https://ios.52leduo.com/FishPublish/" + clientdir_str + "/" + ClientVer + "/" + ipaclientName;
                str = str.Replace("CLIENT_URL", ipaurl);
                str = str.Replace("CLIENT_IMG", IOS_CLIENT_URL_IMG);
                str = str.Replace("CLIENT_VER", ClientVer);
                StreamWriter sw = new StreamWriter(newplist, false, new UTF8Encoding(false));
                sw.Write(str);
                sw.Close();
                File.Copy(icon, newicon);
                //-----------------------------------------------------
            }
            else
            {
                string clientName = "LeDuoBuYu3D.win";
                string str = export == ExportType.Publish ? "/外网" : "/内网";
                string winDir = ExporterTools.GetRootDir(Exporter.VERSION_DIR) +str+"/乐多捕鱼3D_Data/";
                string[] strList = new string[]
                {
                    winDir + "Managed/Assembly-CSharp.dll",
                    winDir + "maindata",
                    winDir + "resources.assets",
                    winDir + "screenselector.bmp",
                    winDir + "sharedassets0.assets",
                };
                winSizeList = new int[strList.Length];
                List<byte> dataList = new List<byte>();
                for(int n = 0; n < strList.Length; ++n)
                {
                    byte[] data = System.IO.File.ReadAllBytes(strList[n]);
                    if (data == null || data.Length == 0)
                    {
                        Debug.Log("未找到Windows客户端文件。");
                        return;
                    }
                    winSizeList[n] = data.Length;
                    Debug.Log("WinSize" + n + ":" + winSizeList[n]);
                    dataList.AddRange(data);
                }
                string destFile = ClientDir + clientName;
                File.Delete(destFile);
                File.WriteAllBytes(destFile, dataList.ToArray());
                clientData[i].fileCrc = Crc.Crc32FromFile(destFile);
                clientData[i].fileSize = (uint)new FileInfo(destFile).Length;
            }
        }
        
        //Copy Plist
        //4.Create Version XML
        //for (int i = 0; i < serverList.ChildNodes.Count; ++i)
        {
            string dir = ResDir + curServer.Attributes["verdir"].Value +"/";
            if (!Directory.Exists(dir))
                Directory.CreateDirectory(dir);
            for(int j = 0; j < clientList.ChildNodes.Count; ++j)
            {
                XmlNode clientNode = clientList.ChildNodes[j];
                string type = clientNode.Attributes["type"].Value;
                string fileext = clientNode.Attributes["xmlext"].Value;
                if (fileext != "")
                    fileext = "_" + fileext;
                if (type != pt.ToString())
                    continue;

                string clientName = "";
                string clientURL="";
                if (type == "Android")
                {
                    
                    clientName = "LeDuoBuYu3D" + fileext + ".apk";
                    clientURL = "ftp://" + serverNode.Attributes["resftp"].Value + "/FishPublish/" + clientdir_str + "/" + ClientVer + "/" + clientName;
                }
                else if (type == "IOS")
                {
                    clientName = "LeDuoBuYu3D" + fileext + ".plist";
                    clientURL = "https://ios.52leduo.com/FishPublish/" + clientdir_str + "/" + ClientVer + "/" + clientName;
                }
                else
                {
                    clientName = "LeDuoBuYu3D.win";
                    clientURL = "https://" + serverNode.Attributes["resftp"].Value + "/FishPublish/" + clientdir_str + "/" + ClientVer + "/" + clientName;
                }
                //if (pt == PlatformType.IOS)
                //{
                    
                //    string IOS_CLIENT_URL_IMG = string.Format("https://{0}/FishPublish/" + clientdir_str + "/{1}/icon.png", serverNode.Attributes["resftp"].Value, ClientVer);
                //    string icon = Application.dataPath + "/PList/icon.png";
                //    string plist = Application.dataPath + "/PList/LeDuoBuYu3D.plist";
                    
                //    string newicon = ClientDir + "icon.png";
                //    string newplist = ClientDir + "LeDuoBuYu3D.plist";
                //    File.Delete(newicon);
                //    File.Delete(newplist);
                //    StreamReader sr = new StreamReader(plist, new UTF8Encoding(false));
                //    string str = sr.ReadToEnd();
                //    string ipaclientName = "LeDuoBuYu3D_com.leduo.buyu.self.ipa";
                //    string ipaurl = "https://" + serverNode.Attributes["resftp"].Value + "/FishPublish/" + clientdir_str + "/" + ClientVer + "/" + ipaclientName;
                //    str = str.Replace("CLIENT_URL", ipaurl);
                //    str = str.Replace("CLIENT_IMG", IOS_CLIENT_URL_IMG);
                //    StreamWriter sw = new StreamWriter(newplist, false, new UTF8Encoding(false));
                //    sw.Write(str);
                //    sw.Close();
                //    File.Copy(icon, newicon);
                //}
                string extrabtn = serverNode.Attributes["extrabtn"].Value;
                string exchange = serverNode.Attributes["exchange"].Value;
                string newftp   = serverNode.Attributes["newftp"].Value;
                string showgame   = serverNode.Attributes["showgame"].Value;
                string thirdbtn = serverNode.Attributes["thirdbtn"].Value;
                string showjbp = serverNode.Attributes["showjbp"].Value;
                string showmatch = serverNode.Attributes["showmatch"].Value;
                string showthirdlogin = serverNode.Attributes["showthirdlogin"].Value;
                for(int k = 0; k < resList.Length; ++k)
                {
                    string ftpURL = "ftp://" + serverNode.Attributes["resftp"].Value + "/FishPublish/" + resdir_str + "/" + ResVer + "/" + type + "_" + resList[0] + "/";

                    StringBuilder sb = new StringBuilder();
                    sb.Append("<Setting>");
                    string net = string.Format("<Net resftp=\"{0}\" clienturl=\"{1}\" runftp=\"ftp://{2}/FishPublish\" >", ftpURL, clientURL, serverNode.Attributes["runftp"].Value);
                    sb.Append(net);
                    sb.Append(serverNode.InnerXml);
                    sb.Append("</Net>");
                    sb.Append("<Client showthirdlogin=\"" + showthirdlogin + "\" extrabtn=\"" + extrabtn + "\" showjbp=\"" + showjbp + "\" showmatch=\"" + showmatch + "\" thirdbtn=\"" + thirdbtn + "\" exchange=\"" + exchange + "\" showgame=\"" + showgame + "\" newftp=\"" + newftp + "\" ping=\"" + showPing + "\" resver=\"" + Utility.VersionToUint(ResVer) + "\" crc=\"" + clientData[j].fileCrc + "\" size=\"" + clientData[j].fileSize + "\"");

                    if(type=="Windows")
                    {
                        for (int n = 0; n < winSizeList.Length; ++n)
                        {
                            sb.Append(" winsize"+(n + 1).ToString()+"=\"" + winSizeList[n].ToString() + "\" ");
                        }
                    }

                    sb.Append(">");
                    sb.Append(OrgVer);
                    sb.Append("</Client>");
                    for(int n = 0; n < (int)ResType.MAX; ++n)
                    {
                        ResType rt = (ResType)n;
                        sb.Append("<" + rt.ToString() + " size=\"" + resDataList[k].ResList[n].fileSize + "\"");
                        sb.Append(" unzipsize=\"" + resDataList[k].ResList[n].unzipFileSize + "\"");
                        sb.Append(" crc=\"" + resDataList[k].ResList[n].fileCrc + "\"");
                        sb.Append(" url=\"" + type +"_"+rt.ToString()+ RES_EXT + "\"");
                        sb.Append(" />");
                    }
                    string appid, appsec;
                    GetPlatformAppID(fileext, out appid, out appsec);
                    share.ChildNodes[0].Attributes["id"].Value = appid;
                    share.ChildNodes[0].Attributes["pwd"].Value = appsec;
                    sb.Append(services.OuterXml);
                    sb.Append(share.OuterXml);
                    sb.Append(reporter.OuterXml);
                    sb.Append("</Setting>");
                    string path = dir + type + "_" + resList[k] + "_Version" + fileext + ".xml";
                    if (File.Exists(path))
                        File.Delete(path);
                    Debug.Log("保存XML成功:" + path);
                    StreamWriter sw = new StreamWriter(path, false, new UTF8Encoding(false));
                    sw.Write(sb.ToString());
                    sw.Close();
                    if(type == "IOS" && fileext =="_com.leduo.buyu.self")
                    {
                        string[] strList = { "_com.leduo.buyu3d"};//, "_com.leduo.buyu.51zs", "_com.leduo.buyu.chuyou", "_com.leduo.ldbuyu.lehaihai", "_com.leduo.buyu.le8", "_com.leduo.buyu.kuaiyong", "_com.leduo.buyu.haima", "_com.leduo.buyu.guopan", "_com.leduo.ldbuyu.dangle", "_com.leduo.buyu.tb" };
                        foreach(string str in strList)
                        {
                            string appid2, appsec2;
                            GetPlatformAppID(str, out appid2, out appsec2);
                            sb = sb.Replace(appid, appid2).Replace(appsec, appsec2);
                            path = dir + type + "_" + resList[k] + "_Version" + str + ".xml";
                            if (File.Exists(path))
                                File.Delete(path);
                            Debug.Log("保存XML成功:" + path);
                            sw = new StreamWriter(path, false, new UTF8Encoding(false));
                            sw.Write(sb.ToString());
                            sw.Close();
                        }
                    }
                }
            }
        }
        reader2.Close();
        reader1.Close();
        EditorUtility.ClearProgressBar();
    }
    
    public static void PackDir(ResType res, BuildTarget bo, bool bcompress, bool bHD)
    {
        bHD = Application.dataPath.LastIndexOf(SD_DIR) == -1;
        string path = res.ToString();
        string platform ="";
        if (path.Length != 0)
        {
            path = path.Replace("Assets/", "");
            string findPath = Application.dataPath + "/Resources/" + path;
            string[] fileEntries = Directory.GetFiles(findPath, "*.*", SearchOption.AllDirectories);
            List<string> objList = new List<string>();
            List<string> prefabList = new List<string>();
            foreach (string fileName in fileEntries)
            {
                if (fileName.LastIndexOf(".meta") != -1)
                    continue;
                if (fileName.IndexOf(".svn") != -1)
                    continue;
                if (fileName.IndexOf(".anim") != -1)
                    continue;
                if (fileName.IndexOf(".controller") != -1)
                    continue;

                string filePath = fileName.Replace("\\", "/");
                filePath = filePath.Replace(Application.dataPath, "");
                string localPath = "Assets" + filePath;

                if (localPath.ToLower().LastIndexOf(".prefab") != -1)
                    prefabList.Add(localPath);
                else
                    objList.Add(localPath);
            }
            objList.AddRange(prefabList);
            //压缩
            
            if (bo == BuildTarget.Android)
                platform = PlatformType.Android.ToString();
            else if (bo == BuildTarget.iOS)
                platform = PlatformType.IOS.ToString();
            else if (bo == BuildTarget.StandaloneWindows)
                platform = PlatformType.Windows.ToString();
            else
                platform = PlatformType.OSX.ToString();
            string compress = bcompress ? "_Zip" : "";
            string hdstr = bHD ? "_HD_" : "_SD_";
            
            string bundlePath = "Assets";
            string streamingAssets ="";
            string normalAssets="";
            if (bcompress)
            {
                streamingAssets += "/FishRes_" + platform + "_Zip";
                normalAssets += "/FishRes_" + platform;
            }
            else
                streamingAssets += "/FishRes_" + platform;
            if (bHD)
            {
                streamingAssets += "/HD";
                normalAssets += "/HD";
            }
            else
            {
                streamingAssets += "/SD";
                normalAssets += "/SD";
            }
            bundlePath += "/Packages" + streamingAssets;
            string RootDir = ExporterTools.GetRootDir(PACKAGE_DIR);
            string TempDir = ExporterTools.GetRootDir(TEMP_DIR);
            string iopath = RootDir + streamingAssets;
            string iopath2 = RootDir + normalAssets;
            string bundleName;
            string bundleName2;
            string bundleName_nocompress;
            if (bcompress)
            {
                bundleName = platform +  "_" + path + ".temp";
                bundleName2 = platform +  "_" + path + RES_EXT;
                bundleName_nocompress = platform + "_" + path + RES_EXT;
            }
            else
            {
                bundleName = platform + "_" + path + ".temp";
                bundleName2 = platform + "_" + path + RES_EXT;
                bundleName_nocompress = "";
            }
            if (!Directory.Exists(bundlePath))
                Directory.CreateDirectory(bundlePath);
            if (!Directory.Exists(iopath))
                Directory.CreateDirectory(iopath);
           // if (!Directory.Exists(iopath2))
            //    Directory.CreateDirectory(iopath2);
             
            if (objList.Count == 0)
                return;
            
            try
            {
                AssetBundleBuild[] assetBundle = new AssetBundleBuild[1];
                assetBundle[0].assetBundleName = bundleName;
                assetBundle[0].assetNames = objList.ToArray();
                string filePath1 = Application.dataPath +"/Packages" + streamingAssets + "/" + bundleName;
                string filePath2 = RootDir + streamingAssets + "/" + bundleName2;
                string filePath3 = RootDir + normalAssets + "/" + bundleName_nocompress;
                File.Delete(filePath1);
                File.Delete(filePath2);
                File.Delete(bundleName2 + ".manifest");
                BuildPipeline.BuildAssetBundles(bundlePath, assetBundle, BuildAssetBundleOptions.DeterministicAssetBundle | BuildAssetBundleOptions.UncompressedAssetBundle, bo);
                if (bcompress)
                {
                    //if(File.Exists(filePath3))
                   //     File.Delete(filePath3);
                    //File.Copy(filePath1, filePath3);
                   
                    EditorUtility.DisplayProgressBar("打包中...", "正在压缩包文件:" + path, 0.5f);
                    LZMA.CompressFile(filePath1, filePath2);
                    //未压缩的CRC
                    //if(bHD)
                    //    RES_CRC_HD[(int)res] = Crc.Crc32FromFile(filePath1);
                    //else
                    //    RES_CRC_SD[(int)res] = Crc.Crc32FromFile(filePath1);
                    File.Delete(filePath1);
                    AssetDatabase.Refresh();
                    EditorUtility.ClearProgressBar();
                }
                else
                {
                    File.Move(filePath1, filePath2);
                }
                string[] files = Directory.GetFiles(bundlePath);
                foreach (string paths in files)
                    File.Delete(paths);
            }
            catch (System.Exception e)
            {
                EditorUtility.DisplayDialog("错误", "打包失败:" + e.ToString(), "确定");
            }
         }
         AssetDatabase.Refresh();
         if(bcompress)
            ClearAllManifest("/FishRes_" + platform + "_Zip/" + (bHD ? "HD" : "SD"));
         else
             ClearAllManifest("/FishRes_" + platform + "/" + (bHD ? "HD" : "SD"));
     }
    [MenuItem("Package/检测所有资源的规范性", false)]
    public static void PackAllCompressResAll1()
    {
        bool bret = CheckImgAndAtlas();
        bret &= BeginResCheck(true);
        ModifyFont();
        if(bret)
        {
            EditorUtility.DisplayDialog("成功", "检测通过!", "确定");
        }
        else
        {
            EditorUtility.DisplayDialog("错误", "发现不规范的资源!", "确定");
        }
        EndResCheck(true);
    }
    
     [MenuItem("Package/打包所有平台资源", false)]
     public static void PackAllCompressRes1()
     {
         PackAllCompress1X_HD();
         PackAllCompress2X_HD();
         PackAllCompress3X_HD();
     }
     [MenuItem("Package/打包移动平台资源", false)]
     public static void PackAllCompressRes1d()
     {
         PackAllCompress2X_HD();
         PackAllCompress3X_HD();
     }
     [MenuItem("Package/打包Windows资源", false)]
     public static void PackAllCompress1X_HD()
     {
         PackDir(ResType.FishRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.PathRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.GlobalRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.LogonRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.HallRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.SceneRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.ExtraRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.GameRes, BuildTarget.StandaloneWindows, true, true);
     }
     [MenuItem("Package/打包Android资源", false)]
     public static void PackAllCompress2X_HD()
     {
         PackDir(ResType.FishRes, BuildTarget.Android, true, true);
         PackDir(ResType.PathRes, BuildTarget.Android, true, true);
         PackDir(ResType.GlobalRes, BuildTarget.Android, true, true);
         PackDir(ResType.LogonRes, BuildTarget.Android, true, true);
         PackDir(ResType.HallRes, BuildTarget.Android, true, true);
         PackDir(ResType.SceneRes, BuildTarget.Android, true, true);
         PackDir(ResType.ExtraRes, BuildTarget.Android, true, true);
         PackDir(ResType.GameRes, BuildTarget.Android, true, true);
     }
     [MenuItem("Package/打包IOS资源", false)]
     public static void PackAllCompress3X_HD()
     {
         PackDir(ResType.FishRes, BuildTarget.iOS, true, true);
         PackDir(ResType.PathRes, BuildTarget.iOS, true, true);
         PackDir(ResType.GlobalRes, BuildTarget.iOS, true, true);
         PackDir(ResType.LogonRes, BuildTarget.iOS, true, true);
         PackDir(ResType.HallRes, BuildTarget.iOS, true, true);
         PackDir(ResType.SceneRes, BuildTarget.iOS, true, true);
         PackDir(ResType.ExtraRes, BuildTarget.iOS, true, true);
         PackDir(ResType.GameRes, BuildTarget.iOS, true, true);
     }
     [MenuItem("Package/打包资源(FishRes)", false)]
     public static void PackAllCompress1_HD()
     {
         PackDir(ResType.FishRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.FishRes, BuildTarget.Android, true, true);
         PackDir(ResType.FishRes, BuildTarget.iOS, true, true);
     }
     [MenuItem("Package/打包资源(PathRes)", false)]
     public static void PackAllCompress12_HD()
     {
         PackDir(ResType.PathRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.PathRes, BuildTarget.Android, true, true);
         PackDir(ResType.PathRes, BuildTarget.iOS, true, true);
     }
     [MenuItem("Package/打包资源(GlobalRes)", false)]
     public static void PackAllCompress13_HD()
     {
         PackDir(ResType.GlobalRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.GlobalRes, BuildTarget.Android, true, true);
         PackDir(ResType.GlobalRes, BuildTarget.iOS, true, true);
     }
     [MenuItem("Package/打包资源(LogonRes)", false)]
     public static void PackAllCompress14_HD()
     {
         PackDir(ResType.LogonRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.LogonRes, BuildTarget.Android, true, true);
         PackDir(ResType.LogonRes, BuildTarget.iOS, true, true);
     }
     [MenuItem("Package/打包资源(HallRes)", false)]
     public static void PackAllCompress15_HD()
     {
         PackDir(ResType.HallRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.HallRes, BuildTarget.Android, true, true);
         PackDir(ResType.HallRes, BuildTarget.iOS, true, true);
     }
     [MenuItem("Package/打包资源(SceneRes)", false)]
     public static void PackAllCompress16_HD()
     {
         PackDir(ResType.SceneRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.SceneRes, BuildTarget.Android, true, true);
         PackDir(ResType.SceneRes, BuildTarget.iOS, true, true);
     }
     [MenuItem("Package/打包资源(ExtraRes)", false)]
     public static void PackAllCompress17_HD()
     {
         PackDir(ResType.ExtraRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.ExtraRes, BuildTarget.Android, true, true);
         PackDir(ResType.ExtraRes, BuildTarget.iOS, true, true);
     }
     [MenuItem("Package/打包资源(GameRes)", false)]
     public static void PackAllCompress18_HD()
     {
         PackDir(ResType.GameRes, BuildTarget.StandaloneWindows, true, true);
         PackDir(ResType.GameRes, BuildTarget.Android, true, true);
         PackDir(ResType.GameRes, BuildTarget.iOS, true, true);
     }
     
     [MenuItem("Package/最终生成所有版本XML_Publish", false)]
     public static void PackAllCompressdddD()
     {
         CreatePublishDir();
         CopyClientToPublishDir();
         SaveVersionFile(PlatformType.Android, ExportType.Publish);
         SaveVersionFile(PlatformType.IOS, ExportType.Publish);
         SaveVersionFile(PlatformType.Windows, ExportType.Publish);
         Debug.Log("生成所有XML完成.");
     }
     [MenuItem("Package/最终生成所有版本XML_Test", false)]
     public static void PackAllCompressdddD222()
     {
         CreatePublishDir();
         CopyClientToPublishDir();
         SaveVersionFile(PlatformType.Android, ExportType.Test);
         SaveVersionFile(PlatformType.IOS, ExportType.Test);
         SaveVersionFile(PlatformType.Windows, ExportType.Test);
         Debug.Log("生成所有XML完成.");
     }
     [MenuItem("Package/生成XML/Windows", false)]
     public static void PackAllCompressdddD1()
     {
         CreatePublishDir();
         CopyClientToPublishDir();
         SaveVersionFile(PlatformType.Windows, ExportType.Publish);
         SaveVersionFile(PlatformType.Windows, ExportType.Test);
         Debug.Log("生成WindowsXML完成.");
     }
     [MenuItem("Package/生成XML/Android", false)]
     public static void PackAllCompressdddD221()
     {
         CreatePublishDir();
         CopyClientToPublishDir();
         SaveVersionFile(PlatformType.Android, ExportType.Publish);
         SaveVersionFile(PlatformType.Android, ExportType.Test);
         Debug.Log("生成AndroidXML完成.");
     }
     [MenuItem("Package/生成XML/iOS", false)]
     public static void PackAllCompressdddD2ff21()
     {
         CreatePublishDir();
         CopyClientToPublishDir();
         SaveVersionFile(PlatformType.IOS, ExportType.Publish);
         SaveVersionFile(PlatformType.IOS, ExportType.Test);
         Debug.Log("生成IOSXML完成.");
     }
     [MenuItem("Package/增加客户端版本号", false)]
     public static void PackAllCompressdddD2ff21ss()
     {
        string path = Application.dataPath + "/Resources/InnerRes/version.bytes";
        StreamReader sr = new StreamReader(path);
        string str = sr.ReadToEnd();
        sr.Close();
        int idx = str.IndexOf("<Version>");
        int end = str.IndexOf("</Version>");
        if(idx == -1 || end == -1)
        {
            Debug.Log("版本更新失败.");
            return ;
        }
        idx += "<Version>".Length;
        string ver = str.Substring(idx, end - idx);
        string oldVer = ver;
        Debug.Log("当前客户端版本号:" + ver);
        uint dd = Utility.VersionToUint(ver);
        if ((dd & 0xff) < 9)
            ++dd;
        else if (((dd >> 8) & 0xff) < 9)
        {
            dd &= 0xffffff00;
            dd += 1 << 8;
        }
        else
        {
            dd &= 0xffff0000;
            dd += 1 << 16;
        }

        ver = Utility.VerToStr(dd);
        str = str.Replace(oldVer, ver);
        StreamWriter sw = new StreamWriter(path, false, new UTF8Encoding(false));
        sw.Write(str);
        sw.Close();
        PlayerSettings.bundleVersion = ver.Replace('_', '.');
        Debug.Log("新的客户端版本号:" + ver);
     }
     [MenuItem("Package/增加资源版本号", false)]
     public static void PackAllCompressdddD2ff21ss2()
     {
         string path = Application.dataPath + "/Resources/InnerRes/version.bytes";
         StreamReader sr = new StreamReader(path);
         string str = sr.ReadToEnd();
         sr.Close();
         int idx = str.IndexOf("<Resource>");
         int end = str.IndexOf("</Resource>");
         if (idx == -1 || end == -1)
         {
             Debug.Log("版本更新失败.");
             return;
         }
         idx += "<Resource>".Length;
         string ver = str.Substring(idx, end - idx);
         string oldVer = ver;
         Debug.Log("当前资源版本号:" + ver);
         uint dd = Utility.VersionToUint(ver);
         if ((dd & 0xff) < 9)
             ++dd;
         else if (((dd >> 8) & 0xff) < 9)
         {
             dd &= 0xffffff00;
             dd += 1 << 8;
         }
         else
         {
             dd &= 0xffff0000;
             dd += 1 << 16;
         }

         ver = Utility.VerToStr(dd);
         str = str.Replace(oldVer, ver);
         StreamWriter sw = new StreamWriter(path, false, new UTF8Encoding(false));
         sw.Write(str);
         sw.Close();
         //PlayerSettings.bundleVersion = ver.Replace('_', '.');
         Debug.Log("新的资源版本号:" + ver);
     }
     [MenuItem("Package/HDXML 生成 SDXML", false)]
     public static void RenameHDToSD()
     {
         string verPath = Application.dataPath + "/Resources/InnerRes/version.bytes";
         string expPath = Application.dataPath + "/ExportSetting/Setting.xml";

         XmlDocument doc1 = new XmlDocument();
         XmlDocument doc2 = new XmlDocument();
         XmlReaderSettings settings = new XmlReaderSettings();
         settings.IgnoreComments = true;
         XmlReader reader1 = XmlReader.Create(verPath, settings);
         XmlReader reader2 = XmlReader.Create(expPath, settings);
         doc1.Load(reader1);
         doc2.Load(reader2);
         XmlElement xe1 = doc1.DocumentElement;
         XmlElement xe2 = doc2.DocumentElement;
         string[] resList = new string[] { "HD", "SD" };
         for (int n = 0; n < (int)PlatformType.MAX; ++n)
         {
             PlatformType pt = (PlatformType)n;
             for (int i = 0; i < (int)ExportType.MAX; ++i)
             {
                 ExportType export = (ExportType)i;

                 XmlNode curServer = xe2.SelectSingleNode("Server/" + export.ToString());
                 XmlNode clientList = xe2.SelectSingleNode("ClientList");
                 XmlNode services = xe2.SelectSingleNode("Services");
                 string clientdir_str = curServer.Attributes["clientdir"].Value;
                 string resdir_str = curServer.Attributes["resdir"].Value;
                 string ClientVer = xe1.SelectSingleNode("Params/Version").FirstChild.Value;
                 string ResVer = xe1.SelectSingleNode("Params/Resource").FirstChild.Value;
                 string OrgVer = ClientVer;
                 string DatStr = ResVer;// "_" + System.DateTime.Now.Year + "_" + System.DateTime.Now.Month + "_" + System.DateTime.Now.Day;
                 string RootDir = ExporterTools.GetRootDir("FishPublish");
                // ClientVer += DatStr;
                 if (!Directory.Exists(RootDir))
                     Directory.CreateDirectory(RootDir);
                 string ResDir = RootDir + "/" + resdir_str + "/" + ResVer + "/";
                 if (!Directory.Exists(ResDir))
                     continue;
                XmlNode serverNode = curServer;
                string dir = ResDir + curServer.Attributes["verdir"].Value+"/";
                if (!Directory.Exists(dir))
                    continue;
                for (int j = 0; j < clientList.ChildNodes.Count; ++j)
                {
                XmlNode clientNode = clientList.ChildNodes[j];
                string type = clientNode.Attributes["type"].Value;
                string extname = clientNode.Attributes["xmlext"].Value;
                if (type != pt.ToString())
                    continue;
                if (extname != "")
                    extname = "_" + extname;
                StringBuilder sb = new StringBuilder();
                string path1;
                string path2;
                path1 = dir + type + "_HD_" + "Version" + extname + ".xml";
                path2 = dir + type + "_SD_" + "Version" + extname + ".xml";
                if (File.Exists(path1) == false)
                    continue;
                    File.Delete(path2);
                    File.Copy(path1, path2);
                }
             }
         }
         Debug.Log("HD->SD 转换完成.");
     }
     static void CopyClientToPublishDir()
     {
         return;
     }
     static void CreatePublishDir()
     {
         return;
         string RootDir = ExporterTools.GetRootDir("FishPublish");
         if(!Directory.Exists(RootDir))
            Directory.CreateDirectory(RootDir);
         RootDir += "/Res/Res_" + System.DateTime.Now.Year + "_" + System.DateTime.Now.Month + "_" + System.DateTime.Now.Day;
         if (!Directory.Exists(RootDir))
             Directory.CreateDirectory(RootDir);
         CUR_RELEASE_DIR = RootDir;
     }
     // [MenuItem("Package/测试", false)]
     //public static void show()
     //{
     //     Object obj = Resources.Load("ExtraRes/ScenePrefab/SceneCamera", typeof(GameObject));
     //     if (obj != null)
     //         Debug.Log("OK");
     //     Resources.UnloadAsset(obj);
     //}

     [MenuItem("Package/清空所有打包文件", false)]
     public static void ClearPack()
     {
         string findPath = Application.dataPath + "/Packages";
         if (Directory.Exists(findPath) == false)
             return;
         string[] fileEntries = Directory.GetFiles(findPath, "*.*", SearchOption.AllDirectories);
         foreach(string s in fileEntries)
         {
             File.Delete(s);
         }
         AssetDatabase.Refresh();
     }
     public static void ClearAllManifest(string path="")
     {
         string findPath = Application.dataPath + "/Packages" + path;
         if (!Directory.Exists(findPath))
             return;
         string[] fileEntries = Directory.GetFiles(findPath, "*.*", SearchOption.AllDirectories);
         foreach (string s in fileEntries)
         {
             if (s.LastIndexOf(".dat") != -1 || s.LastIndexOf(".xml") != -1)
                 continue;
            File.Delete(s);
         }
         AssetDatabase.Refresh();
     }
     public static bool CheckImgAndAtlas()
     {
         string findPath = Application.dataPath + "/Resources";
         if (!Directory.Exists(findPath))
             return false;
         string[] fileEntries = Directory.GetFiles(findPath, "*.*", SearchOption.AllDirectories);
         List<string> fileList = new List<string>();
         for (int i = 0; i < fileEntries.Length; ++i)
         {
             string fileName = fileEntries[i].Replace('\\','/');
             string lowerName = fileName.ToLower();
             if (lowerName.LastIndexOf(".meta") != -1)
                 continue;
             if (lowerName.IndexOf(".svn") != -1)
                 continue;
             if(lowerName.IndexOf("resources/fishres") != -1)
                continue;
             if (lowerName.IndexOf(".mat") != -1)
                continue;
             //if (lowerName.IndexOf(".controller") != -1)
             //    continue;
             //if (lowerName.IndexOf(".fbx") != -1)
             //    continue;
             string filePath = fileName.Replace(Application.dataPath, "");
             string localPath = "Assets" + filePath;
             fileList.Add(localPath);
         }
         if (!ExporterTools.CheckImgAndAtlas(fileList))
         {
             Debug.Log("发现有不规范的资源.");
             return false;
         }
         return true;
     }
     public static bool BeginResCheck(bool bhd)
     {
         if (Application.dataPath.LastIndexOf(SD_DIR) != -1 &&
             File.Exists(Application.dataPath + "/sd.dat") == false)
         {
             EditorUtility.DisplayDialog("错误", "需要生成SD资源之后才能打包.", "OK");
             return false;
         }
         if (!CheckImgAndAtlas())
             return false;
         
         return true;

         string findPath = Application.dataPath + "/Resources";
         if (!Directory.Exists(findPath))
             return false;
         string[] fileEntries = Directory.GetFiles(findPath, "*.*", SearchOption.AllDirectories);
         List<string> fileList = new List<string>();
         for(int i = 0; i < fileEntries.Length; ++i)
         {
             string fileName = fileEntries[i].Replace('\\','/');
             string lowerName = fileName.ToLower();
             if (lowerName.LastIndexOf(".meta") != -1)
                 continue;
             if (lowerName.IndexOf(".svn") != -1)
                 continue;
             if (lowerName.IndexOf(".mat") != -1)
                 continue;
             if (lowerName.IndexOf(".controller") != -1)
                 continue;
             if (lowerName.IndexOf(".fbx") != -1)
                 continue;
             string filePath = fileName.Replace(Application.dataPath, "");
             string localPath = "Assets" + filePath;
             fileList.Add(localPath);
         }
         return ExporterTools.ConvertImgKeep(fileList);
     }
     public static bool EndResCheck(bool bhd)
     {
         return true;
     }
    
     [MenuItem("Package/FBX修改", false)]
     public static void RenameFBX()
     {
         string findPath = Application.dataPath + "/Resources/FishRes/Models";
         if (!Directory.Exists(findPath))
             return;
         string[] fileEntries = Directory.GetFiles(findPath, "*.fbx", SearchOption.AllDirectories);
         foreach (string s in fileEntries)
         {
             string path ="Assets" +  s.Replace(Application.dataPath, "");
             ModelImporter mi = ModelImporter.GetAtPath(path) as ModelImporter;
             if (mi == null) continue;
             mi.meshCompression = ModelImporterMeshCompression.Off;
             mi.SaveAndReimport();
             /*
             if(s.IndexOf("_Skin") != -1)
             {
                 string ss = s.Replace("_Skin", ".fbx");
                 File.Move(s, ss);
             }
             else if (s.IndexOf("_") != -1)
             {
                 string ss = s.Replace("_", "@");
                 File.Move(s, ss);
             }
                  * */
         }
         AssetDatabase.Refresh();
         Debug.Log("转换完成. ");
     }

     [MenuItem("Package/修改字体", false)]
     public static void ModifyFont()
     {
         string findPath = Application.dataPath + "/Resources";
         if (!Directory.Exists(findPath))
             return ;
         string[] fileEntries = Directory.GetFiles(findPath, "*.*", SearchOption.AllDirectories);
         List<string> fileList = new List<string>();
         for (int i = 0; i < fileEntries.Length; ++i)
         {
             string fileName = fileEntries[i].Replace('\\', '/');
             string lowerName = fileName.ToLower();
             if (lowerName.LastIndexOf(".meta") != -1)
                 continue;
             if (lowerName.LastIndexOf(".svn") != -1)
                 continue;
             if (lowerName.LastIndexOf(".prefab") == -1)
                 continue;
             string filePath = fileName.Replace(Application.dataPath, "");
             string localPath = "Assets" + filePath;
             fileList.Add(localPath);
         }
         if (!ExporterTools.ModifyFont(fileList))
         {
             Debug.Log("字体转换失败.");
         }
         else
            Debug.Log("字体转换成功.");
     }

     #region windows打包
     [MenuItem("Package/打包资源(Windows)/FishRes", false)]
     public static void PackAllCompress1_1()
     {
         PackDir(ResType.FishRes, BuildTarget.StandaloneWindows, true, true);
         Debug.Log("打包完成:Windows/FishRes");
     }
     [MenuItem("Package/打包资源(Windows)/PathRes", false)]
     public static void PackAllCompress1_2()
     {
         PackDir(ResType.PathRes, BuildTarget.StandaloneWindows, true, true);
         Debug.Log("打包完成:Windows/PathRes");
     }
     [MenuItem("Package/打包资源(Windows)/SceneRes", false)]
     public static void PackAllCompress1_3()
     {
         PackDir(ResType.SceneRes, BuildTarget.StandaloneWindows, true, true);
         Debug.Log("打包完成:Windows/SceneRes");
     }
     [MenuItem("Package/打包资源(Windows)/HallRes", false)]
     public static void PackAllCompress1_4()
     {
         PackDir(ResType.HallRes, BuildTarget.StandaloneWindows, true, true);
         Debug.Log("打包完成:Windows/HallRes");
     }
     [MenuItem("Package/打包资源(Windows)/GlobalRes", false)]
     public static void PackAllCompress1_5()
     {
         PackDir(ResType.GlobalRes, BuildTarget.StandaloneWindows, true, true);
         Debug.Log("打包完成:Windows/GlobalRes");
     }
     [MenuItem("Package/打包资源(Windows)/ExtraRes", false)]
     public static void PackAllCompress1_6()
     {
         PackDir(ResType.ExtraRes, BuildTarget.StandaloneWindows, true, true);
         Debug.Log("打包完成:Windows/ExtraRes");
     }
     [MenuItem("Package/打包资源(Windows)/LogonRes", false)]
     public static void PackAllCompress1_7()
     {
         PackDir(ResType.LogonRes, BuildTarget.StandaloneWindows, true, true);
         Debug.Log("打包完成:Windows/LogonRes");
     }
     #endregion

     #region android打包
     [MenuItem("Package/打包资源(Android)/FishRes", false)]
     public static void PackAllCompress1_11()
     {
         PackDir(ResType.FishRes, BuildTarget.Android, true, true);
         Debug.Log("打包完成:Android/FishRes");
     }
     [MenuItem("Package/打包资源(Android)/PathRes", false)]
     public static void PackAllCompress1_21()
     {
         PackDir(ResType.PathRes, BuildTarget.Android, true, true);
         Debug.Log("打包完成:Android/PathRes");
     }
     [MenuItem("Package/打包资源(Android)/SceneRes", false)]
     public static void PackAllCompress1_31()
     {
         PackDir(ResType.SceneRes, BuildTarget.Android, true, true);
         Debug.Log("打包完成:Android/SceneRes");
     }
     [MenuItem("Package/打包资源(Android)/HallRes", false)]
     public static void PackAllCompress1_41()
     {
         PackDir(ResType.HallRes, BuildTarget.Android, true, true);
         Debug.Log("打包完成:Android/HallRes");
     }
     [MenuItem("Package/打包资源(Android)/GlobalRes", false)]
     public static void PackAllCompress1_51()
     {
         PackDir(ResType.GlobalRes, BuildTarget.Android, true, true);
         Debug.Log("打包完成:Android/GlobalRes");
     }
     [MenuItem("Package/打包资源(Android)/ExtraRes", false)]
     public static void PackAllCompress1_61()
     {
         PackDir(ResType.ExtraRes, BuildTarget.Android, true, true);
         Debug.Log("打包完成:Android/ExtraRes");
     }
     [MenuItem("Package/打包资源(Android)/LogonRes", false)]
     public static void PackAllCompress1_71()
     {
         PackDir(ResType.LogonRes, BuildTarget.Android, true, true);
         Debug.Log("打包完成:Android/LogonRes");
     }
     #endregion

     #region ios打包
     [MenuItem("Package/打包资源(iOS)/FishRes", false)]
     public static void PackAllCompress1_111()
     {
         PackDir(ResType.FishRes, BuildTarget.iOS, true, true);
         Debug.Log("打包完成:iOS/FishRes");
     }
     [MenuItem("Package/打包资源(iOS)/PathRes", false)]
     public static void PackAllCompress1_211()
     {
         PackDir(ResType.PathRes, BuildTarget.iOS, true, true);
         Debug.Log("打包完成:iOS/PathRes");
     }
     [MenuItem("Package/打包资源(iOS)/SceneRes", false)]
     public static void PackAllCompress1_311()
     {
         PackDir(ResType.SceneRes, BuildTarget.iOS, true, true);
         Debug.Log("打包完成:iOS/SceneRes");
     }
     [MenuItem("Package/打包资源(iOS)/HallRes", false)]
     public static void PackAllCompress1_411()
     {
         PackDir(ResType.HallRes, BuildTarget.iOS, true, true);
         Debug.Log("打包完成:iOS/HallRes");
     }
     [MenuItem("Package/打包资源(iOS)/GlobalRes", false)]
     public static void PackAllCompress1_511()
     {
         PackDir(ResType.GlobalRes, BuildTarget.iOS, true, true);
         Debug.Log("打包完成:iOS/GlobalRes");
     }
     [MenuItem("Package/打包资源(iOS)/ExtraRes", false)]
     public static void PackAllCompress1_611()
     {
         PackDir(ResType.ExtraRes, BuildTarget.iOS, true, true);
         Debug.Log("打包完成:iOS/ExtraRes");
     }
     [MenuItem("Package/打包资源(iOS)/LogonRes", false)]
     public static void PackAllCompress1_711()
     {
         PackDir(ResType.LogonRes, BuildTarget.iOS, true, true);
         Debug.Log("打包完成:iOS/LogonRes");
     }
     #endregion
 }
