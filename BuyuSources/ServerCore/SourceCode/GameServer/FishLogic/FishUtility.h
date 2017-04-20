#pragma once
#ifndef FD_SETSIZE
#define FD_SETSIZE  4000
#endif
#include <stdio.h>
#include <tchar.h>
#include <iostream>

#include <WinSock2.h>
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <tchar.h>
#include <assert.h>
#include <list>
#include <map>
#include <vector>
#include <d3dx9.h>

using namespace std;

//宏定义
//是否运行在小服务器上
#define SIMPLE_SERVER_MODE 0
#define MAX_LAUNCHER_NUM 5
#define FTP_IP			"127.0.0.1"
#define FTP_PORT		10031
#define SOCKET_PORT		14312
#define CAMERA_FOV		20
#define CAMERA_ASPECT   1.777777f
#define CAMERA_NEAR		100
#define CAMERA_FAR		2001
#define CAMERA_WIDTH	1024
#define CAMERA_HEIGHT	576
#define MAX_SEAT_NUM	4
#define MAX_MAP_NAME	9	//地图最大的名称数量，用于房间名

#define SAFE_DELETE(X)		if(X) {delete (X); (X) = NULL;}
#define SAFE_DELETE_ARR(X)	if(X) {delete[] (X); (X) = NULL;}
#define SAFE_RELEASE(X)		if(X) {(X)->Release(); (X) = NULL;}
#define READFILE(X)			fread(&(X), 1, sizeof(X), pFile)

enum SkillType
{
	SKILL_LIGHTING,
	SKILL_FREEZE,
	SKILL_TORNADO,
	SKILL_DISASTER,
	SKILL_LOCK,
	SKILL_MAX
};
enum BulletType
{
	BULLET_TYPE_NORMAL,	//普通子弹
	BULLET_TYPE_FREEZE,	//子弹是冰冻炮
	BULLET_TYPE_PENETRATION //穿透
};

//类型定义
class  FishDesk;
class  FishPlayer;
typedef UINT_PTR		PlayerID;
typedef D3DXVECTOR2		Vector2;
typedef D3DXVECTOR3		Vector3;
typedef D3DXVECTOR4		Vector4;
typedef D3DXMATRIX		Matrix;
typedef D3DXMATRIX		Matrix4x4;
typedef D3DXQUATERNION	Quaternion;
typedef D3DXQUATERNION  Rotation;
typedef unsigned short  FISHID;

//外部使用的全局初始函数
extern bool InitFishData(const WCHAR *pcDir);
extern void ReleaseFishData();

//内部使用的函数和全局变量
extern float RADIN;
extern float InvShortMaxValue;
extern float InvUshortMaxValue;
extern void InnerFishLog(const WCHAR *pcStr, ...);

//template<typename T>
//inline void ListRemoveAt(vector<T>&list, int idx)
//{
//	if (idx != list.size() - 1)
//	{
//		list[idx] = list.back();
//	}
//	list.pop_back();
//}

inline float Dot(const Vector3 &p1, const Vector3 &p2)
{
	return D3DXVec3Dot(&p1, &p2);
}
inline Vector3 Cross(const Vector3 &p1, const Vector3 &p2)
{
	Vector3 ret;
	D3DXVec3Cross(&ret, &p1, &p2);
	return ret;
}
inline Vector3 Normalize(const Vector3 &p)
{
	Vector3 n;
	D3DXVec3Normalize(&n, &p);
	return n;
}
//inline void GetBoxMinMax(const FishBox & box, Vector3 &minpos, Vector3 &maxpos)
//{
//	Vector3 radius = box.Size * 0.5f;
//	minpos = box.Center - radius;
//	maxpos = box.Center + radius;
//}
//inline float GetBoxRadius(const FishBox &box)
//{
//	return max(max(box.Size.x, box.Size.y), box.Size.z) * 0.5f;
//}
inline Vector3 MinVec3(const Vector3 &p1, const Vector3 &p2)
{
	return Vector3(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
}
inline Vector3 MaxVec3(const Vector3 &p1, const Vector3 &p2)
{
	return Vector3(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
}
inline Vector3 MinVec3(const Vector4 &p1, const Vector4 &p2)
{
	return Vector3(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
}
inline Vector3 MaxVec3(const Vector4 &p1, const Vector4 &p2)
{
	return Vector3(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
}
inline Vector3 Vec4ToVec3(const Vector4 &v)
{
	return Vector3(v.x, v.y, v.z);
}
inline float LengthSq(const Vector3 &pos)
{
	return D3DXVec3LengthSq(&pos);
}
inline float Length(const Vector3 &pos)
{
	return D3DXVec3Length(&pos);
}
inline bool IsEqual(float a, float b, float epsilon = 0.001f)
{
	return fabsf(a - b) < epsilon;
}
inline bool IsEqual(const Vector3 &a, const Vector3& b, float epsilon = 1.0f)
{
	return IsEqual(a.x, b.x, epsilon) && IsEqual(a.y, b.y, epsilon) && IsEqual(a.z, b.z, epsilon);
}
inline bool IsEqual(const Quaternion &a, const Quaternion& b, float epsilon = 1.0f)
{
	return IsEqual(a.x, b.x, epsilon) && IsEqual(a.y, b.y, epsilon) && IsEqual(a.z, b.z, epsilon) && IsEqual(a.w, b.w, epsilon);
}
inline UINT StringToIP(const char *pcIP)
{
	int a, b, c, d;
	sscanf_s(pcIP, "%d.%d.%d.%d", &a, &b, &c, &d);
	return (a << 24) | (b << 16) | (c << 8) | d;
}