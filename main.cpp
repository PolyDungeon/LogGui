// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "flag_window.h"
#include "logins_window.h"
#include "authevents_window.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

//ignore this but
// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>

std::vector<std::string> users;
int count = 0;
int numDisplays = 5;
static int currentTabIndex = 0; // Currently selected tab index
void GetAuthLog();
void GetSudoers();
void DisplaySudoers();
void DisplayAuthLog();
void ImGuiSimpleTabBar();
void DisplayHome();
void SwitchTabs();
int width = 1280;
int height = 720;
// Main code
int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("LogGui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
    

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    //GetSudoers();
    GetAuthLog();
    //ImGui::SetNextWindowSize(ImVec2(window.width(), height));
    // Main loop
    bool done = false;
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!done)
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("LogGui");
            ImGui::SetWindowSize(ImVec2(width, height));
            ImGui::SetWindowPos(ImVec2(0,0));
            ImGuiSimpleTabBar();
            SwitchTabs();
            ImGui::End();
            //DisplaySudoers();
            //DisplayAuthLog();
        }
        //logins_window();
        //flag_window();


        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void ImGuiSimpleTabBar() {
  if (numDisplays <= 0) {
    return;
  }

  const bool isOpen = ImGui::BeginTabBar("##SimpleTabBar");
  if (!isOpen) {
    return;
  }

  const char* tabLabels[5] = {"Home", "Sudo Commands", "Auth Events", "Logins", "Flags"}; // Adjust labels and array size
  for (int i = 0; i < numDisplays; i++) {
    const bool isSelected = (currentTabIndex == i);
    if (ImGui::TabItemButton(tabLabels[i], isSelected)) {
      currentTabIndex = i;
    }
  }

  ImGui::EndTabBar();
}

void GetSudoers()
{
    FILE* sudoers_file = fopen("/etc/sudoers", "r");
    if(!sudoers_file)
    {
        ImGui::Text("Failed to open /etc/sudoers");
        ImGui::End();
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), sudoers_file))
    {
        line[strcspn(line, "\n")] = '\0';

        if(line[0] == '#' || strlen(line) == 0)
        {
            continue;
        }

        // Tokenize using strtok with a copy of the line
        char *token = strtok(strdup(line), " "); // Create and tokenize a copy
        while (token)
        {
            users.push_back(std::string(token)); // Store token as a string in vector
            token = strtok(NULL, " ");           // Subsequent tokens from the same copy
        }
        if(!token)
        {
            continue;
        }
        //ImGui::Text("Not Authorized %s", user);
        
        //ImGui::NewLine();
        //DisplayAuthLog();
        count++;
    }        
    fclose (sudoers_file);
}

void DisplaySudoers()
{
    //ImGui::Begin("Sudoers");
    ImGui::BeginChild("Sudo");
  // Print authorized users
  for (const std::string& user : users) {
    ImGui::Text("Authorized: %s", user.c_str());
  }
    ImGui::EndChild();
    //DisplayAuthLog();
    //ImGui::End();
}


struct LogEntry
{
    std::string timestamp;
    std::string username; 
    std::string message;
};

std::vector<LogEntry> LogEntries;
// Store recent log entries
std::vector<std::string> log_entries;

void GetAuthLog()
{
    FILE *log_file = fopen("/var/log/auth.log", "r");
        if (!log_file)
        {
            ImGui::Text("Failed to open /var/log/auth.log");
            ImGui::End();
            return;
        }
        // Regular expression for sudo attempts (improve based on your needs)
        const std::regex sudo_regex(R"(.*sudo.*)");

        // Read lines from the end of the file
        char line[256];
        while (fgets(line, sizeof(line), log_file))
        {
            // Remove trailing newline
            line[strcspn(line, "\n")] = '\0';

            // Skip comments and empty lines
            if (line[0] == '#' || strlen(line) == 0)
            {
                continue;
            }

            // Search for sudo using regex
            if (std::regex_search(line, sudo_regex))
            {
                log_entries.push_back(std::string(line));
            }
        }

        fclose(log_file);

}
void DisplayAuthLog()
{
    //ImGui::Begin("Auth Log");

  // Setup scrollable area
    ImGui::BeginChild("SudoLogList", ImVec2(0, 500), false, ImGuiWindowFlags_HorizontalScrollbar);

  // Display entries in reverse order
  for (int i = log_entries.size() - 1; i >= 0; i--) {
    const std::string& log_line = log_entries[i];

    // Basic parsing (improve parsing logic for better formatting)
    std::string formatted_message = log_line;
    size_t first_colon = log_line.find(':');
    if (first_colon != std::string::npos) {
      //formatted_message = fmt::format(
      //    "(%zu) %s - %s", i + 1, log_line.substr(0, first_colon).c_str(), log_line.substr(first_colon + 1).c_str());
        ImGui::Text( "%s - %s", log_line.substr(0, first_colon).c_str(), log_line.substr(first_colon + 1).c_str());
    }


  }

  ImGui::EndChild();
  //ImGui::End();
    
}

void DisplayHome() {
    ImGui::BeginChild("Sudo");
    ImGui::Text("Welcome to LogGui, a visual representation of your authorization log!");
    ImGui::Text("To get started, click on one of the tabs above.");
    ImGui::Text("The Sudo Commands tab contains information about recent sudo commands.");
    ImGui::Text("The Auth Events tab contains information about certain authorization events in the past 24 hours.");
    ImGui::Text("The Logins tab contains information about ssh login attempts");    
    ImGui::Text("The Flags tab contains flags for suspicious sudoer activity.");
    ImGui::EndChild();
}

void SwitchTabs()
{
    //ImGui::Begin("LogGui");
    switch (currentTabIndex)
    {
    case 0:
        //DisplaySudoers();
        DisplayHome();
        break;
    case 1:
        DisplayAuthLog();
        break;
    case 2:
        authevents_window();
        break;
    case 3:
        logins_window();
        break;
    case 4:
        flag_window();
    default:
        break;
    }
    //ImGui::End();
}