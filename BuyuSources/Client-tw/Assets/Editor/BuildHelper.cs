using UnityEngine;
using System.Collections.Generic;
using UnityEditor;
using System.IO;
using System.Runtime.InteropServices;


public class BuildHelper
{
    static string APP_NAME = "乐多捕鱼3D";
    [MenuItem("Publish/发布Android")]
    public static void BuildAndroid()
    {
        BuildVer(PlatformType.Android);
    }
    [MenuItem("Publish/发布IOS")]
    public static void BuildIOS()
    {
        BuildVer(PlatformType.IOS);
    }
    [MenuItem("Publish/发布Windows")]
    public static void BuildWin()
    {
        BuildVer(PlatformType.Windows);
    }
    
    public static bool BuildVer(PlatformType pt)
    {
        Debug.Log(Application.dataPath);
        if(Application.dataPath.IndexOf(Exporter.PUBLISH_DIR) == -1)
        {
            EditorUtility.DisplayDialog("错误", "只有发布目录才能发布版本!", "OK");
            return false;
        }
        string app_name = APP_NAME;
        string target_dir = ExporterTools.GetRootDir(Exporter.VERSION_DIR);
        string target_name = app_name + RuntimeInfo.GetExt(pt);
        BuildTargetGroup targetGroup;
        BuildTarget buildTarget;
        PlayerSettings.bundleVersion = Exporter.CLIENT_VER.Replace('_', '.');
        switch(pt)
        {
            case PlatformType.Android:
                targetGroup = BuildTargetGroup.Android;
                buildTarget = BuildTarget.Android;
                PlayerSettings.applicationIdentifier = "com.leduo.buyu";
                PlayerSettings.SetScriptingDefineSymbolsForGroup(targetGroup, "BUYU_PUBLISH");
                break;
            case PlatformType.IOS:
                targetGroup = BuildTargetGroup.iOS;
                buildTarget = BuildTarget.iOS;
                PlayerSettings.applicationIdentifier = "com.52leduo.buyu";
                PlayerSettings.SetScriptingDefineSymbolsForGroup(targetGroup, "BUYU_PUBLISH");
                break;
            case PlatformType.Windows:
                targetGroup = BuildTargetGroup.Standalone;
                buildTarget = BuildTarget.StandaloneWindows;
                PlayerSettings.applicationIdentifier = "com.52leduo.buyu";
                PlayerSettings.SetScriptingDefineSymbolsForGroup(targetGroup, "BUYU_PUBLISH");
                break;
            default:
                Debug.Log("未知的发布类型");
                return false;
        }
        string applicationPath = target_dir + "/" + target_name;
        if (File.Exists(applicationPath))
        {
            File.Delete(applicationPath);
        }
        
        bool bRet = GenericBuild(FindEnabledEditorScenes(), target_dir + "/" + target_name, buildTarget, BuildOptions.None);
        if(bRet)
        {
            EditorUtility.DisplayDialog("成功", "发布成功!", "OK");
            Debug.Log("发布成功!");
            return true;
        }
        else
        {
            EditorUtility.DisplayDialog("失败", "发布失败!", "OK");
            Debug.Log("发布失败!");
            return false;
        }
    }

    private static string[] FindEnabledEditorScenes()
    {
        List<string> EditorScenes = new List<string>();
        foreach (EditorBuildSettingsScene scene in EditorBuildSettings.scenes)
        {
            if (!scene.enabled) continue;
            EditorScenes.Add(scene.path);
            Debug.Log("打包场景:" + scene.path);
        }
        return EditorScenes.ToArray();
    }

    static bool GenericBuild(string[] scenes, string target_dir, BuildTarget build_target, BuildOptions build_options)
    {
        EditorUserBuildSettings.SwitchActiveBuildTarget(build_target);
        string res = BuildPipeline.BuildPlayer(scenes, target_dir, build_target, build_options);
        if (res.Length > 0)
        {
            EditorUtility.DisplayDialog("错误", "发布版本失败!", "OK");
            return false;
        }
        return true;
    }

}
