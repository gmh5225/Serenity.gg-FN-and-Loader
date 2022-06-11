#pragma once
#include <Windows.h>
#include <cstdint>

#include "communicate.h"

int pid;

namespace driver
{
	__int64(__fastcall* NtCompositionInputThread)(void* arg01, void* arg02) = nullptr;

	bool setup()
	{
		LoadLibraryA("user32.dll");
		LoadLibraryA("win32u.dll");

		const HMODULE win32u = GetModuleHandleA("win32u.dll");

		if (!win32u)
			return false;

		*(void**)&NtCompositionInputThread = GetProcAddress(win32u, "NtCompositionInputThread");

		return NtCompositionInputThread;
	}

	bool send_cmd(cmd_t* cmd, void* arg02 = NULL)
	{
		RtlSecureZeroMemory(cmd, 0);
		NtCompositionInputThread(cmd, arg02);
		return cmd->success;
	}
}

int getpid(const char* name)
{
	cmd_t cmd{};
	cmd.verification_code = SYSCALL_CODE;
	cmd.operation = get_pid;
	cmd.pidparams.processname = name;
	driver::send_cmd(&cmd);
	return cmd.pidparams.returnpid;
}
static void read1(uintptr_t ReadAddress, PVOID buffer, DWORD size)
{
	cmd_t m;
	m.verification_code = SYSCALL_CODE;
	m.operation = memory_read;
	m.readparams.output = buffer;
	m.readparams.size = size;
	m.readparams.address = ReadAddress;
	driver::send_cmd(&m);
}
uintptr_t getbase(int pid)
{
	cmd_t cmd{};
	cmd.verification_code = SYSCALL_CODE;
	cmd.operation = module_base;
	cmd.baseparams.procid = pid;
	driver::send_cmd(&cmd);
	return cmd.baseparams.returnbase;
}
template <class T>
T read(UINT_PTR ReadAddress, bool secondary = false)
{

	T response{};

	cmd_t m;
	m.verification_code = SYSCALL_CODE;
	m.operation = memory_read;
	m.readparams.size = sizeof(T);
	m.readparams.address = ReadAddress;
	m.readparams.output = &response;
	driver::send_cmd(&m);

	return response;
}
bool WriteVirtualMemoryRaw(PVOID WriteAddress, PVOID SourceAddress, SIZE_T WriteSize)
{
	cmd_t m;
	m.verification_code = SYSCALL_CODE;
	m.operation = memory_write;
	m.writeparams.address = (uintptr_t)WriteAddress;
	m.writeparams.buffer_address = (void*)SourceAddress;
	m.writeparams.size = WriteSize;

	driver::send_cmd(&m);

	return true;
}
PVOID allocmem(SIZE_T size, uint32_t protect, uint64_t address = NULL)
{
	cmd_t m;
	m.verification_code = SYSCALL_CODE;
	m.operation = alloc_mem;
	m.allocmemparrams.procid = pid;
	m.allocmemparrams.addr = address;
	m.allocmemparrams.allocation_type = MEM_COMMIT | MEM_RESERVE;
	m.allocmemparrams.size = size;
	m.allocmemparrams.protect = protect;
	driver::send_cmd(&m);
	return (PVOID)m.allocmemparrams.addr;
}
uintptr_t protectmem(uint64_t address, size_t size, uint32_t protect)
{
	cmd_t m;
	m.verification_code = SYSCALL_CODE;
	m.operation = protect_mem;
	m.protectmemparrams.addr = address;
	m.protectmemparrams.protect = protect;
	m.protectmemparrams.size = size;
	driver::send_cmd(&m);
	return protect;
}
template<typename S>
bool write(UINT_PTR WriteAddress, const S& value)
{
	return WriteVirtualMemoryRaw((PVOID)WriteAddress, (PVOID)&value, sizeof(S));
}


