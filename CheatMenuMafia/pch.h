#pragma once
#define DISCORD_INVITE "https://discord.gg/ZzW7kmf"
#define GITHUB_LINK "https://github.com/user-grinch/Cheat-Menu-Gun"
#define INPUT_BUFFER_SIZE 64

#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>

#include <d3d9.h>
#include <dinput.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_dx9.h"
#include "vendor/imgui/imgui_impl_win32.h"
#include "vendor/patch/injector.hpp"
#include "json.h"
#include "vkeys.h"


using CallbackTable = std::vector<std::pair<std::string, void(*)()>>;

static std::ofstream gLog = std::ofstream("CheatMenu.log");
// why doesn't this work?
// inline CJson gConfig = CJson("config");
extern CJson gConfig;