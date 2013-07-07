#include "stdafx.h"
#include "TEACrypt.h"

namespace Utils
{
	static void encrypt_msg(unsigned char *in, int inlen, unsigned long *key, unsigned char *out, unsigned long *outlen);
	static int decrypt_msg(unsigned char *in, int inlen, unsigned long *key, unsigned char *out, unsigned long *outlen);

	CMemBuffer CTEACrypt::Encrypt(const CMemBuffer data,const byte key[16])
	{
		return Encrypt(data,0,data.GetBufferLength(),key);
	}

	CMemBuffer CTEACrypt::Encrypt( const CMemBuffer data, int nOffset, int nLength, const byte key[16] )
	{
		// 计算数据加密后的大小
		unsigned long ulEncryptDataSize = (nLength + 10) % 8;
		if (ulEncryptDataSize) ulEncryptDataSize = 8- ulEncryptDataSize;
		ulEncryptDataSize += nLength + 10;

		CMemBuffer encryptData(ulEncryptDataSize);

		encrypt_msg((unsigned char*)data.GetData(),data.GetDataLength(),(unsigned long*)key,(unsigned char*)encryptData.GetData(),&ulEncryptDataSize);
		encryptData.SetDataLength(ulEncryptDataSize);

		return encryptData;
	}

	CMemBuffer CTEACrypt::Decrypt( const CMemBuffer data,const byte key[16] )
	{
		return Decrypt(data,0,data.GetBufferLength(),key);
	}

	CMemBuffer CTEACrypt::Decrypt( const CMemBuffer data, int nOffset, int nLength, const byte key[16] )
	{
		// 获得数据解密后的大小
		unsigned long ulDecryptDataSize = 0;
		decrypt_msg((unsigned char*)data.GetData()+nOffset, nLength, (unsigned long*)key, NULL, &ulDecryptDataSize);

		// 解密数据缓冲区
		CMemBuffer decryptData(ulDecryptDataSize);

		decrypt_msg((unsigned char*)data.GetData()+nOffset, nLength, (unsigned long*)key, (unsigned char*)decryptData.GetData(), &ulDecryptDataSize);
		decryptData.SetDataLength(ulDecryptDataSize);

		return decryptData;
	}


	static void encrypt_qword(unsigned long *, unsigned long *, unsigned long *);
	static void decrypt_qword(unsigned long *, unsigned long *, unsigned long *);

	static unsigned int swapu32(unsigned int n)
	{
		return(((n & 0xff000000) >> 24) | ((n & 0x000000ff) << 24) |
			((n & 0x00ff0000) >> 8) | ((n & 0x0000ff00) << 8));
	}

	/* use key to encrypt in buffer
	* key must be 16+ bytes long!
	*/
	static void encrypt_msg(unsigned char *in, int inlen, unsigned long *key, unsigned char *out, unsigned long *outlen)
	{
		register int m, i, j, count, p = 1;
		unsigned char q[12], *q1, *q2, *inp;
		unsigned char mkey[8];
		m = (inlen+10)%8;
		if (m)  m = 8-m;
		q[0] = (rand()&0xf8) | m;
		i = j = 1;
		while(m>0) {
			q[i++] = rand()&0xff;
			m --;
		}
		count = *outlen = 0;
		q2 = q1 = out;
		memset(mkey, 0, sizeof(mkey));
		while( p <= 2 ) {
			if (i < 8) {
				q[i++] = rand()&0xff;
				p ++;
			}
			if (i == 8) {
				for (i = 0; i < 8; i ++)
					q[i] ^= mkey[i];
				encrypt_qword((unsigned long *)q, key, (unsigned long *)out);
				for (i = 0; i < 8; i ++)
					q1[i] ^= mkey[i];
				q2 = q1;
				q1 += 8;
				count += 8;
				memcpy(mkey, q, 8);
				j = i = 0;
			}
		}
		inp = in;
		while (inlen > 0) {
			if (i < 8) {
				q[i] = inp[0];
				inp ++;
				i ++;
				inlen --;
			}
			if (i == 8) {
				for (i = 0; i < 8; i ++)  {
					if (j) q[i] ^= mkey[i];
					else q[i] ^= q2[i];
				}
				j = 0;
				encrypt_qword((unsigned long *)q, key, (unsigned long *)q1);
				for (i = 0; i < 8; i ++)
					q1[i] ^= mkey[i];
				count += 8;
				memcpy(mkey, q, 8);
				q2 = q1;
				q1 += 8;
				i = 0;
			}
		}
		p = 1;
		while (p < 8) {
			if (i < 8) {
				memset(q+i, 0, 4);
				p++;
				i++;
			}
			if (i == 8) {
				for (i = 0; i < 8; i ++)
					q[i] ^= q2[i];
				encrypt_qword((unsigned long *)q, key, (unsigned long *)q1);
				for (i = 0; i < 8; i ++)
					q1[i] ^= mkey[i];
				memcpy(mkey, q, 8);
				count += 8;
				q2 = q1;
				q1 += 8;
				i = 0;
			}
		}
		*outlen = count;
	}

	/* decrypt a encrypted string
	* key must be 16+ bytes long
	* return 0 if failed.
	* otherwise return 1.
	*/
	static int decrypt_msg(unsigned char *in, int inlen, unsigned long *key, unsigned char *out, unsigned long *outlen)
	{
		unsigned char q[8], mkey[8], *q1, *q2, *outp;
		register int count, i, j, p;
		if (inlen%8 || inlen<16) return 0;
		/* get basic information of the packet */
		decrypt_qword((unsigned long *)in, key, (unsigned long *)q);
		j = q[0]&0x7;
		count = inlen - j - 10;
		if ((int)*outlen < count || count < 0) return 0;
		*outlen = count;
		memset(mkey, 0, 8);
		q2 = mkey;
		i = 8; p = 1;
		q1 = in+8;
		j ++;
		while (p <= 2) {
			if (j < 8) {
				j ++;
				p ++;
			} else if (j == 8) {
				q2 = in;
				for (j = 0; j < 8; j ++ ) {
					if (i + j >= inlen) return 0;
					q[j] ^= q1[j];
				}
				decrypt_qword((unsigned long *)q, key, (unsigned long *) q);
				i += 8;
				q1 += 8;
				j = 0;
			}
		}
		outp = out;
		while(count !=0) {
			if (j < 8) {
				outp[0] = q2[j] ^ q[j];
				outp ++;
				count --;
				j ++;
			} else if (j == 8) {
				q2 = q1-8;
				for (j = 0; j < 8; j ++ ) {
					if (i + j >= inlen) return 0;
					q[j] ^= q1[j];
				}
				decrypt_qword((unsigned long *)q, key, (unsigned long *) q);
				i += 8;
				q1 += 8;
				j = 0;
			}
		}
		for (p = 1; p < 8; p ++) {
			if (j < 8) {
				if (q2[j]^q[j])
					return 0;
				j ++;
			} else if (j == 8 ) {
				q2 = q1;
				for (j = 0; j < 8; j ++ ) {
					if (i + j >= inlen) return 0;
					q[j] ^= q1[j];
				}
				decrypt_qword((unsigned long *)q, key, (unsigned long *) q);
				i += 8;
				q1 += 8;
				j = 0;
			}
		}
		return 1;
	}

	static void encrypt_qword(unsigned long *in, unsigned long *key, unsigned long *out)
	{
		unsigned long code[4];
		register unsigned long i = 16, j = 0, m, n;
		m = swapu32(in[0]);
		n = swapu32(in[1]);
		code[0] = swapu32(key[0]); code[1] = swapu32(key[1]);
		code[2] = swapu32(key[2]); code[3] = swapu32(key[3]);
		while(i-->0) {
			j -= 0x61c88647;
			m += (n>>5)+code[1] ^ (n<<4) +code[0] ^ j+n;
			n += (m>>5)+code[3] ^ (m<<4) +code[2] ^ j+m;
		}
		out[0] = swapu32(m);
		out[1] = swapu32(n);
	}

	static void decrypt_qword(unsigned long *in, unsigned long *key, unsigned long *out)
	{
		unsigned long code[4];
		register unsigned long i=16, j=0xe3779B90, m, n;
		m = swapu32(in[0]);
		n = swapu32(in[1]);
		code[0] = swapu32(key[0]); code[1] = swapu32(key[1]);
		code[2] = swapu32(key[2]); code[3] = swapu32(key[3]);
		while(i-- >0) {
			n -= ((m>>5)+code[3])^((m<<4)+code[2])^(j+m);
			m -= ((n>>5)+code[1])^((n<<4)+code[0])^(j+n);
			j += 0x61C88647;
		}
		out[0] = swapu32(m);
		out[1] = swapu32(n);
	}

	static void transform_msg(const unsigned char *in, int inlen, unsigned char *out, unsigned int *number)
	{
		unsigned char c;
		unsigned int i, j;
		for (i = 0, j = 0 ; j < (unsigned int)inlen ; j++) {
			if (i >= 0x3ff) break;
			c = in[j];
			if (c == 0) {
				out[i++] |= 0xff;
				out[i] = 0x30;
			} else if (c == 0xff) {
				out[i++] |= 0xff;
				out[i] |= 0xff;
			} else
				out[i] = c;
			i ++;
		}
		*number = i;
	}

	static void detransform_msg(const unsigned char *in, int inlen, unsigned char *out, unsigned int *number)
	{
		unsigned char c;
		unsigned int i, j;
		for ( i = 0,j = 0; j < (unsigned int)inlen; j ++) {
			if (i >= 0x3ff) break;
			c = in[j];
			if (c == 0xff) {
				if (in[j+1] == 0x30) {
					out[i++] = 0;
					j ++;
					continue;
				}
				if (in[j+1] == 0xff) {
					out[i++] = 0xff;
					j ++;
					continue;
				}
			}
			out[i++] = c;
		}
		out[i] = '\0';
		*number = i;
	}

}