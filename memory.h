#pragma once
#include <string.h>  
#include <stdio.h>  
#include <locale.h>  
#include <tchar.h>
#include <vector>
#include <dwmapi.h>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <codecvt>
#include <sstream>
#include "include.h"
HANDLE process_handle;


struct module_t
{
	DWORD64 modBaseAddr, hModule, modBaseSize;
};

module_t base_address;
template <class t>
t read_mem(uintptr_t address) {
	t read;
	ReadProcessMemory(process_handle, (LPVOID)address, &read, sizeof(t), NULL);
	return read;
}

template <class T>
void write_mem(uintptr_t address, T value) {
	WriteProcessMemory(process_handle, (LPVOID)address, &value, sizeof(T), NULL);
}

module_t get_module_base64(uintptr_t pid, const char* module_name){
	module_t module_ = { 0, 0, 0 };
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (snapshot == INVALID_HANDLE_VALUE) {
		char str[0xff];
		sprintf_s(str, "Failed to get %s, invalid handle value", module_name);
		MessageBoxA(0, str, "ERROR", MB_OK | ERROR);
		return module_;
	}

	MODULEENTRY32 module_entry;
	module_entry.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(snapshot, &module_entry)) {
		do {
			if (_tcsicmp(module_entry.szModule, module_name) == 0) {
				module_ = { (DWORD64)module_entry.modBaseAddr, (DWORD64)module_entry.hModule, module_entry.modBaseSize };
				break;
			}
		} while (Module32Next(snapshot, &module_entry));
	}
	CloseHandle(snapshot);
	return module_;
}

int getProcID(const std::string& p_name){
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 structprocsnapshot = { 0 };
	structprocsnapshot.dwSize = sizeof(PROCESSENTRY32);
	if (snapshot == INVALID_HANDLE_VALUE)return 0;
	if (Process32First(snapshot, &structprocsnapshot) == FALSE)return 0;
	while (Process32Next(snapshot, &structprocsnapshot)){
		if (!strcmp(structprocsnapshot.szExeFile, p_name.c_str())){
			CloseHandle(snapshot);
			return structprocsnapshot.th32ProcessID;
		}
	}
	CloseHandle(snapshot);
	return 0;
}

bool isProcRunning(std::string process) {
	if (getProcID(process) == 0) {
		return false;
	}
	else
	{
		return true;
	}
}