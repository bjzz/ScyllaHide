#pragma once

#include <windows.h>
#include "..\ntdll\ntdll.h"

///////////////////////////////////////////////////////////////////////////////////////
//Evolution of Process Environment Block (PEB) http://blog.rewolf.pl/blog/?p=573
//March 2, 2013 / ReWolf posted in programming, reverse engineering, source code, x64 /
#pragma pack(push)
#pragma pack(1)

template <class T>
struct LIST_ENTRY_T
{
    T Flink;
    T Blink;
};

template <class T>
struct UNICODE_STRING_T
{
    union
    {
        struct
        {
            WORD Length;
            WORD MaximumLength;
        };
        T dummy;
    };
    T _Buffer;
};

template <class T, class NGF, int A>
struct _PEB_T
{
    union
    {
        struct
        {
            BYTE InheritedAddressSpace;
            BYTE ReadImageFileExecOptions;
            BYTE BeingDebugged;
            BYTE _SYSTEM_DEPENDENT_01;
        };
        T dummy01;
    };
    T Mutant;
    T ImageBaseAddress;
    T Ldr;
    T ProcessParameters;
    T SubSystemData;
    T ProcessHeap;
    T FastPebLock;
    T _SYSTEM_DEPENDENT_02;
    T _SYSTEM_DEPENDENT_03;
    T _SYSTEM_DEPENDENT_04;
    union
    {
        T KernelCallbackTable;
        T UserSharedInfoPtr;
    };
    DWORD SystemReserved;
    DWORD _SYSTEM_DEPENDENT_05;
    T _SYSTEM_DEPENDENT_06;
    T TlsExpansionCounter;
    T TlsBitmap;
    DWORD TlsBitmapBits[2];
    T ReadOnlySharedMemoryBase;
    T _SYSTEM_DEPENDENT_07;
    T ReadOnlyStaticServerData;
    T AnsiCodePageData;
    T OemCodePageData;
    T UnicodeCaseTableData;
    DWORD NumberOfProcessors;
    union
    {
        DWORD NtGlobalFlag;
        NGF dummy02;
    };
    LARGE_INTEGER CriticalSectionTimeout;
    T HeapSegmentReserve;
    T HeapSegmentCommit;
    T HeapDeCommitTotalFreeThreshold;
    T HeapDeCommitFreeBlockThreshold;
    DWORD NumberOfHeaps;
    DWORD MaximumNumberOfHeaps;
    T ProcessHeaps;

    //FULL PEB not needed
    T GdiSharedHandleTable;
    T ProcessStarterHelper;
    T GdiDCAttributeList;
    T LoaderLock;
    DWORD OSMajorVersion;
    DWORD OSMinorVersion;
    /*WORD OSBuildNumber;
    WORD OSCSDVersion;
    DWORD OSPlatformId;
    DWORD ImageSubsystem;
    DWORD ImageSubsystemMajorVersion;
    T ImageSubsystemMinorVersion;
    union
    {
    T ImageProcessAffinityMask;
    T ActiveProcessAffinityMask;
    };
    T GdiHandleBuffer[A];
    T PostProcessInitRoutine;
    T TlsExpansionBitmap;
    DWORD TlsExpansionBitmapBits[32];
    T SessionId;
    ULARGE_INTEGER AppCompatFlags;
    ULARGE_INTEGER AppCompatFlagsUser;
    T pShimData;
    T AppCompatInfo;
    UNICODE_STRING_T<T> CSDVersion;
    T ActivationContextData;
    T ProcessAssemblyStorageMap;
    T SystemDefaultActivationContextData;
    T SystemAssemblyStorageMap;
    T MinimumStackCommit;*/
};

typedef _PEB_T<DWORD, DWORD64, 34> PEB32;
typedef _PEB_T<DWORD64, DWORD, 30> PEB64;

#ifdef _WIN64
typedef PEB64 PEB_CURRENT;
#else
typedef PEB32 PEB_CURRENT;
#endif

#pragma pack(pop)

typedef struct _CURDIR {
	UNICODE_STRING DosPath;
	HANDLE Handle;
} CURDIR, *PCURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	ULONG MaximumLength;
	ULONG Length;

	ULONG Flags;
	ULONG DebugFlags;

	HANDLE ConsoleHandle;
	ULONG  ConsoleFlags;
	HANDLE StandardInput;
	HANDLE StandardOutput;
	HANDLE StandardError;

	CURDIR CurrentDirectory;        // ProcessParameters
	UNICODE_STRING DllPath;         // ProcessParameters
	UNICODE_STRING ImagePathName;   // ProcessParameters
	UNICODE_STRING CommandLine;     // ProcessParameters
	PVOID Environment;              // NtAllocateVirtualMemory

	ULONG StartingX;
	ULONG StartingY;
	ULONG CountX;
	ULONG CountY;
	ULONG CountCharsX;
	ULONG CountCharsY;
	ULONG FillAttribute;

	ULONG WindowFlags;
	ULONG ShowWindowFlags;
	UNICODE_STRING WindowTitle;     // ProcessParameters
	UNICODE_STRING DesktopInfo;     // ProcessParameters
	UNICODE_STRING ShellInfo;       // ProcessParameters
	UNICODE_STRING RuntimeData;     // ProcessParameters

} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

#define PEB_PATCH_BeingDebugged 0x00000001      
#define PEB_PATCH_NtGlobalFlag 0x00000002      
#define PEB_PATCH_HeapFlags 0x00000004      
#define PEB_PATCH_StartUpInfo 0x00000008      
//#define PEB_PATCH_BeingDebugged      0x00000010  

void FixPebBeingDebugged(HANDLE hProcess, bool SetToNull);
bool FixPebInProcess(HANDLE hProcess, DWORD EnableFlags);
void ReadPebToBuffer(HANDLE hProcess, unsigned char * buffer, int bufferSize);
