#define _CRT_SECURE_NO_WARNINGS
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_DEFINE_MATH_OPERATORS
#define _CRT_SECURE_NO_WARNINGS
#include "includes.h"
#include <Windows.h>
#include <vector>
#include <string>
#include "fa_solid_900.h"
#include "font_awesome_5.h"
#include "imgui/imgui.h"
#include <wtypes.h>
#include "imgui_notify.h"
#include"nlohmann/json.hpp"
#include <fstream>

const int WINDOW_W = 754;
const int WINDOW_H = 618;
using json = nlohmann::json;

namespace Helper
{
	// Menu Very important variables
	bool g_Running = true; // By default the program is running.
	bool ShowLoginPage = false; // By default the login page is the first thing to appear
	bool ShowMainPage = false; // By default it wont show the main page
	bool ShowLoading = true; // true bcuz we need to initialize and stuff
	static IDirect3DTexture9* CTRLlogo = nullptr;
	static IDirect3DTexture9* mal = nullptr;
	static int Tabs = 0;
	int announcementsSize = 0;
	int programsSize = 0;

	float AlphaAnimation = 0.0f;

	inline static char Username[31];
	inline static char Password[31];

	// Menu Fonts Struct
	struct Fonts
	{
		ImFont* mainFont;
		ImFont* IconFont;
		ImFont* afont;
		ImFont* Boldfont;
		ImFont* Fontie;
	} fonts;


	//Announcements Struct
	struct Announcement
	{
		std::string announcement;
		std::string date;
	};

	//Programs Struct
	struct Program
	{
		std::string name;
		std::string status;
		std::string lastupdated;
		std::string version;
		std::string hash;
		std::string expiry;
		std::string programID;
	};

	//Announcements List
	static std::vector<Announcement> announcements;

	//Programs list
	static std::vector<Program> programs;

	// Menu Add-on Functions
	void Style()
	{
		
		ImGuiStyle& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_FrameBg] = ImColor(11, 11, 11, 255);
		style.Colors[ImGuiCol_FrameBgActive] = ImColor(18, 17, 17, 255);
		style.Colors[ImGuiCol_FrameBgHovered] = ImColor(18, 17, 17, 255);

		style.Colors[ImGuiCol_ChildBg] = ImColor(9, 9, 9, 255);
		style.Colors[ImGuiCol_Border] = ImColor(20, 20, 20, 255);
		style.Colors[ImGuiCol_Separator] = ImColor(21, 21, 21, 255);

		style.Colors[ImGuiCol_WindowBg] = ImColor(12, 12, 12, 255);

		style.FramePadding = ImVec2(12, 12); // thats for the texinput padding.
		style.WindowPadding = ImVec2(12, 12);
		style.WindowRounding = 8.0f;
		style.ChildBorderSize = 0.5;
		style.ChildRounding = 8.0f;
		return;
	}

	std::string getLocalAppDataPath() {
		char* localAppData;
		size_t len;
		if (_dupenv_s(&localAppData, &len, "LOCALAPPDATA") == 0 && localAppData != nullptr) {
			std::string result(localAppData);
			free(localAppData);
			return result;
		}
		else {
			return "";
		}
	}

	std::string tm_to_readable_time(tm ctx) {
		char buffer[80];

		strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

		return std::string(buffer);
	}

	static std::time_t string_to_timet(std::string timestamp) {
		auto cv = strtol(timestamp.c_str(), NULL, 10); // long

		return (time_t)cv;
	}

	static std::tm timet_to_tm(time_t timestamp) {
		std::tm context;

		localtime_s(&context, &timestamp);

		return context;
	}

	bool InputTextWithHintCentered(const char* label, const char* hint, char* buf, size_t buf_size, bool showfrgtpwd = false, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL)
	{
		
		const float input_width = ImGui::CalcItemWidth();
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - input_width) / 2.f);

		ImGui::Text(("%s"), label);

		if (showfrgtpwd == true)
		{
			ImGui::SameLine();
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - input_width) / 1.f);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 248);
			ImGui::TextDisabled(("Forgot Password?"));
			if (ImGui::IsItemClicked())
			{
				WinExec(("start https://google.com"), SW_HIDE);
			}
		}
		ImGui::Spacing();

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - input_width) / 2.f);
		return ImGui::InputTextWithHint(std::string(("##")).append(label).c_str(), hint, buf, buf_size, flags, callback, user_data);
	}


	std::string ReadFromJson(std::string path, std::string section)
	{
		if (!std::filesystem::exists(path))
			return "File Not Found";
		std::ifstream file(path);
		json data = json::parse(file);
		return data[section];
	}

	bool CheckIfJsonKeyExists(std::string path, std::string section)
	{
		if (!std::filesystem::exists(path))
			return "File Not Found";
		std::ifstream file(path);
		json data = json::parse(file);
		return data.contains(section);
	}

	bool WriteToJson(std::string path, std::string name, std::string value, bool userpass, std::string name2, std::string value2)
	{
		json file;
		if (!userpass)
		{
			file[name] = value;
		}
		else
		{
			file[name] = value;
			file[name2] = value2;
		}

		std::ofstream jsonfile(path, std::ios::out);
		jsonfile << file;
		jsonfile.close();
		if (!std::filesystem::exists(path))
			return false;

		return true;
	}
}