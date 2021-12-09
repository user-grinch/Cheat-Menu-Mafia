#pragma once
#include "pch.h"

class Ui
{
private:
	static inline std::string m_HeaderId;

public:
	Ui() = delete;
	Ui(Ui&) = delete;
	void operator=(Ui const&) = delete;

	static void CenterdText(const std::string& text);
	static void DrawHeaders(CallbackTable& data);
	static ImVec2 GetSize(short count = 1, bool spacing = true);
};

