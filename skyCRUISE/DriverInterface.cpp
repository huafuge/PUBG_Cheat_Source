#include "DriverInterface.h"
using namespace std;
#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))
HANDLE KernelDriver::hDriver = INVALID_HANDLE_VALUE;
ULONG KernelDriver::ProcID = 0;

void KernelDriver::Init(LPCSTR modulename, LPCWSTR processname)
{
	hDriver = CreateFile(RegPath, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_TEMPORARY, 0);
	if (!hDriver)
	{
		printf("cannot connect to driver");
		return;
	}

	DWORD Bytes;
	DeviceIoControl(hDriver, IO_Init_REQUEST, &ProcID, sizeof(ProcID), &ProcID, sizeof(ProcID), &Bytes, NULL);

	ULONG CID = GetCurrentProcessId();
	DeviceIoControl(hDriver, IO_PROTECT_REQUEST, &CID, sizeof(CID), &CID, sizeof(CID), &Bytes, NULL);
}

BOOL KernelDriver::LoadDriver() // 커널 로드
{
	hDriver = CreateFile(RegPath, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_TEMPORARY, 0);

	if (hDriver != INVALID_HANDLE_VALUE)
	{
		DWORD Bytes;
		ULONG CID = GetCurrentProcessId();
		DeviceIoControl(hDriver, IO_PROTECT_REQUEST, &CID, sizeof(CID), &CID, sizeof(CID), &Bytes, NULL);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL KernelDriver::UnLoadDriver()
{
	//DWORD Bytes;
	//ULONG CID = 0;
	//DeviceIoControl(hDriver, IO_PROTECT_REQUEST, &CID, sizeof(CID), &CID, sizeof(CID), &Bytes, NULL);

	if (CloseHandle(hDriver))
		return TRUE;
	else
		return FALSE;
}

DWORD_PTR KernelDriver::DriverGetModulebase(OPTIONAL ULONG size)
{
	if (hDriver == INVALID_HANDLE_VALUE || !hDriver) {
		return false;
	}

	DWORD_PTR Address;
	DWORD Bytes;

	if (DeviceIoControl(hDriver, IO_GET_MODULE_REQUEST, &Address, sizeof(Address), &Address, sizeof(Address), &Bytes, NULL)) {
		return Address;
	}
	else
		return false;
}

BOOL KernelDriver::DriverReadMemory(ULONGLONG lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize)
{
	if (hDriver == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD Bytes;
	KERNEL_READ_REQUEST ReadRequest;
	ReadRequest.TargetAddress = lpBaseAddress;
	ReadRequest.ResponseAddress = (DWORD_PTR)lpBuffer;
	ReadRequest.Size = nSize;

	// send code to our driver with the arguments
	if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), &Bytes, NULL))
		return TRUE;
	else
		return FALSE;

}

BOOL KernelDriver::DriverWriteMemory(ULONGLONG lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize)
{
	if (hDriver == INVALID_HANDLE_VALUE || !hDriver)
		return false;

	DWORD Bytes;

	KERNEL_WRITE_REQUEST  WriteRequest;
	WriteRequest.CopiedFromAddress = (DWORD_PTR)lpBuffer;
	WriteRequest.TargetAddress = lpBaseAddress;
	WriteRequest.Size = nSize;

	if (DeviceIoControl(hDriver, IO_WRITE_REQUEST, &WriteRequest, sizeof(WriteRequest), &WriteRequest, sizeof(WriteRequest), &Bytes, NULL))
		return true;
	else
		return false;
}
