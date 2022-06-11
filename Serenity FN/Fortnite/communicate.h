#pragma once

#define SYSCALL_CODE 0x69420


enum operation : int
{
	memory_read,
	get_pid,
	memory_write,
	module_base,
	alloc_mem,
	protect_mem,
};
struct readparamsstruct
{
	UINT_PTR address;
	ULONGLONG size;
	void* output;
};
struct writeparamsstruct
{
	UINT_PTR address;
	ULONGLONG size;
	void* buffer_address;
};
struct getpidparamsstruct
{
	const char* processname;
	int returnpid;
};
struct getbaseparamsstruct
{
	int procid;
	uintptr_t returnbase;
};
struct allocmemstruct
{
	int procid, protect, allocation_type;
	uintptr_t addr;
	SIZE_T size;
};
struct protectmemstruct
{
	int pid, protect;
	ULONGLONG addr;
	SIZE_T size;
};

struct cmd_t
{
	bool success = false;
	unsigned int verification_code = 4;
	operation operation;
	readparamsstruct readparams;
	writeparamsstruct writeparams;
	allocmemstruct allocmemparrams;
	protectmemstruct protectmemparrams;
	getpidparamsstruct pidparams;
	getbaseparamsstruct baseparams;
};