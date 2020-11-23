#pragma once
#include "DriverInterface.h"

class Memory
{

public:
	Memory()
	{
		KernelDriver::Init("TslGame.exe", L"TslGame.exe");
		baseaddr = KernelDriver::DriverGetModulebase(moduleSize);
		moduleBuffer = { 0 };
	}

	~Memory() {
		KernelDriver::UnLoadDriver();
	}

	template<typename T>
	inline T RVM(DWORD_PTR Address)
	{
		T buffer;

		auto ret = KernelDriver::DriverReadMemory(Address, &buffer, sizeof(T));

		if (ret)
			return buffer;
		else
			return T();

	}

	inline bool readSTR(DWORD_PTR Address, char* buff, DWORD buffSize)
	{
		auto ret = KernelDriver::DriverReadMemory(Address, buff, buffSize);
		return ret;
	}

	inline bool readWSTR(DWORD_PTR Address, wchar_t* buff, DWORD buffSize)
	{
		auto ret = KernelDriver::DriverReadMemory(Address, buff, buffSize);
		return ret;
	}

	template<typename T>
	bool WVM(DWORD_PTR address, T buff)
	{
		auto ret = KernelDriver::DriverWriteMemory(address, &buff, sizeof(T));
		return ret;
	}

	inline DWORD_PTR GetProcBase()
	{
		return this->baseaddr;
	}

	HANDLE GetHandle()
	{
		return hProcess;
	}

	void Close()
	{
		CloseHandle(hProcess);
	}

private:
	BYTE* moduleBuffer;
	SIZE_T moduleSize;
	HANDLE hProcess;
	DWORD_PTR baseaddr;
};

extern Memory* mem = NULL;
