#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

PVOID FindRWX(HANDLE pHandle, SIZE_T mSpace = 0) {
    MEMORY_BASIC_INFORMATION mbi = {};
    LPVOID addr = 0;

    while (VirtualQueryEx(pHandle, addr, &mbi, sizeof(mbi))) {
        addr = (LPVOID)((DWORD_PTR)mbi.BaseAddress + mbi.RegionSize);
        if (mbi.Protect == PAGE_EXECUTE_READWRITE && mbi.State == MEM_COMMIT && mbi.Type == MEM_PRIVATE) {
            if (mSpace == 0) {
                return mbi.BaseAddress;
            }
            else {
                if (mbi.RegionSize > mSpace) {
					return mbi.BaseAddress;
				}
                else {
                    return NULL;
                   
				}
            }
        }
    }
    return NULL;
}


int main(){
    HANDLE pSnapshot;
    PVOID memoryAddress;
    MEMORY_BASIC_INFORMATION mbi = {};
    SIZE_T mSpace = 0;
    SIZE_T zerosAddeds = 0;
    cout << "[!] RWX Finder\n";

    pSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);

    // iterate over all processes
    printf("[+] Scanning for RWX memory regions...\n\n");
    // mbi.BaseAddress, mbi.RegionSize / 1024, mbi.RegionSize
    printf("                        BaseAddress 	  Size (kB)   Size (B) \n");
    if (Process32First(pSnapshot, &pEntry)) {
        while (Process32Next(pSnapshot, &pEntry)){
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEntry.th32ProcessID);
            if (hProcess != NULL) {
				memoryAddress = FindRWX(hProcess, mSpace);
                if(memoryAddress != NULL){
                    cout << GetProcessId(hProcess) << endl;
                    VirtualQueryEx(hProcess, memoryAddress, &mbi, sizeof(mbi));
                    printf("Found RWX memory: %#18llx\t(%#6llu kB | %#7llu )\n", mbi.BaseAddress, mbi.RegionSize / 1024, mbi.RegionSize);
                    break;
                }
                else{
                    continue;
                }
				CloseHandle(hProcess);
			}
		};
	}
	CloseHandle(pSnapshot);
	return 0;
}
