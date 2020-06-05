#pragma once

#include "scene.hpp"

class TestScene : public Scene {
public:
  virtual bool Init(Context &context);
  virtual void Tick(Context &context);
  virtual void DrawUI(Context &context);
  virtual void Cleanup(Context &context);

private:
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;
  unsigned int texture;

  unsigned int vertexArray;
  unsigned int vertexBuffer;
};
