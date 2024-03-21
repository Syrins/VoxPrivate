
#include "visuals.h"

#include "icons.h"
#include <D3D11.h>
#include "ImGui/imgui_impl_dx11.h"
#include "icon.h"
#include "ImGui/imgui_internal.h"
#include "imgui_notify.h"
#include "animation.h"

#include "fonts.h"
#include "image.h"
#include "imgui_custom.hpp"

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.f);
HWND window_handle;

ID3D11Device* d3d_device;
ID3D11DeviceContext* d3d_device_ctx;
IDXGISwapChain* d3d_swap_chain;
ID3D11RenderTargetView* d3d_render_target;
D3DPRESENT_PARAMETERS d3d_present_params;


ID3D11ShaderResourceView* Logo = nullptr;

ImVec2 pos;
static int tabs = 1;
static int closeaimbotsubs = 0;
static int weaponconfigsubs = 0;
static int visualsubs = 0;

static int menslide = 0;
static int meng_animation = 0;
static int border_animation = 0;
ImFont* icons;
ImFont* colorpicker;
ImFont* tabfont;
ImFont* MenuFont1;


void TextCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}

namespace n_render {
	class c_render {
	public:

		auto imgui() -> bool {
			SPOOF_FUNC

				DXGI_SWAP_CHAIN_DESC swap_chain_description;
			ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
			swap_chain_description.BufferCount = 2;
			swap_chain_description.BufferDesc.Width = 0;
			swap_chain_description.BufferDesc.Height = 0;
			swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swap_chain_description.BufferDesc.RefreshRate.Numerator = 240;
			swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
			swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swap_chain_description.OutputWindow = window_handle;
			swap_chain_description.SampleDesc.Count = 1;
			swap_chain_description.SampleDesc.Quality = 0;
			swap_chain_description.Windowed = 1;
			swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

			D3D_FEATURE_LEVEL d3d_feature_lvl;

			const D3D_FEATURE_LEVEL d3d_feature_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

			D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, d3d_feature_array, 2, D3D11_SDK_VERSION, &swap_chain_description, &d3d_swap_chain, &d3d_device, &d3d_feature_lvl, &d3d_device_ctx);

			ID3D11Texture2D* pBackBuffer;

			//D3DX11CreateShaderResourceViewFromMemory(d3d_device, rawData, sizeof(rawData), &info,
			//    pump, &Logo, 0);
			d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

			d3d_device->CreateRenderTargetView(pBackBuffer, NULL, &d3d_render_target);

			pBackBuffer->Release();

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

			static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
			ImFontConfig icons_config;

			ImFontConfig CustomFont;
			CustomFont.FontDataOwnedByAtlas = false;

			icons_config.MergeMode = true;
			icons_config.PixelSnapH = true;
			icons_config.OversampleH = 2.5;
			icons_config.OversampleV = 2.5;

			ImGui_ImplWin32_Init(window_handle);
			ImGui_ImplDX11_Init(d3d_device, d3d_device_ctx);


			ImFontConfig font_config;
			font_config.OversampleH = 1;
			font_config.OversampleV = 1;
			font_config.PixelSnapH = 1;


			static const ImWchar ranges[] =
			{
				0x0020, 0x00FF,
				0x0400, 0x044F,
				0,
			};

			MenuFont = io.Fonts->AddFontFromFileTTF(E("C:\\Windows\\Fonts\\Verdana.ttf"), 14.0f);
			IconsFont = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 22.0f, &icons_config, icons_ranges);


			FortniteFont = io.Fonts->AddFontFromFileTTF(E("C:\\fortnite.otf"), settings::fovsize);
			Arial = io.Fonts->AddFontFromFileTTF(E("C:\\Windows\\Fonts\\Arial.ttf"), settings::fovsize);
			Verdana = io.Fonts->AddFontFromFileTTF(E("C:\\Windows\\Fonts\\Verdana.ttf"), settings::fovsize);

			GameFont = io.Fonts->AddFontFromFileTTF(E("C:\\Windows\\Fonts\\Bahnschrift.ttf"), settings::fovsize);

			MenuFont1 = io.Fonts->AddFontFromFileTTF(E("C:\\AGRESSIVE.otf"), 18.f);

			tabfont = io.Fonts->AddFontFromMemoryTTF(&mainfonthxd, sizeof mainfonthxd, 16, NULL, io.Fonts->GetGlyphRangesCyrillic());
			icons = io.Fonts->AddFontFromMemoryTTF(iconshxd, sizeof(iconshxd), 14.0f, &font_config, ranges);
			colorpicker = io.Fonts->AddFontFromMemoryTTF(iconshxd, sizeof(iconshxd), 28.0f, &font_config, ranges);

			ImGui_ImplWin32_Init(window_handle);

			ImGui_ImplDX11_Init(d3d_device, d3d_device_ctx);

			d3d_device->Release();

			return true;

		}

		auto hijack() -> bool {
			// Find the overlay window
			window_handle = FindWindowA_Spoofed(E("MedalOverlayClass"), E("MedalOverlay"));
			if (!window_handle) {
				MessageBoxA(0, skCrypt("{!} Overlay Failed. Contact Support with error {3}"), skCrypt(""), MB_ICONINFORMATION);
				return false;
			}

			// Get the screen dimensions
			int WindowWidth = GetSystemMetrics(SM_CXSCREEN);
			int WindowHeight = GetSystemMetrics(SM_CYSCREEN);

			// Set window attributes for overlay
			SetWindowPos_Spoofed(window_handle, HWND_TOPMOST, 0, 0, WindowWidth, WindowHeight, SWP_SHOWWINDOW);
			SetLayeredWindowAttributes_Spoofed(window_handle, RGB(0, 0, 0), 255, LWA_ALPHA);
			SetWindowLongA_Spoofed(window_handle, GWL_EXSTYLE, GetWindowLong(window_handle, GWL_EXSTYLE) | WS_EX_TRANSPARENT);

			// Extend frame into client area
			MARGINS Margin = { -1 };
			if (FAILED(DwmExtendFrameIntoClientArea(window_handle, &Margin))) {
				return false;
			}

			// Show and update the window
			ShowWindow_Spoofed(window_handle, SW_SHOW);
			UpdateWindow_Spoofed(window_handle);

			// Hide the window (optional)
			// ShowWindow_Spoofed(window_handle, SW_HIDE);

			return true;
		}



		ImVec2 MPos;

		bool a1 = 1, b1 = 1, c1 = 0, d1 = 1, e1 = 0;
		bool a2 = 1, b2 = 0, c2 = 1, d2 = 0, e2 = 1;
		bool a3 = 0, b3 = 0, c3 = 1, d3 = 1, e3 = 1;
		bool a4 = 0, b4 = 0, c4 = 0, d4 = 0, e4 = 0;
		float fv1 = 30, sm1 = 6;
		float fv2 = 20, sm2 = 0;
		float fv3 = 25, sm3 = 4;
		float fv4 = 0, sm4 = 0;

		float in = 10;

		int selectedWeapon = 0;
		const char* weapons[4]{ "Rifle", "Shotgun", "Smg", "Sniper" };



		auto menu() -> void {

			static bool no_titlebar = false;
			static bool no_border = true;
			static bool no_resize = false;
			static bool auto_resize = false;
			static bool no_move = false;
			static bool no_scrollbar = false;
			static bool no_collapse = false;
			static bool no_menu = true;
			static bool start_pos_set = false;
	
			ImGuiIO& io = ::ImGui::GetIO();

			io.IniFilename = NULL;
			ImGuiStyle* style = &ImGui::GetStyle();

			style->Alpha = 1.f;
			style->WindowRounding = 1.f;
			style->FramePadding = ImVec2(4, 3);
			style->WindowPadding = ImVec2(8, 8);
			style->ItemInnerSpacing = ImVec2(4, 4);
			style->ItemSpacing = ImVec2(8, 4);
			style->FrameRounding = 5.f;
			style->ScrollbarSize = 2.f;
			style->ScrollbarRounding = 12.f;

			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.96f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.11f, 0.11f, 0.14f, 1.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
			colors[ImGuiCol_Border] = ImVec4(0.32f, 0.32f, 0.58f, 0.30f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.17f, 0.00f, 0.52f, 0.26f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.38f, 0.54f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.29f, 0.37f, 0.62f, 0.54f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.33f, 0.33f, 0.68f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.81f, 0.66f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.35f, 0.37f, 0.48f, 0.40f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.33f, 0.35f, 0.49f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.42f, 0.32f, 0.67f, 1.00f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.41f, 0.73f, 1.00f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

			io.Fonts->AddFontFromFileTTF("C:/windows/fonts/pivodef.ttf", 15.0f);


			if (settings::render_menu) {
				ImGui::SetNextWindowSize({ 540, 325 });
				if (!start_pos_set) { ImGui::SetNextWindowPos(ImVec2(25, 25)); start_pos_set = true; }
				static auto current_tab = 1;

				ImGui::Begin("", &settings::render_menu, ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoTitleBar);

				TextCentered("VOX PRIVATE");

				ImGui::BeginChild(
					1,
					{ ImGui::GetContentRegionAvail().x * 0.25f, ImGui::GetContentRegionAvail().y },
					true); {
					constexpr auto button_height = 48;
					if (ImGui::Button("Aim", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 1; }
					if (ImGui::Button("Visuals", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 2; }
					if (ImGui::Button("Extras", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 3; }
					if (ImGui::Button("Weapon CFG", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 4; }

					ImGui::EndChild();
				}
				ImGui::SameLine();

				ImGui::BeginChild(
					2,
					ImGui::GetContentRegionAvail(),
					true);
				{
					switch (current_tab) {
					case 1:
						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						ImGui::Checkbox2(E("Enable Aim"), &settings::aimbot);
						ImGui::Checkbox2(E("Enable Memory Aim [ soon ]"), &settings::MemoryAIM);
						ImGui::Checkbox2(E("Enable TriggerBot [VK_CAPITAL]"), &settings::triggerbot);
						ImGui::Checkbox2(E("Enable AimLine"), &settings::target_line);
						ImGui::Checkbox2(E("Visible Check"), &settings::visible_check);
						ImGui::Checkbox2(E("Prediction for All Weapon"), &settings::Prediction);
						ImGui::Checkbox2(E("Only Sniper Prediction"), &settings::Prediction);
						ImGui::Checkbox2(E("Show FOV"), &settings::render_fov);
						ImGui::SliderFloat("Smoothness", &settings::smooth, 0.0f, 20.0f, "%.2f");
						ImGui::SliderFloat("FOV Value", &settings::aimfov, 0.0f, 300.0f, "%.2f");
						ImGui::Combo(E("Aim Bone"), &settings::hitbox, E("Head\0\Neck\0\Chest\0\Pelvis"));
						HotkeyButton(aimkey, ChangeKey, keystatus);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Black button color
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Dark gray button hover color
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Light gray button active color

						break;
					case 2:
						ImGui::Dummy(ImVec2(0.0f, 15.0f));
						ImGui::Checkbox2(E("Enable"), &settings::enable_esp);
						ImGui::Checkbox2(E("Enable RGB Mode"), &settings::RGB);
						ImGui::Checkbox2(E("2D Box"), &settings::NormalBox);
						ImGui::Checkbox2(E("Corner Box"), &settings::CornerBox);
						ImGui::Checkbox2(E("Rounded Box"), &settings::RoundBox);
						ImGui::Checkbox2(E("Filled Box"), &settings::FilledBox);
						ImGui::Checkbox2(E("Skeleton"), &settings::skeleton);
						ImGui::Checkbox2(E("Distance"), &settings::distance);
						ImGui::Checkbox2(E("Snaplines"), &settings::line);
						ImGui::Checkbox2(E("Name Esp"), &settings::username);
						ImGui::Checkbox2(E("Weapon Esp"), &settings::weapon_esp);
						ImGui::Checkbox2(E("Weapon Rarity Check"), &settings::weapon_rarity);
						break;
					case 3:
						ImGui::Dummy(ImVec2(0.0f, 15.0f));
						ImGui::Checkbox2("Outlined ESP", &settings::outline);
						ImGui::Checkbox2(E("Bone Debug"), &settings::debugbone);
						ImGui::SliderInt("Skeleton Thickness", &settings::skeleton_thick, 0.f, 5);
						ImGui::SliderInt("Box Thickness", &settings::box_thick, 0.f, 5);
						ImGui::SliderInt("Line Thickness", &settings::line_thick, 0.f, 5);
						if (settings::visible_check) {
							ImGui::Text("Visible Color");
							ImGui::ColorEdit3("##visiblecolor", (float*)&settings::color_Visible);
							ImGui::Text("Invisible Color");
							ImGui::ColorEdit3("##invisiblecolor", (float*)&settings::color_invisible);
							ImGui::Text("Skeleton Color");
							ImGui::ColorEdit3("##skeletoncolor", (float*)&settings::SkeletonVisible);
						}
						else {
							ImGui::Text("ESP Color");
							ImGui::ColorEdit3("##espcolor", (float*)&settings::color);
						}
						break;
					case 4:
						ImGui::Dummy(ImVec2(0.0f, 15.0f));
						ImGui::Checkbox2(E("CFG"), &settings::weapon_cfg);
						if (settings::weapon_cfg)
						{
							ImGui::Checkbox2(E("Sniper Config"), &settings::SniperConfig);
							ImGui::Checkbox2(E("SMG Config"), &settings::SmgConfig);
							ImGui::Checkbox2(E("ShotGun Config"), &settings::ShotGunConfig);
							ImGui::Checkbox2(E("Rifle Config"), &settings::RifleConfig);
							if (settings::SniperConfig)
							{
								ImGui::Text(E("Sniper"));
								ImGui::Separator();
								ImGui::SliderFloat(E("Sniper Smooth"), &sniper::smooth, 1, 30);
								ImGui::SliderFloat(E("Sniper Fov"), &sniper::aim_fov, 1, 300);
								ImGui::Combo(E("Sniper Bone"), &sniper::hitbox, E("Head\0\Neck\0\Chest\0\Pelvis"));

								ImGui::Text(E("Sniper Key: "));
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 1.0f));
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.33f, 0.33f, 0.33f, 0.6f));
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
								HotkeyButton(sniper::aimkey, ChangeKey4, keystatus4);
							}
							if (settings::SmgConfig)
							{
								ImGui::Text(E("Smg"));
								ImGui::Separator();
								ImGui::SliderFloat(E("Smg Smooth"), &smg::smooth, 1, 30);
								ImGui::SliderFloat(E("Smg Fov"), &smg::aim_fov, 1, 300);
								ImGui::Combo(E("Smg Bone"), &smg::hitbox, E("Head\0\Neck\0\Chest\0\Pelvis"));

								ImGui::Text(E("Smg Key: "));
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 1.0f));
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.33f, 0.33f, 0.33f, 0.6f));
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
								HotkeyButton(smg::aimkey, ChangeKey3, keystatus3);

							}
							if (settings::ShotGunConfig)
							{
								ImGui::Text(E("Shotgun"));
								ImGui::Separator();
								ImGui::SliderFloat(E("Shotgun Smooth"), &shotgun::smooth, 1, 30);
								ImGui::SliderFloat(E("Shotgun Fov"), &shotgun::aim_fov, 1, 300);
								ImGui::Combo(E("Shotgun Bone"), &shotgun::hitbox, E("Head\0\Neck\0\Chest\0\Pelvis"));

								ImGui::Text(E("Shotgun Key: "));
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 1.0f));
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.33f, 0.33f, 0.33f, 0.6f));
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
								HotkeyButton(shotgun::aimkey, ChangeKey2, keystatus2);

							}
							if (settings::RifleConfig)
							{
								ImGui::Text(E("Assault Rifle"));
								ImGui::Separator();
								ImGui::SliderFloat(E("Rifle Smooth"), &rifle::smooth, 1, 30);
								ImGui::SliderFloat(E("Rifle Fov"), &rifle::aim_fov, 1, 300);
								ImGui::Combo(E("Rifle Bone"), &rifle::hitbox, E("Head\0\Neck\0\Chest\0\Pelvis"));

								ImGui::Text(E("Rifle Key: "));
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 1.0f));
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.33f, 0.33f, 0.33f, 0.6f));
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
								HotkeyButton(rifle::aimkey, ChangeKey1, keystatus1);
							}
						}
						break;
					}

				}ImGui::EndChild();
			
				
				
					


				
			}
			ImGui::PopStyleColor();
			ImGui::End();
			// Actual menu rendering code remains unchanged
			if (GetAsyncKeyState_Spoofed(VK_INSERT) & 1)
				settings::render_menu = !settings::render_menu;


		}


		auto draw() -> void {
			// Set clear color
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.f);

			// Start ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			// Run the actor loop
			g_main->actor_loop();

			// Render menu
			menu();

			// Render ImGui frame
			ImGui::Render();

			// Clear render target
			const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
			d3d_device_ctx->OMSetRenderTargets(1, &d3d_render_target, nullptr);
			d3d_device_ctx->ClearRenderTargetView(d3d_render_target, clear_color_with_alpha);

			// Render ImGui draw data
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			// Present the frame
			d3d_swap_chain->Present(settings::Vsync, 0);
		}


		auto render() -> bool {
			SPOOF_FUNC

				MSG msg = { NULL };

			while (msg.message != WM_QUIT)
			{
				if (PeekMessageA_Spoofed(&msg, window_handle, 0, 0, PM_REMOVE))
				{
					TranslateMessage_Spoofed(&msg);
					DispatchMessage(&msg);
				}

				// Update ImGui IO
				ImGuiIO& io = ImGui::GetIO();
				io.ImeWindowHandle = window_handle;
				io.DeltaTime = 1.0f / 60.0f;

				POINT p_cursor;
				GetCursorPos(&p_cursor);
				io.MousePos.x = p_cursor.x;
				io.MousePos.y = p_cursor.y;

				// Check left mouse button state
				io.MouseDown[0] = GetAsyncKeyState_Spoofed(VK_LBUTTON) & 0x8000;

				draw();
			}

			// Shutdown ImGui
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			DestroyWindow(window_handle);

			return true;
		}
	};

} static n_render::c_render* Overlay = new n_render::c_render();