#include "everything.h"
#include "mem.h"


void mem::patchex(byte* dst, byte* src, unsigned int size, HANDLE hProcess)
{
	DWORD oldprotect;

	// Original memory protection settings
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	// Changing whatever we, at
	// the destination we want (dst)
	// with Our code (src)
	// With this many bytes (size)
	
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	// Restore the original memory protection settings
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);

}
// Hey, At this spot in code
// our byte array size
// Handle to our process
void mem::nopex(byte* dst, unsigned int size, HANDLE hProcess)
{
	// allows us to set our array size dynamically in our code (uses unsigned int size)
	BYTE* nopARRAY = new BYTE[size];
	
	// 0x90 is the instruction for no operation, with the size of our handle
	memset(nopARRAY, 0x90, size);

	// At this spot in the memory,
	// Write in memory there is no operation
	// For however many bytes we want
	// In our handle process.
	patchex(dst, nopARRAY, size, hProcess);
	//cleanup for memory leaks, deleting our allocated memory for our noparray
	delete[] nopARRAY;
}


/*
hProcess	The door/key to your house	Permission to enter and do stuff inside
dst			A specific room in your house	The exact memory address you want to access
src			The new furniture or paint	The data you want to write into that room
size		The amount of space you change	How many bytes (how big of an area) you want to overwrite
*/
