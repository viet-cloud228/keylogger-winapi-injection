# KeyLogger-WinAPI-Injection

##Overview

A technical demonstration **DLL Injection** and **Asynchronous Keylogging** using the Windows API. This project was developed for educational purposes to understand memory manipulation and process instrumentation.

## Technical highlights

* **Injection technique: use 'CreateRemoteThread' and 'LoadLibraryA' to inject a custom DLL into a target process (Notepad).

* **Memory management: use 'VirtualAllocEx' and 'WriteProcessMemory' for cross-process memory allocation.

* **Asynchronous Keylogging: the injected DLL spawns a background thread using 'CreateThread' and monitors keystrokes via 'GetAyncKeyState'.

* **Persistence: disable Windows Update services like 'wuauserv' or 'WaaSMedicSvc' to maintain an undisturbed research environment.

  ## Disclaimer: this project is for educational and authorized security research purposes only. The author is not responsible for any misuse of this information.
