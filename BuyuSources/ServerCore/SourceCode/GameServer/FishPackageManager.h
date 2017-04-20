#pragma once
//Àñ°ü¹ÜÀíÆ÷
class CRoleEx;
class FishPackageManager
{
public:
	FishPackageManager();
	virtual ~FishPackageManager();
	bool IsPackageAndAutoUser(DWORD ItemID);
	void OnOpenFishPackage(CRoleEx* pRole, DWORD ItemOnlyID,DWORD PackageItemID);
	void OnAutoOpenFishPackage(CRoleEx* pRole, DWORD PackageItemID);
};