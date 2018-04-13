#include "MemReader.h"


MemReader::MemReader(char* procName, size_t bufferSize ) : m_pID(0),m_bufferSize(bufferSize)
{
	int len = strlen(procName)+1;
	m_pName = new char[len];
	strcpy_s(m_pName,len,procName);

	m_buffer = new byte[bufferSize];

}


MemReader::~MemReader(void)
{
	delete[] m_pName;
	delete[] m_buffer;
}

void MemReader::GetPID()
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); // Snapshot процессов
	PROCESSENTRY32 pInfo; // Получение информации о процесах
	pInfo.dwSize = sizeof(PROCESSENTRY32);

	if(Process32First(snapshot,&pInfo))
	{
		while(Process32Next(snapshot,&pInfo))
		{
			if(_stricmp(m_pName,pInfo.szExeFile) == 0)
			{
				m_pID = pInfo.th32ProcessID;
				CloseHandle(snapshot);
				return;
			}
		}
	}
	CloseHandle(snapshot);
	m_pID = 0;
	return;
}

void MemReader::Open(DWORD accessRight)
{
	GetPID();
	m_hProc = OpenProcess(accessRight,false,m_pID);
}

void MemReader::Close()
{
	CloseHandle(m_hProc);
}

DWORD MemReader::ReturnPID()
{
	return m_pID;
}

void MemReader::Write(void* ptr,DWORD to, size_t size)
{
	WriteProcessMemory(m_hProc,(LPVOID)to,ptr,size,NULL);
}
void MemReader::Write(void* ptr,DWORD to, size_t size,DWORD memProtect)
{
	DWORD defMemProtect = 0;
	VirtualProtectEx(m_hProc,(LPVOID)to,size,memProtect,&defMemProtect);
	WriteProcessMemory(m_hProc,(LPVOID)to,ptr,size,NULL);
	VirtualProtectEx(m_hProc,(LPVOID)to,size,defMemProtect,&defMemProtect);
}
MemReader& MemReader::Read(DWORD from, size_t size)
{
	
	memset(m_buffer,0,m_bufferSize);
	ReadProcessMemory(m_hProc,(LPCVOID)from,m_buffer,size,NULL);
	return *this;

}
MemReader& MemReader::Read(DWORD from, size_t size,DWORD memProtect)
{
	
	DWORD defMemProtect = 0;
	memset(m_buffer,0,m_bufferSize);
	VirtualProtectEx(m_hProc,(LPVOID)from,size,memProtect,&defMemProtect);
	ReadProcessMemory(m_hProc,(LPCVOID)from,m_buffer,size,NULL);
	VirtualProtectEx(m_hProc,(LPVOID)from,size,defMemProtect,&defMemProtect);
	return *this;
}

DWORD MemReader::ReadPtr(DWORD base, DWORD* offsets, int count, size_t CountOfByte)
{
	for(int i(0); i < count ; i++)
	{
		base = Read(base + offsets[i],CountOfByte)._toINT32();
	}
	return base + offsets[count - 1];
}

DWORD MemReader::Alloc(DWORD memProtect)
{
	return (DWORD)VirtualAllocEx(m_hProc,NULL,0x1000,MEM_COMMIT | MEM_RESERVE, memProtect);
}
void MemReader::Free(void* ptr)
{
	VirtualFreeEx(m_hProc,ptr,0,MEM_RELEASE);
}

double MemReader::_toDouble()
{
	double a = 0;
	memcpy(&a,m_buffer,sizeof(double));
	return a;
}
float MemReader::_toFloat()
{
	float a = 0;
	memcpy(&a,m_buffer,sizeof(float));
	return a;
}
UINT64 MemReader::_toUINT64()
{
	UINT64 a = 0;
	memcpy(&a,m_buffer,sizeof(UINT64));
	return a;
}
INT64 MemReader::_toINT64()
{
	INT64 a = 0;
	memcpy(&a,m_buffer,sizeof(INT64));
	return a;
}
UINT32 MemReader::_toUINT32()
{
	UINT32 a = 0;
	memcpy(&a,m_buffer,sizeof(UINT32));
	return a;
}
int MemReader::_toINT32()
{
	int a = 0;
	memcpy(&a,m_buffer,sizeof(int));
	return a;
}
UINT16 MemReader::_toUNIT16()
{
	UINT16 a = 0;
	memcpy(&a,m_buffer,sizeof(UINT16));
	return a;
}
short int MemReader::_toINT16()
{
	short int a = 0;
	memcpy(&a,m_buffer,sizeof(short int));
	return a;
}
UINT8 MemReader::_toUINT8()
{
	UINT8 a = 0;
	memcpy(&a,m_buffer,sizeof(UINT8));
	return a;
}
char MemReader::_toINT8()
{
	char a = 0;
	memcpy(&a,m_buffer,sizeof(char));
	return a;
}
char* MemReader::_toStringA()
{
	return (char*)m_buffer;
}
wchar_t* MemReader::_toStringW()
{
	return (wchar_t*)m_buffer;
}