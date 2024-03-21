#include "spoofer.h"
#include "SkCrypt.h"
#include "sdk.h"
#include "ImGui/imgui.h"
#include <d3d9.h>
#include "ImGui/imgui_impl_win32.h"
#include "driver.hpp"
#include "globals.h"
#include "imports.h"
ImFont* MenuFont;
ImFont* GameFont;
ImFont* IconsFont;

ImFont* FortniteFont;
ImFont* Verdana;
ImFont* Arial;

inline int center_x;
inline int center_y;

namespace LocalPtrs {
	inline uintptr_t PlayerCameraM = 0;
	inline uintptr_t Player = 0; //our local pawn (our player)
	inline uintptr_t LocalPlayerPawnPriv = 0; //our local pawn (our player)
	inline uintptr_t Mesh;
	inline uintptr_t PlayerState = 0;
	inline uintptr_t RootComponent = 0;
	inline uintptr_t LocalPlayers = 0;
	inline uintptr_t PlayerController = 0;
	inline int TeamID = 0; //uworld != gworld u little fuckers
	inline uintptr_t Gworld = 0; //uworld != gworld u little fuckers
	inline std::uintptr_t ulevel = 0;
	inline std::uint32_t level_count = 0;
	inline fvector relative_location;


}

class entity {
public:
	uintptr_t
		entity,
		skeletal_mesh,
		root_component,
		player_state;
	
	int
		team_index;
	fvector
		relative_location;
	float
		lastrendertime;
	bool
		is_visible;
};

std::vector<entity> entity_list;
std::vector<entity> temporary_entity_list;
namespace o {
class p
{
	public:

		uintptr_t
			uworld,
			game_instance,
			game_state,
			local_player,
			player_controller,
			acknowledged_pawn,
			skeletal_mesh,
			player_state,
			root_component,
			player_array;

		int
			team_index,
			player_array_size;
		fvector
			relative_location,
			velocity;
	};
} static o::p* sdk = new o::p();




