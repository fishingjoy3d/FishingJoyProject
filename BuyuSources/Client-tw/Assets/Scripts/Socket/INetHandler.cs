using UnityEngine;
using System.Collections.Generic;

public interface ICmdHandler
{
    bool CanProcessCmd();
    bool Handle(NetCmdPack cmd);
    void StateChanged(NetState state);
}
