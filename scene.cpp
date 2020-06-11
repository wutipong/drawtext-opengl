#include "scene.hpp"

std::unique_ptr<Scene> Scene::currentScene;
std::unique_ptr<Scene> Scene::nextScene;

void Scene::TickCurrent(Context &context) {
  if (currentScene == nullptr)
    return;
  currentScene->Tick(context);
}

void Scene::DrawUICurrent(Context &context) {

  if (nextScene != nullptr) {

    if (currentScene != nullptr)
      currentScene->Cleanup(context);

    currentScene = std::move(nextScene);
    nextScene.reset();
  }

  if (currentScene == nullptr)
    return;

  currentScene->DrawUI(context);
}