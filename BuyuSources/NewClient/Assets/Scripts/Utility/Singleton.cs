using System;
using System.Collections.Generic;


public class Singleton<T> where T : new()
{
    protected static readonly T ms_Instance = new T();//Activator.CreateInstance<T>();
    public static T Instance 
    {
        get
        {
            return ms_Instance;
        }
    }
    protected Singleton()
    {

    }
}
