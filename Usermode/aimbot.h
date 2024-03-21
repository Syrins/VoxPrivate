namespace inp {
	class mouse
	{
	public:
		auto initialize() -> bool {
			auto win32u = LI_FN(LoadLibraryA).safe_cached()(skCrypt("win32u.dll"));
			if (!win32u) return false;

			void* NtUserInjectMouseInputAddress = LI_FN(GetProcAddress).safe_cached()(win32u, skCrypt("NtUserInjectMouseInput"));
			if (!NtUserInjectMouseInputAddress) {
				LI_FN(FreeLibrary).get()(win32u);
				return false;
			}

			_NtUserInjectMouseInput = reinterpret_cast<decltype(_NtUserInjectMouseInput)>(NtUserInjectMouseInputAddress);
			LI_FN(FreeLibrary).get()(win32u);
			return true;
		}

		auto set(int x, int y) -> bool {
			InjectedInputMouseInfo temp = {};
			temp.mouse_data = 0;
			temp.mouse_options = InjectedInputMouseOptions::move;
			temp.move_direction_x = x;
			temp.move_direction_y = y;
			temp.time_offset_in_miliseconds = 0;
			return _NtUserInjectMouseInput(&temp, 1);
		}

		inline void move(fvector2d Head2D) {
			float AimSpeed = settings::smooth;
			float Jitter = 0.01f; // random jitter factor
			float Acceleration = 2.0f; // acceleration factor

			fvector2d ScreenCenter = { static_cast<double>(screen_width) / 2, static_cast<double>(screen_height) / 2 };

			fvector2d Target = {
				(Head2D.x != 0) ? (Head2D.x - ScreenCenter.x) / AimSpeed : 0,
				(Head2D.y != 0) ? (Head2D.y - ScreenCenter.y) / AimSpeed : 0
			};

			Target.x = std::clamp(Target.x, -ScreenCenter.x, ScreenCenter.x);
			Target.y = std::clamp(Target.y, -ScreenCenter.y, ScreenCenter.y);

			// Add random jitter
			Target.x += static_cast<double>(rand() % (int)(Jitter * 1000)) / 1000.0 - Jitter / 2.0;
			Target.y += static_cast<double>(rand() % (int)(Jitter * 1000)) / 1000.0 - Jitter / 2.0;

			// Apply acceleration
			double Magnitude = sqrt(Target.x * Target.x + Target.y * Target.y);
			if (Magnitude > 1.0) {
				Target.x *= Acceleration / Magnitude;
				Target.y *= Acceleration / Magnitude;
			}

			if ((int)Target.x != 0 || (int)Target.y != 0)
				set(static_cast<int>(Target.x), static_cast<int>(Target.y));
		}


	};
}

static inp::mouse* input = new inp::mouse();
