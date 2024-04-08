#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "flag_window.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

using namespace std;

void flag_window(){
    ifstream log;
    log.open("/var/log/auth.log");
    ImGui::Begin("flag window!");
    ImGui::SetWindowSize(ImVec2(600,700));

    int flag_count = 1;
    string month;
    string day;
    string time_stamp;
    string name;
    string command;
    string rest_of_line;

    if(log.is_open()){
        while(!log.eof()){
            log >> month;
            log >> day;
            log >> time_stamp;
            log >> name;
            log >> command;
            log.get();
            
            if(command.compare("sudo:") == 0){
                string user;
                log >> user;

                getline(log, rest_of_line);

                int found = rest_of_line.find_last_of('=');

                command = rest_of_line.substr(found + 1);

                string h = time_stamp.substr(0,2);
                int hour = stoi(h);
                if(hour >= 21 || hour < 7){
                    ImGui::Text("%d. User: %s executed a privledged command (%s) outside of buisness hours.", flag_count, user.c_str(), command.c_str());
                    flag_count++;
                }

                if(rest_of_line.find("authentication failure") != string::npos){
                    
                    int found = rest_of_line.find_last_of('=');

                    user = rest_of_line.substr(found + 1);

                    ImGui::Text("%d. Authentication Failure for User: %s", flag_count, user.c_str());
                    flag_count++;
                }
                else if(rest_of_line.find("user NOT in sudoers") != string::npos){
                    ImGui::Text("%d. %s tried privledged command. does not have permissions", flag_count, user.c_str());
                    flag_count++;
                }
            }
            
        }
    }
    log.close();
    ImGui::End();
}


