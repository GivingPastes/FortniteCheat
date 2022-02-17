#pragma once
class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x;
	float y;
	float z;

	__forceinline bool IsNull()
	{
		return (!x && !y && !z);
	}


	__forceinline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	__forceinline float Distance(Vector3 v)
	{
		auto A = pow((float)v.x - x, (float)2.0);
		auto B = pow((float)v.y - y, (float)2.0);
		auto C = pow((float)v.z - z, (float)2.0);
		return float(sqrtf(A + B + C));
	}

	__forceinline Vector3 ClampAngles()
	{

		if (x < -180.0f)
			x += 360.0f;

		if (x > 180.0f)
			x -= 360.0f;

		if (y > 89.f)
			y -= 360.f;

		if (y < -89.0f)
			y += 360.0f;

		return Vector3(x, y, 0);
	}

	__forceinline double Length()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	__forceinline Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	__forceinline Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	__forceinline Vector3 operator+=(Vector3 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return Vector3(x, y, z);
	}

	__forceinline Vector3 operator-=(Vector3 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return Vector3(x, y, z);
	}

	__forceinline Vector3 operator/(Vector3 v)
	{
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	__forceinline Vector3 operator/(int v)
	{
		return Vector3(x / v, y / v, z / v);
	}

	__forceinline Vector3 operator*(float flNum)
	{
		return Vector3(x * flNum, y * flNum, z * flNum);
	}
};

namespace HUD_Info {
	static float Width;
	static float Height;
	static PVOID RobotoFont;
}

namespace Addresses {
	static uintptr_t UWorld = 0;
	static uintptr_t Free = 0;
	static uintptr_t GetNameByIndex = 0;
	static uintptr_t GetBoneMatrix = 0;
	static uintptr_t LineOfSightTo = 0;
	static uintptr_t UObjectPtr = 0;

	static Vector3 CameraLocation;
	static Vector3 CameraRotation;
	static float FOVAngle;
}

namespace ObjectsAddresses {
	static PVOID FOV;
	static PVOID GetPlayerName;
	static PVOID SetPawnVisibility;
	static PVOID IsInVehicle;
	static PVOID GetVehicle;
	static PVOID K2_TeleportTo;
	static PVOID K2_DrawLine;
	static PVOID K2_DrawPolygon;
	static PVOID K2_DrawText;
}

namespace Settings
{
	static bool MemoryAim = 0;
	static int AimBone_Selector = 0;
	static int AimBone = 66;
	static int MemoryAimSmooth = 4;
	static float FovCircle_Value = 10;
	static bool BoxESP = 0;
	static bool SkeletonESP = 1;
	static bool HeadDotESP = 0;
	static bool LinesESP = 0;
	static bool PlayerNameESP = 0;
	static bool EnemyWeaponESP = 0;
	static bool InstantRevive = 0;
	static bool TP = 0;
	static bool Cham = 0;
	static bool FovChanger = 0;
	static float FovChanger_Value = 50.f;
	static bool water = 0;
	static bool CarFly = 0;
	static float MaxESPDistance = 300;
	static bool ShowFovCircle = true;
}