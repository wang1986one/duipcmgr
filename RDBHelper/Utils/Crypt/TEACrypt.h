#pragma once
#include "Utils/MemBuffer.h"

namespace Utils
{
	class CTEACrypt
	{
	public:
		static CMemBuffer Encrypt(const CMemBuffer data,const byte key[16]);
		static CMemBuffer Decrypt(const CMemBuffer data,const byte key[16]);
		static CMemBuffer Encrypt(const CMemBuffer data, int nOffset, int nLength, const byte key[16]);
		static CMemBuffer Decrypt(const CMemBuffer data, int nOffset, int nLength, const byte key[16]);
	};
}
