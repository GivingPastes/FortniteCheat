#pragma once
#include <Windows.h>
#include <iostream>

#include "windows.h"

#include "Utils/xor.h"
#include "Utils/lw_http.hpp"

namespace syscalls
{
	typedef struct _UNICODE_STRING {
		USHORT Length;
		USHORT MaximumLength;
		PWSTR Buffer;
	} UNICODE_STRING, * PUNICODE_STRING;

	typedef struct _LDR_DATA_TABLE_ENTRY {
		LIST_ENTRY InLoadOrderLinks;
		LIST_ENTRY InMemoryOrderModuleList;
		LIST_ENTRY InInitializationOrderModuleList;
		PVOID DllBase;
		PVOID EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
		ULONG Flags;
		USHORT LoadCount;
		USHORT TlsIndex;
		union {
			LIST_ENTRY HashLinks;
			struct
			{
				PVOID SectionPointer;
				ULONG CheckSum;
			};
		};
		union {
			ULONG TimeDateStamp;
			PVOID LoadedImports;
		};
		PVOID EntryPointActivationContext;
		PVOID PatchInformation;
	} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

	typedef struct _PEB_LDR_DATA {
		BYTE       Reserved1[8];
		PVOID      Reserved2[3];
		LIST_ENTRY InMemoryOrderModuleList;
	} PEB_LDR_DATA, * PPEB_LDR_DATA;

	namespace user
	{
		__forceinline uint64_t GetModuleBaseAddress(const wchar_t* Dll)
		{
			if (!Dll)
				return *(uint64_t*)(__readgsqword(0x60) + 0x10);

			PPEB_LDR_DATA Ldr = *(PPEB_LDR_DATA*)(__readgsqword(0x60) + 0x18);
			PLIST_ENTRY pHeadEntry = &Ldr->InMemoryOrderModuleList;
			PLIST_ENTRY pEntry = pHeadEntry->Flink;

			pEntry = pHeadEntry->Flink;

			while (pEntry != pHeadEntry)
			{
				PLDR_DATA_TABLE_ENTRY Entry = CONTAINING_RECORD(pEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderModuleList);

				if (wcsstr(Entry->BaseDllName.Buffer, Dll))
					return (uint64_t)Entry->DllBase;

				pEntry = pEntry->Flink;
			}

			return (uint64_t)0;
		}

		__forceinline uint64_t GetExportedAddress(uint64_t BaseAddressOfModule, const char* Function)
		{
			auto GetExportDirectory = [&](uint64_t ImageBase)
			{
				auto DosHeader = (PIMAGE_DOS_HEADER)ImageBase;
				auto NtHeader = (PIMAGE_NT_HEADERS64)(ImageBase + DosHeader->e_lfanew);

				return (PIMAGE_EXPORT_DIRECTORY)(ImageBase + NtHeader->OptionalHeader.DataDirectory[0].VirtualAddress);
			};

			auto Exports = GetExportDirectory(BaseAddressOfModule);

			auto ExportNames = (DWORD*)((DWORD*)(BaseAddressOfModule + Exports->AddressOfNames));
			auto ExportFunctionAddresses = (DWORD*)((DWORD*)(BaseAddressOfModule + Exports->AddressOfFunctions));

			for (int i = 0; i < Exports->NumberOfNames; i++)
			{
				if (!strcmp(Function, (char*)(BaseAddressOfModule + ExportNames[i])))
				{
					if (GetModuleBaseAddress(xorstr(L"win32u.dll")) == BaseAddressOfModule || GetModuleBaseAddress(xorstr(L"KERNELBASE.dll")) == BaseAddressOfModule)
						return (uint64_t)(BaseAddressOfModule + ExportFunctionAddresses[i]);
					else
						return (uint64_t)(BaseAddressOfModule + ExportFunctionAddresses[i + 1]);
				}
			}

			return (uint64_t)0;
		}
	}

	static auto GetSyscallIndex(std::string ModuleName, std::string SyscallFunctionName, void* Function) -> bool
	{
		auto ModuleBaseAddress = LI_FN(GetModuleHandleA).safe()(ModuleName.c_str());
		if (!ModuleBaseAddress)
			ModuleBaseAddress = LI_FN(LoadLibraryA)(ModuleName.c_str());
		if (!ModuleBaseAddress)
			return false;

		auto GetFunctionAddress = LI_FN(GetProcAddress).safe()(ModuleBaseAddress, SyscallFunctionName.c_str());
		if (!GetFunctionAddress)
			return false;

		auto SyscallIndex = *(DWORD*)((PBYTE)GetFunctionAddress + 4);

		*(DWORD*)((PBYTE)Function + 4) = SyscallIndex;

		return true;
	}

	static VOID RtlInitUnicodeString_(PUNICODE_STRING DestinationString, PCWSTR SourceString)
	{
		SIZE_T Size;
		CONST SIZE_T MaxSize = (USHRT_MAX & ~1) - sizeof(UNICODE_NULL); // an even number

		if (SourceString)
		{
			Size = wcslen(SourceString) * sizeof(WCHAR);
			__analysis_assume(Size <= MaxSize);

			if (Size > MaxSize)
				Size = MaxSize;
			DestinationString->Length = (USHORT)Size;
			DestinationString->MaximumLength = (USHORT)Size + sizeof(UNICODE_NULL);
		}
		else
		{
			DestinationString->Length = 0;
			DestinationString->MaximumLength = 0;
		}

		DestinationString->Buffer = (PWCHAR)SourceString;
	}


	extern "C"
	{
		NTSTATUS _NtUserSendInput(UINT a1, LPINPUT Input, int Size);
		SHORT _NtUserGetAsyncKeyState(int VKey);
	};

	static VOID mouse_event_(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo, bool autoshoot = true)
	{
		static bool doneonce;
		if (!doneonce)
		{
			if (!GetSyscallIndex(xorstr("win32u.dll"), xorstr("NtUserSendInput"), _NtUserSendInput))
				return;
			doneonce = true;
		}

		INPUT Input[3] = { 0 };
		Input[0].type = INPUT_MOUSE;
		Input[0].mi.dx = dx;
		Input[0].mi.dy = dy;
		Input[0].mi.mouseData = dwData;
		Input[0].mi.dwFlags = dwFlags;
		Input[0].mi.time = 0;
		Input[0].mi.dwExtraInfo = dwExtraInfo;

		if (autoshoot)
		{
			Input[1].type = INPUT_MOUSE;
			Input[2].type = INPUT_MOUSE;
			Input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			Input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
			_NtUserSendInput((UINT)3, (LPINPUT)&Input, (INT)sizeof(INPUT));
		}
		else
			_NtUserSendInput((UINT)1, (LPINPUT)&Input, (INT)sizeof(INPUT));
	}

	static void mousemove(float tarx, float tary, float X, float Y, int smooth, bool autoshoot)
	{
		float ScreenCenterX = (X / 2);
		float ScreenCenterY = (Y / 2);
		float TargetX = 0;
		float TargetY = 0;

		if (tarx != 0)
		{
			if (tarx > ScreenCenterX)
			{
				TargetX = -(ScreenCenterX - tarx);
				TargetX /= smooth;
				if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
			}

			if (tarx < ScreenCenterX)
			{
				TargetX = tarx - ScreenCenterX;
				TargetX /= smooth;
				if (TargetX + ScreenCenterX < 0) TargetX = 0;
			}
		}

		if (tary != 0)
		{
			if (tary > ScreenCenterY)
			{
				TargetY = -(ScreenCenterY - tary);
				TargetY /= smooth;
				if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
			}

			if (tary < ScreenCenterY)
			{
				TargetY = tary - ScreenCenterY;
				TargetY /= smooth;
				if (TargetY + ScreenCenterY < 0) TargetY = 0;
			}
		}
		mouse_event_((DWORD)MOUSEEVENTF_MOVE, (DWORD)TargetX, (DWORD)TargetY, (DWORD)0, (ULONG_PTR)0, autoshoot);
	}

	static SHORT NtGetAsyncKeyState(int key)
	{
		static bool doneonce;
		if (!doneonce)
		{
			if (!GetSyscallIndex(xorstr("win32u.dll"), xorstr("NtUserGetAsyncKeyState"), _NtUserGetAsyncKeyState))
				return 0;
			doneonce = true;
		}
		return _NtUserGetAsyncKeyState(key);
	}
}