#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glm/detail/qualifier.hpp"
#define GL_SILENCE_DEPRECATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "GameManager.hpp"

#define DEBUG true

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, int button, int action, int mod);
void process_input(GLFWwindow *window, float dt);

const unsigned int SCREEN_WIDTH = 1000;
const unsigned int SCREEN_HEIGHT = 600;

GameManager *Astrolabe;

int main() {
  // Setup GLFW window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
  // glfwWindowHint(GLFW_RESIZABLE, false);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                        "Astrolabe", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to init GLAD\n";
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetMouseButtonCallback(window, mouse_callback);

  // OpenGL configuration
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // ImGui configuration
#ifdef DEBUG
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init(glsl_version);
#endif

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  Astrolabe = new GameManager(SCREEN_WIDTH, SCREEN_HEIGHT);
  Astrolabe->Init();

  while (!glfwWindowShouldClose(window)) {
    float currentTime = static_cast<float>(glfwGetTime());
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;


    glfwPollEvents();
    process_input(window, deltaTime);

    Astrolabe->Update(deltaTime);

    glClearColor(0.03f, 0.01f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Astrolabe->Render();

    if (DEBUG) {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      ImGui::Begin("Debug menu"); 
      ImGui::Text("FPS: %.3f", io.Framerate);
      ImGui::End();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    glfwSwapBuffers(window);
  }

  delete Astrolabe;
#ifdef DEBUG
  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();
#endif

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  Astrolabe->Width = width;
  Astrolabe->Height = height;
}

void process_input(GLFWwindow *window, float dt) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    Astrolabe->ProcessKeyAction(GLFW_KEY_W, dt);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    Astrolabe->ProcessKeyAction(GLFW_KEY_A, dt);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    Astrolabe->ProcessKeyAction(GLFW_KEY_S, dt);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    Astrolabe->ProcessKeyAction(GLFW_KEY_D, dt);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  Astrolabe->ProcessScrollAction(yoffset, glfwGetTime());
}

void mouse_callback(GLFWwindow *window, int button, int action, int mod) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  Astrolabe->ProcessMouseAction(button, action, mod, xpos, ypos);
}
