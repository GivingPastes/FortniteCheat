#pragma once

struct FVector2D
{
	float X;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float Y;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)

	inline FVector2D()
		: X(0), Y(0)
	{ }

	inline FVector2D(float x, float y)
		: X(x),
		Y(y)
	{ }


	FVector2D operator+(const FVector2D& v) {
		return FVector2D{ X + v.X, Y + v.Y };
	}
};

struct FVector {
	float X;
	float Y;
	float Z;
};

struct FMatrix
{
	float M[4][4];
};
static FMatrix* myMatrix = new FMatrix();


template<class T>
struct TArray
{
	friend struct FString;

public:
	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline const T& operator[](int i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}


	T* Data;
private:
	int32_t Count;
	int32_t Max;
};

class FText {
public:
	char _padding_[0x28];
	PWCHAR Name;
	DWORD Length;

	inline PWCHAR c_wstr() {
		return Name;
	}
};

struct FString : private TArray<wchar_t>
{
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? crt::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

class UClass {
public:
	BYTE _padding_0[0x40];
	UClass* SuperClass;
};

class UObject {
public:
	PVOID VTableObject;
	DWORD ObjectFlags;
	DWORD InternalIndex;
	UClass* Class;
	BYTE _padding_0[0x8];
	UObject* Outer;

	inline BOOLEAN IsA(PVOID parentClass) {
		for (auto super = this->Class; super; super = super->SuperClass) {
			if (super == parentClass) {
				return TRUE;
			}
		}

		return FALSE;
	}
};

class FUObjectItem {
public:
	UObject* Object;
	DWORD Flags;
	DWORD ClusterIndex;
	DWORD SerialNumber;
	DWORD SerialNumber2;
};

class TUObjectArray {
public:
	FUObjectItem* Objects[9];
};

class GObjects {
public:
	TUObjectArray* ObjectArray;
	BYTE _padding_0[0xC];
	DWORD ObjectCount;
};

#define CHECK_VALID(x)
struct FRotator {
	float                                              Pitch;                                                    // 0x0000(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_SaveGame, CPF_IsPlainOldData)
	float                                              Yaw;                                                      // 0x0004(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_SaveGame, CPF_IsPlainOldData)
	float                                              Roll;                                                     // 0x0008(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_SaveGame, CPF_IsPlainOldData)

	inline FRotator()
		: Pitch(0), Yaw(0), Roll(0) {
	}

	inline FRotator(float x, float y, float z)
		: Pitch(x),
		Yaw(y),
		Roll(z) {
	}

	__forceinline FRotator operator+(const FRotator& V) {
		return FRotator(Pitch + V.Pitch, Yaw + V.Yaw, Roll + V.Roll);
	}

	__forceinline FRotator operator-(const FRotator& V) {
		return FRotator(Pitch - V.Pitch, Yaw - V.Yaw, Roll - V.Roll);
	}

	__forceinline FRotator operator*(float Scale) const {
		return FRotator(Pitch * Scale, Yaw * Scale, Roll * Scale);
	}

	__forceinline FRotator operator/(float Scale) const {
		const float RScale = 1.f / Scale;
		return FRotator(Pitch * RScale, Yaw * RScale, Roll * RScale);
	}

	__forceinline FRotator operator+(float A) const {
		return FRotator(Pitch + A, Yaw + A, Roll + A);
	}

	__forceinline FRotator operator-(float A) const {
		return FRotator(Pitch - A, Yaw - A, Roll - A);
	}

	__forceinline FRotator operator*(const FRotator& V) const {
		return FRotator(Pitch * V.Pitch, Yaw * V.Yaw, Roll * V.Roll);
	}

	__forceinline FRotator operator/(const FRotator& V) const {
		return FRotator(Pitch / V.Pitch, Yaw / V.Yaw, Roll / V.Roll);
	}

	__forceinline float operator|(const FRotator& V) const {
		return Pitch * V.Pitch + Yaw * V.Yaw + Roll * V.Roll;
	}

	__forceinline FRotator& operator+=(const FRotator& v) {
		CHECK_VALID(*this);
		CHECK_VALID(v);
		Pitch += v.Pitch;
		Yaw += v.Yaw;
		Roll += v.Roll;
		return *this;
	}

	__forceinline FRotator& operator-=(const FRotator& v) {
		CHECK_VALID(*this);
		CHECK_VALID(v);
		Pitch -= v.Pitch;
		Yaw -= v.Yaw;
		Roll -= v.Roll;
		return *this;
	}

	__forceinline FRotator& operator*=(const FRotator& v) {
		CHECK_VALID(*this);
		CHECK_VALID(v);
		Pitch *= v.Pitch;
		Yaw *= v.Yaw;
		Roll *= v.Roll;
		return *this;
	}

	__forceinline FRotator& operator/=(const FRotator& v) {
		CHECK_VALID(*this);
		CHECK_VALID(v);
		Pitch /= v.Pitch;
		Yaw /= v.Yaw;
		Roll /= v.Roll;
		return *this;
	}

	__forceinline float operator^(const FRotator& V) const {
		return Pitch * V.Yaw - Yaw * V.Pitch - Roll * V.Roll;
	}

	__forceinline bool operator==(const FRotator& src) const {
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.Pitch == Pitch) && (src.Yaw == Yaw) && (src.Roll == Roll);
	}

	__forceinline bool operator!=(const FRotator& src) const {
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.Pitch != Pitch) || (src.Yaw != Yaw) || (src.Roll != Roll);
	}

	__forceinline float Size() const {
		return sqrt(Pitch * Pitch + Yaw * Yaw + Roll * Roll);
	}


	__forceinline float SizeSquared() const {
		return Pitch * Pitch + Yaw * Yaw + Roll * Roll;
	}

	__forceinline float Dot(const FRotator& vOther) const {
		const FRotator& a = *this;

		return (a.Pitch * vOther.Pitch + a.Yaw * vOther.Yaw + a.Roll * vOther.Roll);
	}

	__forceinline float ClampAxis(float Angle) {
		// returns Angle in the range (-360,360)
		Angle = fmod(Angle, 360.f);

		if (Angle < 0.f) {
			// shift to [0,360) range
			Angle += 360.f;
		}

		return Angle;
	}

	__forceinline float NormalizeAxis(float Angle) {
		// returns Angle in the range [0,360)
		Angle = ClampAxis(Angle);

		if (Angle > 180.f) {
			// shift to (-180,180]
			Angle -= 360.f;
		}

		return Angle;
	}

	__forceinline void Normalize() {
		Pitch = NormalizeAxis(Pitch);
		Yaw = NormalizeAxis(Yaw);
		Roll = NormalizeAxis(Roll);
	}

	__forceinline FRotator GetNormalized() const {
		FRotator Rot = *this;
		Rot.Normalize();
		return Rot;
	}
};

struct FMinimalViewInfo {
	FVector Location;
	FRotator Rotation;
	float FOV;
	float OrthoWidth;
	float OrthoNearClipPlane;
	float OrthoFarClipPlane;
	float AspectRatio;
};

struct FName
{
	int32_t ComparisonIndex;
	int32_t Number;
};

struct FLinearColor
{
	FLinearColor() : R(0.f), G(0.f), B(0.f), A(0.f)
	{

	}

	FLinearColor(float _R, float _G, float _B, float _A) : R(_R), G(_G), B(_B), A(_A)
	{

	}

	float R; // 0x00(0x04)
	float G; // 0x04(0x04)
	float B; // 0x08(0x04)
	float A; // 0x0c(0x04)
};

struct FColor
{
	uint8_t R; // 0x00(0x04)
	uint8_t G; // 0x04(0x04)
	uint8_t B; // 0x08(0x04)
	uint8_t A; // 0x0c(0x04)
};


class UCanvas
{
public:
	unsigned char Padding_jYrEa[0x28];
	float OrgX; // 0x28(0x4)
	// No Padding Required Here
	float OrgY; // 0x2c(0x4)
	// No Padding Required Here
	float ClipX; // 0x30(0x4)
	// No Padding Required Here
	float ClipY; // 0x34(0x4)
	// No Padding Required Here
	struct FColor DrawColor; // 0x38(0x4) yes but define it
	// No Padding Required Here
	char bCenterX : 1; // 0x3c(0x1)
	// No Padding Required Here
	char bCenterY : 1; // 0x3c(0x1)
	// No Padding Required Here
	char bNoSmooth : 1; // 0x3c(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_GsD24 : 5; // 0x3c(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_Pu72n[0x3]; // 0x3d(0x3)
	// No Padding Required Here
	int32_t SizeX; // 0x40(0x4)
	// No Padding Required Here
	int32_t SizeY; // 0x44(0x4)
	// No Padding Required Here
	unsigned char UnknownBuffer_67hdW[0x8]; // 0x48(0x8)
	// No Padding Required Here
	unsigned char ColorModulate[0x10]; // 0x50(0x10)
	// No Padding Required Here
	PVOID DefaultTexture; // 0x60(0x8)
	// No Padding Required Here
	PVOID GradientTexture0; // 0x68(0x8)
	// No Padding Required Here
	PVOID ReporterGraph; // 0x70(0x8)
};

class AHUD
{
public:
	unsigned char Padding_d24Be[0x228];
	char bLostFocusPaused : 1; // 0x228(0x1)
	// No Padding Required Here
	char bShowHUD : 1; // 0x228(0x1)
	// No Padding Required Here
	char bShowDebugInfo : 1; // 0x228(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_HaB72 : 5; // 0x228(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_7vY26[0x3]; // 0x229(0x3)
	// No Padding Required Here
	int32_t CurrentTargetIndex; // 0x22c(0x4)
	// No Padding Required Here
	char bShowHitBoxDebugInfo : 1; // 0x230(0x1)
	// No Padding Required Here
	char bShowOverlays : 1; // 0x230(0x1)
	// No Padding Required Here
	char bEnableDebugTextShadow : 1; // 0x230(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_2aNx7 : 5; // 0x230(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_8HAfk[0x7]; // 0x231(0x7)
	// No Padding Required Here
	TArray<struct AActor*> PostRenderedActors; // 0x238(0x10)
	// No Padding Required Here
	unsigned char UnknownBuffer_h7Nua[0x8]; // 0x248(0x8)
	// No Padding Required Here
	TArray<struct FName> DebugDisplay; // 0x250(0x10)
	// No Padding Required Here
	TArray<struct FName> ToggledDebugCategories; // 0x260(0x10)
	// No Padding Required Here
	UCanvas* Canvas; // 0x270(0x8)
	// No Padding Required Here
	UCanvas* DebugCanvas; // 0x278(0x8)
};

struct FCameraCacheEntry
{
	float                                              Timestamp;                                                // 0x0000(0x0004) (ZeroConstructor, IsPlainOldData)
	unsigned char                                      UnknownData00[0xC];                                       // 0x0004(0x000C) MISSED OFFSET
	FMinimalViewInfo                            POV;                                                      // 0x0010(0x05E0)
};

class AFortPawn;

namespace StructsParams {
	struct UKismetMathLibrary_FindLookAtRotation_Params
	{
		Vector3										Start;                                                    // (ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData)
		Vector3										Target;                                                   // (ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData)
		Vector3									ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct UFortKismetLibrary_GetFortPlayerPawns_Params
	{
		UObject* WorldContextObject;                                       // (Parm, ZeroConstructor, IsPlainOldData)
		TArray<AFortPawn*>                     ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm)
	};
	
	struct AController_ClientSetRotation_Params
	{
		Vector3									NewRotation;                                              // (Parm, ZeroConstructor, IsPlainOldData)
		bool										bResetCamera;                                             // (Parm, ZeroConstructor, IsPlainOldData)
	};

	struct AController_SetIgnoreLookInput_Params
	{
		bool                                               bNewLookInput;                                            // (Parm, ZeroConstructor, IsPlainOldData)
	};

	struct AController_ResetIgnoreLookInput_Params
	{
	};

	struct APlayerController_GetMousePosition_Params
	{
		float                                              LocationX;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
		float                                              LocationY;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
		bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct APlayerController_ClientReturnToMainMenu_Params
	{
		FString                                       ReturnReason;                                             // (ConstParm, Parm, ReferenceParm)
	};

	struct AActor_K2_SetActorLocation_Params
	{
		Vector3										NewLocation;                                              // (Parm, ZeroConstructor, IsPlainOldData)
		bool										bSweep;                                                   // (Parm, ZeroConstructor, IsPlainOldData)
		int											SweepHitResult;                                           // (Parm, OutParm, IsPlainOldData)
		bool										bTeleport;                                                // (Parm, ZeroConstructor, IsPlainOldData)
		bool										ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct AActor_K2_TeleportTo_Params
	{
		Vector3										DestLocation;                                             // (Parm, ZeroConstructor, IsPlainOldData)
		Vector3										DestRotation;                                             // (Parm, ZeroConstructor, IsPlainOldData)
		bool										ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct AFortWeapon_GetProjectileSpeed_Params
	{
		float                                              ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct USceneComponent_GetComponentVelocity_Params
	{
		Vector3                                     ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct AController_ClientSetLocation_Params
	{
		Vector3                                     NewLocation;                                              // (Parm, ZeroConstructor, IsPlainOldData)
		Vector3                                    NewRotation;                                              // (Parm, ZeroConstructor, IsPlainOldData)
	};

	//Engine Renderer

	struct UCanvas_K2_DrawPolygon_Params
	{
		uintptr_t*                                   RenderTexture;                                             // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector2D                                   ScreenPosition;                                            // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector2D                                   Radius;                                                    // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int                                                NumberOfSides;                                             // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FLinearColor                                RenderColor;                                               // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct UCanvas_K2_DrawLine_Params
	{
		FVector2D                                   ScreenPositionA;                                          // (Parm, ZeroConstructor, IsPlainOldData)
		FVector2D                                   ScreenPositionB;                                          // (Parm, ZeroConstructor, IsPlainOldData)
		float                                       Thickness;                                                // (Parm, ZeroConstructor, IsPlainOldData)
		FLinearColor                                RenderColor;                                              // (Parm, ZeroConstructor, IsPlainOldData)
	};

	struct UCanvas_K2_DrawText_Params
	{
		PVOID										RenderFont;                                               // (Parm, ZeroConstructor, IsPlainOldData)
		FString                                     RenderText;                                               // (Parm, ZeroConstructor)
		FVector2D                                   ScreenPosition;                                           // (Parm, ZeroConstructor, IsPlainOldData)
		FVector2D                                   Scale;                                                    // (Parm, ZeroConstructor, IsPlainOldData)
		FLinearColor                                RenderColor;                                              // (Parm, ZeroConstructor, IsPlainOldData)
		float                                       Kerning;                                                  // (Parm, ZeroConstructor, IsPlainOldData)
		FLinearColor                                ShadowColor;                                              // (Parm, ZeroConstructor, IsPlainOldData)
		FVector2D                                   ShadowOffset;                                             // (Parm, ZeroConstructor, IsPlainOldData)
		bool                                        bCentreX;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
		bool                                        bCentreY;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
		bool                                        bOutlined;                                                // (Parm, ZeroConstructor, IsPlainOldData)
		FLinearColor                                OutlineColor;                                             // (Parm, ZeroConstructor, IsPlainOldData)
	};
}