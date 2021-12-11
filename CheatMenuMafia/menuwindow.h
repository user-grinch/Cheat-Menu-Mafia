#pragma once
#include "pch.h"

class MenuWindow
{
private:
	static inline bool m_bShowMenu;

	static inline bool m_bGodMode, m_bNoReload, m_bUnlimitedAmmo;
	static inline HANDLE handle = GetModuleHandle(NULL);

	// teleport tab
	static inline JsonData m_tpData = JsonData("teleport");
	static inline char m_nInputBuffer[INPUT_BUFFER_SIZE], m_nLocationBuffer[INPUT_BUFFER_SIZE];;
public:
	MenuWindow() = delete;
	MenuWindow(MenuWindow&) = delete;
	void operator=(MenuWindow const&) = delete;

	// Core
	static void Draw();
	static void Process();

	// Tabs
	static void MenuTab();
	static void StatsTab();
	static void TeleportTab();

private:
	static inline CallbackTable header{ {"Teleport", TeleportTab}, {"Cheats",&StatsTab},   {"Menu", &MenuTab}};
};

