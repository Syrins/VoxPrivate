#include <string.h>
#include <list>
#include <string>
#include "ue.h"


namespace uee
{
	class ue 
	{
	public:

		auto get_player_name( uintptr_t player_state, bool inlobby ) -> std::string
		{

			int pNameLength;
			_WORD* pNameBufferPointer;
			int i;
			char v25;
			int v26;
			int v29;

			char16_t* pNameBuffer;

			uintptr_t pNameStructure = apdriver-> read < uintptr_t > ( player_state + 0xaf0); //pNameStructure
			if (is_valid(pNameStructure)) {
				pNameLength = apdriver -> read <int> ( pNameStructure + 0x10 );
				if ( pNameLength <= 0 )
					return "NGGA";

				pNameBuffer = new char16_t[pNameLength];
				uintptr_t pNameEncryptedBuffer = apdriver->read <uintptr_t>( pNameStructure + 0x8 );
				if (is_valid(pNameEncryptedBuffer)) {
					apdriver->read_array(pNameEncryptedBuffer, pNameBuffer, pNameLength);

					v25 = pNameLength - 1;
					v26 = 0;
					pNameBufferPointer = (_WORD*)pNameBuffer;

					for (i = (v25) & 3;; *pNameBufferPointer++ += i & 7) {
						v29 = pNameLength - 1;
						if (!(_DWORD)pNameLength)
							v29 = 0;

						if (v26 >= v29)
							break;

						i += 3;
						++v26;
					}

					std::u16string temp_wstring(pNameBuffer);
					delete[] pNameBuffer;
					return std::string(temp_wstring.begin(), temp_wstring.end());
				}
			}
		}


		auto in_screen(fvector2d screen_location) -> bool {

			if (screen_location.x > 0 && screen_location.x < screen_width && screen_location.y > 0 && screen_location.y < screen_height) return true;
			else return false;

		}

		camera_position_s get_camera()
		{

			camera_position_s camera;

			auto location_pointer = apdriver->read <uintptr_t>(sdk->uworld + 0x110);
			auto rotation_pointer = apdriver->read <uintptr_t>(sdk->uworld + 0x120);

			struct FNRot
			{
				double a; //0x0000
				char pad_0008[24]; //0x0008
				double b; //0x0020
				char pad_0028[424]; //0x0028
				double c; //0x01D0
			} fnRot;

			fnRot.a = apdriver->read <double>(rotation_pointer);
			fnRot.b = apdriver->read <double>(rotation_pointer + 0x20);
			fnRot.c = apdriver->read <double>(rotation_pointer + 0x1d0);

			camera.location = apdriver->read <fvector>(location_pointer);
			camera.rotation.x = asin(fnRot.c) * (180.0 / M_PI);
			camera.rotation.y = ((atan2(fnRot.a * -1, fnRot.b) * (180.0 / M_PI)) * -1) * -1;
			camera.fov = apdriver->read <float>((uintptr_t)sdk->player_controller + 0x394) * 90.f;

			return camera;
		}

		inline fvector2d w2s(fvector WorldLocation)
		{

			camera_postion = get_camera();

			if (WorldLocation.x == 0)
				return fvector2d(0, 0);

			_MATRIX tempMatrix = Matrix(camera_postion.rotation);

			fvector vAxisX = fvector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
			fvector vAxisY = fvector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
			fvector vAxisZ = fvector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

			fvector vDelta = WorldLocation - camera_postion.location;
			fvector vTransformed = fvector(vDelta.dot(vAxisY), vDelta.dot(vAxisZ), vDelta.dot(vAxisX));

			if (vTransformed.z < 1.f)
				vTransformed.z = 1.f;

			return fvector2d((screen_width / 2.0f) + vTransformed.x * (((screen_width / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z, (screen_height / 2.0f) - vTransformed.y * (((screen_width / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z);
		}

		fvector GetBoneLOC(uintptr_t skeletal_mesh, int index) const {
		
			if (!skeletal_mesh) return {};

	
			int is_cached = apdriver->read<int>(skeletal_mesh + 0x5E0);

	
			uintptr_t bone_array_ptr = apdriver->read<uintptr_t>(skeletal_mesh + 0x10 * is_cached + 0x598);

	
			FTransform bone_transform = apdriver->read<FTransform>(bone_array_ptr + 0x60 * index);

	
			FTransform ComponentToWorld = apdriver->read<FTransform>(skeletal_mesh + 0x1C0);

			D3DMATRIX bone_matrix = bone_transform.ToMatrixWithScale();
			D3DMATRIX world_matrix = ComponentToWorld.ToMatrixWithScale();

			float bone_x = bone_matrix._41 * world_matrix._11 + bone_matrix._42 * world_matrix._21 + bone_matrix._43 * world_matrix._31 + world_matrix._41;
			float bone_y = bone_matrix._41 * world_matrix._12 + bone_matrix._42 * world_matrix._22 + bone_matrix._43 * world_matrix._32 + world_matrix._42;
			float bone_z = bone_matrix._41 * world_matrix._13 + bone_matrix._42 * world_matrix._23 + bone_matrix._43 * world_matrix._33 + world_matrix._43;

			return fvector(bone_x, bone_y, bone_z);
		}

		fvector target_prediction(fvector target, fvector targetVelocity, float projectileSpeed, float projectileGravityScale, float distance)
		{
			float horizontalTime = distance / projectileSpeed;
			float verticalTime = distance / projectileSpeed;

			target.x += targetVelocity.x * horizontalTime;
			target.y += targetVelocity.y * horizontalTime;
			target.z += targetVelocity.z * verticalTime + abs(-980 * projectileGravityScale) * 0.5f * (verticalTime * verticalTime);

			return target;
		}
		static auto is_visible(uintptr_t skeletal_mesh) -> bool {
			// Check if the skeletal mesh pointer is valid
			if (!skeletal_mesh) return false;

			// Read last_submit and last_render values
			float last_submit, last_render;
			apdriver->read_array<float>(skeletal_mesh + 0x2E8, &last_submit, 1);
			apdriver->read_array<float>(skeletal_mesh + 0x2F0, &last_render, 1);

			// Check visibility condition
			return (last_render + 0.06f >= last_submit);
		}

			
		auto is_shootable(fvector lur, fvector bone) -> bool {
			SPOOF_FUNC

				constexpr float tolerance_x = 20.0f;
			constexpr float tolerance_y = 20.0f;
			constexpr float tolerance_z = 30.0f;

			return (std::abs(lur.x - bone.x) <= tolerance_x &&
				std::abs(lur.y - bone.y) <= tolerance_y &&
				std::abs(lur.z - bone.z) <= tolerance_z);
		}


	};  static uee::ue* Fortnite = new uee::ue();
}

int HeldWeaponType;

enum EFortWeaponType : int
{
	Rifle = 0,
	Shotgun = 1,
	Smg = 2,
	Sniper = 3
};

char* wchar_to_char(const wchar_t* pwchar)
{
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}