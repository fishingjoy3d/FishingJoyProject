using UnityEngine;
using System.Collections.Generic;
using System.Diagnostics;
class GlobalTimer
{
    public static Stopwatch m_Watch = InitWatch();
    static Stopwatch InitWatch()
    {
        Stopwatch st = new Stopwatch();
        st.Start();
        return st;
    }
    public static long Ticks
    {
        get
        {
            return m_Watch.ElapsedMilliseconds;
        }
    }
    
}
