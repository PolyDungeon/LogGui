#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "logins_window.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

using namespace std;

string getUsername(const string& str) {
    istringstream iss(str);
    string username;
    for (int i = 0; i < 3; ++i) {
        iss >> username;
    }
    iss >> username;
    return username;
}

string getRepeatedUsers(const vector<string>& users) {
    unordered_map<string, int> count;
    string returnStr;

    for (const string& u : users) {
        count[u]++;
    }

    for (const auto& pair : count) {
        if (pair.second > 3) {
            if (!returnStr.empty()) {
                returnStr += ", ";
            }
            returnStr += pair.first;
        }
    }
    return returnStr;
}

void logins_window()
{
    ifstream log;
    // log.open("/var/log/auth.log");
    log.open("/home/dhcopel/Documents/CPSC4240/LogGui/examplelog.txt");
    ImGui::BeginChild("logins window!");
    //ImGui::SetWindowSize(ImVec2(500,200));

    int flag_count = 1;
    string month;
    string day;
    string time_stamp;
    string name;
    string command;
    string rest_of_line;

    int success_count = 0;
    int failed_count = 0;
    vector<string> users;

    if (log.is_open())
    {
        while (!log.eof())
        {
            log >> month;
            log >> day;
            log >> time_stamp;
            log >> name;
            log >> command;
            getline(log >> ws, rest_of_line);

            if (rest_of_line.find("Accepted password") != string::npos)
            {

                success_count++;
            }
            else if (rest_of_line.find("Failed password") != string::npos)
            {
                failed_count++;
                string user = getUsername(rest_of_line);
                users.push_back(user);
            }
        }
    }

    int max_val = success_count + failed_count;
    ImGui::Text("Total Login Attempts: %i", max_val);
    ImGui::Text("");

    ImGui::Text("Successes");
    ImGui::SameLine();
    ImGui::ProgressBar(static_cast<float>(success_count) / max_val);
    ImGui::Text("Total Successful Login Attempts: %i", success_count);

    ImGui::Text("");

    ImGui::Text("Failures");
    ImGui::SameLine();
    ImGui::ProgressBar(static_cast<float>(failed_count) / max_val);
    ImGui::Text("Total Failed Login Attempts: %i", failed_count);
    ImGui::Text("");
    ImGui::Text("Users with more than 3 failed logins: %s", getRepeatedUsers(users).c_str());

    log.close();
    ImGui::EndChild();
}
