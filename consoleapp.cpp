#include "everything.h"
#include "Proc.h"
#include "mem.h"
int main()
{



	uintptr_t modbase = 0, localplayer = 0, healthaddr = 0;
	bool bHealth = false, bAmmo = false, bRecoil = false;
	HANDLE hProcess = 0;

	const int everythang = 5555;
	// entering process.
	DWORD procid = GetProcId(L"ac_client.exe");
	if (procid)
	{

		modbase = GetModuleBaseAddress(procid, L"ac_client.exe");
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, procid);

		// local player stuff
		localplayer = modbase + 0x10f4f4;
		std::vector<unsigned int> localaddr{ 0x0 };
		uintptr_t localplayeraddr = FindDMAAddy(hProcess, localplayer, localaddr);

		// Health address
		healthaddr = localplayeraddr + 0xf8;




		std::cout << std::hex << "0x" << localplayer << std::endl;
		WriteProcessMemory(hProcess, (BYTE*)healthaddr, &everythang, sizeof(everythang), 0);

	}
	else {
		std::cout << "No process found. Exitting in 5 sec";
		Sleep(5000);
		return 0;
	}

	DWORD dwExit = 0;
	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bHealth = !bHealth;
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bAmmo = !bAmmo;
			if (bAmmo)
			{
				// xFF & x06 = inc[esi]
				// This is saying in english 
				// Hey! At this point of memory in our module
				// Change the memory of these 2 bytes to \xFF and \x06 , using our handle to the game
				mem::patchex((byte*)(modbase + 0x637e9), (BYTE*)"\xFF\x06", 2, hProcess);
			}

			// 0x637e9 is ORIGINALLY FF 0E 
			// We change it to FF 06 -> Increment (our ammo)
			// FF 0E Which -> Decrement esi (our ammo)
			else
			{
				// ff 0e = dec[esi]
				// 0e is the decrement instruction i would assume? While 06 is our increment instruction
				mem::patchex((byte*)(modbase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProcess);
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bRecoil = !bRecoil;

			if (bRecoil)
			{
				// Nopping the bytes at this module
				// We nop 10 bytes that control our recoil
				mem::nopex((byte*)(modbase + 0x63786), 10, hProcess);
			}
			else
			{
				// Setting back everything in memory set so it works like we never touched it
				// So, Our recoil is actually just x50 to x51 however if we remove 8b - d2 Untoggling it will crash the game
				// Will investigate tmrw
				mem::patchex((byte*)(modbase + 0x63786),(byte*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10 ,hProcess);
			}
		}
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			// Insert closes our game
			return 0;
		}



		// WRITE OVER AND OVER
		if (bHealth)
		{
			// Pretty much just getting out health address
			// Setting it to what we want it to be set to 
			// in the handle of the game
			mem::patchex((byte*)healthaddr, (byte*)&everythang, sizeof(everythang), hProcess);
		}
		Sleep(10);
	}

	std::cout << "Process not found, Press enter to exit\n";
	getchar();
	return 0;
}
