#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <ctime>
#include <string>
#include <sstream>

uintptr_t modulebaseaddr;
DWORD procId;
HWND csgowindow;
HANDLE hProcess;
bool toggle = true;

// Get module base address by searching in snapshoted process state
uintptr_t GetModuleBaseAddress() {
	contact me : rogex@tutanota.com
}

// Anti VAC, random assembly instructions every time we run cheatcode. Code adds more bytes in mem and change signature of the cheat code (runtime signature changer)
void polymorphic() {
	std::srand(std::time(0));	// time of the system as seed to srand()

	int count = 0;
	
	for (count; count < 10; count++) 
	{
		int index = rand() % (6 - 0 + 1) + 0;	// random number between 0..6

		// random as case, __volatile blocks the compiler optimizer for asm instruction
		switch (index)
		{
		case 0:
			__asm __volatile
			{
				sub eax, 3
				add eax, 1
				add eax, 2
			}

		case 1:
			__asm __volatile
			{
				push eax
				pop eax
			}

		case 2:
			__asm __volatile
			{
				inc eax
				dec eax
			}

		case 3:
			__asm __volatile
			{
				dec eax
				add eax, 1
			}

		case 4:
			__asm __volatile
			{
				pop eax
				push eax
			}
		
		case 5:
			__asm __volatile
			{
				mov eax, eax
				sub eax, 1
				add eax, 1
			}

		case 6:
			__asm __volatile
			{
				xor eax, eax
				mov eax, eax
			}
		}
	}
}

auto TitleGen = [](int num) {
	std::string nameoftitle;
	for (int i = 0; i < num; i++)
		nameoftitle += rand() % 255 + 1;

	return nameoftitle;
};

struct offsets
{
	uintptr_t localPlayeraddr = 0xD892AC;	// dwLocalPlayer
	uintptr_t GlowIndex = 0xA438;			// m_iGlowIndex
	uintptr_t GlowObject = 0x52E9348;		// dwGlowObjectManager
	uintptr_t PlayerList = 0x4DA0D54;		// dwEntityList
	uintptr_t Isdormant = 0xED;				// m_bDormant
	uintptr_t GetTeam = 0xF4;				// m_iTeamNum
	uintptr_t health = 0x100;				// m_iHealth
	int MaxPlayers = 64;
} addr;  // is a handle for interractions with data

struct glowconfig
{
	contact me : rogex@tutanota.com
} config;

// uintptr_t is an x86/64 analog of DWORD, using for address store (unsigned - may contain neg.numbers)
// Function for call everytime - optimisation and stuff..
uintptr_t Read(uintptr_t addr) {
	uintptr_t val;	// var for reading value into
	ReadProcessMemory(hProcess, (LPCVOID)addr, &val, sizeof(val), NULL);	// addr ->store-> &val in size of val

	return val;
}

// another VAC bypassing, generates random string put as name of console window
auto TitileGen = [](int num) {
	std::string titlename;
	for (int i = 0; i < num; i++)
	{
		titlename += rand() % 255 + 1;		// rand numbers for cmd
	}

	return titlename;
};

int main()
{
	polymorphic();
	SetConsoleTitleA(TitleGen(rand() % 100 + 30).c_str());

	HWND hconsole = GetConsoleWindow(); // handle to a console window
	SetWindowPos(hconsole, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // when a console runs first time it displayer in foreground, in the center
	// std::cout << "Cheat by D3C0Y for CS GO with love, enjoy. Toggling - [F7]" << std::endl;
	std::cout << "" << std::endl;

	// Show previous text and cheath_console goes on the bottom, under CS window
	Sleep(4000);
  SetWindowPos(hconsole, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// Window and process hook
	csgowindow = FindWindowA(NULL, "Counter-Strike: Global Offensive");
	GetWindowThreadProcessId(csgowindow, &procId);

	// handles to csgo procId
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);	// params: (read/write access, no inherit handle, procId)
	modulebaseaddr = GetModuleBaseAddress("client.dll");

	// functionality if we want to stop cheat -> pressing [F1]
	while (true) 
	{	
		// if key press not a keyhold -> &1
		if (GetAsyncKeyState(VK_F7) & 1)
		{
			toggle = !toggle;	// set cheat off
		}

		// Finding offsets in game section
		if (toggle) {
			uintptr_t LocalPlayer = Read(modulebaseaddr + addr.localPlayeraddr);	// get local player address [from modulebase -> +localplayeraddr]

			do
			{
				uintptr_t LocalPlayer = Read(modulebaseaddr + addr.localPlayeraddr);
			} while (LocalPlayer == NULL);	// if localPlayer offset not found, try one more time

			uintptr_t GlowManager = Read(modulebaseaddr + addr.GlowObject);
			int MyTeam = Read(LocalPlayer + addr.GetTeam);
			polymorphic();

			// Each player entities
			for (int i = 1; i < addr.MaxPlayers; i++) 
			{
				uintptr_t Entity = Read(modulebaseaddr + addr.PlayerList + i * 0x10);	// 0x10 - mem between each player in struct; 
				int GlowIndex = Read(Entity + addr.GlowIndex);	// value for each Player entity
				int EntityTeam = Read(Entity + addr.GetTeam);
				int EntityHealth = Read(Entity + addr.health);

				// checking if player is a spectator no need to mget 'em entity
				int isDormant = Read(Entity + addr.Isdormant);

				// call anti VAC
				polymorphic();

				// not a spectator, is alive
				if (MyTeam != EntityTeam && isDormant == NULL && EntityHealth != 0)
				{
					// set color of esp for this players, write the color structure in cs go game memory and replace it with em structure
					config.red = 1;
					config.blue = 0.2;
					config.green = 0.2;

					WriteProcessMemory(hProcess, (LPVOID)(GlowManager + (GlowIndex * 0x38) + 0x4), &config, sizeof(config), NULL);	// the start of the address where the structure of original color replace ours [CALLBACK]
				}
				
				// if teammate
				else if (MyTeam == EntityTeam && isDormant == NULL && EntityHealth != 0)
				{
					// teammate colors
					config.red = 0;
					config.blue = 0.5;
					config.green = 0.5;

					WriteProcessMemory(hProcess, (LPVOID)(GlowManager + (GlowIndex * 0x38) + 0x4), &config, sizeof(config), NULL);	// the start of the address where the structure of original color replace ours [CALLBACK]
				}
			}

			Sleep(0.5);	// anti flickering
		}
	}
}