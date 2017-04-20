#pragma once
#include "Random.h"
#include "FishResData.h"

struct BulletStartPosData
{
	Vector3 Center;
	Vector3 Pos;
	Vector3 Dir;
	float   Length;
};
//统计运行时信息数据
class RuntimeInfo
{
public:
	static void Init();
	static float            NearClipPlane;
	static float            BulletSpeed ;
	static float            InvShortMaxValue;
	static float            InvUShortMaxValue;
	
	static Vector3          NearLeftTopPoint;
	static Vector3          NearRightBottomPoint;
	static Vector3          FarLeftTopPoint;
	static Vector3          FarRightBottomPoint;
	static Vector3          NearLeftTopFlagPoint;
	static Vector3          NearRightBottomFlagPoint;

	static float			ScreenScalingY;
	static float			ScreenScalingX;
	static Matrix			ProjectMatrix;

	static float			NearX;
	static float			NearY;
	static BulletStartPosData	BulletCenterList[MAX_SEAT_NUM];
};
extern void GetBulletDirAndPos(short angleShort, byte seat, Vector3 &dir, Vector3 &pos);
inline float Vec2Length(const Vector2 &v)
{
	return D3DXVec2Length(&v);
}
inline float Vec2LengthSq(const Vector2 &v)
{
	return D3DXVec2LengthSq(&v);
}
inline Vector3 WorldToProjection(const Vector3 &worldPos)
{
	Vector3 pos;
	D3DXVec3TransformCoord(&pos, &worldPos, &RuntimeInfo::ProjectMatrix);
	return pos;
}
inline Vector2 ProjectionToViewport(const Vector3 &pos)
{
	return Vector2(pos.x * RuntimeInfo::NearRightBottomPoint.x, pos.y * RuntimeInfo::NearLeftTopPoint.y);
}
inline Vector2 WorldToViewport(const Vector3 &worldPos)
{
	Vector3 pos;
	D3DXVec3TransformCoord(&pos, &worldPos, &RuntimeInfo::ProjectMatrix);
	return ProjectionToViewport(pos);
}

inline Vector3 MulAdd(const Vector3 &p1, const Vector3 & p2, const Vector3 & p3)
{
	return Vector3(
		p1.x * p2.x + p3.x,
		p1.y * p2.y + p3.y,
		p1.z * p2.z + p3.z);
}

inline Quaternion MulAdd(const Quaternion &p1, const Quaternion & p2, const Quaternion & p3)
{
	return Quaternion(
		p1.x * p2.x + p3.x,
		p1.y * p2.y + p3.y,
		p1.z * p2.z + p3.z,
		p1.w * p2.w + p3.w);
}
inline Vector3 Lerp(const Vector3 &p1, const Vector3 & p2, float t)
{
	Vector3 vec3;
	D3DXVec3Lerp(&vec3, &p1, &p2, t);
	return vec3;
}
inline Quaternion Slerp(const Quaternion &p1, const Quaternion &p2, float t)
{
	Quaternion rot;
	D3DXQuaternionSlerp(&rot, &p1, &p2, t);
	return rot;
}
inline float LerpFloat(float a, float b, float t)
{
	return a + (b - a ) * t;
}

inline float Clamp(float value, float minvalue, float maxvalue)
{
	return min(max(value, minvalue), maxvalue);
}
inline bool InFrustum(const Vector3 & pos)
{
	return pos.x < RuntimeInfo::NearRightBottomPoint.x &&
		pos.x > RuntimeInfo::NearLeftTopPoint.x &&
		pos.y < RuntimeInfo::NearLeftTopPoint.y &&
		pos.y > RuntimeInfo::NearRightBottomPoint.y;
}
inline void ReductionToFloat(byte scl, byte d1, byte d2, byte d3, float& fscl, float fdu[3])
{
	fscl = scl / 255.0f;
	fdu[0] = d1 * FISH_DURATION_TIME;
	fdu[1] = d2 * FISH_DURATION_TIME;
	fdu[2] = d3 * FISH_DURATION_TIME;
}