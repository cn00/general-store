#pragma once
#ifdef _USRDLL
#   include "ccMacros.h"

class CC_DLL rc4
#else
class rc4
#endif
{
public:
	static void const rc4_init(unsigned char*key, unsigned long Len);
	static void const rc4_crypt(unsigned char*Data, unsigned long Len);
// private:
// 	static void const native_rc4_init(unsigned char *s, unsigned char *key, unsigned long Len);
// 	static void const native_rc4_crypt(unsigned char *s, unsigned char *Data, unsigned long Len
};
