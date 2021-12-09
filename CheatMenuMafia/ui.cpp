#include "pch.h"
#include "ui.h"

ImVec2 Ui::GetSize(short count, bool spacing)
{
	if (count == 1)
	{
		spacing = false;
	}

	float factor = ImGui::GetStyle().ItemSpacing.x / 2.0f;
	float x;

	if (count == 3)
	{
		factor = ImGui::GetStyle().ItemSpacing.x / 1.403f;
	}

	if (spacing)
	{
		x = ImGui::GetWindowContentRegionWidth() / count - factor;
	}
	else
	{
		x = ImGui::GetWindowContentRegionWidth() / count;
	}

	return ImVec2(x, ImGui::GetFrameHeight() * 1.3f);
}

void Ui::CenterdText(const std::string& text)
{
	ImVec2 size = ImGui::CalcTextSize(text.c_str());
	ImGui::NewLine();
	ImGui::SameLine(
		((ImGui::GetWindowContentRegionWidth() - size.x) / 2)
	);

	ImGui::Text(text.c_str());
}

void Ui::DrawHeaders(CallbackTable& data)
{
	static void* pCallback;
	static int buttonInRow = 3;
	static ImVec2 size = GetSize(buttonInRow, false);
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 buttonCol = style.Colors[ImGuiCol_Button];

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	for (size_t i = 0; i < data.size(); ++i)
	{
		const char* btn_text = data[i].first.c_str();

		if (btn_text == m_HeaderId)
		{
			style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_ButtonActive];
			pCallback = data[i].second;
		}
		if (ImGui::Button(btn_text, size))
		{
			m_HeaderId = btn_text;
			pCallback = data[i].second;
		}
		style.Colors[ImGuiCol_Button] = buttonCol;

		if (i % buttonInRow != 2)
		{
			ImGui::SameLine();
		}
	}
	ImGui::PopStyleVar();
	ImGui::Dummy(ImVec2(0, 5));

	if (m_HeaderId == "")
	{
		// Show Welcome page
		ImGui::NewLine();

		CenterdText("Welcome to Cheat Menu");
		CenterdText("Author: Grinch_");

		ImGui::NewLine();
		ImGui::TextWrapped("Please ensure you have the latest version from GitHub.");
		ImGui::NewLine();
		if (ImGui::Button("Discord server", ImVec2(GetSize(2))))
		{
			ShellExecute(nullptr, "open", DISCORD_INVITE, nullptr, nullptr, SW_SHOWNORMAL);
		}

		ImGui::SameLine();

		if (ImGui::Button("GitHub repo", ImVec2(GetSize(2))))
		{
			ShellExecute(nullptr, "open", GITHUB_LINK, nullptr, nullptr, SW_SHOWNORMAL);
		}

		ImGui::NewLine();
		ImGui::TextWrapped("If you find bugs or have suggestions, you can let me know on discord :)");
		ImGui::Dummy(ImVec2(0, 15));
		CenterdText("Copyright Grinch_ 2021-2022. All rights reserved.");
	}
	else
	{
		if (pCallback != nullptr && ImGui::BeginChild("TABSBAR"))
		{
			static_cast<void(*)()>(pCallback)();
			ImGui::EndChild();
		}
	}
}
