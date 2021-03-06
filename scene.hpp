#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL2/SDL.h>
#include <memory>

#include "context.hpp"

class Scene {
public:
  virtual bool Init(Context &context) = 0;
  virtual void Tick(Context &context) = 0;
  virtual void DrawUI(Context &context) = 0;
  virtual void Cleanup(Context &context) = 0;

  static void TickCurrent(Context &context);
  static void DrawUICurrent(Context &context);
  static std::unique_ptr<Scene> &Current() { return currentScene; }

  template <class T> static void ChangeScene(Context &context);

private:
  static std::unique_ptr<Scene> currentScene;
  static std::unique_ptr<Scene> nextScene;
};

template <class T> void Scene::ChangeScene(Context &context) {
  auto newScene = std::make_unique<T>();

  if (!newScene->Init(context))
    return;

  nextScene = std::move(newScene);
}

#endif
