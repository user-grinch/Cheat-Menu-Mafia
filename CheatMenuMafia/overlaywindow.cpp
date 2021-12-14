#include "pch.h"
#include "overlaywindow.h"
#include "game/CVector.h"
#include "game/CWorld.h"

void OverlayWindow::GetCPUUsageInit()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	mTotalRam = static_cast<int>(memInfo.ullTotalPhys * 1e-6); // Bytes -> MegaBytes

	PdhOpenQuery(nullptr, NULL, &cpuQuery);
	PdhAddEnglishCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);
}

double OverlayWindow::GetCurrentCPUUsage()
{
	PDH_FMT_COUNTERVALUE counterVal;

	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, nullptr, &counterVal);
	return counterVal.doubleValue;
}

void OverlayWindow::Draw()
{
	bool m_bShowMenu = bCoord || bFPS || bCpuUsage || bMemUsage;

	const float offset = 10.0f;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	if (mSelectedPos == DISPLAY_POS::CUSTOM)
	{
		if (fPosX != NULL && fPosY != NULL)
		{
			gConfig.SetValue("overlay.posx", fPosX);
			gConfig.SetValue("overlay.posy", fPosY);
			ImGui::SetNextWindowPos(ImVec2(fPosX, fPosY), ImGuiCond_Once);
		}
	}
	else
	{
		window_flags |= ImGuiWindowFlags_NoMove;
		ImVec2 pos, pivot;

		if (mSelectedPos == DISPLAY_POS::TOP_LEFT)
		{
			pos = ImVec2(offset, offset);
			pivot = ImVec2(0.0f, 0.0f);
		}

		if (mSelectedPos == DISPLAY_POS::TOP_RIGHT)
		{
			pos = ImVec2(io.DisplaySize.x - offset, offset);
			pivot = ImVec2(1.0f, 0.0f);
		}

		if (mSelectedPos == DISPLAY_POS::BOTTOM_LEFT)
		{
			pos = ImVec2(offset, io.DisplaySize.y - offset);
			pivot = ImVec2(0.0f, 1.0f);
		}

		if (mSelectedPos == DISPLAY_POS::BOTTOM_RIGHT)
		{
			pos = ImVec2(io.DisplaySize.x - offset, io.DisplaySize.y - offset);
			pivot = ImVec2(1.0f, 1.0f);
		}

		ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
	}

	ImGui::SetNextWindowBgAlpha(bTransparent ? 0.0f : 0.5f);

	if (m_bShowMenu && ImGui::Begin("Overlay", nullptr, window_flags))
	{
		CVector pos{ 0,0,0 };
		pos = CWorld::GetInstance()->pPlayer->Position;

		static float lastTimer = 0;
		float timer = ImGui::GetTime();
		if (timer - lastTimer > 1.0f)
		{
			fCpuUsage = static_cast<float>(GetCurrentCPUUsage());

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			int mUsedRam = static_cast<int>((memInfo.ullTotalPhys - memInfo.ullAvailPhys) * 1e-6);
			fMemUsage = 100.0f * (static_cast<float>(mUsedRam) / static_cast<float>(mTotalRam));

			mFPS = static_cast<size_t>(io.Framerate);
			lastTimer = timer;
		}

		if (bCoord)
		{
			ImGui::Text("Coord: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
		}

		if (bCpuUsage)
		{
			ImGui::Text("CPU usage: %.2f%%", fCpuUsage);
		}

		if (bFPS)
		{
			ImGui::Text("Frames: %d", mFPS);
		}

		if (bMemUsage)
		{
			ImGui::Text("RAM usage: %.2f%%", fMemUsage);
		}

		ImVec2 windowPos = ImGui::GetWindowPos();
		fPosX = windowPos.x;
		fPosY = windowPos.y;

		ImGui::End();
	}
}