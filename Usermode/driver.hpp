#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

#define code_rw CTL_CODE(FILE_DEVICE_UNKNOWN, 0x175, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_ba CTL_CODE(FILE_DEVICE_UNKNOWN, 0x113, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_get_guarded_region CTL_CODE(FILE_DEVICE_UNKNOWN, 0x254, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_security 0x86e3da6
#define code_reattach 0x659

typedef struct _rw {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct reattach {
	INT32 security;
} reattach, * rth;

typedef struct _ba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _ga {
	INT32 security;
	ULONGLONG* address;
} ga, * pga;

namespace a {
	class drv
	{
	public:

		HANDLE driver_handle;
		INT32 process_id;
		HANDLE process_id1;
		uintptr_t image_base;

		bool init() {
			driver_handle = CreateFileW(L"\\\\.\\{7e66-4cdd-ad13-de42e2dde530}", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			return driver_handle && (driver_handle != INVALID_HANDLE_VALUE);
		}
		void read_physical(PVOID address, PVOID buffer, DWORD size) {

			_rw arguments = { 0 };
			arguments.security = code_security;
			arguments.address = (ULONGLONG)address;
			arguments.buffer = (ULONGLONG)buffer;
			arguments.size = size;
			arguments.process_id = process_id;
			arguments.write = FALSE;

			DeviceIoControl(driver_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
		}

		void read_physical1(uint64_t address, void* buffer, uint32_t size) {

			_rw arguments = { 0 };
			arguments.security = code_security;
			arguments.address = (ULONGLONG)address;
			arguments.buffer = (ULONGLONG)buffer;
			arguments.size = size;
			arguments.process_id = process_id;
			arguments.write = FALSE;

			DeviceIoControl(driver_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
		}

		void re_attach()
		{
			reattach arguments = { 0 };

			DeviceIoControl(driver_handle, code_reattach, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
		}

		inline bool read1(const std::uintptr_t address, void* buffer, const std::size_t size)
		{
			if (buffer == nullptr || size == 0) {
				return false;
			}
			read_physical(reinterpret_cast<PVOID>(address), buffer, static_cast<DWORD>(size));
		}

		uintptr_t find_image() {
			uintptr_t image_address = { NULL };
			_ba arguments = { NULL };

			arguments.security = code_security;
			arguments.process_id = process_id;
			arguments.address = (ULONGLONG*)&image_address;

			DeviceIoControl(driver_handle, code_ba, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

			return image_address;
		}

		INT32 find_process(LPCTSTR process_name) {


			PROCESSENTRY32 pt;
			HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			pt.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hsnap, &pt)) {
				do {
					if (!lstrcmpi(pt.szExeFile, process_name)) {
						CloseHandle(hsnap);
						process_id = pt.th32ProcessID;
						return pt.th32ProcessID;
					}
				} while (Process32Next(hsnap, &pt));
			}
			CloseHandle(hsnap);

			return { NULL };
		}

		template <typename T>
		T read(uint64_t address) {
			T buffer{ };
			read_physical((PVOID)address, &buffer, sizeof(T));
			return buffer;
		}


		template<typename T>
		bool read_array(uintptr_t address, T out[], size_t len)
		{
			for (size_t i = 0; i < len; ++i)
			{
				out[i] = read<T>(address + i * sizeof(T));
			}
			return true; // you can add additional checks to verify successful reads
		}

	};
} std::unique_ptr<a::drv> apdriver = std::make_unique<a::drv>();

bool is_valid(const uint64_t address) {
	return address > 0x400000 && address != 0xCCCCCCCCCCCCCCCC && reinterpret_cast<void*>(address) != nullptr && address <= 0x7FFFFFFFFFFFFFFF;
}