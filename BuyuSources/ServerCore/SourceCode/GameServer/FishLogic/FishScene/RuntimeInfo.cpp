#include "stdafx.h"

#include "RuntimeInfo.h"

const float LAUNCHER_X = 15.5f;
const float LAUNCHER_X_RIGHT = 10.5f;

const float LAUNCHER_Y = 11.5f;
const float LAUNCHER_RANGE = 3.14159f * 0.5f;


float	RuntimeInfo::NearClipPlane			= CAMERA_NEAR;
float   RuntimeInfo::BulletSpeed			= 40;
float   RuntimeInfo::InvShortMaxValue		= 1.0f / SHRT_MAX;
float   RuntimeInfo::InvUShortMaxValue		= 1.0f / USHRT_MAX;
float	RuntimeInfo::NearX = 0;;
float	RuntimeInfo::NearY = 0;
Vector3  RuntimeInfo::NearLeftTopPoint;
Vector3  RuntimeInfo::NearRightBottomPoint;
Vector3  RuntimeInfo::FarLeftTopPoint;
Vector3  RuntimeInfo::FarRightBottomPoint;
Vector3  RuntimeInfo::NearLeftTopFlagPoint;
Vector3  RuntimeInfo::NearRightBottomFlagPoint;
Matrix   RuntimeInfo::ProjectMatrix;
BulletStartPosData	RuntimeInfo::BulletCenterList[MAX_SEAT_NUM];
float RuntimeInfo::ScreenScalingY = 0;
float RuntimeInfo::ScreenScalingX = 0;
void RuntimeInfo::Init()
{
	Vector3 pt0 = Vector3(-1, 1, 0.0f);
	Vector3 pt1 = Vector3(1, -1, 0.0f);
	Vector3 pt2 = Vector3(-1, 1, 1.0f);
	Vector3 pt3 = Vector3(1, -1, 1.0f);

	Matrix mat;
	
	D3DXMatrixPerspectiveFovLH(&mat, D3DXToRadian(CAMERA_FOV), CAMERA_ASPECT, CAMERA_NEAR, CAMERA_FAR);
	ProjectMatrix = mat;

	D3DXMatrixInverse(&mat, 0, &mat);

	D3DXVec3TransformCoord(&NearLeftTopPoint, &pt0, &mat);
	D3DXVec3TransformCoord(&NearRightBottomPoint, &pt1, &mat);
	D3DXVec3TransformCoord(&FarLeftTopPoint, &pt2, &mat);
	D3DXVec3TransformCoord(&FarRightBottomPoint, &pt3, &mat);

	NearLeftTopFlagPoint.x = NearLeftTopPoint.x + 2.0f;
	NearLeftTopFlagPoint.y = NearLeftTopPoint.y - 0.6f;
	NearLeftTopFlagPoint.z = NearLeftTopPoint.z;

	NearRightBottomFlagPoint.x = NearRightBottomPoint.x - 2.0f;
	NearRightBottomFlagPoint.y = NearRightBottomPoint.y + 0.6f;
	NearRightBottomFlagPoint.z = NearRightBottomPoint.z;

	NearX = NearRightBottomPoint.x;
	NearY = NearLeftTopPoint.y;
	
	float height = NearLeftTopPoint.y;
	//×Óµ¯µÄÎ»ÖÃ
	//		3	2
	//		0,	1
	BulletStartPosData pd;
	pd.Center	= Vector3(-LAUNCHER_X, -height, 0);
	pd.Pos		= Vector3(-LAUNCHER_X, -LAUNCHER_Y, 0);
	pd.Dir		= pd.Pos - pd.Center;
	pd.Length	= D3DXVec3Length(&pd.Dir);
	pd.Dir		/= pd.Length;
	BulletCenterList[0] = pd;

	pd.Center	= Vector3(LAUNCHER_X_RIGHT, -height, 0);
	pd.Pos		= Vector3(LAUNCHER_X_RIGHT, -LAUNCHER_Y, 0);
	pd.Dir		= pd.Pos - pd.Center;
	pd.Length	= D3DXVec3Length(&pd.Dir);
	pd.Dir		/= pd.Length;
	BulletCenterList[1] = pd;

	pd.Center	= Vector3(LAUNCHER_X_RIGHT, height, 0);
	pd.Pos		= Vector3(LAUNCHER_X_RIGHT, LAUNCHER_Y, 0);
	pd.Dir		= pd.Pos - pd.Center;
	pd.Length	= D3DXVec3Length(&pd.Dir);
	pd.Dir		/= pd.Length;
	BulletCenterList[2] = pd;

	pd.Center	= Vector3(-LAUNCHER_X, height, 0);
	pd.Pos		= Vector3(-LAUNCHER_X, LAUNCHER_Y, 0);
	pd.Dir		= pd.Pos - pd.Center;
	pd.Length	= D3DXVec3Length(&pd.Dir);
	pd.Dir		/= pd.Length;
	BulletCenterList[3] = pd;

}
void GetBulletDirAndPos(short angleShort, byte seat, Vector3 &dir, Vector3 &pos)
{
	float angle = angleShort * InvShortMaxValue * LAUNCHER_RANGE;
	BulletStartPosData & pd = RuntimeInfo::BulletCenterList[seat];

	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, angle);

	D3DXVec3TransformNormal(&dir, &pd.Dir, &mat);
	pos = dir * pd.Length + pd.Center;
	pos.z = RuntimeInfo::NearLeftTopPoint.z;
}