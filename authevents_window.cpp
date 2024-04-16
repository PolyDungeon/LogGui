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
    ImGui::SetWindowSize(ImVec2(1050,800));
    int hour;
    string month;
    string day;
    string time_stamp;
    string name;
    string command;
    string rest_of_line;
    int lines = 0 ;
    int hourCount[24] = {0};
    if(log.is_open()){
        while(!log.eof()){
            lines++;
            log >> month;
            log >> day;
            log >> time_stamp;
            log >> name;
            log >> command;
            getline(log, rest_of_line);
            if (command.find("polkitd") != string::npos) {
                continue;
            }
                if(rest_of_line.find("passwd") != string::npos && !(rest_of_line.find("polkitd")!=string::npos)){
                string user;
                getline(log, rest_of_line); // Read the entire line after the command
                hour = stoi(time_stamp.substr(0,2));
                hourCount[hour]++;

                
                ImGui::Text("Line Number %d: On %s %s at %s User %s changed their password (%s)",lines,month.c_str(),day.c_str(), time_stamp.c_str(),name.c_str(), command.c_str());
                
                }else if (rest_of_line.find("su") != string::npos && !(rest_of_line.find("sudo" )!= string::npos)&& (!(rest_of_line.find("polkitd")!= string::npos))){
                getline(log, rest_of_line);
                size_t suPos = rest_of_line.find("su:");
                string afterCommand;
                if(suPos != string::npos){
                    size_t startPos = suPos+3;
                    while(startPos < rest_of_line.size() & isspace(rest_of_line[startPos])){
                            startPos++;
                    }
                    afterCommand = rest_of_line.substr(startPos);
                }
                hour = stoi(time_stamp.substr(0,2));
                hourCount[hour]++;
                
                ImGui::Text("Line Number %d: On %s %s at %s User %s attempted to use %s with result %s",lines,month.c_str(),day.c_str(), time_stamp.c_str(),name.c_str(), command.c_str(),afterCommand.c_str());
                
                
                }
                
            
            
        }
        int authCount = 0;
        for(int i = 0; i < 23; i++){

            authCount+= hourCount[i];
        }

         ImGui::Text("Total Auth Events: %d",authCount)   ; 
          float hourCountFloat[24];
    for (int i = 0; i < 23; i++) {
        hourCountFloat[i] = static_cast<float>(hourCount[i]);
    }
    
        ImGui::Text("Authorization Attempts By Hour");
          ImGui::PlotHistogram(" ",hourCountFloat,24,0,NULL,0.0f,10.0f,(ImVec2(1000,400)));
    ImGui::Text("00:00 01:00 02:00 03:00 04:00 05:00 06:00 07:00 08:00 09:00 10:00 11:00 12:00 13:00 14:00 15:00 16:00 17:00 18:00 19:00 20:00 21:00 22:00 23:00");
         
          
    }
    log.close();
    
    ImGui::End();
}
