using System;
using System.Collections.Generic;
public class ReductionDataByte
{
    public byte Speed;
    public byte Duration1;
    public byte Duration2;
    public byte Duration3;
}
public class ReductionData
{
    public ReductionData(float speed, float d1, float d2, float d3)
    {
        Speed = speed;
        Duration1 = d1;
        Duration2 = d2;
        Duration3 = d3;
    }
    public ReductionData()
    {

    }
    public float Speed;
    public float Duration1;
    public float Duration2;
    public float Duration3;
}