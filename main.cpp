#include <GL/gl3w.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include "scene.hpp"
#include "test_font_scene.hpp"

#include <SDL.h>
#include <stdio.h>

#include <ft2build.h>
#include FT_FREETYPE_H

int main(int, char **) {
  if (SDL_Init(SDL_INIT_EVERYTHING) !=
      0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  const char *glsl_version = "#version 450";
  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_WindowFlags window_flags = (SDL_WindowFlags)(
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  constexpr int windowWidth = 800;
  constexpr int windowHeight = 600;

  SDL_Window *window = SDL_CreateWindow(
      "DrawText sample using OpenGL, FreeType2, Harfbuzz.",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, window_flags);
  SDL_SetWindowMinimumSize(window, windowWidth, windowHeight);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  gl3wInit();

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImFontConfig config;
  config.MergeMode = true;

  io.Fonts->AddFontFromFileTTF("SourceSansPro-Regular.ttf", 20.0f);
  io.Fonts->AddFontFromFileTTF("Sarabun-Regular.ttf", 20.0f, &config,
                               io.Fonts->GetGlyphRangesThai());
  io.Fonts->AddFontFromFileTTF("mplus-1c-regular.ttf", 20.0f, &config,
                               io.Fonts->GetGlyphRangesJapanese());

  io.Fonts->Build();

  Context context;
  context.isDone = false;
  FT_Init_FreeType(&context.ftLibrary);

  Scene::ChangeScene<TestFontScene>(context);

  SDL_GetWindowSize(window, &context.windowWidth, &context.windowHeight);

  while (!context.isDone) {
    context.events.clear();
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);

      if (event.type == SDL_QUIT)
        context.isDone = true;

      context.events.push_back(event);
    }

    if (context.isDone) {
      break;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    Scene::DrawUICurrent(context);

    ImGui::Render();
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    Scene::TickCurrent(context);
    SDL_GetWindowSize(window, &context.windowWidth, &context.windowHeight);
    glViewport(0, 0, context.windowWidth, context.windowHeight);
    glDisable(GL_SCISSOR_TEST);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
  }

  Scene::Current()->Cleanup(context);
  FT_Done_FreeType(context.ftLibrary);

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}