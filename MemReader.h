#pragma once
#include <Windows.h>
#include <TlHelp32.h>


class MemReader
{
public:
	MemReader(char* procName, size_t bufferSize = 64);
	~MemReader(void);
	DWORD ReturnPID();
	//Main functional :
	void Open(DWORD accessRight = PROCESS_ALL_ACCESS);
	void Close();
	
	void Write(void* ptr,DWORD to, size_t size);
	void Write(void* ptr,DWORD to, size_t size,DWORD memProtect = PAGE_EXECUTE_READWRITE); //� ���������� � ��������� ������ ������� ������ 
	
	MemReader& Read(DWORD from, size_t size);
	MemReader& Read(DWORD from, size_t size,DWORD memProtect); //� ���������� � ��������� ������ ������� ������
	DWORD ReadPtr(DWORD base, DWORD* offsets, int count,size_t CountOfByte);// ������� ��������� � ������ offset � 16�����(hex)
	
	DWORD Alloc(DWORD memProtect = PAGE_EXECUTE_READWRITE); // ��������� ������
	void Free(void* ptr); // ������������
	
	//Conertors :
	double    _toDouble();
	float     _toFloat();
	 
	UINT64    _toUINT64();
	INT64     _toINT64();

	UINT32    _toUINT32();
	int       _toINT32();

	UINT16    _toUNIT16();
	short int _toINT16();

	UINT8     _toUINT8();
	char      _toINT8();

	char*     _toStringA();
	wchar_t*   _toStringW();

private:
	void GetPID();
	char* m_pName;
	HANDLE m_hProc;
	DWORD m_pID;
	byte* m_buffer;
	size_t m_bufferSize;
};

