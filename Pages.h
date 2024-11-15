#pragma once
#include "includes.h"
#include "ApplicationManager.h"
#include "Bytes.h"
#include "font_awesome_5.h"
#include <iostream>
#include <winsock.h>
#include <vector>
#include <thread>
#include <bitset>

#pragma comment(lib, "ws2_32.lib")

#define maxx(a,b)            (((a) > (b)) ? (a) : (b))

namespace Pages
{
    namespace Main
    {
        void LoadingIndicatorCircle(const char* label, const float indicator_radius,
            const ImVec4& main_color, const ImVec4& backdrop_color,
            const int circle_count, const float speed)
        {
            
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            if (window->SkipItems) {
                return;
            }

            ImGuiContext& g = *GImGui;
            const ImGuiID id = window->GetID(label);

            const ImVec2 pos = window->DC.CursorPos;
            const float circle_radius = indicator_radius / 10.0f;
            const ImRect bb(pos, ImVec2(pos.x + indicator_radius * 2.0f,
                pos.y + indicator_radius * 2.0f));
            ImGui::ItemSize(bb, ImGui::GetStyle().FramePadding.y);
            if (!ImGui::ItemAdd(bb, id)) {
                return;
            }
            const float t = g.Time;
            const auto degree_offset = 2.0f * IM_PI / circle_count;
            for (int i = 0; i < circle_count; ++i) {
                const auto x = indicator_radius * std::sin(degree_offset * i);
                const auto y = indicator_radius * std::cos(degree_offset * i);
                const auto growth = maxx(0.0f, std::sin(t * speed - i * degree_offset));
                ImVec4 color;
                color.x = main_color.x * growth + backdrop_color.x * (1.0f - growth);
                color.y = main_color.y * growth + backdrop_color.y * (1.0f - growth);
                color.z = main_color.z * growth + backdrop_color.z * (1.0f - growth);
                color.w = 1.0f;
                window->DrawList->AddCircleFilled(ImVec2(pos.x + indicator_radius + x,
                    pos.y + indicator_radius - y),
                    circle_radius + growth * circle_radius,
                    ImGui::GetColorU32(color));
            }
        }

        DWORD WINAPI WaitThread(LPVOID lpParam) {
            HANDLE process_handle = reinterpret_cast<HANDLE>(lpParam);
            DWORD wait_result = WaitForSingleObject(process_handle, INFINITE);
            if (wait_result == WAIT_OBJECT_0) {
                return 0;
            }
            else {
                return 1;
            }
        }

        int RunProgram(LPSTR ProgramName)
        {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            // launch the program with a new console window
            if (!CreateProcess(NULL, ProgramName, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
                DWORD error = GetLastError();
                LPSTR messageBuffer = nullptr;
                size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

                std::string message(messageBuffer, size);

                LocalFree(messageBuffer);

                std::string errorText = "Creating The Process has failed with error " + std::to_string(error) + ": " + message;

                ImGui::InsertNotification({ ImGuiToastType_Error, 1000, errorText.c_str() });

                return error;
            }

            // Wrap process and thread handles in unique_ptr
            std::unique_ptr<void, decltype(&CloseHandle)> process_handle(pi.hProcess, CloseHandle);
            std::unique_ptr<void, decltype(&CloseHandle)> thread_handle(pi.hThread, CloseHandle);

            // Create a separate thread to wait for the process to finish
            HANDLE wait_thread_handle = CreateThread(NULL, 0, WaitThread, pi.hProcess, 0, NULL);
            CloseHandle(wait_thread_handle);

            return 10;
        }


        static void Tabs()
        {
            
            if (Helper::Tabs == 0) // Dashboard
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9); // maybe 8
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12); // maybe 8
                ImGui::PushFont(Helper::fonts.Boldfont);

                ImGui::Text(("Dashboard"));

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 577.558);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 16.502);

                ImGui::PushFont(Helper::fonts.IconFont);

                ImGui::Text(ICON_FA_TIMES);
                if (ImGui::IsItemClicked())
                {
                    exit(EXIT_SUCCESS);
                }

                ImGui::PopFont();

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 12, 12 });
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 12, 12 });
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(0, 0, 0, 0)));

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

                ImGui::BeginChild(("##HardwareID"), ImVec2(ImGui::GetContentRegionAvail().x - 3, 110), true);

                ImGui::Spacing(); ImGui::Spacing();

                ImGui::Text(("Reset HardwareID"));
                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 358);
                if (ImGui::ButtonNew(("Reset"), ImVec2(110, 35)))
                {
                    WinExec("cmd.exe /c start https://google.com", SW_HIDE);
                }

                ImGui::Spacing();

                ImGui::PopFont();

                ImGui::Separator();

                {
                    ImGui::PushFont(Helper::fonts.mainFont);

                    ImGui::Spacing(); ImGui::Spacing();

                    ImGui::TextDisabled(std::string(("You have ")).append("5").append((" Hardware ID resets left, you can request one on our website.")).c_str());

                    ImGui::PopFont();
                }

                ImGui::EndChild();

                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor();

                //User Subscription
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 12, 12 });
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 12, 12 });
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(0, 0, 0, 0)));

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

                ImGui::BeginChild(("##ApplicationSubscription"), ImVec2(ImGui::GetContentRegionAvail().x - 3, 119), true);

                ImGui::Spacing(); ImGui::Spacing();

                ImGui::PushFont(Helper::fonts.Boldfont);
                ImGui::Text(("Application Subscription"));
                ImGui::PopFont();

                ImGui::Spacing(); ImGui::Spacing();

                ImGui::Separator();
                
                ImGui::Spacing();

                {
                    ImGui::PushFont(Helper::fonts.mainFont);
                    ImGui::Text("User Expiration: 5-5-2027");

                    ImGui::Spacing(); ImGui::Spacing();

                    ImGui::Text("User Subscription: ");
                    ImGui::SameLine();

                    static ImVec4 color;

                    static const std::string Subscription = "Developer";

                    // Determine the color based on the subscription type
                    if (Subscription == (std::string)("Developer")) {
                        color = ImVec4(ImColor(240, 73, 55, 255));
                    }
                    else if (Subscription == (std::string)("Admin")) {
                        color = ImVec4(ImColor(255, 99, 67, 255));
                    }
                    else if (Subscription == (std::string)("Owner")) {
                        color = ImVec4(ImColor(255, 53, 53, 255));
                    }
                    else {
                        color = ImVec4(ImColor(221, 62, 62, 255));
                    }

                    ImGui::TextColored(color, std::string(Subscription).c_str());

                    ImGui::PopFont();
                }

                ImGui::EndChild();

                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor();
            }

            if (Helper::Tabs == 1) // Applications
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9); // maybe 8
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12); // maybe 8
                ImGui::PushFont(Helper::fonts.Boldfont);

                ImGui::Text(("Applications"));
                ImGui::PopFont();

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 577.558);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 16.502);

                ImGui::PushFont(Helper::fonts.IconFont);

                ImGui::Text(ICON_FA_TIMES);
                if (ImGui::IsItemClicked())
                {
                    exit(EXIT_SUCCESS);

                }

                ImGui::PopFont();
                
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(0, 0, 0, 0)));
                ImGui::BeginChild(("##AllAplications"), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));


                Applications::AddApplication(std::string("Smoke Chair").c_str(), std::string("5-24-2024").c_str(), std::string("Working").c_str(), std::string("1.0").c_str(), std::string("197104610").c_str());
                Applications::AddApplication(std::string("Aio 1337").c_str(), std::string("2-1-2022").c_str(), std::string("Maintenance").c_str(), std::string("1.2").c_str(), std::string("927014610").c_str());
                Applications::AddApplication(std::string("Radiant #1 mouse settings").c_str(), std::string("8-15-2023").c_str(), std::string("Disabled").c_str(), std::string("4.0").c_str(), std::string("97104260").c_str());


                ImGui::PopStyleColor();
                ImGui::EndChild();
            }

            if (Helper::Tabs == 2) // News
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9); // maybe 8
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12); // maybe 8
                ImGui::PushFont(Helper::fonts.Boldfont);

                ImGui::Text(("News"));

                ImGui::PopFont();

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 577.558);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 16.502);

                ImGui::PushFont(Helper::fonts.IconFont);

                ImGui::Text(ICON_FA_TIMES);

                if (ImGui::IsItemClicked())
                {
                    exit(EXIT_SUCCESS);
                }

                ImGui::PopFont();

                Applications::AddNews(std::string("Welcome to Control!").c_str(), std::string("7-23-2023").c_str());

                //Server::LoadAnnouncements();
            }

            if (Helper::Tabs == 3)
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9); // maybe 8
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);
                ImGui::PushFont(Helper::fonts.Boldfont);

                ImGui::Text(Applications::currentApp.Appname.c_str());
                ImGui::PopFont();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
                ImGui::PushFont(Helper::fonts.afont);
                ImGui::BeginChild(("##123321"), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 70.039), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                ImGui::TextDisabled(("Subscription"));
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text(("Product: %s"), Applications::currentApp.Appname.c_str());
                ImGui::Spacing();

                // You for sure dont need this.
                //if (Applications::currentApp.Appexpiry.c_str() != (std::string)("452058211"))
                //{
                //    ImGui::Text(("Product Expiration: %s"), Applications::currentApp.Appexpiry.c_str());

                //    //if that spacing is out side of the if statement there will be 2 spacing when we only need one for each ImGui::Text.
                //    ImGui::Spacing();
                //}
                
                ImGui::Text(("Version: %s"), Applications::currentApp.Appversion.c_str());
                ImGui::Spacing();
                
                ImGui::EndChild();
                ImGui::PopStyleVar();


                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 19.455);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.294);
                ImGui::TextDisabled(("Go Back"));
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    Helper::AlphaAnimation = 0;
                    Helper::Tabs = 1;
                }

                ImGui::SameLine();

                if (Applications::currentApp.Appstatus == (std::string)("Working"))
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 385.802);
                    ImGui::TextColored(ImColor(140, 217, 39, 255), Applications::currentApp.Appstatus.c_str()); // or the application status
                }
                else if (Applications::currentApp.Appstatus == (std::string)("Maintenance"))
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 348.765);
                    ImGui::TextColored(ImColor(217, 208, 39, 255), Applications::currentApp.Appstatus.c_str()); // or the application status
                }
                else
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 373.457);
                    ImGui::TextColored(ImColor(217, 39, 39, 255), Applications::currentApp.Appstatus.c_str()); // or the application status
                }
                
                ImGui::SameLine();

                if (Applications::currentApp.Appstatus == (std::string)("Working"))
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 24.691);
                }
                else if (Applications::currentApp.Appstatus == (std::string)("Maintenance"))
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 24.691);
                }
                else
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 33.951);
                }
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11);

                if (ImGui::ButtonNew(("Run"), ImVec2(96, 35)))
                {
                    if (Applications::currentApp.Appstatus == (std::string)("Working"))
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Success, 3000, ("app should be opened now!") });
                    }
                    else if (Applications::currentApp.Appstatus == (std::string)("Maintenance"))
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Info, 3000, ("Sorry, The program is under maintenance and you cannot run it for now.") });
                    }
                    else if (Applications::currentApp.Appstatus == (std::string)("Disabled"))
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Info, 3000, ("Sorry, The program is disabled and you cannot run it for now.") });
                    }
                    else {
                        ImGui::InsertNotification({ ImGuiToastType_Info, 3000, ("Error.") });
                    }
                }

                ImGui::PopFont();
            }
        }

        void RenderMainPage()
        {
            Helper::ShowLoading = false;
            Helper::ShowLoginPage = false;

            ImGui::PushFont(Helper::fonts.mainFont);

            ImGui::Columns(2, 0, false);
            ImGui::SetColumnOffset(1, 90);

            //Left Side
            {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
                ImGui::BeginChild(("##CrackingHuh?"), ImVec2(85, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                static const ImVec2 C_logo_size = { 80, 80 };

                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) - (C_logo_size.x / 2) - 4);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); // if you change the - to +, it will go lower. the bigger the number the higher/lower
                ImGui::Image(Helper::mal, C_logo_size);

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();

                static int ButtonWidth = 40;
                static int ButtonHeight = 35;

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - ButtonWidth) / 2.f - 4); // centering the Programs Button

                ImGui::PushFont(Helper::fonts.IconFont);
                if (ImGui::ButtonNew(ICON_FA_HOME, ImVec2(ButtonWidth, ButtonHeight)))
                {
                    Helper::AlphaAnimation = 0;
                    Helper::Tabs = 0;
                }
                ImGui::PopFont();

                ImGui::Spacing();

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - ButtonWidth) / 2.f - 4); // centering the Programs Button

                ImGui::PushFont(Helper::fonts.IconFont);
                if (ImGui::ButtonNew(ICON_FA_DATABASE, ImVec2(ButtonWidth, ButtonHeight)))
                {
                    Helper::AlphaAnimation = 0;
                    Helper::Tabs = 1;
                }
                ImGui::PopFont();

                ImGui::Spacing();

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - ButtonWidth) / 2.f - 4); // centering the Programs Button

                ImGui::PushFont(Helper::fonts.IconFont);
                if (ImGui::ButtonNew(ICON_FA_BELL, ImVec2(ButtonWidth, ButtonHeight)))
                {
                    Helper::AlphaAnimation = 0;
                    Helper::Tabs = 2;
                }
                ImGui::PopFont();

                ImGui::PopStyleVar();

                ImGui::EndChild();
            }
            
            ImGui::NextColumn();


            // Right Side
            {
                Tabs();
            }
            

            ImGui::PopFont();
        }
    }
    namespace Login
    {
        void* LoginProcess()
        {
            ImGui::InsertNotification({ ImGuiToastType_Info, 1000, std::string(("Logging in...")).c_str() });
            
            Helper::AlphaAnimation = 0, Helper::ShowMainPage = true, Helper::ShowLoginPage = false;

            return NULL;
        }

        void RenderLoginPage()
        {
            // upper section
            {
                static const ImVec2 CTRL_logo_size = { 380, 168 }; // logo size

                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) - (CTRL_logo_size.x / 2));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 45); // if you change the - to +, it will go lower. the bigger the number the higher/lower
                ImGui::Image(Helper::CTRLlogo, CTRL_logo_size);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 35); //reset lol

                ImGui::PushFont(Helper::fonts.afont);
                imguipp::center_text(("Thank you for choosing us!"), 0, false, true);

                ImGui::Spacing();

                imguipp::center_text(("Sign in to continue to CTRLS's Application"), 1, false);
                ImGui::PopFont();
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20); // Spacing between "CTRL" and username&password input text boxes.

            // Input text boxes
            {
                ImGui::PushFont(Helper::fonts.mainFont);

                Helper::InputTextWithHintCentered(("Username:"), ("Enter your Username"), Helper::Username, IM_ARRAYSIZE(Helper::Username), false);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

                if (Helper::InputTextWithHintCentered(("Password:"), ("Enter your Password"), Helper::Password, IM_ARRAYSIZE(Helper::Password), true,
                    ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    CreateThread(0, 0, (PTHREAD_START_ROUTINE)LoginProcess, 0, 0, 0);
                }

                const float input_width = ImGui::CalcItemWidth();
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - input_width) / 2.f); // centering the Login Button
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 35);

                if (ImGui::Button(("Login"), ImVec2(input_width, 40)))
                {
                    CreateThread(0, 0, (PTHREAD_START_ROUTINE)LoginProcess, 0, 0, 0);
                }

                ImGui::PopFont();
            }
        }
    }
}