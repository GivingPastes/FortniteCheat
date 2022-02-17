#include "includes.h"

bool menu_open = true;
bool InstantRevive_AlreadyDisabled = true;
bool DBNO_AlreadyDisabled = true;

__forceinline void draw_line(int x1, int y1, int x2, int y2, int thickness, FLinearColor clr) {
	if (HUD_Info::EngineRenderer)
		HUD_Info::EngineRenderer->K2_DrawLine(FVector2D(x1, y1), FVector2D(x2, y2), clr, 1);
}

__forceinline void draw_corner_box(int x, int y, int w, int h, int thickness, FLinearColor clr) {
	if (HUD_Info::EngineRenderer) {
		draw_line(x, y, x, y + (h / 5), thickness, clr);
		draw_line(x, y, x + (w / 5), y, thickness, clr);
		draw_line(x + w, y, x + w - (w / 5), y, thickness, clr);
		draw_line(x + w, y, x + w, y + (h / 5), thickness, clr);
		draw_line(x, y + h, x + (w / 5), y + h, thickness, clr);
		draw_line(x, y + h, x, y + h - (h / 5), thickness, clr);
		draw_line(x + w, y + h, x + w - (w / 5), y + h, thickness, clr);
		draw_line(x + w, y + h, x + w, y + h - (h / 5), thickness, clr);
	}
}

__forceinline void draw_circle(int x, int y, int radius, FLinearColor clr) {
	if (HUD_Info::EngineRenderer)
		HUD_Info::EngineRenderer->K2_DrawPolygon(0, FVector2D(x, y), FVector2D(radius, radius), 32, clr);
}

__forceinline BOOL Initialize_ESP(UEngineRenderer* EngineRenderer, AFortPawn* CurrentPawn, FVector2D Head, FVector2D Pelvis, bool IsVisible) {

	if (Addresses::LocalPawn) {
		float WorldDistance;
		if (Addresses::LocalPawn->IsInVehicle()) {
			WorldDistance = Addresses::LocalPawn->GetVehicle()->RootComponent()->RelativeLocation().Distance(CurrentPawn->GetBone(Bone_Head));
		}
		else {
			WorldDistance = Addresses::LocalPawn->RootComponent()->RelativeLocation().Distance(CurrentPawn->GetBone(Bone_Head));
		}

		if (WorldDistance / 100.f > Settings::MaxESPDistance) return false;
	}

	FLinearColor Color = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), TextColor;

	if (IsVisible)
		TextColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	else
		TextColor = FLinearColor(1.f, 0, 0, 1.f);

	FVector2D local_root_2d = Addresses::PlayerController->WorldToScreen(Addresses::LocalPawn->GetBone(Bone_Rooot));
	FVector2D root_2d = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_Rooot));
	FVector2D head_2d = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_Head));
	FVector2D Neck = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_Neck));
	FVector2D ChestAtLeft = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_ChestAtLeft));
	FVector2D Spine = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_Spine));
	FVector2D Spine3 = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_Spine3));
	FVector2D Spine4 = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_Pelvis));
	FVector2D ChestAtRight = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_ChestAtRight));
	FVector2D Chest = { ChestAtLeft.X + (ChestAtRight.X - ChestAtLeft.X) / 2, ChestAtLeft.Y };
	FVector2D LeftShoulder = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_LeftShoulder));
	FVector2D RightShoulder = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_RightShoulder));
	FVector2D RightArm1 = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_R_Shoulder));
	FVector2D LeftElbow = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_LeftElbow));
	FVector2D RightElbow = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_RightElbow));
	FVector2D LeftHand = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_LeftHand));
	FVector2D RightHand = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_RightHand));
	FVector2D LeftLeg = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_LeftLeg));
	FVector2D RightLeg = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_RightLeg));
	FVector2D LeftLeg3 = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_L_SKIN));
	FVector2D LeftThigh = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_LeftThigh));
	FVector2D RightThigh = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_RightThigh));
	FVector2D LeftFoot = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_LeftFoot));
	FVector2D RightFoot = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_RightFoot));
	FVector2D LeftFeet = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_LeftFeet));
	FVector2D RightFeet = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_RightFeet));
	FVector2D LeftFeetFinger = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_LeftFeetFinger));
	FVector2D LeftLeg4 = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(L_FOOT_ROOT));
	FVector2D RightFeetFinger = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_RightFeetFinger));
	FVector2D RightLeg3 = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_R_SKIN));
	FVector2D RightLeg4 = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_R_FOOT_ROOT));
	FVector2D Bottom = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(0));

	float x = head_2d.X;
	float y = head_2d.Y;
	float h = root_2d.Y - y - 2;
	float w = h / 1.5;

	if (Settings::BoxESP)
		draw_corner_box(x - (w / 2), y, w, h, 2, { 138, 43, 226, 255 });

	if (Settings::HeadDotESP)
		draw_circle(head_2d.X, head_2d.Y, 5, { 138, 43, 226, 150 });

	if (Settings::SkeletonESP)
	{
		EngineRenderer->K2_DrawLine(FVector2D(Head.X, Head.Y), FVector2D(Neck.X, Neck.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(Neck.X, Neck.Y), FVector2D(Spine.X, Spine.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(Spine.X, Spine.Y), FVector2D(Chest.X, Chest.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(Chest.X, Chest.Y), FVector2D(Spine3.X, Spine3.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(Spine3.X, Spine3.Y), FVector2D(Spine4.X, Spine4.Y), Color, 1);

		EngineRenderer->K2_DrawLine(FVector2D(Neck.X, Neck.Y), FVector2D(LeftShoulder.X, LeftShoulder.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(LeftShoulder.X, LeftShoulder.Y), FVector2D(LeftElbow.X, LeftElbow.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(LeftElbow.X, LeftElbow.Y), FVector2D(LeftHand.X, LeftHand.Y), Color, 1);

		EngineRenderer->K2_DrawLine(FVector2D(Neck.X, Neck.Y), FVector2D(RightArm1.X, RightArm1.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(RightArm1.X, RightArm1.Y), FVector2D(RightElbow.X, RightElbow.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(RightElbow.X, RightElbow.Y), FVector2D(RightHand.X, RightHand.Y), Color, 1);

		EngineRenderer->K2_DrawLine(FVector2D(Spine4.X, Spine4.Y), FVector2D(LeftThigh.X, LeftThigh.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(LeftThigh.X, LeftThigh.Y), FVector2D(LeftFoot.X, LeftFoot.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(LeftFoot.X, LeftFoot.Y), FVector2D(LeftLeg3.X, LeftLeg3.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(LeftLeg3.X, LeftLeg3.Y), FVector2D(LeftLeg4.X, LeftLeg4.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(LeftLeg4.X, LeftLeg4.Y), FVector2D(LeftFeetFinger.X, LeftFeetFinger.Y), Color, 1);

		EngineRenderer->K2_DrawLine(FVector2D(Spine4.X, Spine4.Y), FVector2D(RightThigh.X, RightThigh.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(RightThigh.X, RightThigh.Y), FVector2D(RightFoot.X, RightFoot.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(RightFoot.X, RightFoot.Y), FVector2D(RightLeg3.X, RightLeg3.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(RightLeg3.X, RightLeg3.Y), FVector2D(RightLeg4.X, RightLeg4.Y), Color, 1);
		EngineRenderer->K2_DrawLine(FVector2D(RightLeg4.X, RightLeg4.Y), FVector2D(RightFeetFinger.X, RightFeetFinger.Y), Color, 1);
	}

	if (Settings::LinesESP) {
		if (Addresses::LocalPawn)
			draw_line(local_root_2d.X, local_root_2d.Y, root_2d.X, root_2d.Y, 2, { 138, 43, 226, 255 });
	}

	if (Settings::EnemyWeaponESP) {
		FText* EnemyWeaponName = CurrentPawn->CurrentWeapon()->WeaponData()->DisplayName();
		if (EnemyWeaponName) {
			EngineRenderer->K2_DrawText(HUD_Info::RobotoFont, FVector2D(Bottom.X, Bottom.Y), EnemyWeaponName->c_wstr(), TextColor, true, true);
		}
	}

	if (Settings::PlayerNameESP) {
		FString EnemyName = CurrentPawn->PlayerState()->GetPlayerName();
		EngineRenderer->K2_DrawText(HUD_Info::RobotoFont, FVector2D(Head.X, Head.Y), EnemyName, TextColor, true, true);
	}


	struct FEngine_FHitResult
	{
		char blockinghit[0x1000];
	};

	return TRUE;
}

__forceinline BOOL Initialize_MODS(AFortPawn* ClosestPawn) {

	if (Settings::FovChanger) {
		Addresses::PlayerController->FOV(Settings::FovChanger_Value);
	}

	//if (Settings::Cham)
	//{
	//	//if (localPlayerTeamIndex != actorTeamIndex)
	//	//{
	//	auto FpsColorcacaa = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });

	//	*reinterpret_cast<PVOID*>(reinterpret_cast<PBYTE>(state) + offsets::FortPlayerStateAthena::TeamIndex) = Index;
	//	*reinterpret_cast<PVOID*>(reinterpret_cast<PBYTE>(state) + 0x10ac) = SquadID;
	//	DrawOutlinedText(m_pFont, xorstr("ENABLEDDINOOOOOOOOWIOJGWEIHGNJIWUOQGBIQEUWGBQUIEBGIQWG"), ImVec2(100, 400), 16.0f, FpsColorcacaa, false);
	//	//}
	//}

	if (utils::IsValidPointer((uintptr_t)Addresses::LocalPawn)) {

		if (Settings::InstantRevive) {
			Addresses::LocalPawn->SetReviveFromDBNOTime(0.3f);
			InstantRevive_AlreadyDisabled = false;
		}
		else {
			if (!InstantRevive_AlreadyDisabled) {
				Addresses::LocalPawn->SetReviveFromDBNOTime(10.f);
				InstantRevive_AlreadyDisabled = true;
			}

		}

		if (Settings::CarFly) {
			AFortPawn* CurrentVehicle = Addresses::LocalPawn->GetVehicle();

			if (!CurrentVehicle) return false;
			if (syscalls::NtGetAsyncKeyState(0x57)) { // W
				Vector3 CarRelativeLocation = CurrentVehicle->RootComponent()->RelativeLocation();

				CurrentVehicle->K2_TeleportTo(CarRelativeLocation, Addresses::CameraRotation);
			}
		}
	}


	return TRUE;
}

__forceinline BOOL TestLoop() {

	if (Settings::AimBone_Selector == 0) Settings::AimBone = Bone_Head;
	if (Settings::AimBone_Selector == 1) Settings::AimBone = Bone_Neck;
	if (Settings::AimBone_Selector == 2) Settings::AimBone = Bone_ChestAtLeft;
	if (Settings::AimBone_Selector == 3) Settings::AimBone = Bone_Pelvis;
	if (Settings::AimBone_Selector == 4) Settings::AimBone = Bone_RightLeg;
	if (Settings::AimBone_Selector == 5) Settings::AimBone = Bone_LeftFeet;

	auto EngineRenderer = HUD_Info::EngineRenderer;

	if (menu_open)
	{
		auto Mouse = GaGui::CursorPos();
		EngineRenderer->K2_DrawLine(FVector2D(Mouse.X - 11, Mouse.Y), FVector2D(Mouse.X + 11, Mouse.Y), FLinearColor(1.f, 1.f, 1.f, 1.f), 1);
		EngineRenderer->K2_DrawLine(FVector2D(Mouse.X, Mouse.Y - 11), FVector2D(Mouse.X, Mouse.Y + 11), FLinearColor(1.f, 1.f, 1.f, 1.f), 1);

		EngineRenderer->K2_DrawLine(FVector2D(Mouse.X - 10, Mouse.Y), FVector2D(Mouse.X + 10, Mouse.Y), FLinearColor(1.f, 1.f, 1.f, 1.f), 1);
		EngineRenderer->K2_DrawLine(FVector2D(Mouse.X, Mouse.Y - 10), FVector2D(Mouse.X, Mouse.Y + 10), FLinearColor(1.f, 1.f, 1.f, 1.f), 1);
	}

	AFortPawn* ClosestPawn = 0;
	float ClosestDistance = FLT_MAX;

	auto GWorld = *(uintptr_t*)(Addresses::UWorld); if (!GWorld) return false;
	auto World = (UWorld*)GWorld;

	Addresses::PlayerController = World->OwningGameInstance()->LocalPlayers()->LocalPlayer()->PlayerController();
	auto PlayerCameraManager = Addresses::PlayerController->PlayerCameraManager();

	PlayerCameraManager->GetPlayerViewPoint(&Addresses::CameraLocation, &Addresses::CameraRotation);

	Addresses::FOVAngle = PlayerCameraManager->GetFOVAngle();

	Addresses::LocalPawn = Addresses::PlayerController->LocalPawn();

	auto PersistentLevel = World->PersistentLevel();

	auto AActor = *(TArray<AFortPawn*>*)((uintptr_t)&PersistentLevel[0x98]);


	for (int i = 0; i < AActor.Num(); i++) //Actor loop
	{
		auto CurrentPawn = AActor[i];
		if (!CurrentPawn)
			continue;

	//	if (CurrentPawn == Addresses::LocalPawn) continue;    //Removed For Self ESP
		auto CurrentPawnName = CurrentPawn->ObjectName();

		if (CurrentPawn->IsPlayer())
		{
			bool IsVisible = true;

			FVector2D Head = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_Head));
			FVector2D Pelvis = Addresses::PlayerController->WorldToScreen(CurrentPawn->GetBone(Bone_Pelvis));

			if (utils::IsValidPointer(uintptr_t(Addresses::LocalPawn))) {
				IsVisible = Addresses::PlayerController->IsVisible(CurrentPawn);
			}
			else IsVisible = true;

			Initialize_ESP(EngineRenderer, CurrentPawn, Head, Pelvis, IsVisible);

			if (utils::IsValidPointer((uintptr_t)Addresses::LocalPawn) && Settings::MemoryAim)
			{
				if
					(
						!CurrentPawn->IsDBNO() ||
						!CurrentPawn->IsDying() ||
						!Addresses::LocalPawn->IsDBNO() ||
						!Addresses::LocalPawn->IsDying()
					)
				{
					auto dx = Head.X - (HUD_Info::Width / 2);
					auto dy = Head.Y - (HUD_Info::Height / 2);
					auto dist = crt::sqrtf(dx * dx + dy * dy);

					if (IsVisible) {
						{
							if (dist < (Settings::FovCircle_Value * HUD_Info::Width / Addresses::FOVAngle) / 2 && dist < ClosestDistance) {
								ClosestDistance = dist;
								ClosestPawn = CurrentPawn;
							}
						}
					}
				}
			}
		}
	}

	if (utils::IsValidPointer(uintptr_t(Addresses::LocalPawn)) && isSelected() && Settings::MemoryAim)
	{
		if (ClosestPawn) {
			if (Addresses::LocalPawn and ClosestPawn) {

				uintptr_t LocalTeamIndex = Addresses::LocalPawn->PlayerState()->TeamIndex();

				uintptr_t CurrentPawnTeamIndex = ClosestPawn->PlayerState()->TeamIndex();

				if (syscalls::NtGetAsyncKeyState(VK_RBUTTON) && (LocalTeamIndex != CurrentPawnTeamIndex)) {
					FVector2D TargetBone = Addresses::PlayerController->WorldToScreen(ClosestPawn->GetBone(Settings::AimBone));
					syscalls::mousemove(TargetBone.X, TargetBone.Y, HUD_Info::Width, HUD_Info::Height, Settings::MemoryAimSmooth, false);
				}
			}
		}
	}

	Initialize_MODS(ClosestPawn);

	return TRUE;
}


__forceinline void Menu()
{
	GaGui::Input::Handle();

	if (syscalls::NtGetAsyncKeyState((DWORD)VK_INSERT) & 1) {
		menu_open = !menu_open;
	}

	static FVector2D pos{ 300, 300 };

	if (GaGui::Window(xorstr("Inverse - Micca#1337"), &pos, FVector2D{ 450.0f, 400 }, menu_open))
	{

		static bool text_check = false;
		static int tab = 0;
		GaGui::Text(xorstr(" "));
		GaGui::NextColumn(30.0f);
		if (GaGui::ButtonTab(xorstr("Aim"), FVector2D{ 110, 25 }, tab == 1)) tab = 1;
		GaGui::NextColumn(160.0f);
		if (GaGui::ButtonTab(xorstr("Visuals"), FVector2D{ 110, 25 }, tab == 2)) tab = 2;
		GaGui::NextColumn(290.0f);
		if (GaGui::ButtonTab(xorstr("Misc"), FVector2D{ 110, 25 }, tab == 3)) tab = 3;

		if (tab == 0)
		{
			GaGui::NextColumn(30.0f);
			GaGui::Text(xorstr(" "));

			std::string date = __DATE__;
			std::string miccatext = xorstr("                                               Date:  ") + date;
			GaGui::Text(miccatext.c_str(), true);
			GaGui::Text(xorstr("                                                PostRender Hook Test "), true);
			GaGui::Text(xorstr("                                                ESP Might Be Broken "), true);


		}

		if (tab == 1)
		{
			GaGui::NextColumn(30.0f);
			GaGui::Text(xorstr(" "));
			GaGui::Checkbox(xorstr("Memory Aimbot"), &Settings::MemoryAim);
			GaGui::SliderInt(xorstr("Aimbot Smooth"), &Settings::MemoryAimSmooth, 3.0f, 10.f);
			GaGui::SliderFloat(xorstr("Fov Circle"), &Settings::FovCircle_Value, 1, 360, xorstr("%.1f"));
			GaGui::Combobox(xorstr(""), FVector2D(261, 21), &Settings::AimBone_Selector, "Head", "Neck", "Chest", "Pelvis", "Leg", "Feet", NULL);

		}
		if (tab == 2)
		{
			GaGui::NextColumn(30.0f);
			GaGui::Text(xorstr(" "));
			GaGui::Checkbox(xorstr("Box"), &Settings::BoxESP);
			GaGui::Checkbox(xorstr("Skeleton"), &Settings::SkeletonESP);
			GaGui::Checkbox(xorstr("Head Dot"), &Settings::HeadDotESP);
			GaGui::Checkbox(xorstr("Snaplines"), &Settings::LinesESP);
			GaGui::Checkbox(xorstr("Name ESP"), &Settings::PlayerNameESP);
			GaGui::Checkbox(xorstr("Weapon ESP"), &Settings::EnemyWeaponESP);
		}
		if (tab == 3)
		{
			GaGui::NextColumn(30.0f);
			GaGui::Text(xorstr(" "));
			GaGui::Checkbox(xorstr("Vehicle Fly (Might be broken)"), &Settings::CarFly);

			GaGui::Checkbox(xorstr("Instant Revive"), &Settings::InstantRevive);

			GaGui::Checkbox(xorstr("Fov Slider"), &Settings::FovChanger);
			GaGui::SliderFloat(xorstr(""), &Settings::FovChanger_Value, 30.f, 170.f, xorstr("%.1f"));
		}
	}

	GaGui::Render();
}


VOID(*o_PostRender)(uintptr_t _this, UCanvas* Canvas) = nullptr;

VOID hk_PostRender(uintptr_t _this, UCanvas* Canvas)
{


	if (Canvas)
	{
		//printf(xorstr("CANVAS: 0x%llx    "), Canvas);
	//	printf(xorstr("X: %d  Y: %d\n"), Canvas->SizeX, Canvas->SizeY);

		HUD_Info::EngineRenderer = (UEngineRenderer*)Canvas;
		HUD_Info::Width = Canvas->SizeX;
		HUD_Info::Height = Canvas->SizeY;

		if (Settings::ShowFovCircle)
			HUD_Info::EngineRenderer->DrawCircle(FVector2D(HUD_Info::Width / 2, HUD_Info::Height / 2), (Settings::FovCircle_Value * HUD_Info::Width / Addresses::FOVAngle) / 2, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), 1450);

		TestLoop();

		if (isSelected())
			Menu();
	}


	return o_PostRender(_this, Canvas);
}


int Test = 0;
void TestLog() {
	Test = printf("Injecting") + 1;
	std::cout << ("Test Num #", Test) << std::endl;
	Sleep(2000);
}

BOOL DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH) return FALSE;

	CreateConsole();
	TestLog();
	LoadSignatures();
	TestLog();
	LoadObjects();

	TestLog();


	auto u_world = *(uintptr_t*)(Addresses::UWorld); if (!u_world) return false;
	auto g_world = (UWorld*)u_world;

	auto localplayer = g_world->OwningGameInstance()->LocalPlayers()->LocalPlayer();

	auto viewport = localplayer->ViewportClient();

	void** PostRender_vt = *(void***)(viewport);

	DWORD old_protect;
	VirtualProtect(&PostRender_vt[107], 8, PAGE_EXECUTE_READWRITE, &old_protect);
	o_PostRender = decltype(o_PostRender)(PostRender_vt[107]);
	PostRender_vt[107] = &hk_PostRender;
	VirtualProtect(&PostRender_vt[107], 8, old_protect, &old_protect);


	TestLog();

	return TRUE;
}
