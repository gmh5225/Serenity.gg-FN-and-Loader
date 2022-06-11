#pragma once
#define E
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define TopWindowGame 11
#define TopWindowMvoe 22

HRESULT DirectXInit(HWND hWnd);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
void SetupWindow();
WPARAM MainLoop();
void CleanuoD3D(); void ChangeClickability(bool canclick, HWND ownd);

int isTopwin();
void Render(int Topwin, bool showimgui);
void SetWindowToTarget();

namespace sdk {
	DWORD process_id;
	DWORD64 module_base;
}

typedef struct _FNlEntity {
	uint64_t Actor;
	int ID;
	uint64_t mesh;
	std::string name;
}FNlEntity;

std::vector<FNlEntity> entityList;
std::vector<FNlEntity> itemslist;
Vector3 LocalRelativeLocation;

namespace Settings {

	namespace Aimbot {
		bool Mouse = true;
		bool Memory = true;
		bool Prediction = false;
		float AimbotFOVValue = 120.f;
		float AimbotHipfireFOVValue = 120.f;
		int AimbotSmoothingValue = 10.0f;
		int AimbotDistanceValue = 100.0f;
		int hitbox = 7;
		int Aimkey = VK_RBUTTON;
	}

	namespace ESP {
		bool snaplines = false;
		bool distanceESP = false;
		bool visualName = false;
		bool box = false;
		bool outline = false;
		bool skeleton = false;
		bool skeletonvisible = false;
		int boxMode = 1;
		bool showHead = false;
		int MaxEspDistance = 100;

		//world
		bool chest = false;
		bool weapon = false;
	}

	namespace Misc {
		bool VisibleCheck = false;
		bool DrawFOV = false;
		bool Crosshair = false;
		bool dynamicfov = false;
	}

	namespace Exploits {
		bool InstantRevive = false;
		bool ADSAir = false;
		bool PlayerFlight = false;
		bool Backtrack = false;
		bool RapidFire = false;
		bool VehicleFly = false;
		bool RocketLeauge = false;
		bool VehicleTest = false;

		bool AnitAim = false;
		bool FOVChanger = false;
		int fovscale = 100;
		bool airstuck = false;
		bool bigplayers = false;
		int hitboxsize = 1;
		int vehicleFlySpeed = 100;
	}

	namespace MajorValues {
		DWORD_PTR VehicleState = 0x1ee0; //0x1EA8 //0x1ee0

		ImColor SkeletonColor;
		ImColor BoxColor;
		DWORD_PTR LocalWeapon;
		DWORD_PTR LocalPlayer = 0, LocalPawn = 0, LocalPawnRootComponent = 0, actorcount = 0, aactors = 0, Uworld = 0, Gameinstance = 0, LocalPlayers = 0, Localplayer = 0, PlayerController = 0, PlayerState = 0, Rootcomp = 0, Persistentlevel = 0, Ulevel = 0;
		Vector3 RelativeLocation;
		int LocalPlayerID = 0, CorrectbSpotted = 0;
		bool showmenu = true;
		int enemyID = 0;
		bool Targeting;
		Vector3 selection;
		Vector3 localactorpos;
		Vector3 LocalPlayerRelativeLocation = Vector3(0.0f, 0.0f, 0.0f);
		float ClosestActorDistance = FLT_MAX;
		int Width, Height, Depth;
		DWORD ScreenCenterX, ScreenCenterY, ScreenCenterZ;
		Vector3 ClosestActorMouseAimbotPosition = Vector3(0.0f, 0.0f, 0.0f);
	}
}
static HWND get_process_wnd(uint32_t pid) {
	std::pair<HWND, uint32_t> params = { 0, pid };

	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
		uint32_t processId = 0;

		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
			SetLastError((uint32_t)-1);
			pParams->first = hwnd;
			return FALSE;
		}

		return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}

__forceinline static std::string ReadGetNameFromFName(int key) {
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = read<uint64_t>(sdk::module_base + 0xC42D200 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset);
	uint16_t nameEntry = read<uint16_t>(NamePoolChunk);

	int nameLength = nameEntry >> 6;
	char buff[1024];
	if ((uint32_t)nameLength) {
		char* v2 = buff; // rbx
		unsigned int v4 = nameLength;
		unsigned int v5; // eax
		__int64 result; // rax
		int v7; // ecx
		unsigned int v8; // kr00_4
		__int64 v9; // ecx

		read1(NamePoolChunk + 4, buff, v4);

		v5 = 0;
		result = 28i64;
		if (v4) {
			do {
				++v2;
				v7 = v5++ | 0xB000;
				v8 = v7 + result;
				BYTE(v7) = v8 ^ ~*(BYTE*)(v2 - 1);
				result = v8 >> 2;
				*(BYTE*)(v2 - 1) = v7;
			} while (v5 < v4);
		}
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	else {
		return "";
	}
}

__forceinline static std::string GetNameFromFName(int key) {
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = read<uint64_t>(sdk::module_base + 0xC42D200 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
	if (read<uint16_t>(NamePoolChunk) < 64) {
		auto a1 = read<DWORD>(NamePoolChunk + 4);
		return ReadGetNameFromFName(a1);
	}
	else {
		return ReadGetNameFromFName(key);
	}
}

void world() {
	while (true) {
		std::vector<FNlEntity> tmpList;
		Settings::MajorValues::Uworld = read<DWORD_PTR>(sdk::module_base + 0xcc65a08);
		Settings::MajorValues::Gameinstance = read<DWORD_PTR>(Settings::MajorValues::Uworld + 0x1a8);
		Settings::MajorValues::LocalPlayers = read<DWORD_PTR>(Settings::MajorValues::Gameinstance + 0x38);
		Settings::MajorValues::Localplayer = read<DWORD_PTR>(Settings::MajorValues::LocalPlayers);
		Settings::MajorValues::PlayerController = read<DWORD_PTR>(Settings::MajorValues::Localplayer + 0x30);
		Settings::MajorValues::LocalPawn = read<DWORD_PTR>(Settings::MajorValues::PlayerController + 0x310); //0x338
		Settings::MajorValues::PlayerState = read<DWORD_PTR>(Settings::MajorValues::LocalPawn + 0x290);
		Settings::MajorValues::Rootcomp = read<DWORD_PTR>(Settings::MajorValues::LocalPawn + 0x188);
		Settings::MajorValues::LocalWeapon = read<uint64_t>(Settings::MajorValues::LocalPawn + 0x7a0); //cur weapon
		if (Settings::MajorValues::LocalPawn != 0) {
			Settings::MajorValues::LocalPlayerID = read<int>(Settings::MajorValues::LocalPawn + 0x18);
		}
		Settings::MajorValues::Persistentlevel = read<DWORD_PTR>(Settings::MajorValues::Uworld + 0x30);
		DWORD ActorCount = read<DWORD>(Settings::MajorValues::Persistentlevel + 0xA0);
		DWORD_PTR AActors = read<DWORD_PTR>(Settings::MajorValues::Persistentlevel + 0x98);
		for (int i = 0; i < ActorCount; i++) {
			uint64_t CurrentActor = read<uint64_t>(AActors + i * 0x8);
			int curactorid = read<int>(CurrentActor + 0x18);
			std::string fnames = GetNameFromFName(curactorid); // takes ~15% cpu time
			if (!CurrentActor || CurrentActor == Settings::MajorValues::LocalPawn) continue;
			auto ghettocheck = read<float>(CurrentActor + 0x3C78);
			if (ghettocheck != 10.f) continue;
			FNlEntity fnlEntity{ };
			fnlEntity.Actor = CurrentActor;
			fnlEntity.mesh = read<uint64_t>(CurrentActor + 0x300);
			fnlEntity.name = "player";
			tmpList.push_back(fnlEntity);
			if (strstr(fnames.c_str(), "MangPlayerPawn_Boss_") || strstr(fnames.c_str(), "BP_PlayerPawn_Athena_Phoebe_C") || strstr(fnames.c_str(), "BP_PlayerPawn_Tandem_C") || strstr(fnames.c_str(), "BP_IOPlayerPawn_Base_C") || strstr(fnames.c_str(), "BP_MangPlayerPawn"))
			{

				fnlEntity.Actor = CurrentActor;
				fnlEntity.mesh = read<uint64_t>(CurrentActor + 0x300);
				fnlEntity.name = "bot";
				tmpList.push_back(fnlEntity);

			}
		}
		entityList = tmpList;
		Sleep(1);
	}
}

void cache()
{
	while (true) {
		std::vector<FNlEntity> tmpList;

		for (int i = 0; i < Settings::MajorValues::actorcount; i++) {
			uint64_t CurrentActor = read<uint64_t>(Settings::MajorValues::aactors + (i * sizeof(uintptr_t)));

			int curactorid = read<int>(CurrentActor + 0x18);

			std::string fnames = GetNameFromFName(curactorid); // takes ~15% cpu time

			if (!CurrentActor || CurrentActor == Settings::MajorValues::LocalPawn) continue;

			FNlEntity fnlEntity{ };

			if (strstr(fnames.c_str(), "PlayerPawn_Athena_C")) {
				fnlEntity.Actor = CurrentActor;
				fnlEntity.mesh = read<uint64_t>(CurrentActor + 0x300);
				fnlEntity.name = "player";
				tmpList.push_back(fnlEntity);
			}
			else if (strstr(fnames.c_str(), "BP_PlayerPawn_Athena_Phoebe_C") || strstr(fnames.c_str(), "MangPlayerPawn_Boss_") || strstr(fnames.c_str(), "BP_PlayerPawn_Tandem_C") || strstr(fnames.c_str(), "BP_IOPlayerPawn_Base_C") || strstr(fnames.c_str(), "BP_MangPlayerPawn"))
			{
				fnlEntity.Actor = CurrentActor;
				fnlEntity.mesh = read<uint64_t>(CurrentActor + 0x300);
				fnlEntity.name = "bot";
				tmpList.push_back(fnlEntity);
			}
			else if (strstr(fnames.c_str(), "TeleportationDrone_C"))
			{
				fnlEntity.Actor = CurrentActor;
				fnlEntity.mesh = read<uint64_t>(CurrentActor + 0x300);
				fnlEntity.name = "zombie";
				tmpList.push_back(fnlEntity);
			}
		}

		entityList.clear();
		entityList = tmpList;

		if (Settings::MajorValues::LocalPawn) {
		std::vector<FNlEntity> levelslist;

		for (auto itemlevels_i = 0UL; itemlevels_i < read<DWORD>(Settings::MajorValues::Uworld + (0x148 + sizeof(PVOID))); ++itemlevels_i)
		{
			uintptr_t itemlevels = read<uintptr_t>(Settings::MajorValues::Uworld + 0x148);
			if (!itemlevels) return;

			uintptr_t itemlevel = read<uintptr_t>(itemlevels + (itemlevels_i * sizeof(uintptr_t)));
			if (!itemlevel) return;

			for (int i = 0; i < read<DWORD>(itemlevel + (0x98 + sizeof(PVOID))); ++i) {

				FNlEntity levelitemsstruct;

				uintptr_t ItemsPawns = read<uintptr_t>(itemlevel + 0x98);
				if (!ItemsPawns) return;

				uintptr_t CurrentItemPawn = read<uintptr_t>(ItemsPawns + (i * sizeof(uintptr_t)));

				int itemid = read<int>(CurrentItemPawn + 0x18);

				std::string itemname = GetNameFromFName(itemid);

				if (strstr(itemname.c_str(), ("Tiered_Chest")) || strstr(itemname.c_str(), ("Tiered_Ammo")) || strstr(itemname.c_str(), ("Vehicl")) || strstr(itemname.c_str(), ("Valet_TaxiCab_Vehicle_C")) || strstr(itemname.c_str(), ("Valet_BasicCar_Vehicle_C")) || strstr(itemname.c_str(), ("Valet_BigRig_Vehicle_C")) || strstr(itemname.c_str(), ("Valet_BasicCar_Vehicle_C")) || strstr(itemname.c_str(), ("Valet_BasicCar_Vehicle_C")) || strstr(itemname.c_str(), ("WeakSpot_C")))
				{
					if (Settings::ESP::chest && strstr(itemname.c_str(), ("Tiered_Chest")))
					{
						levelitemsstruct.Actor = CurrentItemPawn;
						levelitemsstruct.name = (char*)("chest");
						levelslist.push_back(levelitemsstruct);
					}

					/*if (Settings::AmmoBoxESP && strstr(itemname.c_str(), ("Tiered_Ammo")))
					{
						levelitemsstruct.Actor = CurrentItemPawn;
						levelitemsstruct.name = (char*)("ammo");
						levelslist.push_back(levelitemsstruct);
					}

					if (Settings::VehiclesESP && strstr(itemname.c_str(), ("Vehicl")) || strstr(itemname.c_str(), ("Valet_TaxiCab_Vehicle_C")) || strstr(itemname.c_str(), ("Valet_BasicCar_Vehicle_C")) || strstr(itemname.c_str(), ("Valet_BigRig_Vehicle_C")) || strstr(itemname.c_str(), ("Valet_BasicCar_Vehicle_C")) || strstr(itemname.c_str(), ("Valet_BasicCar_Vehicle_C")) || strstr(itemname.c_str(), ("WeakSpot_C")))
					{
						levelitemsstruct.Actor = CurrentItemPawn;
						levelitemsstruct.name = (char*)("vehicle");
						levelslist.push_back(levelitemsstruct);
					}

					if (Settings::WeakSpotAimbot || Settings::WeakSpotESP && strstr(itemname.c_str(), ("WeakSpot_C")))
					{
						levelitemsstruct.Actor = CurrentItemPawn;
						levelitemsstruct.name = (char*)("weakspot");
						levelslist.push_back(levelitemsstruct);
					}*/
				}
			}
		}
		itemslist.clear();
		itemslist = levelslist;
	}

		Sleep(1000);
	}
}
extern Vector3 CameraEXT(0, 0, 0);
float FovAngle;


struct Camera
{
	float FieldOfView;
	Vector3 Rotation;
	Vector3 Location;
};

Camera GetCamera(__int64 a1)
{
	Camera LocalCamera;
	__int64 v1;
	__int64 v6;
	__int64 v7;
	__int64 v8;

	v1 = read<__int64>(Settings::MajorValues::Localplayer + 0xC8);
	__int64 v9 = read<__int64>(v1 + 8);

	LocalCamera.FieldOfView = 80.f / (read<double>(v9 + 0x690) / 1.19f);
	LocalCamera.Rotation.x = read<double>(v9 + 0x7E0);
	LocalCamera.Rotation.y = read<double>(a1 + 0x148);

	v6 = read<__int64>(Settings::MajorValues::Localplayer + 0x70);
	v7 = read<__int64>(v6 + 0x98);
	v8 = read<__int64>(v7 + 0xF8);

	LocalCamera.Location = read<Vector3>(v8 + 0x20);
	return LocalCamera;
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	Camera vCamera = GetCamera(Settings::MajorValues::Rootcomp);
	vCamera.Rotation.x = (asin(vCamera.Rotation.x)) * (180.0 / M_PI);

	D3DMATRIX tempMatrix = Matrix(vCamera.Rotation);

	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - vCamera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return Vector3((Settings::MajorValues::Width / 2.0f) + vTransformed.x * (((Settings::MajorValues::Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Settings::MajorValues::Height / 2.0f) - vTransformed.y * (((Settings::MajorValues::Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, 0);
}
bool IsVisible(uint64_t mesh)
{
	float tik = read<float>(mesh + 0x330);
	float tok = read<float>(mesh + 0x334);
	const float tick = 0.06f;
	return tok + tick >= tik;

}
FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray;
	bonearray = read<DWORD_PTR>(mesh + 0x590);

	if (bonearray == NULL)
	{
		bonearray = read<DWORD_PTR>(mesh + 0x590);
	}
	return read<FTransform>(bonearray + (index * 0x60));
}

Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = read<FTransform>(mesh + 0x240);

	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

Vector3 AimbotCorrection(float bulletVelocity, float bulletGravity, float targetDistance, Vector3 targetPosition, Vector3 targetVelocity) {
	Vector3 recalculated = targetPosition;
	float gravity = fabs(bulletGravity);
	float time = targetDistance / fabs(bulletVelocity);
	float bulletDrop = (gravity / 250) * time * time;
	recalculated.z += bulletDrop * 120;
	recalculated.x += time * (targetVelocity.x);
	recalculated.y += time * (targetVelocity.y);
	recalculated.z += time * (targetVelocity.z);
	return recalculated;
}
void aimbot(float x, float y, float z) {
	float ScreenCenterX = (Settings::MajorValues::Width / 2);
	float ScreenCenterY = (Settings::MajorValues::Height / 2);
	float ScreenCenterZ = (Settings::MajorValues::Depth / 2);
	int AimSpeedX = Settings::Aimbot::AimbotSmoothingValue;
	int AimSpeedY = Settings::Aimbot::AimbotSmoothingValue;
	int AimSpeedZ = Settings::Aimbot::AimbotSmoothingValue;
	float TargetX = 0;
	float TargetY = 0;
	float TargetZ = 0;

	if (x != 0) {
		if (x > ScreenCenterX) {
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeedX;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX) {
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeedX;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}
	if (y != 0) {
		if (y > ScreenCenterY) {
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeedY;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY) {
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeedY;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}
	if (z != 0) {
		if (z > ScreenCenterZ) {
			TargetZ = -(ScreenCenterZ - z);
			TargetZ /= AimSpeedZ;
			if (TargetZ + ScreenCenterZ > ScreenCenterZ * 2) TargetZ = 0;
		}

		if (z < ScreenCenterZ) {
			TargetZ = z - ScreenCenterZ;
			TargetZ /= AimSpeedZ;
			if (TargetZ + ScreenCenterZ < 0) TargetZ = 0;
		}
	}
	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);
	return;
}
double GetCrossDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

int AimBoneInt = 0;
static const char* Aimbone_TypeItems[]{
	"head",
	"chest",
	"pelvis"
};


void mouselock(DWORD_PTR entity) {
	uint64_t currentactormesh = read<uint64_t>(entity + 0x300);
	auto rootHead = GetBoneWithRotation(currentactormesh, Settings::Aimbot::hitbox);

	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead);
	if (rootHeadOut.x != 0 || rootHeadOut.y != 0 || rootHeadOut.z != 0) {
		if ((GetCrossDistance(rootHeadOut.x, rootHeadOut.y, rootHeadOut.z, Settings::MajorValues::Width / 2, Settings::MajorValues::Height / 2, Settings::MajorValues::Depth / 2) <= Settings::Aimbot::AimbotFOVValue * 1)) {
			aimbot(rootHeadOut.x, rootHeadOut.y, rootHeadOut.z);
		}
	}
}

void disablemouselock(DWORD_PTR entity) {
	uint64_t currentactormesh = read<uint64_t>(entity + 0x300);
	auto rootHead = GetBoneWithRotation(currentactormesh, Settings::Aimbot::hitbox);
	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead);
	if (rootHeadOut.x != 0 || rootHeadOut.y != 0 || rootHeadOut.z != 0) {
	}
}


void drawfilledbox(int x, int y, int w, int h, const ImU32& color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0, 0);
}
void CornerBox(int X, int Y, int W, int H, const ImU32& color, int thickness) {
	float lineW = (W / 3);
	float lineH = (H / 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}
VOID AddLine(ImGuiWindow& window, float width, float height, float a[3], float b[3], ImU32 color, float& minX, float& maxX, float& minY, float& maxY) {
	float ac[3] = { a[0], a[1], a[2] };
	float bc[3] = { b[0], b[1], b[2] };
	window.DrawList->AddLine(ImVec2(ac[0], ac[1]), ImVec2(bc[0], bc[1]), color, 1.0f);

	minX = min(ac[0], minX);
	minX = min(bc[0], minX);

	maxX = max(ac[0], maxX);
	maxX = max(bc[0], maxX);

	minY = min(ac[1], minY);
	minY = min(bc[1], minY);

	maxY = max(ac[1], maxY);
	maxY = max(bc[1], maxY);
}
void Draw2DBoundedBox(int x, int y, int w, int h, const ImU32& color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(w, h), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0, 0, 3);
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(w, h), ImGui::GetColorU32(color), 0, 0, thickness);
}
void GetPlayersBone(DWORD_PTR mesh)
{
	ImColor col;
	col = ImColor(255, 255, 255);
	Vector3 vHeadBone = GetBoneWithRotation(mesh, 110);
	Vector3 vHip = GetBoneWithRotation(mesh, 2);
	Vector3 vNeck = GetBoneWithRotation(mesh, 67);
	Vector3 vNeck2 = GetBoneWithRotation(mesh, 98);
	Vector3 vUpperArmLeft = GetBoneWithRotation(mesh, 9);
	Vector3 vUpperArmRight = GetBoneWithRotation(mesh, 38);
	Vector3 vLeftHand = GetBoneWithRotation(mesh, 10);
	Vector3 vRightHand = GetBoneWithRotation(mesh, 39);
	Vector3 vLeftHand1 = GetBoneWithRotation(mesh, 11);
	Vector3 vRightHand1 = GetBoneWithRotation(mesh, 40);
	Vector3 vRightThigh = GetBoneWithRotation(mesh, 76);
	Vector3 vLeftThigh = GetBoneWithRotation(mesh, 69);
	Vector3 vRightCalf = GetBoneWithRotation(mesh, 77);
	Vector3 vLeftCalf = GetBoneWithRotation(mesh, 70);
	Vector3 vLeftFoot = GetBoneWithRotation(mesh, 73);
	Vector3 vRightFoot = GetBoneWithRotation(mesh, 80);
	Vector3 vHeadBoneOut = ProjectWorldToScreen(vHeadBone);
	Vector3 vHipOut = ProjectWorldToScreen(vHip);
	Vector3 vNeckOut = ProjectWorldToScreen(vNeck);
	Vector3 vUpperArmLeftOut = ProjectWorldToScreen(vUpperArmLeft);
	Vector3 vUpperArmRightOut = ProjectWorldToScreen(vUpperArmRight);
	Vector3 vLeftHandOut = ProjectWorldToScreen(vLeftHand);
	Vector3 vRightHandOut = ProjectWorldToScreen(vRightHand);
	Vector3 vLeftHandOut1 = ProjectWorldToScreen(vLeftHand1);
	Vector3 vRightHandOut1 = ProjectWorldToScreen(vRightHand1);
	Vector3 vRightThighOut = ProjectWorldToScreen(vRightThigh);
	Vector3 vLeftThighOut = ProjectWorldToScreen(vLeftThigh);
	Vector3 vRightCalfOut = ProjectWorldToScreen(vRightCalf);
	Vector3 vLeftCalfOut = ProjectWorldToScreen(vLeftCalf);
	Vector3 vLeftFootOut = ProjectWorldToScreen(vLeftFoot);
	Vector3 vRightFootOut = ProjectWorldToScreen(vRightFoot);

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vHipOut.x, vHipOut.y), ImVec2(vNeckOut.x, vNeckOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vUpperArmLeftOut.x, vUpperArmLeftOut.y), ImVec2(vNeckOut.x, vNeckOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vUpperArmRightOut.x, vUpperArmRightOut.y), ImVec2(vNeckOut.x, vNeckOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vLeftHandOut.x, vLeftHandOut.y), ImVec2(vUpperArmLeftOut.x, vUpperArmLeftOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vRightHandOut.x, vRightHandOut.y), ImVec2(vUpperArmRightOut.x, vUpperArmRightOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vLeftHandOut.x, vLeftHandOut.y), ImVec2(vLeftHandOut1.x, vLeftHandOut1.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vRightHandOut.x, vRightHandOut.y), ImVec2(vRightHandOut1.x, vRightHandOut1.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vLeftThighOut.x, vLeftThighOut.y), ImVec2(vHipOut.x, vHipOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vRightThighOut.x, vRightThighOut.y), ImVec2(vHipOut.x, vHipOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vLeftCalfOut.x, vLeftCalfOut.y), ImVec2(vLeftThighOut.x, vLeftThighOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vRightCalfOut.x, vRightCalfOut.y), ImVec2(vRightThighOut.x, vRightThighOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vLeftFootOut.x, vLeftFootOut.y), ImVec2(vLeftCalfOut.x, vLeftCalfOut.y), Settings::MajorValues::SkeletonColor);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vRightFootOut.x, vRightFootOut.y), ImVec2(vRightCalfOut.x, vRightCalfOut.y), Settings::MajorValues::SkeletonColor);
}

static float GetFovAngle(uintptr_t player_camera_manager)
{
	auto fn = reinterpret_cast<float(*)(UINT64, char*)>(*(ULONG_PTR*)(*(ULONG_PTR*)player_camera_manager + 0x738)); //0x3790 //0x294
	return 0;
}

std::string random_string(std::size_t length)
{

	const std::string CHARACTERS = ("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ1234567890");

	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

	std::string random_string;

	for (std::size_t i = 0; i < length; ++i)
	{
		random_string += CHARACTERS[distribution(generator)];
	}

	return random_string;
}