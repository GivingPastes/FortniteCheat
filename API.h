#pragma once

#define RVA(addr, size) ((uintptr_t)((UINT_PTR)(addr) + *(PINT)((UINT_PTR)(addr) + ((size) - sizeof(INT))) + (size)))
#define M_PI	3.14159265358979323846264338327950288419716939937510
#define ToRadians(degr) ( degr * (3.14159265358979323846264338327950288419716939937510 / 180))

#define FIRST_PERSON 1

#define Bone_Rooot 0
#define Bone_Head 66
#define Bone_Spine 6
#define Bone_Spine3 5
#define Bone_Neck 65
#define Bone_Pelvis 2
#define Bone_ChestAtLeft 36
#define Bone_R_Shoulder 62
#define Bone_ChestAtRight 8
#define Bone_LeftShoulder 9
#define L_FOOT_ROOT 69
#define Bone_RightShoulder 37
#define Bone_LeftElbow 10
#define Bone_RightElbow 38
#define Bone_LeftHand 11
#define Bone_RightHand 39
#define Bone_LeftLeg 67
#define Bone_RightLeg 74
#define Bone_LeftThigh 73
#define Bone_RightThigh 80
#define Bone_LeftFoot 68
#define Bone_RightFoot 75
#define Bone_R_SKIN 77
#define Bone_LeftFeet 71
#define Bone_RightFeet 78
#define Bone_R_FOOT_ROOT 76
#define Bone_LeftFeetFinger 72
#define Bone_RightFeetFinger 79
#define Bone_L_SKIN 70

#include "syscalls.h"

inline bool isSelected()
{
	HWND FortniteHWND = SpoofThis(FindWindowA, (LPCSTR)xorstr("UnrealWindow"), (LPCSTR)xorstr("Fortnite  "));
	HWND SelectedHWND = SpoofThis(GetForegroundWindow);

	if (FortniteHWND == SelectedHWND)
		return true;

	return false;
}

namespace FN {

	static GObjects* objects = nullptr;


	static void FreeFN(__int64 address)
	{
		if (!utils::IsValidPointer(address)) return;

		auto fFreeFN = reinterpret_cast<void(__fastcall*)(__int64 address)>(Addresses::Free);

		SpoofThis(fFreeFN, address);
	}

	static const char* GetObjectName(uintptr_t Object)
	{
		if (!utils::IsValidPointer(Object)) return "";

		auto fGetNameByIndex = reinterpret_cast<FString * (__fastcall*)(int* index, FString * result)>(Addresses::GetNameByIndex);

		int index = *(int*)(Object + 0x18);

		FString result;
		SpoofThis(fGetNameByIndex, &index, &result);

		if (result.c_str() == NULL)
			return "";

		auto result_str = result.ToString();

		if (result.c_str() != NULL)
			FreeFN((__int64)result.c_str());

		return result_str.c_str();
	}

	static const char* GetUObjectNameLoop(UObject* Object) {

		if (!utils::IsValidPointer((uintptr_t)Object)) return "";

		std::string name("");
		for (auto i = 0; Object; Object = Object->Outer, ++i) {

			auto fGetNameByIndex = reinterpret_cast<FString * (__fastcall*)(int* index, FString * result)>(Addresses::GetNameByIndex);
			int index = *(int*)(reinterpret_cast<uint64_t>(Object) + 0x18);

			FString internalName;
			SpoofThis(fGetNameByIndex, &index, &internalName);

			if (internalName.c_str() == NULL) {
				break;
			}

			auto objectName = internalName.ToString();

			name = objectName + std::string(i > 0 ? xorstr(".") : xorstr("")) + name;
			FreeFN((__int64)internalName.c_str());

		}
		return name.c_str();
	}

	static void eraseSubStr(std::string& mainStr, const std::string& toErase)
	{
		size_t pos = mainStr.find(toErase);
		if (pos != std::string::npos)
		{
			mainStr.erase(pos, toErase.length());
		}
	}

	static PVOID FindAllObjects(const char* a1, const char* a2 = "", bool OuterObject = false) {

		for (auto array : objects->ObjectArray->Objects) {
			auto fuObject = array;
			for (auto i = 0; i < 0x10000 && fuObject->Object; ++i, ++fuObject)
			{
				auto object = fuObject->Object;

				std::string objname = GetUObjectNameLoop(object);

				if (strstr(objname.c_str(), a1)) {
					eraseSubStr(objname, a1);
					if (strstr(objname.c_str(), a2)) {
						return object;
					}
				}
			}
		}
		return 0;
	}

	static PVOID FindObject(const char* name) {
		for (auto array : objects->ObjectArray->Objects) {
			auto fuObject = array;
			for (auto i = 0; i < 0x10000 && fuObject->Object; ++i, ++fuObject)
			{
				auto object = fuObject->Object;

				if (object->ObjectFlags != 0x41) {
					continue;
				}

				if (strstr(GetUObjectNameLoop(object), name)) {
					return object;
				}
			}
		}
		return 0;
	}

	static BOOL IsNotInScreen(FVector2D Pos) {
		if (((Pos.X <= 0 or Pos.X > HUD_Info::Width) and (Pos.Y <= 0 or Pos.Y > HUD_Info::Height)) or ((Pos.X <= 0 or Pos.X > HUD_Info::Width) or (Pos.Y <= 0 or Pos.Y > HUD_Info::Height))) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	static FVector2D ProjectWorldLocationToScreen(Vector3 world_location)
	{
		FVector2D output;
		float delta[3];
		float sp = 0, cp = 0, sy = 0, cy = 0, sr = 0, cr = 0;
		float axisx[3];
		float axisy[3];
		float axisz[3];
		float transformed[3];

		delta[0] = world_location.x - Addresses::CameraLocation.x;
		delta[1] = world_location.y - Addresses::CameraLocation.y;
		delta[2] = world_location.z - Addresses::CameraLocation.z;

		sp = crt::sinf(Addresses::CameraRotation.x * M_PI / 180);
		cp = crt::cosf(Addresses::CameraRotation.x * M_PI / 180);
		sy = crt::sinf(Addresses::CameraRotation.y * M_PI / 180);
		cy = crt::cosf(Addresses::CameraRotation.y * M_PI / 180);
		sr = crt::sinf(Addresses::CameraRotation.z * M_PI / 180);
		cr = crt::cosf(Addresses::CameraRotation.z * M_PI / 180);

		axisx[0] = cp * cy;
		axisx[1] = cp * sy;
		axisx[2] = sp;

		axisy[0] = sr * sp * cy - cr * sy;
		axisy[1] = sr * sp * sy + cr * cy;
		axisy[2] = -sr * cp;

		axisz[0] = -(cr * sp * cy + sr * sy);
		axisz[1] = cy * sr - cr * sp * sy;
		axisz[2] = cr * cp;

		transformed[0] = delta[0] * axisy[0] + delta[1] * axisy[1] + delta[2] * axisy[2];
		transformed[1] = delta[0] * axisz[0] + delta[1] * axisz[1] + delta[2] * axisz[2];
		transformed[2] = delta[0] * axisx[0] + delta[1] * axisx[1] + delta[2] * axisx[2];

		if (transformed[2] < 1)
			transformed[2] = 1;

		if (transformed[2] / 100 > 0)
		{
			float tmpx = 0, tmpy = 0;

			tmpx = (float)(HUD_Info::Width / 2);
			tmpy = (float)(HUD_Info::Height / 2);

			output.X = tmpx + transformed[0] * tmpx / crt::tanf(Addresses::FOVAngle * M_PI / 360) / transformed[2];
			output.Y = tmpy - transformed[1] * tmpx / crt::tanf(Addresses::FOVAngle * M_PI / 360) / transformed[2];

			return output;
		}

		return FVector2D(0, 0);
	}

	static BOOL ProcessEvent(uintptr_t address, void* fnobject, void* parms)
	{
		if (!utils::IsValidPointer(address)) return FALSE;
		auto index = *reinterpret_cast<void***>(address); if (!index) return FALSE;
		auto fProcessEvent = static_cast<void(*)(void* address, void* fnobject, void* parms)>(index[0x4B]); if (!fProcessEvent) return FALSE;
		SpoofThis(fProcessEvent, (void*)address, (void*)fnobject, (void*)parms);
		return TRUE;
	}
}

class APlayerState {
public:

	uintptr_t TeamIndex() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return *(uintptr_t*)&this[0xF28];
	}

	FString GetPlayerName() {
		if (!utils::IsValidPointer((uintptr_t)this)) return L"";
		FString PlayerName;
		FN::ProcessEvent((uintptr_t)this, ObjectsAddresses::GetPlayerName, &PlayerName);
		if (!PlayerName.IsValid()) return L"";
		return PlayerName;
	}
};

class ARootComponent {
public:

	Vector3 RelativeLocation() {
		if (!utils::IsValidPointer((uintptr_t)this)) return { 0, 0, 0 };
		return *(Vector3*)&this[0x11c];
	}

	Vector3 RelativeRotation() {
		if (!utils::IsValidPointer((uintptr_t)this)) return { 0, 0, 0 };
		return *(Vector3*)&this[0x128];
	}
};

class AFortItemDefinition {
public:

	FText* DisplayName() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return reinterpret_cast<FText*>(*(uintptr_t*)&this[0x88]);
	}
};

class AFortWeapon {
public:

	AFortItemDefinition* WeaponData() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (AFortItemDefinition*)(*(uintptr_t*)&this[0x378]);
	}
};

class AFortPawn {
public:

	const char* ObjectName() {
		if (!utils::IsValidPointer((uintptr_t)this)) return "";
		return FN::GetObjectName((uintptr_t)this);
	}

	uintptr_t Mesh() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return *(uintptr_t*)&this[0x280];
	}

	AFortWeapon* CurrentWeapon() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (AFortWeapon*)(*(uintptr_t*)&this[0x5F8]);
	}

	ARootComponent* RootComponent() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (ARootComponent*)(*(uintptr_t*)&this[0x130]);
	}

	APlayerState* PlayerState() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (APlayerState*)(*(uintptr_t*)&this[0x238]);
	}

	VOID SetReviveFromDBNOTime(float value) {
		if (!utils::IsValidPointer((uintptr_t)this)) return;
		*(float*)&this[0x3758] = value;
	}

	VOID SetbADSWhileNotOnGround(bool value) {
		if (!utils::IsValidPointer((uintptr_t)this)) return;
		*(bool*)&this[0x3E51] = value;
	}


	BOOL IsDBNO() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return *(bool*)&this[0x572];
	}

	BOOL IsDying() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return *(bool*)&this[0x548];
	}

	BOOL IsPlayer() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		if (strstr(ObjectName(), xorstr("PlayerPawn_Athena_"))) return TRUE;
		else return FALSE;
	}

	Vector3 GetBone(int boneidx)
	{
		if (!utils::IsValidPointer((uintptr_t)this)) return { 0, 0, 0 };
		Vector3 result;
		auto fGetBoneMatrix = ((FMatrix * (__fastcall*)(uintptr_t, FMatrix*, int))(Addresses::GetBoneMatrix));
		SpoofThis(fGetBoneMatrix, Mesh(), myMatrix, boneidx);

		result.x = myMatrix->M[3][0];
		result.y = myMatrix->M[3][1];
		result.z = myMatrix->M[3][2];

		return result;
	}

	BOOL IsInVehicle() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		bool result;
		FN::ProcessEvent((uintptr_t)this, ObjectsAddresses::IsInVehicle, &result);
		return result;
	}

	AFortPawn* GetVehicle() {

		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		uintptr_t result;

		FN::ProcessEvent((uintptr_t)this, ObjectsAddresses::GetVehicle, &result);

		return (AFortPawn*)result;
	}

	BOOL K2_TeleportTo(const Vector3& DestLocation, const Vector3& DestRotation)
	{
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		StructsParams::AActor_K2_TeleportTo_Params params;
		params.DestLocation = DestLocation;
		params.DestRotation = DestRotation;

		FN::ProcessEvent((uintptr_t)this, ObjectsAddresses::K2_TeleportTo, &params);

		return params.ReturnValue;
	}

};

class APlayerCameraManager {
public:

	float GetFOVAngle()
	{
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		auto fGetFOVAngle = reinterpret_cast<float(*)(UINT64, char*)>(*(uintptr_t*)(*(uintptr_t*)this + 0x740));
		return SpoofThis(fGetFOVAngle, (uintptr_t)this, (char*)0);
	}

	bool GetPlayerViewPoint(Vector3* vCameraPos, Vector3* vCameraRot)
	{
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;

		static uintptr_t GetPlayerViewPoint = 0;
		if (!GetPlayerViewPoint)
		{
			uintptr_t VTable = *(uintptr_t*)this;
			if (!VTable)  return false;

			GetPlayerViewPoint = *(uintptr_t*)(VTable + 0x788);//nvm its notupdated
			if (!GetPlayerViewPoint)  return false;
		}

		auto fGetPlayerViewPoint = reinterpret_cast<void(__fastcall*)(uintptr_t, Vector3*, Vector3*)>(GetPlayerViewPoint);

		SpoofThis(fGetPlayerViewPoint, (uintptr_t)this, vCameraPos, vCameraRot);

		return true;
	}
};

class APlayerController {
public:

	AFortPawn* LocalPawn() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (AFortPawn*)(*(uintptr_t*)&this[0x2A8]);
	}

	APlayerCameraManager* PlayerCameraManager() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (APlayerCameraManager*)(*(uintptr_t*)&this[0x2C0]);
	}

	BOOL IsAiming() {
		if (isSelected())
			return syscalls::NtGetAsyncKeyState(VK_RBUTTON);
	}

	BOOL IsVisible(PVOID TargetActor) {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		Vector3 ViewPoint;
		auto fLineOfSightTo = reinterpret_cast<bool(__fastcall*)(PVOID PlayerController, PVOID TargetActor, Vector3 * ViewPoint)>(Addresses::LineOfSightTo);
		return SpoofThis(fLineOfSightTo, (PVOID)this, TargetActor, &ViewPoint);
	}

	FVector2D WorldToScreen(Vector3 WorldLocation) {
		if (!utils::IsValidPointer((uintptr_t)this)) return { 0, 0 };
		return FN::ProjectWorldLocationToScreen(WorldLocation);
	}

	BOOL FOV(float NewFOV) {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		FN::ProcessEvent((uintptr_t)this, ObjectsAddresses::FOV, &NewFOV);
		return TRUE;
	}
};

class ULocalPlayer {
public:
	APlayerController* PlayerController() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (APlayerController*)(*(uintptr_t*)&this[0x30]);
	}

	uintptr_t ViewportClient() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return *(uintptr_t*)&this[0x70];
	}
};

class ULocalPlayers {
public:
	ULocalPlayer* LocalPlayer() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (ULocalPlayer*)(*(uintptr_t*)(this));
	}
};

class UGameInstance {
public:
	ULocalPlayers* LocalPlayers() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (ULocalPlayers*)(*(uintptr_t*)&this[0x38]);
	}
};

class AActors {
public:

	AFortPawn* GetCurrentActor(int i) {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (AFortPawn*)(*(uintptr_t*)&this[i * sizeof(uintptr_t)]);
	}
};

class ULevel {
public:
	AActors* Actors() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (AActors*)(*(uintptr_t*)&this[0x98]);
	}
};

class UEngineRenderer {
public:

	VOID K2_DrawLine(const FVector2D& ScreenPositionA, const FVector2D& ScreenPositionB, const FLinearColor& RenderColor, float Thickness)
	{
		if (!utils::IsValidPointer((uintptr_t)this)) return;
		auto canvas = (UCanvas*)this;
		StructsParams::UCanvas_K2_DrawLine_Params params;
		params.ScreenPositionA = ScreenPositionA;
		params.ScreenPositionB = ScreenPositionB;
		params.Thickness = Thickness;
		params.RenderColor = RenderColor;

		FN::ProcessEvent(uintptr_t(canvas), ObjectsAddresses::K2_DrawLine, &params);
	}

	void K2_DrawPolygon(uintptr_t* RenderTexture, const struct FVector2D& ScreenPosition, const struct FVector2D& Radius, int NumberOfSides, const struct FLinearColor& RenderColor)
	{
		if (!utils::IsValidPointer((uintptr_t)this)) return;
		auto canvas = (UCanvas*)this;
		StructsParams::UCanvas_K2_DrawPolygon_Params params;
		params.RenderTexture = RenderTexture;
		params.ScreenPosition = ScreenPosition;
		params.Radius = Radius;
		params.NumberOfSides = NumberOfSides;
		params.RenderColor = RenderColor;

		FN::ProcessEvent(uintptr_t(canvas), ObjectsAddresses::K2_DrawPolygon, &params);
	}

	VOID K2_DrawText(PVOID RenderFont, const FVector2D& ScreenPosition, const FString& RenderText, const FLinearColor& RenderColor, bool centered, bool bOutlined)
	{
		if (!utils::IsValidPointer((uintptr_t)this)) return;
		auto canvas = (UCanvas*)this;
		if (!utils::IsValidPointer((uintptr_t)RenderFont)) return;
		StructsParams::UCanvas_K2_DrawText_Params params;
		params.RenderFont = RenderFont;
		params.RenderText = RenderText;
		params.ScreenPosition = ScreenPosition;
		params.Scale = FVector2D{ 0.6f, 0.6f };
		params.RenderColor = RenderColor;
		params.Kerning = false;
		params.ShadowColor = FLinearColor{ 0.0f, 0.0f, 0.0f, 0.0f };
		params.ShadowOffset = FVector2D{ ScreenPosition.X + 1, ScreenPosition.Y + 1 };
		if (centered) {
			params.bCentreX = true;
			params.bCentreY = true;
		}
		else {
			params.bCentreX = false;
			params.bCentreY = false;
		}
		params.bOutlined = bOutlined;
		params.OutlineColor = { 0.0f, 0.0f, 0.0f, 1.f };
		FN::ProcessEvent(uintptr_t(canvas), ObjectsAddresses::K2_DrawText, &params);
	}

	void DrawCircle(FVector2D pos, int radius, FLinearColor Color, int numSides = 450)
	{
		if (!utils::IsValidPointer((uintptr_t)this)) return;
		float Step = M_PI * 2.0 / numSides;
		int Count = 0;
		FVector2D V[128];
		for (float a = 0; a < M_PI * 2.0; a += Step) {
			float X1 = radius * crt::cosf(a) + pos.X;
			float Y1 = radius * crt::sinf(a) + pos.Y;
			float X2 = radius * crt::cosf(a + Step) + pos.X;
			float Y2 = radius * crt::sinf(a + Step) + pos.Y;
			V[Count].X = X1;
			V[Count].Y = Y1;
			V[Count + 1].X = X2;
			V[Count + 1].Y = Y2;
			K2_DrawLine(FVector2D{ V[Count].X, V[Count].Y }, FVector2D{ X2, Y2 }, Color, 1.f);
		}
	}
};

class UWorld {
public:

	UGameInstance* OwningGameInstance() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (UGameInstance*)(*(uintptr_t*)&this[0x190]);
	}

	ULevel* PersistentLevel() {
		if (!utils::IsValidPointer((uintptr_t)this)) return 0;
		return (ULevel*)(*(uintptr_t*)&this[0x30]);
	}
};


namespace Addresses {
	static APlayerController* PlayerController;
	static AFortPawn* LocalPawn;
}

namespace HUD_Info {
	static UEngineRenderer* EngineRenderer;
}

namespace FN {
	namespace CanvasFunctions {
		static VOID K2_DrawText(PVOID RenderFont, const FString& RenderText, const FVector2D& ScreenPosition, const FVector2D& Scale, const FLinearColor& RenderColor, float Kerning, const FLinearColor& ShadowColor, const FVector2D& ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, const FLinearColor& OutlineColor)
		{
			if (!utils::IsValidPointer((uintptr_t)HUD_Info::EngineRenderer)) return;
			auto canvas = (uintptr_t)HUD_Info::EngineRenderer;
			StructsParams::UCanvas_K2_DrawText_Params params;
			params.RenderFont = RenderFont;
			params.RenderText = RenderText;
			params.ScreenPosition = ScreenPosition;
			params.Scale = Scale;
			params.RenderColor = RenderColor;
			params.Kerning = Kerning;
			params.ShadowColor = ShadowColor;
			params.ShadowOffset = ShadowOffset;
			params.bCentreX = bCentreX;
			params.bCentreY = bCentreY;
			params.bOutlined = bOutlined;
			params.OutlineColor = OutlineColor;

			FN::ProcessEvent(canvas, ObjectsAddresses::K2_DrawText, &params);
		}

		static VOID K2_DrawLine(const FVector2D& ScreenPositionA, const FVector2D& ScreenPositionB, float Thickness, const FLinearColor& RenderColor)
		{
			if (!utils::IsValidPointer((uintptr_t)HUD_Info::EngineRenderer)) return;
			auto canvas = (uintptr_t)HUD_Info::EngineRenderer;
			StructsParams::UCanvas_K2_DrawLine_Params params;
			params.ScreenPositionA = ScreenPositionA;
			params.ScreenPositionB = ScreenPositionB;
			params.Thickness = Thickness;
			params.RenderColor = RenderColor;

			FN::ProcessEvent(canvas, ObjectsAddresses::K2_DrawLine, &params);
		}
	}
}

VOID LoadObjects() {
	ObjectsAddresses::FOV = FN::FindObject(xorstr("FOV"));
	ObjectsAddresses::GetPlayerName = FN::FindObject(xorstr("GetPlayerName"));
	ObjectsAddresses::IsInVehicle = FN::FindObject(xorstr("IsInVehicle"));
	ObjectsAddresses::GetVehicle = FN::FindAllObjects(xorstr("FortPlayerPawn"), xorstr("GetVehicle"));
	ObjectsAddresses::K2_TeleportTo = FN::FindObject(xorstr("K2_TeleportTo"));
	HUD_Info::RobotoFont = FN::FindAllObjects(xorstr("Fonts/Roboto"), xorstr("Roboto_"));//font
	ObjectsAddresses::K2_DrawLine = FN::FindAllObjects(xorstr("Canvas"), xorstr("K2_DrawLine"));
	ObjectsAddresses::K2_DrawText = FN::FindAllObjects(xorstr("Canvas"), xorstr("K2_DrawText"));
	ObjectsAddresses::K2_DrawPolygon = FN::FindAllObjects(xorstr("Canvas"), xorstr("K2_DrawPolygon"));
}

VOID LoadSignatures() {
	Addresses::UWorld = utils::PatternScan(xorstr("48 89 05 ? ? ? ? 48 8B 4B 78"));
	Addresses::Free = utils::PatternScan(xorstr("48 85 C9 0F 84 82 00 00 00 48 89 5C 24 08 57 48 83 EC 20 48 8B 3D 56 67"));
	Addresses::GetNameByIndex = utils::PatternScan(xorstr("48 89 5C 24 18 48 89 6C 24 20 56 57 41 56 48 81 EC 40 08 00 00 48 8B 05 48 77 4B 0A 48 33 C4 48 89 84 24 30 08 00 00 48 8B DA 4C 8B"));
	Addresses::GetBoneMatrix = utils::PatternScan(xorstr("E8 ? ? ? ? 48 8B 47 30 F3 0F 10 45"));
	Addresses::LineOfSightTo = utils::PatternScan(xorstr("48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 20 45 8A E1"));
	Addresses::UObjectPtr = utils::PatternScan(xorstr("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1"));

	Addresses::UWorld = RVA(Addresses::UWorld, 7);
	Addresses::GetBoneMatrix = RVA(Addresses::GetBoneMatrix, 5);
	Addresses::UObjectPtr = RVA(Addresses::UObjectPtr, 7);
	FN::objects = decltype(FN::objects)(Addresses::UObjectPtr);

	return;
}

void CreateConsole()
{
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
}
