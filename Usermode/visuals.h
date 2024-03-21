#include <list>
#include <string>
#include "offsets.h"
#include <string.h>
#include "xorst.h"
#include "functions.h"
#include <iostream>
#include "aimbot.h"


using namespace uee;

#define debucache false

#define debuweapons  false

float distanceok;

void draw_filled_rect(int x, int y, int w, int h, const ImColor color)
{
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}
struct Vector2D {
	float x;
	float y;
};

struct RGBColor {
	int red;
	int green;
	int blue;
};
bool is_crosshair_over_target(const fvector& crosshairLocation, const fvector& targetHitbox)
{
	// Get screen coordinates of the target hitbox and crosshair
	fvector2d targetScreenPos = Fortnite->w2s(targetHitbox);
	fvector2d crosshairScreenPos = Fortnite->w2s(crosshairLocation);

	// Define a tolerance radius around the crosshair for considering it "over" the target
	constexpr float toleranceRadius = 10.0f; // Adjust as needed

	// Check if the target hitbox is within the tolerance radius of the crosshair
	return std::abs(targetScreenPos.x - crosshairScreenPos.x) <= toleranceRadius &&
		std::abs(targetScreenPos.y - crosshairScreenPos.y) <= toleranceRadius;
}

// Define settings namespace
namespace settings {
	// Define rgb_color variable to store RGB color values
	RGBColor rgb_color;
}
__forceinline auto optimizedskeleton(uintptr_t mesh) -> void {
	const int color = ImColor(settings::SkeletonVisible[0], settings::SkeletonVisible[1], settings::SkeletonVisible[2]);

	fvector2d bonePositions[17];
	const int boneIndices[] = { 109, 3, 67, 9, 38, 10, 39, 11, 40, 78, 71, 79, 72, 75, 82, 76, 83 }; // Include feet bones (83 and 76)

	// Retrieve bone positions
	for (int i = 0; i < 17; ++i) { // Update the loop condition
		bonePositions[i] = Fortnite->w2s(Fortnite->GetBoneLOC(mesh, boneIndices[i]));
	}

	// Define bone pairs, including connections from feet bones to other bones
	std::pair<int, int> bonePairs[] = {
		{1, 2}, {3, 4}, {4, 3}, {5, 3}, {6, 4}, {5, 7}, {6, 8},
		{10, 1}, {9, 1}, {12, 10}, {11, 9}, {13, 12}, {14, 11},
		{15, 13}, // Connect left foot (83) to lower left leg (75)
		{16, 14}  // Connect right foot (76) to lower right leg (82)
	};

	auto* drawList = ImGui::GetBackgroundDrawList();
	drawList->PushClipRectFullScreen();

	// Draw skeleton lines
	for (const auto& pair : bonePairs) {
		ImVec2 start(bonePositions[pair.first].x, bonePositions[pair.first].y);
		ImVec2 end(bonePositions[pair.second].x, bonePositions[pair.second].y);

		// Adjust neck bone length
		if (pair.first == bones::HumanHead) {
			end.y += 20;
		}

		drawList->AddLine(start, end, color, settings::skeleton_thick);
	}

	drawList->PopClipRect();

	if (settings::outline) {
		const ImColor outlineColor(0, 0, 0);
		const int outlineThickness = settings::skeleton + 1.5;

			fvector2d bonePositions[17];
	const int boneIndices[] = { 109, 3, 67, 9, 38, 10, 39, 11, 40, 78, 71, 79, 72, 75, 82, 76, 83 }; // Include feet bones (83 and 76)

	// Retrieve bone positions
	for (int i = 0; i < 17; ++i) { // Update the loop condition
		bonePositions[i] = Fortnite->w2s(Fortnite->GetBoneLOC(mesh, boneIndices[i]));
	}

	// Define bone pairs, including connections from feet bones to other bones
	std::pair<int, int> bonePairs[] = {
		{1, 2}, {3, 4}, {4, 3}, {5, 3}, {6, 4}, {5, 7}, {6, 8},
		{10, 1}, {9, 1}, {12, 10}, {11, 9}, {13, 12}, {14, 11},
		{15, 13}, // Connect left foot (83) to lower left leg (75)
		{16, 14}  // Connect right foot (76) to lower right leg (82)
	};

	auto* drawList = ImGui::GetBackgroundDrawList();
	drawList->PushClipRectFullScreen();

	// Draw skeleton lines
	for (const auto& pair : bonePairs) {
		ImVec2 start(bonePositions[pair.first].x, bonePositions[pair.first].y);
		ImVec2 end(bonePositions[pair.second].x, bonePositions[pair.second].y);

		// Adjust neck bone length
		if (pair.first == bones::HumanHead) {
			end.y += 20;
		}

		drawList->AddLine(start, end, outlineColor, outlineThickness);
	}

	drawList->PopClipRect();
	}
}






template<typename T>
class TArray {
public:
	// Member functions
	int Num() const {
		return NumElements;
	}

	T& operator[](int Index) {
		return Data[Index];
	}

	const T& operator[](int Index) const {
		return Data[Index];
	}

	// Data members
	int NumElements = 0;
	T* Data = nullptr;
};


namespace loop {
	class fn	 {
	public:

	

		fvector2d head;
		fvector2d root;
		fvector2d pelvis;
		fvector2d left_shoulder;
		fvector2d left_elbow;
		fvector2d left_hand;
		fvector2d right_shoulder;
		fvector2d neck;

		fvector2d right_elbow;
		fvector2d right_hand;
		fvector2d left_knee;
		fvector2d left_foot;
		fvector2d right_knee;
		fvector2d right_foot;



		Vector3 relative_location;
		int speed;
		double gravity;
	
	
		auto cache_entities( ) -> void {


			

			temporary_entity_list.clear ( );

			sdk-> uworld = apdriver->read <uintptr_t> ( apdriver->image_base + offset::uworld );
			sdk-> game_instance = apdriver->read <uintptr_t> (sdk-> uworld + offset::game_instance );
			sdk-> local_player = apdriver->read <uintptr_t> (apdriver->read <uintptr_t>(sdk-> game_instance + offset::local_player ) );
			sdk-> player_controller = apdriver->read <uintptr_t> (sdk-> local_player + offset::player_controller );
			sdk-> acknowledged_pawn = apdriver->read <uintptr_t> (sdk-> player_controller + offset::acknowledged_pawn );
			sdk-> skeletal_mesh = apdriver->read <uintptr_t> (sdk-> acknowledged_pawn + offset::skeletal_mesh );
			sdk-> player_state = apdriver->read <uintptr_t> (sdk-> acknowledged_pawn + offset::player_state );
			sdk-> root_component = apdriver->read <uintptr_t> (sdk-> acknowledged_pawn + offset::root_component );
			sdk-> relative_location = apdriver->read <fvector> (sdk-> root_component + offset::relative_location );
			sdk-> team_index = apdriver->read <int> (sdk-> player_state + offset::team_index );



			bool inlobby;

			if (sdk->acknowledged_pawn == NULL)
			{
				inlobby = true;
			}
			else
			{
				inlobby = false;
			}


			if ( !sdk->acknowledged_pawn )
			{
				settings::in_lobby = true;
			}
			else
			{
				settings::in_lobby = false;
			}
			
	
			if (sdk->acknowledged_pawn) {

				if (settings::weapon_cfg)
				{
				
					uint64_t player_weapon = apdriver -> read <uint64_t>(sdk->acknowledged_pawn + 0xA20); // 	struct AFortWeapon* CurrentWeapon;

					if (is_valid(player_weapon)) {

						uint64_t weapon_data =  apdriver -> read <uint64_t>(player_weapon + 0x4D0);	//struct UFortWeaponItemDefinition* WeaponData;

						if (is_valid(weapon_data)) {

							uint64_t ftext_ptr = apdriver -> read <uint64_t>(weapon_data + 0x38);

							if (is_valid(ftext_ptr)) {
								uint64_t ftext_data = apdriver -> read <uint64_t>(ftext_ptr + 0x28);
								int ftext_length = apdriver -> read <int>(ftext_ptr + 0x30);
								if (ftext_length > 0 && ftext_length < 50) {
									wchar_t* ftext_buf = new wchar_t[ftext_length];
									apdriver -> read1 (ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));

									wchar_t* WeaponName = ftext_buf;

									delete[] ftext_buf;

									if (wcsstr(WeaponName, E(L"Assault Rifle")) || wcsstr(WeaponName, E(L"Havoc Suppressed Assault Rifle")) || wcsstr(WeaponName, E(L"Red-Eye Assault Rifle"))
										|| wcsstr(WeaponName, E(L"Suppressed Assault Rifle")) || wcsstr(WeaponName, E(L"Striker Burst Rifle")) || wcsstr(WeaponName, E(L"Burst Assault Rifle"))
										|| wcsstr(WeaponName, E(L"Ranger Assault Rifle")) || wcsstr(WeaponName, E(L"Flapjack Rifle")) || wcsstr(WeaponName, E(L"Heavy Assault Rifle"))
										|| wcsstr(WeaponName, E(L"MK-Seven Assault Rifle")) || wcsstr(WeaponName, E(L"MK-Alpha Assault Rifle")) || wcsstr(WeaponName, E(L"Combat Assault Rifle"))
										|| wcsstr(WeaponName, E(L"Nemesis AR")) || wcsstr(WeaponName, E(L"Ambush Striker AR")) || wcsstr(WeaponName, E(L"Striker AR"))
										|| wcsstr(WeaponName, E(L"Tactical Assault Rifle")) || wcsstr(WeaponName, E(L"Hammer Assault Rifle")) || wcsstr(WeaponName, E(L"Sideways Rifle")) || wcsstr(WeaponName, E(L"Makeshift Rifle")) || wcsstr(WeaponName, E(L"Drum Gun"))) {
										HeldWeaponType = EFortWeaponType::Rifle;
										settings::aimbot = rifle::aimbot;
										settings::visible_check = rifle::visible_check;
										settings::target_line = rifle::target_line;
										settings::isdead = rifle::ignore_downed;
										settings::aimfov = rifle::aim_fov;
										settings::smooth = rifle::smooth;
										settings::hitbox = rifle::hitbox;
										aimkey = rifle::aimkey;
										if (debuweapons)
										{
											std::cout << E("rifle") << std::endl;
										}
									}
									if (wcsstr(WeaponName, E(L"Shotgun")) || wcsstr(WeaponName, E(L"Frenzy Auto Shotgun")) || wcsstr(WeaponName, E(L"Iron Warrior Hammer Pump Shogtun")) || wcsstr(WeaponName, E(L"Peter Griffin's Pump Shotgun")) || wcsstr(WeaponName, E(L"PETER GRIFFIN'S PUMP SHOTGUN"))) {
										HeldWeaponType = EFortWeaponType::Shotgun;
										settings::aimbot = shotgun::aimbot;
										settings::visible_check = shotgun::visible_check;
										settings::target_line = shotgun::target_line;
										settings::isdead = rifle::ignore_downed;
										settings::aimfov = shotgun::aim_fov;
										settings::smooth = shotgun::smooth;
										settings::hitbox = shotgun::hitbox;
										aimkey = shotgun::aimkey;
										if (debuweapons)
										{
											std::cout << E("shogtun") << std::endl;
										}
									}
									if (wcsstr(WeaponName, E(L"Smg")) || wcsstr(WeaponName, E(L"Ambush Hyper SMG")) || wcsstr(WeaponName, E(L"Submachine Gun")) || wcsstr(WeaponName, E(L"Combat Smg")) || wcsstr(WeaponName, E(L"Pistol")) || wcsstr(WeaponName, E(L"Machine Smg"))
										|| wcsstr(WeaponName, E(L"Scoped Burst SMG")) || wcsstr(WeaponName, E(L"Hyper SMG")) || wcsstr(WeaponName, E(L"Thunger Burst SMG"))) {
										HeldWeaponType = EFortWeaponType::Smg;
										settings::aimbot = smg::aimbot;
										settings::visible_check = smg::visible_check;
										settings::target_line = smg::target_line;
										settings::isdead = rifle::ignore_downed;
										settings::aimfov = smg::aim_fov;
										settings::smooth = smg::smooth;
										settings::hitbox = smg::hitbox;
										aimkey = smg::aimkey;
										if (debuweapons)
										{
											std::cout << E("smg") << std::endl;
										}
									}
									if (wcsstr(WeaponName, E(L"Hunting Rifle")) || wcsstr(WeaponName, E(L"Heavy Sniper Rifle")) || wcsstr(WeaponName, E(L"Suppressed Sniper Rifle"))
										|| wcsstr(WeaponName, E(L"Storm Scout")) || wcsstr(WeaponName, E(L"Bolt-Action Sniper Rifle")) || wcsstr(WeaponName, E(L"Automatic Sniper Rifle"))
										|| wcsstr(WeaponName, E(L"DMR")) || wcsstr(WeaponName, E(L"Thermal DMR")) || wcsstr(WeaponName, E(L"Hunter Bolt-Action Sniper"))
										|| wcsstr(WeaponName, E(L"Reaper Sniper Rifle")) || wcsstr(WeaponName, E(L"Semi-Auto Sniper Rifle"))
										|| wcsstr(WeaponName, E(L"Crossbow")) || wcsstr(WeaponName, E(L"Mechanical Bow")) || wcsstr(WeaponName, E(L"Hand Cannon"))) {
										HeldWeaponType = EFortWeaponType::Sniper;
										settings::aimbot = sniper::aimbot;
										settings::visible_check = sniper::visible_check;
										settings::target_line = sniper::target_line;
										settings::isdead = rifle::ignore_downed;

										settings::aimfov = sniper::aim_fov;
										settings::smooth = sniper::smooth;
										settings::hitbox = sniper::hitbox;
										aimkey = sniper::aimkey;
										if (debuweapons)
										{
											std::cout << ("sniper") << std::endl;
										}
									}
								}
							}
						}
					}
				}
			}

			sdk-> game_state = apdriver->read <uintptr_t> (sdk-> uworld + offset::game_state );
			sdk-> player_array = apdriver->read <uintptr_t> (sdk-> game_state + offset::player_array );
			sdk-> player_array_size = apdriver->read <int> (sdk-> game_state + ( offset::player_array + sizeof ( uintptr_t ) ) );

			for ( int i = 0; i < sdk-> player_array_size; ++i ) {
				auto player_state = apdriver->read <uintptr_t> (sdk-> player_array + (i * sizeof ( uintptr_t ) ) );
				auto current_actor = apdriver->read <uintptr_t> ( player_state + offset::pawn_private );

				auto root_component = apdriver->read <uintptr_t> ( current_actor + offset::root_component );

				auto relative_location = apdriver->read <fvector> ( root_component + offset::relative_location) ;

				auto pawn_private = apdriver->read <uintptr_t>(player_state + 0x308);

				fvector2d screen_location;
				screen_location = Fortnite->w2s ( relative_location );
				if ( !Fortnite->in_screen ( screen_location ) ) continue;

				auto skeletalmesh = apdriver->read <uintptr_t> ( current_actor + offset::skeletal_mesh );

				auto is_despawning = ( apdriver->read <char> ( current_actor + 0x93a) >> 3 );

				auto team_index = apdriver->read <int> ( player_state + offset::team_index );
				if (sdk-> team_index == team_index && sdk-> acknowledged_pawn ) continue;

				entity cached_actors{ };
				cached_actors.entity = current_actor;
				cached_actors.skeletal_mesh = skeletalmesh;
				cached_actors.root_component = root_component;
				cached_actors.relative_location = relative_location;
				cached_actors.player_state = player_state;
				cached_actors.team_index = team_index;

				temporary_entity_list.push_back( cached_actors );
			}

			entity_list.clear( );
			entity_list = temporary_entity_list;
			std::this_thread::sleep_for ( std::chrono::milliseconds (150) );

		}
		fvector2d prediction;
		fvector2d head_box;
		fvector2d root_box;
		fvector2d root_box1;

		// Assuming you have a function to check if the hotkey is currently pressed


		// Assuming you have defined your hotkey elsewhere
		// Replace VK_F1 with the appropriate key code for your hotkey
		const int hotkey = aimkey;

		// Initialize a flag to track whether the hotkey is being held
		bool isHotkeyHeld = false;

		// Main rendering loop


		auto actor_loop() -> void {
			ImGui::PushFont(GameFont);

			ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
			float target_dist = FLT_MAX;
			uintptr_t target_entity = 0;

			if (settings::fps) {
				char fps[64];
				sprintf_s(fps, sizeof(fps), "%.3f", ImGui::GetIO().Framerate);
				DrawString(15, 15, 15, ImColor(255, 255, 255), false, true, fps);
			}

			if (settings::Vsync) {
				char vsync[] = "Vsync Enabled";
				DrawString(15, 15, 35, ImColor(255, 255, 255), false, true, vsync);
			}

			if (settings::WaterMark) {
				char WaterMark[] = "Vox Private";
				DrawString(15, 15, 55, ImColor(255, 255, 255), false, true, WaterMark);
			}

			
			if (settings::render_fov) {
				ImVec2 center(screen_width / 2, screen_height / 2);

				// Set the color based on whether the hotkey is being held
				ImColor fovColor = isHotkeyHeld ? ImColor(255, 0, 0) : ImColor(255, 255, 255);

				// Draw the circle with the appropriate color
				ImGui::GetBackgroundDrawList()->AddCircle(center, settings::aimfov, fovColor, 64, 1);

				float dotRadius = 2.0f;
				ImGui::GetBackgroundDrawList()->AddCircleFilled(center, dotRadius, ImColor(255, 255, 255)); // Assuming white color for the dot
			}



			const float centerWidth = screen_width / 2;
			const float centerHeight = screen_height / 2;

			for (auto& cached : entity_list) {
				auto root_bone = Fortnite->GetBoneLOC(cached.skeletal_mesh, bone::Root);
				auto head_bone = Fortnite->GetBoneLOC(cached.skeletal_mesh, bone::Head);
				auto pawn_private1 = apdriver->read<uintptr_t>(cached.player_state + 0x308);

				// Compute world-space coordinates of relevant points only once
				auto root = Fortnite->w2s(root_bone);
				auto head = Fortnite->w2s(head_bone);
				auto root_box = Fortnite->w2s(fvector(root_bone.x, root_bone.y, root_bone.z + 30));
				auto root_box1 = Fortnite->w2s(fvector(root_bone.x, root_bone.y, root_bone.z - 15));
				auto head_box = Fortnite->w2s(fvector(head_bone.x, head_bone.y, head_bone.z + 18));

				// Compute box dimensions and distance
				float box_height = abs(head.y - root_box1.y);
				float box_width = box_height * 0.41f;
				float distance = camera_postion.location.distance(root_bone) / 100;

				distanceok = camera_postion.location.distance(root_bone) / 100;
		
				if (settings::isdead) {
					bool bIsDyingasd = (apdriver->read<BYTE>(pawn_private1 + 0x758) & 0x10);
					bool isDBNO = (apdriver->read<BYTE>(pawn_private1 + 0x93A) >> 4) & 1; // Checking DBNO status

					if (isDBNO || bIsDyingasd) {
						// Change the color to red
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

						// Render your visuals here with the modified color

						// After rendering, restore the default color
						ImGui::PopStyleColor();
					}
				}



				if (distance > settings::render_distance && sdk->acknowledged_pawn) continue;

				if (settings::aimbot) {

					auto dx = head.x - (screen_width / 2);
					auto dy = head.y - (screen_height / 2);
					auto dist = sqrtf(dx * dx + dy * dy);

					if (settings::visible_check) {
						if (Fortnite->is_visible(cached.skeletal_mesh)) {
							if (dist < settings::aimfov && dist < target_dist) {

								target_dist = dist;
								target_entity = cached.entity;
							}
						}
					}
					else {
						if (dist < settings::aimfov && dist < target_dist) {


							target_dist = dist;
							target_entity = cached.entity;
						}
					}
				}

				bool inlobby;

				if (sdk->acknowledged_pawn == NULL)
				{
					inlobby = true;
				}
				else
				{
					inlobby = false;
				}

				ImColor vis;
				ImColor invis;
				ImColor normal;

				vis = ImVec4(settings::color_Visible[0], settings::color_Visible[1], settings::color_Visible[2], 1.0f);
				invis = ImVec4(settings::color_invisible[0], settings::color_invisible[1], settings::color_invisible[2], 1.0f);
				normal = ImVec4(settings::color[0], settings::color[1], settings::color[2], 1.0f);

				ImColor visibleColor;
				if (settings::visible_check)
				{
					visibleColor = Fortnite->is_visible(cached.skeletal_mesh) ? ImColor(vis) : ImColor(invis);
				}
				else
				{
					visibleColor = ImColor(normal);
				}

				if (settings::line)
				{
					if (settings::line_type == 0) {
						draw_list->AddLine(ImVec2(screen_width / 2, 0), ImVec2(head_box.x, head_box.y), visibleColor, settings::line_thick);
					}

					if (settings::line_type == 1) {
						draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(head_box.x, head_box.y), visibleColor, settings::line_thick);
					}

					if (settings::line_type == 2) {
						draw_list->AddLine(ImVec2(screen_width / 2, screen_height - 5), ImVec2(head_box.x, head_box.y), visibleColor, settings::line_thick);
					}
				}

				if (settings::box_esp && settings::enable_esp) {
	
					if (Fortnite->is_visible(cached.skeletal_mesh))
					{
						if (settings::NormalBox) {
							const float halfWidth = box_width / 2.0f;
							const ImVec2 topLeft(head_box.x - halfWidth, head_box.y);
							const ImVec2 topRight(root.x + halfWidth, head_box.y);
							const ImVec2 bottomLeft(head_box.x - halfWidth, root.y);
							const ImVec2 bottomRight(root.x + halfWidth, root.y);


							if (settings::outline) {
								const ImColor outlineColor(0, 0, 0, 100); // Slightly transparent black outline
								const int outlineThickness = settings::box_thick + 2;

								draw_list->AddRect(topLeft, bottomRight, outlineColor, 0.0f, ImDrawCornerFlags_All, outlineThickness);
							}

							// Draw the main box with anti-aliasing and blending
							const ImColor boxColor(0, 255, 0, 65); // Green box color with slight transparency
							const int boxThickness = settings::box_thick;

							draw_list->AddRectFilled(topLeft, bottomRight, boxColor, 0.0f, ImDrawCornerFlags_All);
						}
						else
						{
							if (!Fortnite->is_visible(cached.skeletal_mesh))
							{
								const float halfWidth = box_width / 2.0f;
								const ImVec2 topLeft(head_box.x - halfWidth, head_box.y);
								const ImVec2 topRight(root.x + halfWidth, head_box.y);
								const ImVec2 bottomLeft(head_box.x - halfWidth, root.y);
								const ImVec2 bottomRight(root.x + halfWidth, root.y);

								// Draw outline if enabled
								if (settings::outline) {
									const ImColor outlineColor(0, 0, 0, 100); // Slightly transparent black outline
									const int outlineThickness = settings::box_thick + 2;

									draw_list->AddRect(topLeft, bottomRight, outlineColor, 0.0f, ImDrawCornerFlags_All, outlineThickness);
								}

								// Draw the main box with anti-aliasing and blending
								const ImColor boxColor(255, 0, 0, 65); // Red box color with slight transparency

								const int boxThickness = settings::box_thick;

								draw_list->AddRectFilled(topLeft, bottomRight, boxColor, 0.0f, ImDrawCornerFlags_All);
							}

						}
					}
					if (settings::debugbone)
					{
						if (!sdk->acknowledged_pawn)
						{
							ImVec2 testPositions[200];

							for (int i = 0; i < 200; ++i) {
								auto test = Fortnite->w2s(Fortnite->GetBoneLOC(cached.skeletal_mesh, i + 1));
								testPositions[i] = ImVec2(test.x, test.y);
								ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 10.0f, testPositions[i], IM_COL32_WHITE, std::to_string(i + 1).c_str());
							}
						}
					}
					else if (settings::RoundBox) {
						const ImVec2 boxTopLeft(head_box.x - box_width / 2, head_box.y);
						const ImVec2 boxBottomRight(head_box.x - box_width / 2 + box_width, head_box.y + box_height);

			
						if (settings::outline) {
							const ImColor outlineColor(0, 0, 0);
							const float cornerRadius = 10.0f;
							const int outlineThickness = settings::box_thick + 2;

							draw_list->AddRect(boxTopLeft, boxBottomRight, outlineColor, cornerRadius, ImDrawCornerFlags_All, outlineThickness);
						}

						const ImColor boxColor = visibleColor;
						const int boxThickness = settings::box_thick;

						draw_list->AddRect(boxTopLeft, boxBottomRight, boxColor, 10.0f, ImDrawCornerFlags_All, boxThickness);
					}

					else if (settings::CornerBox) {
						
						if (settings::outline) {
							DrawCorneredBox(root.x - (box_width / 2), head_box.y, box_width, box_height, ImColor(0, 0, 0), settings::box_thick + 2.0, 0, 0);
						}
						DrawCorneredBox(root.x - (box_width / 2), head_box.y, box_width, box_height, visibleColor, settings::box_thick, 0, 0);
					}

				}

				if (settings::FilledBox) {
					ImVec4 fillColor = ImVec4(0.0f, 1.0f, 0.0f, 0.3f); // Green color with 30% opacity
					draw_filled_rect(root.x - (box_width / 2), head_box.y, box_width, box_height, fillColor);
				}

				if (settings::weapon_esp && settings::enable_esp && sdk->acknowledged_pawn) {
					WeaponInformation held_weapon{};

					uint64_t player_weapon = apdriver->read<uint64_t>(cached.entity + 0xa20);
					if (is_valid(player_weapon)) {
						held_weapon.ammo_count = apdriver->read<int32_t>(player_weapon + 0xE1C);

						uint64_t weapon_data = apdriver->read<uint64_t>(player_weapon + 0x4D0);
						if (is_valid(weapon_data)) {
							held_weapon.tier = apdriver->read<BYTE>(weapon_data + 0x12B);

							uint64_t ftext_ptr = apdriver->read<uint64_t>(weapon_data + 0x38);
							if (is_valid(ftext_ptr)) {
								uint64_t ftext_data = apdriver->read<uint64_t>(ftext_ptr + 0x28);
								int ftext_length = apdriver->read<int>(ftext_ptr + 0x30);
								if (ftext_length > 0 && ftext_length < 50) {
									wchar_t* ftext_buf = new wchar_t[ftext_length];
									apdriver->read1(ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
									std::wstring wstr_buf(ftext_buf);
									held_weapon.weapon_name = std::string(wstr_buf.begin(), wstr_buf.end());
									delete[] ftext_buf;
								}
							}
						}

						WeaponInfo = held_weapon;

						std::string final = ("") + WeaponInfo.weapon_name + ("");

						ImVec2 text_size = ImGui::CalcTextSize(final.c_str());
						int add = settings::distance ? 25 : 5;

						ImColor textColor;
						if (settings::weapon_rarity) {
							switch (WeaponInfo.tier) {
							case 0: textColor = strstr(WeaponInfo.weapon_name.c_str(), "Pickaxe") != nullptr ? ImColor(255, 255, 255) : ImColor(255, 255, 255); break; // Modify color as needed
							case 1: textColor = ImColor(170, 165, 169); break;
							case 2: textColor = ImColor(30, 255, 0); break;
							case 3: textColor = ImColor(0, 112, 221); break;
							case 4: textColor = ImColor(163, 53, 238); break;
							case 5: textColor = ImColor(255, 128, 0); break;
							case 6: textColor = ImColor(255, 255, 0); break;
							}
						}
						else {
							textColor = WeaponInfo.tier == 0 ? ImColor(255, 255, 255) : visibleColor;
						}

						DrawString(settings::fovsize, root.x - (text_size.x / 2), root.y + add, textColor, false, true, final.c_str());
					}
				}


				if (settings::username && settings::enable_esp) {
					std::string username_str = Fortnite->get_player_name(cached.player_state, inlobby);
					if (!username_str.empty()) {
						ImVec2 text_size = ImGui::CalcTextSize(username_str.c_str());
						DrawString(settings::fovsize, head_box.x - (text_size.x / 2), head_box.y - 20, ImColor(255, 255, 255), false, true, username_str.c_str());
					}
				}

				if (settings::wireframe) {
					auto PawnMaterials = apdriver->read<TArray<uintptr_t>>(cached.player_state + 0x5C40);
					for (int i = 0; i < PawnMaterials.Num(); i++) { // Use Num() instead of Size() for TArray
						auto MaterialInstance = PawnMaterials[i];
						auto Material = apdriver->read<uintptr_t>(MaterialInstance + 0x18);

						// Check if Material is valid before attempting to modify
						if (Material != NULL) {
							// Update the material to enable wireframe rendering
							//apdriver->write<int>(Material + 0x1b8, 6); // Wireframe is the 6th index
						}
					}
				}



				if (settings::distance && settings::enable_esp) {
					std::string distance_str = "[ " + std::to_string(static_cast<int>(distance)) + ".0 ]";
					ImVec2 text_size = ImGui::CalcTextSize(distance_str.c_str());

					DrawString(settings::fovsize, root.x - (text_size.x / 2), root.y + 5, ImColor(255, 255, 255), false, true, distance_str.c_str());
				}


				if (Fortnite->is_visible(cached.skeletal_mesh))
				{
					if (settings::skeleton && settings::enable_esp && settings::visible_check && distance <= 100) {
						optimizedskeleton(cached.skeletal_mesh);
					}
				}
	
			}

			if (GetAsyncKeyState(VK_CAPITAL) & 0x8000)
			{
				if (settings::triggerbot)
				{
					auto closest_mesh = apdriver->read<std::uint64_t>(target_entity + offset::skeletal_mesh);
					fvector hitbox;
					switch (settings::hitbox) {
					case(0):
						hitbox = Fortnite->GetBoneLOC(closest_mesh, bone::Head);
						break;
					case(1):
						hitbox = Fortnite->GetBoneLOC(closest_mesh, bone::Neck);
						break;
					case(2):
						hitbox = Fortnite->GetBoneLOC(closest_mesh, bone::Chest);
						break;
					case(3):
						hitbox = Fortnite->GetBoneLOC(closest_mesh, bone::Pelvis);
						break;
					}

					bool isCrosshairOverTarget = is_crosshair_over_target(apdriver->read<fvector>(sdk->player_controller + offset::location_under_reticle), hitbox);

					if (isCrosshairOverTarget) {
						std::mt19937 rng(std::random_device{}());
						std::uniform_int_distribution<int> delayDistribution(4, 50);
						int delay = delayDistribution(rng);
						std::this_thread::sleep_for(std::chrono::milliseconds(delay));

						mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
					}
				}
			}


			

			if (target_entity && settings::aimbot)
			{
				auto closest_mesh = apdriver->read<uint64_t>(target_entity + offset::skeletal_mesh);
				auto root = apdriver->read<uintptr_t>(target_entity + offset::root_component);

				fvector Velocity = apdriver->read<fvector>(root + offset::velocity);

				fvector hitbox;
				float projectileSpeed = 60000.f;//0;
				float projectileGravityScale = 3.5f;	//0;
				// Check if closest_mesh is valid before proceeding
				if (closest_mesh) {
					// Declare hitbox and hitbox_screen outside the switch case
					fvector hitbox;
					fvector2d hitbox_screen;

					fvector Velocity = apdriver->read<fvector>(root + 0x168);

					// Switch case to determine hitbox
					switch (settings::hitbox) {
					case 0:
						hitbox = Fortnite->GetBoneLOC(closest_mesh, bone::Head);
						break;
					case 1:
						hitbox = Fortnite->GetBoneLOC(closest_mesh, bone::Neck);
						break;
					case 2:
						hitbox = Fortnite->GetBoneLOC(closest_mesh, bone::Chest);
						break;
					case 3:
						hitbox = Fortnite->GetBoneLOC(closest_mesh, bone::Pelvis);
						break;
					default:
			
						break;
					}
			
					float projectileSpeed = 60000.f;
					float projectileGravityScale = 3.5f;
					if (!settings::Prediction)
					{
						if (strstr(WeaponInfo.weapon_name.c_str(), "Sniper") || strstr(WeaponInfo.weapon_name.c_str(), "Rifle"))
						{
							settings::Prediction = true;
							if (strstr(WeaponInfo.weapon_name.c_str(), "Dragon's Breath Sniper") || strstr(WeaponInfo.weapon_name.c_str(), "Storm Scout") ||
								strstr(WeaponInfo.weapon_name.c_str(), "Storm Scout Sniper Rifle") || strstr(WeaponInfo.weapon_name.c_str(), "Hunting Rifle") ||
								strstr(WeaponInfo.weapon_name.c_str(), "Explosive Repeater Rifle") || strstr(WeaponInfo.weapon_name.c_str(), "Bolt-Action Sniper Rifle") ||
								strstr(WeaponInfo.weapon_name.c_str(), "Suppressed Sniper Rifle") || strstr(WeaponInfo.weapon_name.c_str(), "Lever Action Rifle") ||
								strstr(WeaponInfo.weapon_name.c_str(), "Boom Sniper Rifle")) {

								projectileSpeed = 65000.f;
								projectileGravityScale = 2.5f;
							}
							else if (strstr(WeaponInfo.weapon_name.c_str(), "Reaper Sniper Rifle"))
							{
								projectileSpeed = 62000.f;
								projectileGravityScale = 4.0f;
							}
						}
						else
						{
							settings::Prediction = false;
						}
					}


					auto Distance = camera_postion.location.distance(hitbox);

					if (projectileSpeed)
					{
						hitbox = Fortnite->target_prediction(hitbox, Velocity, projectileSpeed, projectileGravityScale, Distance);  //1
					}
					hitbox_screen = Fortnite->w2s(hitbox);

				
					if (hitbox_screen.x != -1 && hitbox_screen.y != -1 &&
						get_cross_distance(hitbox_screen.x, hitbox_screen.y, screen_width / 2, screen_height / 2) <= settings::aimfov) {
				
						if (settings::target_line)
							draw_list->AddLine(ImVec2(centerWidth, centerHeight), ImVec2(hitbox_screen.x, hitbox_screen.y), ImColor(0, 0, 0), settings::line_thick);

						if (GetAsyncKeyState_Spoofed(aimkey))
							input->move(hitbox_screen);
					}
				}
				else {
					
					target_dist = FLT_MAX;
					target_entity = NULL; 
				}
			}




			ImGui::PopFont();
		}

	};
} static loop::fn* g_main = new loop::fn();