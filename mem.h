#pragma once
#include "everything.h"
#include "Proc.h"

namespace mem
{
	void patchex(byte* dst, byte* src, unsigned int size, HANDLE hProcess);
	void nopex(byte* dst,unsigned int size, HANDLE hProcess);

}
