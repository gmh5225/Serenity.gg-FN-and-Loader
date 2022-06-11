#pragma once
bool ShowMenu = true;
void shortcurts()
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		Settings::MajorValues::showmenu = !Settings::MajorValues::showmenu;
		SetConsoleTitleA(random_string(25).c_str());
	}
}
void menu() {
	if (Settings::MajorValues::showmenu)
	{
		ImGui::SetNextWindowSize({ 511.f,430.f });
		if (ImGui::Begin("Serenity - For Cheaters by Cheaters")) {
			ImGui::SetCursorPos({ 16.f,27.f });
			if (ImGui::Button("AIMBOT TAB", { 92.f,19.f }))
			{
				current = 1;
			}
			ImGui::SetCursorPos({ 112.f,27.f });
			if (ImGui::Button("VISUALS TAB", { 92.f,19.f }))
			{
				current = 2;
			}
			ImGui::SetCursorPos({ 208.f,27.f });
			if (ImGui::Button("MISC TAB", { 92.f,19.f }))
			{
				current = 3;
			}
			ImGui::SetCursorPos({ 304.f,27.f });
			if (ImGui::Button("SERVER TAB", { 92.f,19.f }))
			{
				current = 4;
			}
			ImGui::SetCursorPos({ 400.f,27.f });
			if (ImGui::Button("CLIENT TAB", { 92.f,19.f }))
			{
				current = 5;
			}

			ImGui::SetCursorPos({ 2.f,48.f });
			ImGui::BeginChild("##tabs", { 521.f,374.f }, true);

			if (current == 1) {
				ImGui::Checkbox("Mouse aimbot", &Settings::Aimbot::Mouse);
				//ImGui::Checkbox("Weakspot aimbot", &Settings::Aimbot::WeakspotAim);
				ImGui::SliderInt("Smoothing ", &Settings::Aimbot::AimbotSmoothingValue, 1, 25);
				ImGui::SliderFloat("FOV", &Settings::Aimbot::AimbotFOVValue, 1, 300);
				ImGui::SliderInt("Max aimbot distance ", &Settings::Aimbot::AimbotDistanceValue, 1, 280);
				ImGui::Text(" ");
				/*ImGui::Text("Aimbot bind: ");
				ImGui::SameLine();
				HotkeyButton(hotkeys::aimkey, ChangeKey, keystatus);
				ImGui::Text("Hipfire bind:");
				ImGui::SameLine();
				HotkeyButton(hotkeys::hipfirekey, ChangeKeys, keystatuss);*/
			}
			if (current == 2) {
				ImGui::Checkbox("Player box", &Settings::ESP::box);
				ImGui::Combo(("Box mode"), &Settings::ESP::boxMode, ESP_Box_TypeItems, sizeof(ESP_Box_TypeItems) / sizeof(*ESP_Box_TypeItems));
				ImGui::Checkbox("Player skeleton", &Settings::ESP::skeleton);
				if (Settings::ESP::skeleton) { Settings::ESP::skeletonvisible = false; }
				ImGui::Checkbox("Player skeleton (visible only)", &Settings::ESP::skeletonvisible);
				if (Settings::ESP::skeletonvisible) { Settings::ESP::skeleton = false; }
				ImGui::Checkbox("Player name", &Settings::ESP::visualName);
				ImGui::Checkbox("Snaplines", &Settings::ESP::snaplines);
				ImGui::Text(" ");
				ImGui::Checkbox("Chest", &Settings::ESP::chest);
				ImGui::Text(" ");
				ImGui::SliderInt("Max esp distance ", &Settings::ESP::MaxEspDistance, 1, 280);
			}

			if (current == 3) {
				ImGui::Checkbox("Render fov circle", &Settings::Misc::DrawFOV);
				ImGui::Checkbox("Render crosshair", &Settings::Misc::Crosshair);
				ImGui::Checkbox("Dynamic fov", &Settings::Misc::dynamicfov);
			}

			if (current == 4) {
				ImGui::Checkbox("Backtrack", &Settings::Exploits::Backtrack);
				ImGui::Checkbox("Instant Revive", &Settings::Exploits::InstantRevive);
				ImGui::Checkbox("AimWhileJumping", &Settings::Exploits::ADSAir);
				ImGui::Checkbox("Rapidfire (bursts / snipers)", &Settings::Exploits::RapidFire);
				ImGui::Checkbox("Player Flight", &Settings::Exploits::PlayerFlight);
				ImGui::Checkbox("Vehicle Fly ( hold shift )", &Settings::Exploits::VehicleFly);
				ImGui::SliderInt("Fly Speed", &Settings::Exploits::vehicleFlySpeed, 0, 600);
				ImGui::Checkbox("RocketLeauge ( hold shit )", &Settings::Exploits::RocketLeauge);
				ImGui::Checkbox("vehicle test", &Settings::Exploits::VehicleTest);
			}

			if (current == 5) {
				ImGui::Checkbox("Spinbot", &Settings::Exploits::AnitAim);
				ImGui::Checkbox("Airstuck ( shift )", &Settings::Exploits::airstuck);
				ImGui::Checkbox("Extended hitboxes", &Settings::Exploits::bigplayers);
				ImGui::SliderInt("Hitbox size", &Settings::ESP::MaxEspDistance, 1, 5);
				ImGui::Checkbox("Custom fov", &Settings::Exploits::FOVChanger);
				ImGui::SliderInt("Fov scale", &Settings::Exploits::fovscale, 80, 200);
			}
		}
	}
}



void render() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	actorloop();


	if (Settings::Misc::DrawFOV) {
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY), float(Settings::Aimbot::AimbotFOVValue), ImColor(0, 251, 255), 50, 1);
	}
	if (Settings::Misc::Crosshair) {
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(Settings::MajorValues::Width / 2 - 7, Settings::MajorValues::Height / 2), ImVec2(Settings::MajorValues::Width / 2 + 1, Settings::MajorValues::Height / 2), ImColor(255, 0, 0, 255), 1.0);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(Settings::MajorValues::Width / 2 + 8, Settings::MajorValues::Height / 2), ImVec2(Settings::MajorValues::Width / 2 + 1, Settings::MajorValues::Height / 2), ImColor(255, 0, 0, 255), 1.0);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(Settings::MajorValues::Width / 2, Settings::MajorValues::Height / 2 - 7), ImVec2(Settings::MajorValues::Width / 2, Settings::MajorValues::Height / 2), ImColor(255, 0, 0, 255), 1.0);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(Settings::MajorValues::Width / 2, Settings::MajorValues::Height / 2 + 8), ImVec2(Settings::MajorValues::Width / 2, Settings::MajorValues::Height / 2), ImColor(255, 0, 0, 255), 1.0);
	}
	shortcurts();
	if (Settings::MajorValues::showmenu)
	{
		ImGui::GetIO().MouseDrawCursor = 1;
		menu();
	}
	else {
		ImGui::GetIO().MouseDrawCursor = 0;
	}

	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}
	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&p_Params);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}