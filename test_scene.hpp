#pragma once

#include "scene.hpp"

class TestScene : public Scene {
public:
  virtual bool Init(Context &context) { return true; };
  virtual void Tick(Context &context);
  virtual void DrawUI(Context &context);
  virtual void Cleanup(Context &context){};
};