using UnityEngine;
using System.Collections.Generic;

public class GUIHelper
{
    public static void ShowLabel(string str)
    {
        ShowLabel(str, new Rect(0, 0, Screen.width, Screen.height));
    }
    public static void ShowLabel(string str, Color color)
    {
        int y = (int)(Camera.main.rect.y * Screen.height) + 10;
        ShowLabel(str, new Rect(10, y, Screen.width, Screen.height), color);
    }
    public static void ShowLabel(string str, Color color, int fontSize, TextAnchor anchor = TextAnchor.UpperLeft)
    {
        ShowLabel(str, new Rect(0, 0, Screen.width, Screen.height), color, fontSize, anchor);
    }
    public static void ShowLabel(string str, Rect rect, int fontSize = 24, TextAnchor anchor = TextAnchor.UpperLeft)
    {
        ShowLabel(str, rect, Color.white, fontSize, anchor);
    }
    public static void ShowLabel(string str, Rect rect, Color color, int fontSize = 25, TextAnchor anchor = TextAnchor.UpperLeft)
    {
        GUI.skin.label.fontSize = fontSize;
        GUI.skin.label.alignment = anchor;
        GUI.color = color;
        GUI.Label(rect, str);
    }
    public static void ShowImg(Texture2D img, Rect rect, Color color, bool alpha)
    {
        GUI.color = color;
        GUI.DrawTexture(rect, img, ScaleMode.StretchToFill, alpha);
    }
}
