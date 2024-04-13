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

    string month;
    string day;
    string time_stamp;
    string name;
    string command;
    string rest_of_line;
    int lines = 0 ;
    if(log.is_open()){
        while(!log.eof()){
            lines++;
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


                
                ImGui::Text("Line Number %d: On %s %s at %s User %s changed their password (%s)",lines,month.c_str(),day.c_str(), time_stamp.c_str(),name.c_str(), command.c_str());
                ImGui::Text("Month: %s, Day: %s, Time: %s, Name: %s, Command: %s",month.c_str(),day.c_str(),time_stamp.c_str(),name.c_str(),command.c_str());
                }
                
            
            
        }
    }
    log.close();
    ImGui::End();
}
