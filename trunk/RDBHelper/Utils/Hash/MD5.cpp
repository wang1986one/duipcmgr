// MD5.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<math.h> 
#include<ctype.h> 

#include "MD5.h"
#include "Utils/StringUtil.h"

// �й��ඨ�����Ϣ������� MD5.h
CMD5::CMD5()
{ 
}

CMD5::~CMD5( void )
{
}

/* 
Digests a string and prints the result. 
*/ 
// CString CMD5::MD5( IN const char *szText)
// {
// 	char szMD5StringBuffer[33];
// 	memset(szMD5StringBuffer,0,33);
// 
// 	MD5_CTX context; 
// 	MD5Init( &context); 
// 
// 	MD5Update( &context, (unsigned char*)szText, strlen (szText) );
// 
// 	unsigned char digest[16]; 
// 	MD5Final( digest, &context ); 
// 	for (int i = 0; i < 16; i++) 
// 	{ 
// 		sprintf(&(szMD5StringBuffer[2*i]),"%02x",(unsigned char)digest[i]);
// 	}
// 
// 	return CString(szMD5StringBuffer);
// }


/* 
MD5 initialization. Begins an MD5 operation, writing a new context. 
*/ 
void CMD5::MD5Init( MD5_CTX *context ) 
{ 
	context->count[0] = context->count[1] = 0; 
	/* 
	Load magic initialization constants. 
	*/ 
	context->state[0] = 0x67452301; 
	context->state[1] = 0xefcdab89; 
	context->state[2] = 0x98badcfe; 
	context->state[3] = 0x10325476; 
} 

/* 
MD5 block update operation. Continues an MD5 message-digest 
operation, processing another message block, and updating the 
context. 
*/ 
void CMD5::MD5Update( 
					 MD5_CTX *context, /* context */ 
					 unsigned char *input, /* input block */ 
					 size_t inputLen /* length of input block */ 
					 ) 
{ 
	size_t i, index, partLen; 

	/* Compute number of bytes mod 64 */ 
	index = (size_t)((context->count[0] >> 3) & 0x3F); 

	/* Update number of bits */ 
	if ((context->count[0] += ((UINT4)inputLen << 3)) 
		< ((UINT4)inputLen << 3)) 
		context->count[1]++; 
	context->count[1] += ((UINT4)inputLen >> 29); 

	partLen = 64 - index; 

	/* Transform as many times as possible. */ 
	if (inputLen >= partLen) { 
		MD5_memcpy 
			((POINTER)&context->buffer[index], (POINTER)input, partLen); 
		MD5Transform (context->state, context->buffer); 

		for (i = partLen; i + 63 < inputLen; i += 64) 
			MD5Transform (context->state, &input[i]); 

		index = 0; 
	} 
	else 
		i = 0; 

	/* Buffer remaining input */ 
	MD5_memcpy 
		((POINTER)&context->buffer[index], (POINTER)&input[i], 
		inputLen-i); 
} 

/* 
MD5 finalization. Ends an MD5 message-digest operation, writing the 
the message digest and zeroizing the context. 
*/ 
void CMD5::MD5Final( 
					unsigned char digest[16], /* message digest */ 
					MD5_CTX *context /* context */ 
					) 
{ 
	unsigned char bits[8]; 
	size_t index, padLen; 

	/* Save number of bits */ 
	Encode (bits, context->count, 8); 

	/* Pad out to 56 mod 64. */ 
	index = (size_t)((context->count[0] >> 3) & 0x3f); 
	padLen = (index < 56) ? (56 - index) : (120 - index); 
	MD5Update (context, PADDING, padLen); 

	/* Append length (before padding) */ 
	MD5Update (context, bits, 8); 

	/* Store state in digest */ 
	Encode (digest, context->state, 16); 

	/* Zeroize sensitive information. */ 
	MD5_memset ((POINTER)context, 0, sizeof (*context)); 
} 

/* 
MD5 basic transformation. Transforms state based on block. 
*/ 
void CMD5::MD5Transform( 
						UINT4 state[4], 
						unsigned char block[64] 
) 
{ 
	UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16]; 

	Decode (x, block, 64); 

	/* Round 1 */ 
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */ 
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */ 
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */ 
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */ 
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */ 
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */ 
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */ 
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */ 
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */ 
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */ 
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */ 
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */ 
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */ 
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */ 
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */ 
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */ 

	/* Round 2 */ 
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */ 
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */ 
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */ 
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */ 
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */ 
	GG (d, a, b, c, x[10], S22, 0x2441453); /* 22 */ 
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */ 
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */ 
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */ 
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */ 
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */ 
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */ 
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */ 
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */ 
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */ 
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */ 

	/* Round 3 */ 
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */ 
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */ 
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */ 
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */ 
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */ 
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */ 
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */ 
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */ 
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */ 
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */ 
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */ 
	HH (b, c, d, a, x[ 6], S34, 0x4881d05); /* 44 */ 
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */ 
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */ 
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */ 
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */ 

	/* Round 4 */ 
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */ 
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */ 
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */ 
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */ 
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */ 
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */ 
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */ 
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */ 
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */ 
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */ 
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */ 
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */ 
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */ 
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */ 
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */ 
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */ 

	state[0] += a; 
	state[1] += b; 
	state[2] += c; 
	state[3] += d; 

	/* Zeroize sensitive information. */ 
	MD5_memset ((POINTER)x, 0, sizeof (x)); 
} 

/* 
Encodes input (UINT4) into output (unsigned char). 
Assumes len is a multiple of 4. 
*/ 
void CMD5::Encode( 
				  unsigned char *output, 
				  UINT4 *input, 
				  size_t len 
				  ) 
{ 
	size_t i, j; 

	for (i = 0, j = 0; j < len; i++, j += 4) { 
		output[j] = (unsigned char)(input[i] & 0xff); 
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff); 
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff); 
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff); 
	} 
} 

/* 
Decodes input (unsigned char) into output (UINT4). 
Assumes len is a multiple of 4. 
*/ 
void CMD5::Decode( 
				  UINT4 *output, 
				  unsigned char *input, 
				  size_t len 
				  ) 
{ 
	size_t i, j; 

	for (i = 0, j = 0; j < len; i++, j += 4) 
		output[i] = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) | 
		(((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24); 
} 

/* 
Note: Replace "for loop" with standard memcpy if possible. 
*/ 
void CMD5::MD5_memcpy( 
					  POINTER output, 
					  POINTER input, 
					  size_t len 
					  ) 
{ 
	size_t i; 

	for (i = 0; i < len; i++) 
		output[i] = input[i]; 
} 

/* 
Note: Replace "for loop" with standard memset if possible. 
*/ 
void CMD5::MD5_memset( 
					  POINTER output, 
					  int value, 
					  size_t len 
					  ) 
{ 
	size_t i; 

	for (i = 0; i < len; i++) 
		((char *)output)[i] = (char)value; 
} 



/* 
get the string add one. 
*/ 
void CMD5::StringAddOne( char * orstring ) 
{ 
	size_t len; 
	size_t i,n; 

	len = strlen(orstring); 
	n = len - 1; 
	for(i = n; i >= 0; i--) 
	{ 
		if(orstring[i]=='9') 
		{ 
			orstring[i] = 'A'; 
			break; 
		} 
		else if(orstring[i]=='Z') 
		{ 
			orstring[i]='a'; 
			break; 
		} 
		else if(orstring[i]=='z') 
		{ 
			orstring[i]='0'; 
			continue; 
		} 
		else 
			orstring[i] += 1; 
		break; 
	} 
}

CString CMD5::HashToString( CMemBuffer hashBuffer )
{
	char szMD5wstringBuffer[33];
	memset(szMD5wstringBuffer,0,33);
	for (int i = 0; i < 16; i++) 
	{ 
		sprintf(&(szMD5wstringBuffer[2*i]),"%02x",hashBuffer[i]);
	}

	return szMD5wstringBuffer;
}

CMemBuffer CMD5::GetBufferHash( CMemBuffer dataBuffer )
{
	CMD5 md5;
	return md5.toMD5Byte(dataBuffer.GetData(),dataBuffer.GetDataLength());
}

CMemBuffer CMD5::GetBufferHash( void* dataBuffer, unsigned nLength )
{
	CMD5 md5;
	return md5.toMD5Byte(dataBuffer, nLength);
}

CMemBuffer CMD5::GetStringHash( LPCSTR lpString )
{
	CMD5 md5;
	return md5.toMD5Byte(lpString);
}

CMemBuffer CMD5::GetStringHash( LPCWSTR lpString )
{
	CStringA str=Utils::UnicodeToMbcs(lpString);
	return GetBufferHash((LPVOID)str.GetString(),str.GetLength());
}

CMemBuffer CMD5::GetFileHash( LPCTSTR lpFileName, size_t nOffset /*= 0*/, size_t nLength /*= -1*/ )
{
	CMD5 md5;
	return md5.toMD5Byte(lpFileName, nOffset, nLength);
}

CMemBuffer CMD5::toMD5Byte( LPVOID dataBuffer, unsigned nLength )
{
	MD5_CTX context; 
	MD5Init( &context); 
	MD5Update( &context, (unsigned char*)dataBuffer, nLength);
	CMemBuffer digest;
	digest.SetDataLength(16);
	MD5Final((unsigned char*)digest.GetData(), &context ); 
	return digest;
}

CMemBuffer CMD5::toMD5Byte( LPCTSTR lpFileName, size_t nOffset, size_t nLength )
{
	size_t readed;
	size_t total_readed=0;
	MD5_CTX context; 
	MD5Init( &context); 
	FILE* fp = _tfopen(lpFileName, _T("rb"));
	if (fp)
	{
		unsigned char buf[4096];
		fseek(fp, nOffset, SEEK_SET);
		while((readed = fread(buf, 1, 4096, fp))>=0)
		{
			total_readed+=readed;
			if (readed==0)
				break;
			if (total_readed>nLength)
			{
				MD5Update( &context, buf, nLength&4095 );
				break;
			}
			MD5Update( &context, buf, readed );
		}
		fclose(fp);
		fp = NULL;
	}

	CMemBuffer digest;
	digest.SetDataLength(16);
	MD5Final((unsigned char*)digest.GetData(), &context ); 
	return digest;
}

CMemBuffer CMD5::toMD5Byte( LPCSTR lpString )
{
	MD5_CTX context; 
	MD5Init( &context); 
	MD5Update( &context, (unsigned char*)lpString, strlen(lpString));
	CMemBuffer digest;
	digest.SetDataLength(16);
	MD5Final((unsigned char*)digest.GetData(), &context ); 
	return digest;
}
