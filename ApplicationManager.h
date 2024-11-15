#pragma once
#include "imgui/imgui.h"
#include "Helper.h"

namespace Applications
{
    struct CurrentApplication
    {
        std::string Appname;
        std::string AppLastUpdate;
        std::string Appstatus;
        std::string Appversion;
        std::string AppprogramID;
    } currentApp;

    void AddApplication(const char* Name, const char* LastUpdate, const char* Status, const char* version, const char* programid)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 12, 12 });
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 12, 12 });
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(0, 0, 0, 0)));
        ImGui::BeginChild(std::string("##").append(Name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 3, 120), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImGui::PushFont(Helper::fonts.Boldfont);

        ImGui::Text(Name);
        ImGui::PopFont();

        ImGui::Spacing();

        ImGui::PushFont(Helper::fonts.mainFont);

        // Green: 140, 217, 39, 255
        // Red : 217, 39, 39, 255
        // Yellow : 217, 208, 39, 255
        ImGui::TextColored((std::string)Status == "Working" ? ImColor(140, 217, 39, 255)
            : (std::string)Status == "Maintenance" ? ImColor(217, 208, 39, 255)
            : ImColor(217, 39, 39, 255), Status);

        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosX(494);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 22);
        if (ImGui::ButtonNew("View", ImVec2(110, 35)))
        {
            currentApp.Appname = Name;
            currentApp.AppLastUpdate = LastUpdate;
            currentApp.Appstatus = Status;
            currentApp.Appversion = version;
            currentApp.AppprogramID = programid;
            Helper::AlphaAnimation = 0;

            Helper::Tabs = 3;
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Separator();

        ImGui::PushFont(Helper::fonts.mainFont);
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("Last Updated:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);
        ImGui::TextDisabled(LastUpdate);
        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
    }

    void AddNews(const char* News, const char* Date)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 12, 12 });
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 12, 12 });
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(0, 0, 0, 0)));
        ImGui::BeginChild(std::string("##").append(News).c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 3, ImGui::CalcTextSize(News).y + 60), true);

        ImGui::PushFont(Helper::fonts.Fontie);
        ImGui::TextColored(ImColor(217, 39, 39, 255), "Application Update");
        ImGui::PopFont();

        ImGui::SameLine();

        ImGui::PushFont(Helper::fonts.Fontie);
        ImGui::TextDisabled(Date);

        ImGui::Spacing();

        ImGui::PushTextWrapPos(605);
        ImGui::Text(News);
        ImGui::PopFont();
        ImGui::PopTextWrapPos();

        ImGui::EndChild();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
    }
}