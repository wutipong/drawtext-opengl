// dear imgui: standalone example application for SDL2 + OpenGL
// If you are new to dear imgui, see examples/README.txt and documentation at
// the top of imgui.cpp. (SDL is a cross-platform general purpose library for
// handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation,
// etc.) (GL3W is a helper library to access OpenGL functions since there is no
// standard header to access modern OpenGL functions easily. Alternatives are
// GLEW, Glad, etc.)

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "scene.hpp"
#include "test_font_scene.hpp"

#include <SDL.h>
#include <stdio.h>

#include <ft2build.h>
#include FT_FREETYPE_H

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load
//  OpenGL function pointers. Helper libraries are often used for this purpose!
//  Here we are supporting a few common ones (gl3w, glew, glad). You may use
//  another loader/header of your choice (glext, glLoadGen, etc.), or chose to
//  manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h> // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h> // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h> // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE // GLFW including OpenGL headers causes ambiguity or
                          // multiple definition errors.
#include <glbinding/Binding.h> // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE // GLFW including OpenGL headers causes ambiguity or
                          // multiple definition errors.
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h> // Initialize with glbinding::initialize()
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Main code
int main(int, char **) {
  // Setup SDL
  // (Some versions of SDL before <2.0.10 appears to have performance/stalling
  // issues on a minority of Windows systems, depending on whether
  // SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version
  // of SDL is recommended!)
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
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

  constexpr int windowWidth = 1600;
  constexpr int windowHeight = 900;

  SDL_Window *window = SDL_CreateWindow(
      "DrawText sample using OpenGL, FreeType2, Harfbuzz.",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, window_flags);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
  bool err = false;
  glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
  bool err = false;
  glbinding::initialize([](const char *name) {
    return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name);
  });
#else
  bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader
                    // is likely to requires some form of initialization.
#endif
  if (err) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    return 1;
  }

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

  constexpr int viewportWidth = 1280;
  constexpr int viewportHeight = 720;

  SDL_Rect viewport{
      windowWidth/2 - viewportWidth/2, 
      windowHeight/2  - viewportHeight /2, 
      viewportWidth, viewportHeight
  };

  context.windowWidth = viewportWidth;
  context.windowHeight = viewportHeight;

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

    glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
    glEnable(GL_SCISSOR_TEST);
    glScissor(viewport.x, viewport.y, viewport.w, viewport.h);
    
    Scene::TickCurrent(context);
    glViewport(0, 0, windowWidth, windowHeight);
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