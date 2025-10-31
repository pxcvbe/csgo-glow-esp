/*
* Simple [CSGO] Glow Esp External
* Inspired by: @Cyborg Elf from YT (https://www.youtube.com/watch?v=-FWjLMlD0Rs)
* Source Code by: Ivan Kurniawan
* Special thanks: @cazzywastaken & @pxcvbe
*/
#include <iostream>
#include <Windows.h>
#include "offsets.h"

DWORD getLocalPlayer() {
	return RPM<DWORD>(moduleBase + offsets::dwLocalPlayer);
}

DWORD isDormant(unsigned int Entity) {
	return RPM<DWORD>(Entity + offsets::m_bDormant);
}

int main()
{
	hwnd = FindWindowA(0, "Counter-Strike: Global Offensive - Direct3D 9");
	GetWindowThreadProcessId(hwnd, &procID);
	moduleBase = GetModuleBaseAddress("client.dll");
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);

	// loop and set panic keys
	while (!GetAsyncKeyState(VK_END))
	{
		uintptr_t dwGlowObjManager = RPM<uintptr_t>(moduleBase + offsets::dwGlowObjectManager);
		int localTeam = RPM<int>(getLocalPlayer() + offsets::m_iTeamNum);

		// loop through entity
		for (int i = 1; i < 64; i++)
		{
			uintptr_t dwEntity = RPM<uintptr_t>(moduleBase + offsets::dwEntityList + i * 0x10); // // tiap entity berikutnya didalam game, jaraknya 10! (contoh: tempatke1 adalah 0, tempatke2 adalah 10, tempatke2 adalah 20, dst)

			int iGlowIndex = RPM<int>(dwEntity + offsets::m_iGlowIndex);
			int enemyHealth = RPM<int>(dwEntity + offsets::m_iHealth);
			int entityTeam = RPM<int>(dwEntity + offsets::m_iTeamNum);
			isDormant(dwEntity + offsets::m_bDormant);

			// skip draw dead player & health > 100
			if (enemyHealth < 1 || enemyHealth > 100)
				continue;

			// skip bDormant
			if (isDormant(dwEntity))
				continue;

			if (localTeam == entityTeam) // buat warna team menjadi hijau (green)
			{
				WPM<float>(dwGlowObjManager + (iGlowIndex * 0x38) + 0x8, 0.f); // red
				WPM<float>(dwGlowObjManager + (iGlowIndex * 0x38) + 0xC, 1.f); // green
				WPM<float>(dwGlowObjManager + (iGlowIndex * 0x38) + 0x10, 0.f); // blue
				WPM<float>(dwGlowObjManager + (iGlowIndex * 0x38) + 0x14, 1.f); // alpha
			}
			else // buat warna musuh menjadi merah (red)
			{
				WPM<float>(dwGlowObjManager + (iGlowIndex * 0x38) + 0x8, 1.f); // red
				WPM<float>(dwGlowObjManager + (iGlowIndex * 0x38) + 0xC, 0.f); // green
				WPM<float>(dwGlowObjManager + (iGlowIndex * 0x38) + 0x10, 0.f); // blue
				WPM<float>(dwGlowObjManager + (iGlowIndex * 0x38) + 0x14, 1.f); // alpha
			}

			// buat glow menjadi 'true'
			WPM<bool>(dwGlowObjManager + (iGlowIndex * 0x38) + 0x27, true);
			WPM<bool>(dwGlowObjManager + (iGlowIndex * 0x38) + 0x28, true);
		}
	}

	return 0;

}
