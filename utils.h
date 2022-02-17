#pragma once

bool is_bad_ptr(uintptr_t* ptr) {
	if (!ptr || (uint64_t)ptr > 0xffffffffffff)
		return true;

	__try
	{
		volatile auto result = *ptr;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return true;
	}
	return false;
}

namespace utils {
	static BOOL IsValidPointer(uintptr_t address)
	{
		if (!is_bad_ptr((uintptr_t*)address))
			return TRUE;
		else 
			return FALSE;
	}

	static uintptr_t PatternScanEx(uintptr_t pModuleBaseAddress, const char* sSignature, size_t nSelectResultIndex = 0)
	{
		static auto patternToByte = [](const char* pattern)
		{
			auto       bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + crt::strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else
					bytes.push_back(strtoul(current, &current, 16));
			}
			return bytes;
		};

		const auto dosHeader = (PIMAGE_DOS_HEADER)pModuleBaseAddress;
		const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)pModuleBaseAddress + dosHeader->e_lfanew);

		const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto       patternBytes = patternToByte(sSignature);
		const auto scanBytes = reinterpret_cast<std::uint8_t*>(pModuleBaseAddress);

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		size_t nFoundResults = 0;

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;

			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}

			if (found)
			{
				if (nSelectResultIndex != 0)
				{
					if (nFoundResults < nSelectResultIndex)
					{
						nFoundResults++;
						found = false;
					}
					else
						return reinterpret_cast<uintptr_t>(&scanBytes[i]);
				}
				else
					return reinterpret_cast<uintptr_t>(&scanBytes[i]);
			}
		}

		return NULL;
	}

	static uintptr_t PatternScan(const char* sSignature, size_t nSelectResultIndex = 0)
	{
		static bool bIsSetted = false;

		static MODULEINFO info = { 0 };

		if (!bIsSetted)
		{
			SpoofThis(GetModuleInformation, (HANDLE)SpoofThis(GetCurrentProcess), (HMODULE)crt::GetCurrentImageBase(), &info, (DWORD)sizeof(info));
			bIsSetted = true;
		}
		return PatternScanEx((uintptr_t)info.lpBaseOfDll, sSignature, nSelectResultIndex);
	}
}