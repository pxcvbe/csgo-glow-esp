#pragma once
#include <TlHelp32.h>

// declare variable
uintptr_t moduleBase;
DWORD procID;
HWND hwnd;
HANDLE hProcess;

// offsets up2date from: https://github.com/frk1/hazedumper/blob/master/csgo.hpp (hazedumper)
namespace offsets
{
	// client.dll
	const uintptr_t dwLocalPlayer = 0xDEB99C;
	const uintptr_t dwGlowObjectManager = 0x535BAD0;
	const uintptr_t dwEntityList = 0x4E0102C;

	// entity object
	const uintptr_t m_iGlowIndex = 0x10488;
	const uintptr_t m_iTeamNum = 0xF4;
	const uintptr_t m_iHealth = 0x100;
	const uintptr_t m_bDormant = 0xED;
}

// utils
uintptr_t GetModuleBaseAddress(const char* modName) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!strcmp(modEntry.szModule, modName)) {
					CloseHandle(hSnap);
					return (uintptr_t)modEntry.modBaseAddr;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
}

// Simplified Write process memory
template<typename T> T RPM(SIZE_T address) {
	T buffer;
	ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}

// Simplified Read process memory
template<typename T> void WPM(SIZE_T address, T buffer) {
	WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), NULL);
}
