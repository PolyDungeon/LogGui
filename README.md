# LogGui
## INTRODUCTION
LogGui is a linux log manager that makes viewing the complicated and bloated text logs much more managable to the average person. 
Linux log files are valuable resources for system administrators monitoring system performance as they provide a detailed record of actions taken on the system, which users executed the actions, and what the result of the actions was. By analyzing the output, system administrators ensure that security is being maintained, the system is not compromised, and that the system is operating as intended. 
## BACKGROUND
The authorization log, or auth log for short, is the file where Linux based systems store records of authorization events in the system. It is plain text which means it is human readable. It stores successful and unsuccessful login attempts and authentication events, such as sudo command calls. It can be found at /var/log/auth.log for Debian systems and /var/log/secure for Redhat and CentOS [1]. During our initial research, we found some similar projects that others have created [2, 3, 4]. While we drew some inspiration from these projects, we developed a unique tool for analyzing the authorization log.
The auth log is difficult to fully read and understand by hand. Even though it is in plain text format, it can contain thousands of lines of events. Many of these lines can occur within the same minute. It can be difficult to monitor the authorization events of a system just by looking at the auth log. The goal of our project was to make a visualization of the auth log to increase the likelihood that suspicious activity is noticed.  We decided to make several visualizations and text windows to summarize the auth log. These visualizations include authorization events and login attempts. The text windows include sudo command calls, users, groups, and flagged events. 
## METHODOLOGY
In order to ease the process of reading log files, and to ensure that administrators have easy access to the important information, we identified various types of log file entries to monitor. The metrics we chose are: Authorization events such as signing in as a different user, attempting to use sudo, or changing a users password, Login events and whether they were successful, as well as implementing a system that detects if flags events based on criteria that makes the action appear suspicious, such as the action taking place outside of normal business operating hours. We feel that these metrics offer system administrators an easier alternative to finding logs that may interest them as opposed to manually combing through the log file searching for suspicious activity. Additionally, we provide several visuals to allow the user to more easily interpret the information presented. 

## Credit:
LogGui was created as part of a class projecct by Hayden Copeland, Holly Ammell, Lindsey Fry, and Parker Hundley.
LogGui is largely built using Ocornut's "ImGui" credit to him where it is due: https://github.com/ocornut/imgui 

## How to Build
### Linux and similar Unixes

Use our Makefile 
- access location of file using terminal and simply run: "Make all"
Will likely need to update you path scripts inside the makefile to fit your system 

or directly:

```
c++ `sdl2-config --cflags` -I .. -I ../.. -I ../../backends
  main.cpp ../../backends/imgui_impl_sdl2.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp
  `sdl2-config --libs` -lGL -ldl
```
# Images

![LogGui6](https://github.com/PolyDungeon/LogGui/assets/114162108/aa68bebf-2c08-479d-b4a2-14983ab764c9)
![LogGui5](https://github.com/PolyDungeon/LogGui/assets/114162108/5b53f884-2085-471e-90a0-b4effd88cf28)
![LogGui4](https://github.com/PolyDungeon/LogGui/assets/114162108/adbb81aa-2943-4a6d-bafb-c862a4adc9d9)
![LogGui3](https://github.com/PolyDungeon/LogGui/assets/114162108/2a761d96-5939-4996-bc2d-6a0d0869077e)
![LogGui2](https://github.com/PolyDungeon/LogGui/assets/114162108/e6a15095-3f62-41fd-92b0-d379ead21c25)
![LogGui1](https://github.com/PolyDungeon/LogGui/assets/114162108/a7b697b6-412d-4e16-a638-5b090c53c8f8)


