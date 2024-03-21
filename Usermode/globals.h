#include <mutex>
#include <memory>
namespace settings
{
	inline bool aimbot = 1;

	inline bool Prediction = 1;

	inline bool lootesp = 0;

	inline bool visible_check = 1;

	inline bool line = 0;

	inline bool isdead = 1;

	inline bool w2c = 0;

	inline bool fps = 1;

	inline bool WaterMark = 1;

	inline bool RGB = 0;

	inline bool radar = 0;

	inline int line_type = 0;

	inline float headradius = 10;

	inline bool MemoryAIM = 0;
	inline bool BulletTP = 0;
	bool Vsync = true;
	inline bool random_aim_bone;

	inline bool wireframe = 0;
	
	inline inline bool textoutline = 0;

	float aimfov = 100;

	inline int hitbox = 0;
	const char* hitbox1[4] = { "Head","Neck", "Chest", "Pelvis", };

	inline inline bool target_line;

	inline inline bool skeleton = 1;
	inline bool debugbone = 0;
	inline bool box_esp = 1;

	inline static int triggerbot_current_key = VK_RBUTTON;
	inline float trigger_bot_distance = 250;
	inline bool SniperConfig = 0;
	inline bool RifleConfig = 0;
	inline bool SmgConfig = 0;
	inline bool ShotGunConfig = 0;

	inline bool enable_esp = 1;

	inline bool distance = 1;

	inline bool username = 1;

	inline bool head = 0;

	inline bool snapline = 0;

	inline bool weapon_cfg = 1;

	float fovsize = 14;
	static float fovcolor[3] = { 255.0f, 0.0f, 255.0f };
	static float SkeletonVisible[3] = { 255.0f, 0.0f, 255.0f };
	static float filledcolor[3] = { 255.0f, 0.0f, 255.0f };
	static float color_Visible[3] = { 255.0f, 0.0f, 255.0f };
	static float color_invisible[3] = { 173 / 255.0f, 17 / 0.0f, 17 / 0.0f };
	static float color[3] = { 1.00f, 1.00f, 1.00f };

	static int color_fov[3] = { 1.00f, 1.00f, 1.00f };

	float render_distance = 300;

	inline static bool Spinbot = false;

	inline bool weapon_esp = 1;

	inline bool platform = 0;

	bool NormalBox = false;
	bool CornerBox = false;
	bool RoundBox = false;
	bool FilledBox = false;
	inline int box_type = 0;

	inline bool outline = 1;


	inline int box_thick = 1;

	inline int line_thick = 2;

	inline int skeleton_thick = 1;

	inline float aim_max_dist = 275;

	float smooth = 0;

	inline bool render_fov = 1;

	inline bool weapon_rarity = true;

	inline bool render_crosshair = 0;

	inline bool render_menu = 0;

	inline bool shotguns_only = 1;

	inline bool ignore_keybind = 0;

	inline bool in_lobby;

	bool triggerbot = 0;
	float maximum_distance = 10;
	float custom_delay = 20;
	bool has_clicked;
	std::chrono::steady_clock::time_point tb_begin;
	std::chrono::steady_clock::time_point tb_end;
	int tb_time_since;
}

namespace rifle
{

	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool visible_check = 1;
	inline bool target_line = 0;
	inline float aim_fov = 15;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;
}

namespace shotgun
{

	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool visible_check = 1;
	inline bool target_line = 0;
	inline float aim_fov = 15;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;

}

namespace smg
{
	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool visible_check = 1;
	inline bool target_line = 0;
	inline 	float aim_fov = 15;
	inline 	float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;

}

namespace sniper
{
	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool visible_check = 1;
	inline bool target_line = 0;
	inline	float aim_fov = 15;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;
}