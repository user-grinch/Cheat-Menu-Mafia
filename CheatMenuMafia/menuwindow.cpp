#include "pch.h"
#include "menuwindow.h"
#include "menuinfo.h"
#include "ui.h"
#include "game/CWorld.h"
#include "game/CHud.h"

void MenuWindow::Draw()
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = m_bShowMenu;

    if (m_bShowMenu)
    {
        static bool bInit;

        // Set a proper size
        if (!bInit)
        {
            RECT rect;
            if (GetWindowRect(GetForegroundWindow(), &rect))
            {
                float width = (rect.right - rect.left) / 4.0f;
                float height = (rect.bottom - rect.top) / 1.2f;

                ImGui::SetNextWindowSize(ImVec2(width, height));
            }
            bInit = true;
        }
        
        ImGui::Begin(MENU_TITLE, &m_bShowMenu, ImGuiWindowFlags_NoCollapse);
        Ui::DrawHeaders(header);
        ImGui::End();
    }
}

void MenuWindow::Process()
{
    CWorld* pWorld = CWorld::GetInstance();
    if (pWorld && pWorld->pPlayer)
    {
        // TODO: Implement proper hotkeys
        if ((GetKeyState(VK_LCONTROL) & 0x8000) && (GetKeyState(VK_KEY_M) & 0x8000) != 0)
        {
            m_bShowMenu = !m_bShowMenu;
            Sleep(250);
        }

        if (m_bNoReload)
        {
            pWorld->pPlayer->Ammo = 99;
        }

        if (m_bGodMode)
        {
            pWorld->pPlayer->Health = 100.0f;
            CHud::GetInstance()->Health = 100;
        }

        if (m_bUnlimitedAmmo)
        {
            pWorld->pPlayer->AmmoInClip = 99;
        }
    }
}

void MenuWindow::TeleportTab()
{
	if (ImGui::BeginTabBar("Teleport", ImGuiTabBarFlags_NoTooltip + ImGuiTabBarFlags_FittingPolicyScroll))
	{
		ImGui::Spacing();
		if (ImGui::BeginTabItem("Teleport"))
		{
			ImGui::Spacing();
			if (ImGui::BeginChild("Teleport Child"))
			{
				ImGui::Spacing();
				ImGui::InputTextWithHint("Coordinates", "x, y, z", m_nInputBuffer, IM_ARRAYSIZE(m_nInputBuffer));
				ImGui::Spacing();

				if (ImGui::Button("Teleport to coord", Ui::GetSize(2)))
				{
					try
					{
                        CVector pos;
						sscanf(m_nInputBuffer, "%f,%f,%f", &pos.x, &pos.y, &pos.z);
						pos.z += 1.0f;
                        CWorld::GetInstance()->pPlayer->Position = pos;
					}
					catch (...)
					{
						// SetHelpMessage("Invalid coordinate", false, false, false);
					}
				}
				ImGui::SameLine();

				if (ImGui::Button("Insert Coordinates", Ui::GetSize(2)))
				{
                    CVector pos = CWorld::GetInstance()->pPlayer->Position;

                    strcpy(m_nInputBuffer,
                        (std::to_string(static_cast<int>(pos.x)) + ", " + std::to_string(static_cast<int>(pos.y)) +
                            ", " + std::to_string(static_cast<int>(pos.z))).c_str());
				}

				ImGui::EndChild();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Search"))
		{
			ImGui::Spacing();
            Ui::DrawJSON(m_tpData, 
                [](std::string& rootkey, std::string& bLocName, std::string& loc)
                {
                    try
                    {
                        CVector &pos = CWorld::GetInstance()->pPlayer->Position;
                        sscanf(loc.c_str(), "%f,%f,%f", &pos.x, &pos.y, &pos.z);
                    }
                    catch (...)
                    {
                        //SetHelpMessage("Invalid location", false, false, false);
                    }
                }, 
                [](std::string& cat, std::string& key, std::string& val)
                {
                    if (cat == "Custom")
                    {
                        m_tpData.m_pJson->m_Data["Custom"].erase(key);
                        //SetHelpMessage("Location removed", false, false, false);
                        m_tpData.m_pJson->WriteToDisk();
                    }
                    else
                    {
                        //SetHelpMessage("You can only remove custom location", false, false, false);
                    }
                });
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Custom"))
		{
			ImGui::Spacing();
			ImGui::InputTextWithHint("Location", "Groove Street", m_nLocationBuffer, IM_ARRAYSIZE(m_nInputBuffer));
			ImGui::InputTextWithHint("Coordinates", "x, y, z", m_nInputBuffer, IM_ARRAYSIZE(m_nInputBuffer));
			ImGui::Spacing();
			if (ImGui::Button("Add location", Ui::GetSize()))
			{
				m_tpData.m_pJson->m_Data["Custom"][m_nLocationBuffer] = std::string(m_nInputBuffer);
				m_tpData.m_pJson->WriteToDisk();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void MenuWindow::StatsTab()
{
    ImGui::Columns(2, NULL, false);
    ImGui::Checkbox("God mode", &m_bGodMode);
    ImGui::Checkbox("No reload", &m_bNoReload);
    ImGui::NextColumn();
    ImGui::Checkbox("Unlimited ammo", &m_bUnlimitedAmmo);
    ImGui::Columns(1);

    ImGui::Spacing();
    CWorld *pWorld = CWorld::GetInstance();
    ImGui::InputInt("Ammo", &pWorld->pPlayer->Ammo);
    ImGui::InputInt("Ammo in Clip", &pWorld->pPlayer->AmmoInClip);
    if (ImGui::SliderFloat("Health", &pWorld->pPlayer->Health, 0.0f, 100.0f))
    {
        CHud::GetInstance()->Health = (int)pWorld->pPlayer->Health;
    }

    ImGui::Dummy(ImVec2(0, 10));
}

void MenuWindow::MenuTab()
{
    if (ImGui::Button("Check update", ImVec2(Ui::GetSize(3))))
    {
        ShellExecute(NULL, "open", "https://github.com/user-grinch/Cheat-Menu/releases/", NULL, NULL, SW_SHOWNORMAL);
    }

    ImGui::SameLine();

    if (ImGui::Button("Discord server", ImVec2(Ui::GetSize(3))))
    {
        ShellExecuteA(nullptr, "open", DISCORD_INVITE, nullptr, nullptr, SW_SHOWNORMAL);
    }

    ImGui::SameLine();

    if (ImGui::Button("GitHub repo", ImVec2(Ui::GetSize(3))))
    {
        ShellExecuteA(nullptr, "open", GITHUB_LINK, nullptr, nullptr, SW_SHOWNORMAL);
    }
    ImGui::Spacing();

    if (ImGui::BeginChild("AboutChild"))
    {
        ImGui::Columns(2, nullptr, false);
        ImGui::Text("Author: Grinch_");

        ImGui::Text("Version: %s", MENU_VERSION_NUMBER);

        ImGui::NextColumn();
        ImGui::Text("ImGui: %s", ImGui::GetVersion());
        ImGui::Text("Build: %s", BUILD_NUMBER);

        ImGui::Columns(1);

        ImGui::Dummy(ImVec2(0, 10));
        ImGui::TextWrapped("Thanks to Darkpassanger123 for sharing his classes.");
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::TextWrapped("If you find bugs or have suggestions, let me know on discord.");
        ImGui::Dummy(ImVec2(0, 20));
        Ui::CenterdText("Copyright Grinch_ 2021-2022. All rights reserved");

        ImGui::EndChild();
    }
}
