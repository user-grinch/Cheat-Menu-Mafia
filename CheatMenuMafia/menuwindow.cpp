#include "pch.h"
#include "menuwindow.h"
#include "menuinfo.h"
#include "ui.h"
#include "overlaywindow.h"
#include "game/CWorld.h"
#include "game/CHud.h"
#include "vendor/patch/assembly.hpp"

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
            OverlayWindow::GetCPUUsageInit();
            bInit = true;
        }
        
        ImGui::Begin(MENU_TITLE, &m_bShowMenu, ImGuiWindowFlags_NoCollapse);
        Ui::DrawHeaders(header);
        ImGui::End();
    }

    OverlayWindow::Draw();
}

void MenuWindow::Process()
{
    CWorld* pWorld = CWorld::GetInstance();
    if (pWorld && pWorld->pPlayer)
    {
        // TODO: Implement proper hotkeys
        ImGuiIO& io = ImGui::GetIO();
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            io.KeyCtrl = true;
            if ((GetKeyState(VK_KEY_M) & 0x8000) != 0)
            {
                m_bShowMenu = !m_bShowMenu;
                Sleep(250);
            }
        }
        else
        {
            io.KeyCtrl = false;
        }

        if (GetKeyState(VK_SHIFT) & 0x8000)
        {
            io.KeyShift = true;
        }
        else
        {
            io.KeyShift = false;
        }

        if (m_bNoReload)
        {
            pWorld->pPlayer->Ammo = 99;
        }

        if (m_bGodMode)
        {
            CHud::GetInstance()->Health = m_bGodModeHealth;
        }

        if (m_bInfiniteAmmo)
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
    CVehicle* pVeh = CWorld::GetInstance()->pVehicle;

  /*  if (ImGui::Button("Drop random weapon", ImVec2(Ui::GetSize())))
    {
        ((int(*__fastcall)(int))0x585D90)(reinterpret_cast<int>(CWorld::GetInstance()->pPlayer));
    }
    ImGui::Spacing();*/
    if (ImGui::BeginTabBar("STatsTabBar"))
    {
        if (ImGui::BeginTabItem("Checkbox"))
        {
            ImGui::BeginChild("CheckboxStats");
            ImGui::Spacing();
            ImGui::Columns(2, NULL, false);
            static bool fastCarUnlock;
            if (ImGui::Checkbox("Fast car unlock", &fastCarUnlock))
            {
                if (fastCarUnlock)
                {
                    injector::MakeInline<0x595ABC, 0x595AC2>([](injector::reg_pack& regs)
                        {
                            *(float*)(regs.esi + 0x44c) = 0.0f;
                        });
                }
                else
                {
                    injector::WriteMemoryRaw(0x595ABC, (void*)"\x89\x86\x4C\x04\x00\x00", 6, true);
                }
            }
            static bool fastKill;
            if (ImGui::Checkbox("Fast kill", &fastKill))
            {
                if (fastKill)
                {
                    injector::MakeInline<0X5768C6, 0X5768CC>([](injector::reg_pack& regs)
                        {
                            float health = (float)regs.eax;
                            bool isPlayer = (*(WORD*)(GetModuleHandle(NULL) + 0x2CC) != 1);

                            if (health < 100 && !isPlayer)
                            {
                                *(int*)regs.esi = 0;
                            }
                        });
                }
                else
                {
                    injector::WriteMemoryRaw(0x5768C6, (void*)"\xD9\x86\x44\x06\x00\x00", 6, true);
                }
            }
            if (ImGui::Checkbox("God mode", &m_bGodMode))
            {
                if (m_bGodMode)
                {
                    injector::MakeInline<0x59427F, 0x594285>([](injector::reg_pack& regs)
                    {
                        *(float*)(regs.esi + 0x644) = 999.0f;
                    });
                    m_bGodModeHealth = CHud::GetInstance()->Health;
                }
                else
                {
                    CWorld::GetInstance()->pPlayer->Health = (float)CHud::GetInstance()->Health;
                    injector::WriteMemoryRaw(0x59427F, (void*)"\xD9\x86\x44\x06\x00\x00", 6, true);
                }
            }
            ImGui::Checkbox("Infinite ammo", &m_bInfiniteAmmo);
            static bool infiniteFuel;
            if (ImGui::Checkbox("Infinite fuel", &infiniteFuel))
            {
                if (infiniteFuel)
                {
                    injector::MakeInline<0x4CB6FB, 0x4CB701>([](injector::reg_pack& regs)
                        {
                            *(float*)(regs.ecx + 0xC30) = 150.0f;
                        });
                }
                else
                {
                    injector::WriteMemoryRaw(0x4CB6FB, (void*)"\xD9\x99\x30\x0C\x00\x00", 6, true);
                }
            }
            ImGui::NextColumn();
            static bool infiniteMissionTimer;
            if (ImGui::Checkbox("Infinite mission timer", &infiniteMissionTimer))
            {
                if (infiniteMissionTimer)
                {
                    injector::MakeNOP(0x60286E, 6, true);
                    injector::MakeNOP(0x6028AA, 6, true);
                    injector::MakeNOP(0x6028C9, 6, true);
                }
                else
                {
                    injector::WriteMemoryRaw(0x60286E, (void*)"\x89\x85\x70\x41\x00\x00", 6, true);
                    injector::WriteMemoryRaw(0x6028AA, (void*)"\x89\x85\x6C\x41\x00\x00", 6, true);
                    injector::WriteMemoryRaw(0x6028C9, (void*)"\x89\x95\x6C\x41\x00\x00", 6, true);
                }
            }
            static bool weaponPrecision;
            if (ImGui::Checkbox("Max weapon precision", &weaponPrecision))
            {
                if (weaponPrecision)
                {
                    injector::WriteMemoryRaw(0x5F8940, (void*)"\xC7\x81\x3C\x42\x00\x00\x00\x00\x80\x3F", 10, true); // crosshair

                    // recoil
                    injector::MakeInline<0x5953C2, 0x5953C8>([](injector::reg_pack& regs)
                        {
                            *(float*)(regs.esi + 0xAD0) = 0.0f;
                        });
                }
                else
                {
                    injector::WriteMemoryRaw(0x5F8940, (void*)"\x8B\x44\x24\x04\x89\x81\x3C\x42\x00\x00", 10, true);
                    injector::WriteMemoryRaw(0x5953C2, (void*)"\xD9\x96\xD0\x0A\x00\x00", 6, true);
                }
            }
            static bool noCarDamage;
            if (ImGui::Checkbox("No car damage", &noCarDamage))
            {
                if (noCarDamage)
                {
                    injector::MakeInline<0x426242, 0x426248>([](injector::reg_pack& regs)
                        {
                            *(float*)(regs.esi + 0x20c4) = 100.0f;
                        });
                }
                else
                {
                    injector::WriteMemoryRaw(0x426242, (void*)"\xD9\x96\xC4\x20\x00\x00", 6, true);
                }
            }
            ImGui::Checkbox("No reload", &m_bNoReload);
            static bool superSpeed, flag1;
            if (ImGui::Checkbox("Super speed", &superSpeed))
            {
                injector::MakeInline<0x57D3E7, 0x57D3ED>([](injector::reg_pack& regs)
                    {
                        *(float*)(regs.ecx + 0x65c) = superSpeed ? 10.0f : 1.0f;
                        flag1 = true;
                    });
            }

            if (flag1)
            {
                injector::WriteMemoryRaw(0x57D3E7, (void*)"\xD8\x89\x5C\x06\x00\x00", 6, true);
                flag1 = false;
            }

            ImGui::Columns(1);

            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Misc"))
        {
            ImGui::Spacing();
            CWorld* pWorld = CWorld::GetInstance();
            ImGui::InputInt("Ammo", &pWorld->pPlayer->Ammo);
            ImGui::InputInt("Ammo in Clip", &pWorld->pPlayer->AmmoInClip);
            if (ImGui::SliderFloat("Health", &pWorld->pPlayer->Health, 0.0f, 100.0f))
            {
                CHud::GetInstance()->Health = (int)pWorld->pPlayer->Health;
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void MenuWindow::MenuTab()
{
    if (ImGui::BeginTabBar("MenuTabBar"))
    {
        if (ImGui::BeginTabItem("Overlay"))
        {
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::SameLine();
            if (Ui::ListBox("Overlay", OverlayWindow::posNames, (int&)OverlayWindow::mSelectedPos))
            {
                gConfig.SetValue("overlay.selected_pos", OverlayWindow::mSelectedPos);
            }

            ImGui::Spacing();

            ImGui::Columns(2, nullptr, false);
            if (ImGui::Checkbox("No background", &OverlayWindow::bTransparent))
            {
                gConfig.SetValue("overlay.transparent", OverlayWindow::bTransparent);
            }

            if (ImGui::Checkbox("Show coordinates", &OverlayWindow::bCoord))
            {
                gConfig.SetValue("overlay.coord", OverlayWindow::bCoord);
            }

            if (ImGui::Checkbox("Show CPU usage", &OverlayWindow::bCpuUsage))
            {
                gConfig.SetValue("overlay.cpu_usage", OverlayWindow::bCpuUsage);
            }

            ImGui::NextColumn();

            if (ImGui::Checkbox("Show FPS", &OverlayWindow::bFPS))
            {
                gConfig.SetValue("overlay.fps", OverlayWindow::bFPS);
            }

            if (ImGui::Checkbox("Show RAM usage", &OverlayWindow::bMemUsage))
            {
                gConfig.SetValue("overlay.mem_usage", OverlayWindow::bMemUsage);
            }
            ImGui::Columns(1);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("About"))
        {
            ImGui::Spacing();
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
                ImGui::TextWrapped("Thanks to Darkpassanger123 & l0wb1t for sharing their work.");
                ImGui::Dummy(ImVec2(0, 10));
                ImGui::TextWrapped("If you find bugs or have suggestions, let me know on discord.");
                ImGui::Dummy(ImVec2(0, 20));
                Ui::CenterdText("Copyright Grinch_ 2021-2022.");

                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
}
