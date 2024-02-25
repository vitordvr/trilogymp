// Windows Includes
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <malloc.h>
#include <Tchar.h>

// C++ Standard Library
#include <vector>

// StealthLib
#include "Injector.h"
#include "Cleanup.h"
#include "StringUtil.h"
#include "UniUtil.h"

// Static data
Injector* Injector::m_pSingleton = nullptr;

// Get injector singleton
Injector* Injector::Get()
{
    if (!m_pSingleton)
        m_pSingleton = new Injector();
    return m_pSingleton;
}

// Injector constructor
Injector::Injector()
{ }


// Injects a module (fully qualified path) via process id
void Injector::InjectLib(DWORD ProcID, const std::wstring& Path)
{
    // Get a handle for the target process.
    EnsureCloseHandle Process(OpenProcess(
        PROCESS_QUERY_INFORMATION |   // Required by Alpha
        PROCESS_VM_READ |   // For EnumProcessModules
        PROCESS_CREATE_THREAD |   // For CreateRemoteThread
        PROCESS_VM_OPERATION |   // For VirtualAllocEx/VirtualFreeEx
        PROCESS_VM_WRITE,             // For WriteProcessMemory
        FALSE, ProcID));
    if (!Process)
        throw std::runtime_error("Could not get handle to process.");

    // Calculate the number of bytes needed for the DLL's pathname
    size_t Size = (Path.length() + 1) * sizeof(wchar_t);

    // Allocate space in the remote process for the pathname
    EnsureReleaseRegionEx LibFileRemote(VirtualAllocEx(Process, NULL, Size, MEM_COMMIT, PAGE_READWRITE),
        Process);
    if (!LibFileRemote)
        throw std::runtime_error("Could not allocate memory in remote process.");

    // Copy the DLL's pathname to the remote process' address space
    if (!WriteProcessMemory(Process, LibFileRemote,
        Path.c_str(), Size, NULL))
        throw std::runtime_error("Could not write to memory in remote process.");;

    // Get the real address of LoadLibraryW in Kernel32.dll
    HMODULE hKernel32 = GetModuleHandle(TEXT("Kernel32"));
    if (!hKernel32)
        throw std::runtime_error("Could not get handle to Kernel32.");
    PTHREAD_START_ROUTINE pfnThreadRtn = reinterpret_cast<PTHREAD_START_ROUTINE>
        (GetProcAddress(hKernel32, "LoadLibraryW"));
    if (!pfnThreadRtn)
        throw std::runtime_error("Could not get pointer to LoadLibraryW.");

    // Create a remote thread that calls LoadLibraryW(DLLPathname)
    EnsureCloseHandle Thread(CreateRemoteThread(Process, NULL, 0, pfnThreadRtn,
        LibFileRemote, 0, NULL));
    if (!Thread)
        throw std::runtime_error("Could not create thread in remote process.");

    // Wait for the remote thread to terminate
    WaitForSingleObject(Thread, INFINITE);

}
