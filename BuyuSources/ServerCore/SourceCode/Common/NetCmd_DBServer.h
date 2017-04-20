#pragma once


enum DBServerCmd
{
	DB_HEARBEAT,
	DB_REGISTER,
	DB_LOGON,
	DB_LOGON_RESULT,
	DB_LOGOFF,
	DB_SELECT,
	DB_SELECT_RESULT,
	DB_COMMIT,
};

struct DBNetCmdRegister:public NetCmd
{
	WCHAR	Account[ACCOUNT_NAME_LENGTH];
	UINT	Crc1;
	UINT	Crc2;
	UINT	Crc3;
};

struct DBNetCmdLogon :public NetCmd
{
	WCHAR	Account[ACCOUNT_NAME_LENGTH];
	UINT	Crc1;
	UINT	Crc2;
	UINT	Crc3;
};

struct DBNetCmdLogonResult :public NetCmd
{
	WCHAR	Account[ACCOUNT_NAME_LENGTH];
	bool	Result;
};
struct DBNetCmdLogoff :public NetCmd
{
	UINT	UID;
};
struct DBNetCmdSelect :public NetCmd
{
	BYTE	TableName;
	USHORT	QuerySize;
	BYTE	Query[1];
};
struct DBNetCmdCommit :public NetCmd
{
	BYTE	TableName;
	USHORT	Size;
	BYTE	TableData[1];
};
struct DBNetCmdSelect_Result :public NetCmd
{
	BYTE	TableName;
	USHORT	QuerySize;
	BYTE	Query[1];
};
