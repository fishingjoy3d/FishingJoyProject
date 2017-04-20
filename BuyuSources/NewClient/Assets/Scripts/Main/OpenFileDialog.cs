using UnityEngine;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
#if (UNITY_STANDALONE_WIN || UNITY_EDITOR)
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
public class OpenFileName
{
    public int structSize = 0;
    public IntPtr dlgOwner = IntPtr.Zero;
    public IntPtr instance = IntPtr.Zero;
    public String filter = null;
    public String customFilter = null;
    public int maxCustFilter = 0;
    public int filterIndex = 0;
    public String file = null;
    public int maxFile = 0;
    public String fileTitle = null;
    public int maxFileTitle = 0;
    public String initialDir = null;
    public String title = null;
    public int flags = 0;
    public short fileOffset = 0;
    public short fileExtension = 0;
    public String defExt = null;
    public IntPtr custData = IntPtr.Zero;
    public IntPtr hook = IntPtr.Zero;
    public String templateName = null;
    public IntPtr reservedPtr = IntPtr.Zero;
    public int reservedInt = 0;
    public int flagsEx = 0;
}

public class DllTest
{

    public static int WinPtr = 0;
    public static IntPtr WinPtr_Org = IntPtr.Zero;
    [DllImport("User32.dll", EntryPoint = "FindWindow")]
    private static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

    public static IntPtr GetProcessWnd()
    {
        //查到窗体，得到整个窗体
#if UNITY_EDITOR
        IntPtr winPtr = FindWindow("UnityContainerWndClass", null);
#else
        IntPtr winPtr = FindWindow("UnityWndClass", null);
#endif   
        WinPtr = winPtr.ToInt32();
        WinPtr_Org = winPtr;
        UnityEngine.Debug.Log(winPtr.ToInt32().ToString());
        return winPtr;
    }
    
    [DllImport("Comdlg32.dll", SetLastError = true, ThrowOnUnmappableChar = true, CharSet = CharSet.Auto)]
    public static extern bool GetOpenFileName([In, Out] OpenFileName ofn);
    public static bool GetOpenFileName1([In, Out] OpenFileName ofn)
    {
        return GetOpenFileName(ofn);
    }

    public static void GetFile()
    {
        OpenFileName ofn = new OpenFileName();

        ofn.structSize = Marshal.SizeOf(ofn);

        ofn.filter = "JPG Files\0*.jpg\0\0";

        ofn.file = new string(new char[256]);

        ofn.maxFile = ofn.file.Length;

        ofn.fileTitle = new string(new char[64]);

        ofn.maxFileTitle = ofn.fileTitle.Length;

        ofn.initialDir = UnityEngine.Application.dataPath;//

        ofn.title = "选择头像";

        ofn.defExt = "JPG";//

        //OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST| OFN_ALLOWMULTISELECT|OFN_NOCHANGEDIR
        ofn.flags = 0x00080000 | 0x00001000 | 0x00000800 | 0x00000200 | 0x00000008;

        if (DllTest.GetOpenFileName(ofn))
        {
            string path = ofn.file;
            path = path.Replace('\\', '/');
            HeaderManager.Instance.UploadHeader(path);
        }
    }
   
    //Create Process
    //=============================================================
    [StructLayout(LayoutKind.Sequential)]
        public class SECURITY_ATTRIBUTES
        {
            public int nLength;  
            public string lpSecurityDescriptor;  
            public bool bInheritHandle;
        }

    [StructLayout(LayoutKind.Sequential)]
        public struct STARTUPINFO
        {
            public int cb;  
            public string lpReserved;  
            public string lpDesktop;  
            public int lpTitle;  
            public int dwX;  
            public int dwY;  
            public int dwXSize;  
            public int dwYSize;  
            public int dwXCountChars;  
            public int dwYCountChars;  
            public int dwFillAttribute;  
            public int dwFlags;  
            public int wShowWindow;  
            public int cbReserved2;  
            public byte lpReserved2;  
            public IntPtr hStdInput;  
            public IntPtr hStdOutput;  
            public IntPtr hStdError;
        }

    [StructLayout(LayoutKind.Sequential)]
        public struct PROCESS_INFORMATION
        {
            public IntPtr hProcess;  
            public IntPtr hThread;  
            public int dwProcessId;  
            public int dwThreadId;
        }
    [DllImport("Kernel32.dll",CharSet=CharSet.Ansi)]
    public static extern bool CreateProcess(StringBuilder lpApplicationName,StringBuilder lpCommandLine,
        SECURITY_ATTRIBUTES lpProcessAttributes,
        SECURITY_ATTRIBUTES lpThreadAttributes,
        bool bInheritHandles,
        int dwCreationFlags,
        StringBuilder lpEnvironment,
        StringBuilder lpCurrentDirectory,
        ref STARTUPINFO lpStartupInfo,
        ref PROCESS_INFORMATION lpProcessInformation
        );
    [DllImport("Kernel32.dll",CharSet=CharSet.Ansi)]
    public static extern int WaitForSingleObject(IntPtr hHandle,int dwMilliseconds);

    [DllImport("Kernel32.dll",CharSet=CharSet.Ansi)]
    public static extern bool CloseHandle(IntPtr hObject);

    [DllImport("user32.dll")]
    public static extern IntPtr SetFocus(IntPtr hWnd);//设定焦点

    [DllImport("user32.dll")]
    public static extern IntPtr SetActiveWindow(IntPtr hWnd);//设定焦点
    [DllImport("user32.dll")]
    public static extern IntPtr SetForegroundWindow(IntPtr hWnd);//设定焦点
    public static bool WaitExecuteResult = false;
    static IntPtr ExecuteProcessHandle = IntPtr.Zero;
    static IntPtr ExecuteThreadHandle = IntPtr.Zero;
    public static void Execute(string path)
    {
        STARTUPINFO sInfo = new STARTUPINFO();
        PROCESS_INFORMATION pInfo = new PROCESS_INFORMATION();
        StringBuilder sb = new StringBuilder(path);
        if (!CreateProcess(null, sb, null, null, false, 0, null, null, ref sInfo, ref pInfo))
        {
            LogMgr.Log("打开支付界面失败.");
            return;
        }
        
        WaitExecuteResult = true;
        ExecuteProcessHandle = pInfo.hProcess;
        ExecuteThreadHandle = pInfo.hThread;
    }
    public static void SetExecuteFocus()
    {
        if (WaitExecuteResult == false)
            return;
        IntPtr winPtr = FindWindow("LeDuoBuYu3DPayWindow", null);
        SetForegroundWindow(winPtr);
        SetActiveWindow(winPtr);
        SetFocus(winPtr);
    }
    public static bool CheckExecuteResult()
    {
        if (WaitExecuteResult == false)
            return true;
        if (WaitForSingleObject(ExecuteProcessHandle, 10) == 258)
        {
            return false;
        }
        else
        {
            WaitExecuteResult = false;
            CloseHandle(ExecuteProcessHandle);
            CloseHandle(ExecuteThreadHandle);
            return true;
        }
    }
}

#endif