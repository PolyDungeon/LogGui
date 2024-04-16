#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "userAuth_windows.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <unordered_set>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

using namespace std;

std::vector<std::string> getUsersInGroups(const std::vector<std::string> &groups)
{
    std::ifstream group_file("/etc/group");
    if (!group_file.is_open())
    {
        std::cerr << "Error opening /etc/group" << std::endl;
        return {};
    }

    std::unordered_set<std::string> target_groups(groups.begin(), groups.end());
    std::unordered_set<std::string> users;

    std::string line;
    while (std::getline(group_file, line))
    {
        std::stringstream ss(line);
        std::string group_name, password, gid;
        std::vector<std::string> user_list;

        // Parse group information
        std::getline(ss, group_name, ':');
        std::getline(ss, password, ':');
        std::getline(ss, gid, ':');
        std::getline(ss, line); // Read remaining user list

        // Split user list
        std::stringstream user_list_stream(line);
        std::string user;
        while (std::getline(user_list_stream, user, ','))
        {
            user_list.push_back(user);
        }

        // Check if group matches target groups
        if (target_groups.count(group_name))
        {
            // Add users to result set
            for (const std::string &user : user_list)
            {
                users.insert(user);
            }
        }
    }

    group_file.close();
    return std::vector<std::string>(users.begin(), users.end());
}

void DisplayGroups(std::vector<std::string> &userz)
{

    ImGui::BeginChild("GroupWindow");
    if (userz.empty())
    {
        ImGui::TextWrapped("No users found in target groups.");
    }
    else
    {
        ImGui::Text("Users in adm, root, or sudo groups:");
        for (const std::string &user : userz)
        {
            ImGui::Text("%s", user.c_str());
        }
    }
    ImGui::EndChild();
}

std::vector<std::string> getUsernames()
{
    std::vector<std::string> usernames;
    std::ifstream passwd_file("/etc/passwd");
    if (!passwd_file.is_open())
    {
        std::cerr << "Error opening /etc/passwd" << std::endl;
        return {};
    }
    std::string line;

  while (std::getline(passwd_file, line, '\n')) {
    // Extract username (part before first colon)
    std::string username;
    std::size_t colon_pos = line.find(':');
    if (colon_pos != std::string::npos) {
      username = line.substr(0, colon_pos);

      // Extract specific information (between second and third colons)
      std::string info;
      std::size_t second_colon = line.find(':', colon_pos + 1);
      if (second_colon != std::string::npos) {
        std::size_t third_colon = line.find(':', second_colon + 1);
        if (third_colon != std::string::npos) {
            std::size_t fourth_colon = line.find(':', third_colon + 1);
            if (fourth_colon != std::string::npos) {
                std::size_t fifth_colon = line.find(':', fourth_colon + 1);
                if (fifth_colon != std::string::npos)
                {
                    info = line.substr(fourth_colon + 1, fifth_colon - fourth_colon - 1);
                }
                
            }
        }
      }

      // Combine username and info (if any)
      usernames.push_back(username + (info.empty() ? ": no additional info" : ": \"" + info + "\""));
    }
  }

  return usernames;

}

void DisplayUsers(std::vector<std::string> &usernamez)
{
    ImGui::BeginChild("UsersWindow");
    if (usernamez.empty())
    {
        ImGui::Text("No usernames found.");
    }
    else
    {
        ImGui::Text("Current Users:");
        for (const std::string &username : usernamez)
        {
            ImGui::Text("%s", username.c_str());
        }
    }
    ImGui::EndChild();
}