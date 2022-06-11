#include <iostream>
#include <Windows.h>
#include "Vector.h"
#include <dwmapi.h>
#include <vector>
#include "driver.h"
#include "imgui/imgui_internal.h"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <random>
#include <lmcons.h>
#include "util.hpp"
#include "c_world.h"
#include "Imgui/EngineGui.h"

IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };
HWND MyWnd = NULL;
HWND GameWnd = NULL;
RECT GameRect = { NULL };
MSG Message = { NULL };

HRESULT DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);
	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Settings::MajorValues::Width;
	p_Params.BackBufferHeight = Settings::MajorValues::Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.PixelSnapH = true;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF,
		0x0400, 0x044F,
		0,
	};

	int loaded = 0;
	if (loaded == 0) {

		loaded = 1;
	}

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);
	ImGuiStyle& Style = ImGui::GetStyle();
	auto Color = Style.Colors;


	p_Object->Release();
	return S_OK;
}

bool CheckInScreen(Vector3 Pos, uintptr_t CurrentActor, int Width, int Height) {
	if (CurrentActor)
	{
		if (((Pos.x <= 0 or Pos.x > Width) and (Pos.y <= 0 or Pos.y > Height)) or ((Pos.x <= 0 or Pos.x > Width) or (Pos.y <= 0 or Pos.y > Height))) {
			return false;
		}
		else {
			return true;
		}

	}
}

void SetupWindow()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		("devcon"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};
	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);
	RegisterClassExA(&wcex);
	MyWnd = CreateWindowExA(NULL, E("devcon"), E("devcon"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margin = { -6 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);
}

static const char* ESP_Box_TypeItems[]{
	"   Box",
	"   Cornered",
	"   3D Box"
};
void actorloop() {
	auto GWorld = read<uintptr_t>(Settings::MajorValues::Uworld);
	auto World = (UWorld*)GWorld;

	uintptr_t LocalVehicle = read<uintptr_t>(Settings::MajorValues::LocalPawn + Settings::MajorValues::VehicleState);
	if (Settings::Exploits::VehicleFly)
	{
		if (GetAsyncKeyState(VK_SHIFT)) {
			//write<float>(LocalVehicle + 0x678, 1); //vehicle gravity
			write<float>(LocalVehicle + 0x678, 1); //vehicle gravity
		}
		else {
			//write<float>(LocalVehicle + 0x678, -150); //vehicle gravity
			write<float>(LocalVehicle + 0x678, 1); //vehicle gravity
		}
	}
	if (Settings::Exploits::FOVChanger) {
		uint64_t CamManager = read<uint64_t>(Settings::MajorValues::PlayerController + 0x338);
		write<float>(CamManager + 0x298, Settings::Exploits::fovscale); // + 0x4
	}
	if (Settings::Exploits::InstantRevive) {
		write<float>(Settings::MajorValues::LocalPawn + 0x3c78, .000000000000000000000001);
	}
	else
	{
		write<float>(Settings::MajorValues::LocalPawn + 0x3c78, 10);
	}
	uintptr_t CurrentWeapon = read<uintptr_t>(Settings::MajorValues::LocalPawn + 0x7a0); //CurrentWeapon Offset
	if (Settings::Exploits::RapidFire) {
		float a = 0;
		float b = 0;
		if (CurrentWeapon) {
			a = read<float>(CurrentWeapon + 0xa70); //LastFireTime Offset
			b = read<float>(CurrentWeapon + 0xa74); //LastFireTimeVerified Offset
			write<float>(CurrentWeapon + 0xa70, a + b - 1); //LastFireTime Offset
		}
	}
	if (Settings::Exploits::RocketLeauge) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			write<bool>(LocalVehicle + 0x179c, true); //bBoosting offset
		}
		else {
			write<bool>(LocalVehicle + 0x179c, false); //bBoosting offset
		}
	}
	uintptr_t weaponstats = read<uintptr_t>(CurrentWeapon + 0x16c);
	if (Settings::Exploits::VehicleTest)
	{
		float lastreload = 0;
		float currentreload = 0;
		if (CurrentWeapon) {
			lastreload = read<float>(Settings::MajorValues::LocalPawn + 0xaf0);
			currentreload = read<float>(Settings::MajorValues::LocalPawn + 0xaf4);
			write<float>(Settings::MajorValues::LocalPawn + 0xaf0, lastreload + currentreload - 0.1);
		}
	}
	if (Settings::Exploits::PlayerFlight)
	{
		write<float>(Settings::MajorValues::LocalPawn + 0x1c40, 1000.f);
		write<bool>(Settings::MajorValues::LocalPawn + 0x1bf0 + 0x18, true);
	}
	if (Settings::Exploits::airstuck) {
		if (GetAsyncKeyState(VK_SHIFT)) { //shift Keybind
			write<float>(Settings::MajorValues::LocalPawn + 0x64, 0.01); //CustomTimeDilation Offset
		}
		else {
			write<float>(Settings::MajorValues::LocalPawn + 0x64, 1); //CustomTimeDilation Offset
		}
	}
	if (Settings::Exploits::ADSAir) {
		World->OwningGameInstance()->LocalPlayers()->LocalPlayer()->PlayerController()->LocalPawn()->CurrentWeapon()->bADSWhileNotOnGround();
	}
	if (Settings::Exploits::AnitAim) {
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			while_rotating = true;
			if (Settings::MajorValues::LocalPawn) {

				if (OldMesh == 0 && OldRotation == 0) {
					OldMesh = read<uint64_t>(Settings::MajorValues::LocalPawn + 0x300);
					OldRotation = read<uint64_t>(OldMesh + 0x140);
				}

				//int increased_value;
				//int decreased_value;

				uint64_t MeshA = read<uint64_t>(Settings::MajorValues::LocalPawn + 0x300);

				increased_value = increased_value + 15;

				write<Vector3>(MeshA + 0x140, Vector3(1, increased_value, 1));

				if (increased_value == 360) {
					decreased_value = decreased_value - 15;

					if (decreased_value == -360) {
						decreased_value = 1;
					}

					write<Vector3>(MeshA + 0x140, Vector3(1, decreased_value, 1));
				}
			}
		}
		else if (!GetAsyncKeyState(VK_RBUTTON))
		{
			if (Settings::MajorValues::LocalPawn) {
				if (OldMesh != 0 && OldRotation != 0) {
					write<uint64_t>(OldMesh + 0x140, OldRotation);
					while_rotating = false;
				}
			}
		}
	}

	gameexploits();
	ImDrawList* Renderer = ImGui::GetOverlayDrawList();
	Vector3 LocalRelativeLocation;
	auto entityListCopy = entityList;
	float closestDistance = FLT_MAX;
	DWORD_PTR closestPawn = NULL;
	DWORD_PTR AActors = read<DWORD_PTR>(Settings::MajorValues::Ulevel + 0x98);
	int ActorTeamId = read<int>(0x1068);
	int curactorid = read<int>(0x18);
	for (unsigned long i = 0; i < entityListCopy.size(); ++i) {
		FNlEntity entity = entityListCopy[i];
		uint64_t CurrentActor = read<uint64_t>(AActors + i * 0x8);

		uint64_t CurActorRootComponent = read<uint64_t>(entity.Actor + 0x188);
		if (CurActorRootComponent == (uint64_t)nullptr || CurActorRootComponent == -1 || CurActorRootComponent == NULL)
			continue;

		Vector3 actorpos = read<Vector3>(CurActorRootComponent + 0x128);
		Vector3 actorposW2s = ProjectWorldToScreen(actorpos);
		DWORD64 otherPlayerState = read<uint64_t>(entity.Actor + 0x2A0);
		if (otherPlayerState == (uint64_t)nullptr || otherPlayerState == -1 || otherPlayerState == NULL)
			continue;

		Settings::MajorValues::localactorpos = read<Vector3>(Settings::MajorValues::Rootcomp + 0x128);

		Vector3 bone66 = GetBoneWithRotation(entity.mesh, 98);
		Vector3 neck = GetBoneWithRotation(entity.mesh, 66);
		Vector3 bone7 = GetBoneWithRotation(entity.mesh, 7);
		Vector3 bone37 = GetBoneWithRotation(entity.mesh, 37);
		Vector3 bone2 = GetBoneWithRotation(entity.mesh, 2);
		Vector3 bone78 = GetBoneWithRotation(entity.mesh, 78);
		Vector3 bone0 = GetBoneWithRotation(entity.mesh, 0);
		Vector3 LocalBone2 = GetBoneWithRotation(Settings::MajorValues::LocalPawn, 2);
		Vector3 LocalPelvis = ProjectWorldToScreen(Vector3(LocalBone2.x, LocalBone2.y, LocalBone2.z));
		Vector3 top = ProjectWorldToScreen(bone66);
		Vector3 neck2 = ProjectWorldToScreen(neck);
		Vector3 chest = ProjectWorldToScreen(bone66);
		Vector3 aimbotspot = ProjectWorldToScreen(bone7);
		Vector3 bottom = ProjectWorldToScreen(bone0);
		Vector3 Head = ProjectWorldToScreen(Vector3(bone66.x, bone66.y, bone66.z));
		Vector3 boneChest = ProjectWorldToScreen(Vector3(bone37.x, bone37.y, bone37.z));
		Vector3 bonePelvis = ProjectWorldToScreen(Vector3(bone2.x, bone2.y, bone2.z));
		Vector3 boneBottom = ProjectWorldToScreen(Vector3(bone78.x, bone78.y, bone78.z));
		Vector3 player_screen = ProjectWorldToScreen(bone0);

		float distance = Settings::MajorValues::localactorpos.Distance(bone66) / 100.f;
		float BoxHeight = (float)(Head.y - bottom.y);
		float BoxWidth = BoxHeight * 0.6;
		//0.46
		float CornerHeight = abs(Head.y - bottom.y);
		float CornerWidth = BoxHeight * 0.46;

		int MyTeamates = read<int>(Settings::MajorValues::PlayerState + 0x1068);
		int TeamIndex = read<int>(otherPlayerState + 0x1068);
		int curactorid = read<int>(CurrentActor + 0x98);
		uintptr_t MyTeamIndex, EnemyTeamIndex;
		uint32_t object_id = read<uint32_t>(CurrentActor + 0x18);
		std::string Names2 = GetNameFromFName(object_id);

		if (MyTeamates != TeamIndex) {
			if (IsVisible(entity.mesh)) {
				Settings::MajorValues::SkeletonColor = ImColor(14, 218, 245);
				Settings::MajorValues::BoxColor = ImColor(14, 218, 245);
			}
			else {
				Settings::MajorValues::SkeletonColor = ImColor(145, 14, 245);
				Settings::MajorValues::BoxColor = ImColor(145, 14, 245);
			}
			if (Settings::ESP::visualName && entity.name == "player" && distance < Settings::ESP::MaxEspDistance)
			{
				char dist[255];
				sprintf_s(dist, E("HUMAN"));
				ImGui::GetOverlayDrawList()->AddText(ImVec2(Head.x - 3, Head.y - 35), ImColor(255, 0, 0), dist);
			}

			if (Settings::ESP::visualName && entity.name == "bot" && distance < Settings::ESP::MaxEspDistance)
			{
				char dist[255];
				sprintf_s(dist, E("NPC"));
				ImGui::GetOverlayDrawList()->AddText(ImVec2(Head.x - 3, Head.y - 35), ImColor(255, 0, 0), dist);
			}
			if (Settings::ESP::skeleton && distance < Settings::ESP::MaxEspDistance) {
				GetPlayersBone(entity.mesh);
			}
			if (Settings::ESP::skeletonvisible && distance < Settings::ESP::MaxEspDistance) {
				if (IsVisible(entity.mesh)) {
					GetPlayersBone(entity.mesh);
				}
			}
			if (Settings::ESP::box && distance < Settings::ESP::MaxEspDistance)
			{
				if (Settings::ESP::boxMode == 0) { //2d box
					Draw2DBoundedBox(bottom.x + (BoxWidth / 2), bottom.y, Head.x - (BoxWidth / 2), Head.y, Settings::MajorValues::BoxColor, 1.5);
				}
				if (Settings::ESP::boxMode == 1) { //corner box
					CornerBox(neck2.x - (CornerWidth / 2), neck2.y, CornerWidth, CornerHeight, Settings::MajorValues::BoxColor, 1.5);
				}
				if (Settings::ESP::boxMode == 2) { //3D box
					Vector3 bottom1 = ProjectWorldToScreen(Vector3(bone0.x + 30, bone0.y - 30, bone0.z));
					Vector3 bottom2 = ProjectWorldToScreen(Vector3(bone0.x - 30, bone0.y - 30, bone0.z));
					Vector3 bottom3 = ProjectWorldToScreen(Vector3(bone0.x - 30, bone0.y + 30, bone0.z));
					Vector3 bottom4 = ProjectWorldToScreen(Vector3(bone0.x + 30, bone0.y + 30, bone0.z));

					Vector3 top1 = ProjectWorldToScreen(Vector3(bone66.x + 30, bone66.y - 30, bone66.z));
					Vector3 top2 = ProjectWorldToScreen(Vector3(bone66.x - 30, bone66.y - 30, bone66.z));
					Vector3 top3 = ProjectWorldToScreen(Vector3(bone66.x - 30, bone66.y + 30, bone66.z));
					Vector3 top4 = ProjectWorldToScreen(Vector3(bone66.x + 30, bone66.y + 30, bone66.z));

					Renderer->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(top1.x, top1.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(top2.x, top2.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(top3.x, top3.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(top4.x, top4.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(bottom2.x, bottom2.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(bottom3.x, bottom3.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(bottom4.x, bottom4.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(bottom.x, bottom.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(top1.x, top1.y), ImVec2(top2.x, top2.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(top2.x, top2.y), ImVec2(top3.x, top3.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(top3.x, top3.y), ImVec2(top4.x, top4.y), ImColor(255, 0, 0, 255), 1.f);
					Renderer->AddLine(ImVec2(top4.x, top4.y), ImVec2(top1.x, top1.y), ImColor(255, 0, 0, 255), 1.f);
				}
			}
			if (Settings::ESP::snaplines) {
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Settings::MajorValues::Width / 2, Settings::MajorValues::Height / 1), ImVec2(bottom.x, bottom.y), ImColor(255, 0, 0, 255), 0.01f);
			}
			if (Settings::Misc::dynamicfov) {
				Settings::Aimbot::AimbotHipfireFOVValue = +1;
			}
			if (Settings::Aimbot::Mouse) {
				auto dx = aimbotspot.x - (Settings::MajorValues::Width / 2);
				auto dy = aimbotspot.y - (Settings::MajorValues::Height / 2);
				auto dz = aimbotspot.z - (Settings::MajorValues::Depth / 2);
				auto dist = sqrtf(dx * dx + dy * dy + dz * dz) / 100.0f;
				if (dist < Settings::Aimbot::AimbotFOVValue && dist < closestDistance) {
					closestDistance = dist;
					closestPawn = entity.Actor;
				}
			}
			if (Settings::Aimbot::Mouse && distance < Settings::Aimbot::AimbotDistanceValue) {
				if (closestPawn != 0) {
					if (Settings::Aimbot::Mouse && closestPawn && IsVisible(entity.mesh) && GetAsyncKeyState(VK_RBUTTON) < 0) {
						mouselock(closestPawn);
						Settings::MajorValues::Targeting = true;
					}
				}
				else {
					Settings::MajorValues::Targeting = false;
					disablemouselock(closestPawn);
				}
			}
			/*if (Settings::Aimbot::Mouse && distance < Settings::Aimbot::AimbotDistanceValue) {
				if (closestPawn != 0) {
					if (Settings::Aimbot::Mouse && closestPawn && IsVisible(entity.mesh) && GetAsyncKeyState(hotkeys::hipfirekey) < 0) {
						mouselock(closestPawn);
						Settings::MajorValues::Targeting = true;
					}
				}
				else {
					Settings::MajorValues::Targeting = false;
					disablemouselock(closestPawn);
				}
			}*/
		}
		else if (CurActorRootComponent != CurrentActor) {
			if (Settings::ESP::box && distance < Settings::ESP::MaxEspDistance)
			{
				Draw2DBoundedBox(bottom.x + (BoxWidth / 2), bottom.y, Head.x - (BoxWidth / 2), Head.y, ImColor(47, 255, 0), 1.5);
			}
		}
	}
	if (Settings::MajorValues::LocalPawn) {
		for (unsigned long i = 0; i < itemslist.size(); ++i)
		{
			FNlEntity items = itemslist[i];

			uint64_t CurActorRootComponent = read<uint64_t>(items.Actor + 0x188); //rootcomp
			Vector3 curactorrelativelov = read<Vector3>(CurActorRootComponent + 0x128); // relative location

			Vector3 itempos = ProjectWorldToScreen(curactorrelativelov); // get actor pos on screen

			float distance = LocalRelativeLocation.Distance(curactorrelativelov) / 100.f;

			if (Settings::ESP::chest && items.name == (char*)("chest") && distance < Settings::ESP::MaxEspDistance)
			{
				if (CheckInScreen(itempos, items.Actor, Settings::MajorValues::Width, Settings::MajorValues::Height)) {
					//ABuildingContainer	bAlreadySearched : 1	0xf49	char
					int ChestState = read<int>(items.Actor + 0xf49);
					//std::cout << "ChestState: " << ChestState << std::endl;
					if (ChestState == 9) {
						char name[64];
						sprintf_s(name, ("chest %.fm"), distance);
						ImGui::GetForegroundDrawList()->AddText(ImVec2(itempos.x, itempos.y), ImColor(255, 234, 0, 255), name);
					}
				}
			}

			/*if (Settings::AmmoBoxESP && items.name == (char*)better_crypt("ammo") && distance < Settings::ObjectsDistance)
			{
				if (CheckInScreen(itempos, items.Actor, Settings::Width, Settings::Height)) {
					int AmmoBoxState = Read<int>(items.Actor + 0xf49);
					//std::cout << "AmmoBoxState: " << AmmoBoxState << std::endl;
					if (AmmoBoxState == 9) {
						char name[64];
						sprintf_s(name, better_crypt("ammo %.fm"), distance);
						ImGui::GetForegroundDrawList()->AddText(ImVec2(itempos.x, itempos.y), ImColor(70, 70, 70, 255), name);
					}
				}
			}

			if (Settings::VehiclesESP && items.name == (char*)better_crypt("vehicle") && distance < Settings::ObjectsDistance)
			{
				if (CheckInScreen(itempos, items.Actor, Settings::Width, Settings::Height)) {
					char name[64];
					sprintf_s(name, better_crypt("vehicle %.fm"), distance);
					ImGui::GetForegroundDrawList()->AddText(ImVec2(itempos.x, itempos.y), ImColor(255, 255, 255, 255), name);
				}
			}

			if (Settings::WeakSpotESP && items.name == (char*)better_crypt("weakspot") && distance < Settings::ObjectsDistance) {
				if (CheckInScreen(itempos, items.Actor, Settings::Width, Settings::Height)) {
					//ABuildingWeakSpot	bActive : 1	0x2b0	char
					char WeakSpotState = Read<char>(items.Actor + 0x2b0);
					if (WeakSpotState) {
						std::string weakspot_text = (char*)better_crypt("(WS)");
						ImGui::GetForegroundDrawList()->AddText(ImVec2(itempos.x, itempos.y), ImColor(255, 0, 0, 255), weakspot_text.c_str());

						if (Settings::WeakSpotAimbot) {
							float dx = itempos.x - (Settings::Width / 2);
							float dy = itempos.y - (Settings::Height / 2);
							float dist = custom::sqrtf_(dx * dx + dy * dy) / 100.f;

							if (dist < FOV && dist < closest_weakspot_Distance) {
								closest_weakspot_Distance = dist;
								closest_weakspot_Pawn = items.Actor;
							}

							if (safe_call(GetAsyncKeyState)(hotkeys::mousekey)) {
								if ((GetDistance(itempos.x, itempos.y, itempos.z, Settings::Width / 2, Settings::Height / 2) <= FOV)) {
									if (GetClosestPlayerToCrossHair(itempos, FOV_Max, FOV)) {
										move_to(itempos.x, itempos.y);
									}
								}
							}
						}
					}
				}
			}*/
		}
	}
}


#include "menu.h"

WPARAM MainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{

			old_rc = rc;

			Settings::MajorValues::Width = rc.right;
			Settings::MajorValues::Height = rc.bottom;

			p_Params.BackBufferWidth = Settings::MajorValues::Width;
			p_Params.BackBufferHeight = Settings::MajorValues::Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Settings::MajorValues::Width, Settings::MajorValues::Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanuoD3D();
	DestroyWindow(MyWnd);

	return Message.wParam;
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		CleanuoD3D();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}
void CleanuoD3D()
{
	if (p_Device != NULL)
	{
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL)
	{
		p_Object->Release();
	}
}
int isTopwin()
{
	HWND hWnd = GetForegroundWindow();
	if (hWnd == GameWnd)
		return TopWindowGame;
	if (hWnd == MyWnd)
		return TopWindowMvoe;

	return 0;
}
void SetWindowToTarget()
{
	while (true)
	{
		GameWnd = get_process_wnd(sdk::process_id);
		if (GameWnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(GameWnd, &GameRect);
			Settings::MajorValues::Width = GameRect.right - GameRect.left;
			Settings::MajorValues::Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(GameWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 5;
				GameRect.left += 8;
				GameRect.right += 11;
				GameRect.bottom += 71;
				Settings::MajorValues::Height -= 2;
			}
			Settings::MajorValues::ScreenCenterX = Settings::MajorValues::Width / 2;
			Settings::MajorValues::ScreenCenterY = Settings::MajorValues::Height / 2;
			MoveWindow(MyWnd, GameRect.left, GameRect.top, Settings::MajorValues::Width, Settings::MajorValues::Height, true);
		}
	}
}

int main() {
	try {
		driver::setup();

		const char* proc_name = "FortniteClient-Win64-Shipping.exe";

		sdk::process_id = getpid(proc_name);
		printf("pid -> %i\n", sdk::process_id);
		sdk::module_base = getbase(sdk::process_id);
		printf("base -> 0x%p\n", (void*)sdk::module_base);

		SetupWindow();

		DirectXInit(MyWnd);
		//system("pause");
		printf(("enjoy division fortnite"));
		//lootespniggerrat();
		//to_clipboard();
		HANDLE worldaceh = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(world), nullptr, NULL, nullptr);
		HANDLE handle = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(cache), nullptr, NULL, nullptr);
		CloseHandle(worldaceh);
		CloseHandle(handle);

		MainLoop();
	}
	catch (...) { printf("failed\n"); }
}