#pragma once
#include "../FishCallbak.h"
struct NetCmd;
class FishSendInterface
{
public:
	virtual void SendAll(USHORT groupIndex, USHORT pathIndex, USHORT startID) = 0;
	virtual void SendAll(USHORT groupIndex, USHORT startID) = 0;
	virtual void SendAll(NetCmd* pCmd) = 0;
};

class NetSendInterface
{
public:
	virtual void Send(PlayerID, NetCmd*) = 0;
	virtual void SendBySeat(byte seatID, NetCmd*) = 0;
	virtual bool IsUseSeat()const = 0;
};

class SimpleSendInterface:public NetSendInterface
{
public:
	void Send(PlayerID id, NetCmd* pCmd)
	{
		FishCallback::SendData(id, pCmd);
	}
	void SendBySeat(byte seatID, NetCmd*)
	{
		InnerFishLog(L"不支持位置发送");
	}
	bool IsUseSeat()const
	{
		return false;
	}
};