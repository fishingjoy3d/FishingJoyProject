#pragma once
struct BuyEntityOnce
{
	DWORD			OnlyID;
	DWORD			UserID;
	//地址 手机 名称 
	TCHAR			Name[MAX_NAME_LENTH + 1];//姓名
	unsigned __int64 Phone;//手机号码
	TCHAR			Addres[MAX_ADDRESS_LENGTH + 1];//现实地址
	//物品数据
	BYTE			ShopID;
	BYTE			ShopOnlyID;
	DWORD			ItemID;
	DWORD			ItemSum;

	DWORD			GameServerClientID;
};
class BuyEntityManager
{
public:
	BuyEntityManager();
	virtual ~BuyEntityManager();

	bool OnBuyEntityItem(ServerClientData* pClient, GO_Cmd_BuyEntityItem*pMsg);
	void OnBuyEntityItemResult(BYTE ErrorID,bool Result, DWORD dwUserID,DWORD OnlyID);
private:
	HashMap<DWORD,BuyEntityOnce>	m_BuyEntityMap;
	DWORD							m_OnlyID;
};