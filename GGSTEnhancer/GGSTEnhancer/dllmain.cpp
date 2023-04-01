// dllmain.cpp : Defines the entry point for the DLL application.
#include "Features.h"

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>

inline const char ConfigFileName[] = ".\\RED\\Binaries\\Win64\\GGSTEnhancer.ini";

void PatchAll()
{
	SelectedRewardAura = 3;

	std::ifstream in(ConfigFileName);

	auto CreateConsole = [](const char* name) {
		FILE* ConsoleIO;
		if (!AllocConsole())
			return;

		freopen_s(&ConsoleIO, "CONIN$", "r", stdin);
		freopen_s(&ConsoleIO, "CONOUT$", "w", stderr);
		freopen_s(&ConsoleIO, "CONOUT$", "w", stdout);

		SetConsoleTitleA(name);
	};

	if (in.is_open())
	{
		std::string param;
		unsigned int value = 0;

		while (in >> param >> value)
		{
			if (!param.compare("Console"))
			{
				if (value == 1) CreateConsole("GGST Enhancer");
				std::cout << "[*] GGST Enhancer" << std::endl;
				std::cout << "[+] Settings Loaded: " << std::endl;
				std::cout << param << " " << value << std::endl;
			}
			if (!param.compare("UnlockNonexistentColors"))
			{
				bUnlockNonexistentColors = value == 1;
				std::cout << param << " " << bUnlockNonexistentColors << std::endl;
			}
			if (!param.compare("GetRewards"))
			{
				bGetRewards = value == 1;
				std::cout << param << " " << bGetRewards << std::endl;
			}
			if (!param.compare("RewardAura"))
			{
				SelectedRewardAura = (value >= 0 && value <= 3) ? value : 0;
				std::cout << param << " " << SelectedRewardAura << std::endl;
			}
			if (!param.compare("GetRewardBadges"))
			{
				bGetRewardBadges = value == 1;
				std::cout << param << " " << bGetRewardBadges << std::endl;
			}
		}

		in.close();
	}
	else
	{
		std::ofstream ConfigFile;

		ConfigFile.open(ConfigFileName, std::ofstream::out | std::ofstream::trunc);

		ConfigFile << "Console 0" << std::endl;
		ConfigFile << "UnlockNonexistentColors 0" << std::endl;
		ConfigFile << "GetRewards 0" << std::endl;
		ConfigFile << "RewardAura 0" << std::endl;
		ConfigFile << "GetRewardBadges 0" << std::endl;

		ConfigFile.close();
	}

	std::cout << "[+] Unlocking Color Selection" << std::endl;
	while (!UnlockColorSelection()) {};
	std::cout << "[+] Success!" << std::endl;

	std::cout << "[+] Uncensoring Museum" << std::endl;
	while (!UncensorMuseum()) {};
	std::cout << "[+] Success!" << std::endl;

	std::cout << "[+] Improving Fishing" << std::endl;
	while (!ImproveFishing()) {};
	std::cout << "[+] Success!" << std::endl;

	if (bGetRewards)
	{
		std::cout << "[+] Unlocking Aura" << std::endl;
		while (!UnlockAura()) {};
		std::cout << "[+] Success!" << std::endl;
	}

	std::cout << "[+] Done!" << std::endl;

}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)PatchAll, hModule, 0, nullptr);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
