#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "authevents_window.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <string_view>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif


using namespace std;

void authevents_window(){
    ifstream log;
    log.open("/var/log/auth.log");
    ImGui::Begin("Auth Events window!");
    ImGui::SetWindowSize(ImVec2(1000,300));

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
            //log.get();
            getline(log, rest_of_line);
                if(rest_of_line.find("passwd") != std::string::npos){
                string user;
                getline(log, rest_of_line); // Read the entire line after the command

                // Extract the username from the line
                size_t found = rest_of_line.find_last_of(']');
                user = rest_of_line.substr(0, found + 1);

                int found_eq = rest_of_line.find_last_of('=');
                command = rest_of_line.substr(found_eq + 1);

                string h = time_stamp.substr(0,2);
                int hour = stoi(h);
                
                ImGui::Text("User: %s changed their password (%s)", user.c_str(), command.c_str());
                }
                
            
            
        }
    }
    log.close();
    ImGui::End();
}
