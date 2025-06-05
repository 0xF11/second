#include "Proc.h"

DWORD GetProcId(const wchar_t* ProcName)
{
	DWORD procID = 0; // Error checking
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// hsnap - Getting a snapshot of the process we are fucking.

	if (hSnap != INVALID_HANDLE_VALUE) // All this error checking or mitigation does is only runs our function if we get a snapshot.
	{

		PROCESSENTRY32 procEntry; // Process Info
		procEntry.dwSize = sizeof(procEntry);   // All the info is just gonna be the byte size it needs

		if (Process32First(hSnap, &procEntry)) // Grabs our first process in a our 32Snapshot, & stores it to proc entry
		{




			do
			{
				//   Cycles through all of the snapshots of processes till it finds a process matching the desired name

				if (!_wcsicmp(procEntry.szExeFile, ProcName))
				{
					// when found - we break out of our loop
					procID = procEntry.th32ProcessID;  
					// tells process id that this is the process id of the process name we gave
					break;
				}
			} while (Process32Next(hSnap, &procEntry)); // Looping through until we find the right process
		}
		CloseHandle(hSnap); // Closing our handle to prevent memory leaks (like assigning a pointer bytes in memory, we close it)
		return procID; //  gives us our processID 
	}
}


uintptr_t GetModuleBaseAddress(DWORD ProcID, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0; // error check
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcID); // Creating a snapshot of the module and 
	if (hSnap != INVALID_HANDLE_VALUE) // same as before
	{
		MODULEENTRY32 modEntry; // getting a list of all modules
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{

			do
			{
				if (!_wcsicmp(modEntry.szModule, modName)) // cycles through all modules till it matches our module parameters 
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));

		}
		CloseHandle(hSnap);
		return modBaseAddr; // returns our module address 

	}
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) // handle to our process, our localplayer ptr address, dereferencing offsets we find within localplayer
{
	uintptr_t addr = ptr; // Assigning our base address to a variable

	for (int i = 0; i < offsets.size(); i++) // I will loop through our offsets until completed
	{
		// BARNEY STYLE!
		// Reads the memory within the game (hProc)
		// Located at our address
		// References our address
		// Gets the size of our vector (how many offsets there are)
		// Not worried about the final one.
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);

		// This is dereferencing our multisystem pointer
		addr += offsets[i];
	}
	// Returns the dereferenced address
	return addr;

}
