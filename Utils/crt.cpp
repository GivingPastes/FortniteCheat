#pragma once
#include <iostream>
#include <Windows.h>
#include <intrin.h>
#include <sysinfoapi.h>
#include "crt.h"

float crt::sqrtf(float _X) { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(_X))); }
float crt::expf(float _X) { return _mm_cvtss_f32(_mm_exp_ps(_mm_set_ss(_X))); }

float crt::sinf(float _X) { return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(_X))); }
float crt::cosf(float _X) { return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(_X))); }

float crt::tanf(float _X) { return _mm_cvtss_f32(_mm_tan_ps(_mm_set_ss(_X))); }
float crt::asinf(float _X) { return _mm_cvtss_f32(_mm_asin_ps(_mm_set_ss(_X))); }
float crt::atan2f(float _X, float _Y) { return _mm_cvtss_f32(_mm_atan2_ps(_mm_set_ss(_X), _mm_set_ss(_Y))); }
float crt::acosf(float _X) { return _mm_cvtss_f32(_mm_acos_ps(_mm_set_ss(_X))); }

int crt::toupper(int c)
{
	if (c >= 'a' && c <= 'z') return c - 'a' + 'A';
	return c;
}
int crt::tolower(int c)
{
	if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
	return c;
}

void* crt::memcpy(void* dest, const void* src, unsigned __int64 count)
{
	char* char_dest = (char*)dest;
	char* char_src = (char*)src;
	if ((char_dest <= char_src) || (char_dest >= (char_src + count)))
	{
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest++;
			char_src++;
			count--;
		}
	}
	else
	{
		char_dest = (char*)dest + count - 1;
		char_src = (char*)src + count - 1;
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest--;
			char_src--;
			count--;
		}
	}
	return dest;
}
void* crt::memset(void* src, int val, unsigned __int64 count)
{
	__stosb((unsigned char*)((unsigned long long)(volatile char*)src), val, count);
	return src;
}
void* crt::memccpy(void* to, const void* from, int c, unsigned __int64 count)
{
	char t;
	unsigned __int64 i;
	char* dst = (char*)to;
	const char* src = (const char*)from;
	for (i = 0; i < count; i++)
	{
		dst[i] = t = src[i];
		if (t == 0) break;
		if (t == c) return &dst[i + 1];
	}
	return 0;
}
void* crt::memchr(const void* s, int c, unsigned __int64 n)
{
	if (n)
	{
		const char* p = (const char*)s;
		do
		{
			if (*p++ == c) return (void*)(p - 1);
		} while (--n != 0);
	}
	return 0;
}
int crt::memcmp(const void* s1, const void* s2, unsigned __int64 n)
{
	if (n != 0)
	{
		const unsigned char* p1 = (unsigned char*)s1, * p2 = (unsigned char*)s2;
		do
		{
			if (*p1++ != *p2++) return (*--p1 - *--p2);
		} while (--n != 0);
	}
	return 0;
}
int crt::memicmp(const void* s1, const void* s2, unsigned __int64 n)
{
	if (n != 0)
	{
		const unsigned char* p1 = (unsigned char*)s1, * p2 = (unsigned char*)s2;
		do
		{
			if (crt::toupper(*p1) != crt::toupper(*p2)) return (*p1 - *p2);
			p1++;
			p2++;
		} while (--n != 0);
	}
	return 0;
}
void* crt::memmove(void* dest, const void* src, unsigned __int64 count)
{
	char* char_dest = (char*)dest;
	char* char_src = (char*)src;
	if ((char_dest <= char_src) || (char_dest >= (char_src + count)))
	{
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest++;
			char_src++;
			count--;
		}
	}
	else
	{
		char_dest = (char*)dest + count - 1;
		char_src = (char*)src + count - 1;
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest--;
			char_src--;
			count--;
		}
	}
	return dest;
}

char* crt::_cslwr(char* x)
{
	char* y = x;
	while (*y)
	{
		*y = crt::tolower(*y);
		y++;
	}
	return x;
}
char* crt::_csupr(char* x)
{
	char* y = x;
	while (*y)
	{
		*y = crt::tolower(*y);
		y++;
	}
	return x;
}

int crt::strlen(const char* string)
{
	int cnt = 0;
	if (string)
	{
		for (; *string != 0; ++string) ++cnt;
	}
	return cnt;
}

int crt::wcslen(const wchar_t* string)
{
	int cnt = 0;
	if (string)
	{
		for (; *string != 0; ++string) ++cnt;
	}
	return cnt;
}

const char* crt::strcpy(char* buffer, const char* string)
{
	char* ret = buffer;
	while (*string) *buffer++ = *string++;
	*buffer = 0;
	return ret;
}

int crt::strcmp(const char* cs, const char* ct)
{
	if (cs && ct)
	{
		while (*cs == *ct)
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return *cs - *ct;
	}
	return -1;
}
int crt::stricmp(const char* cs, const char* ct)
{
	if (cs && ct)
	{
		while (crt::tolower(*cs) == crt::tolower(*ct))
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return crt::tolower(*cs) - crt::tolower(*ct);
	}
	return -1;
}