#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "GameManager.hpp"

#define DEBUG true

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void process_mouse_input(GLFWwindow *window);
void process_input(GLFWwindow *window, float dt);

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

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

  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

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

  // OpenGL configuration
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // ImGui configuration
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char *glsl_version = "#version 150";
  ImGui_ImplOpenGL3_Init(glsl_version);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  float timeConsideringSpeed = 0.0f;
  float timeSpeed = 1.0f;

  Astrolabe = new GameManager(SCREEN_WIDTH, SCREEN_HEIGHT);
  Astrolabe->Init();

  while (!glfwWindowShouldClose(window)) {
    float currentTime = static_cast<float>(glfwGetTime());
    deltaTime = timeSpeed * (currentTime - lastFrame);
    lastFrame = currentTime;
    timeConsideringSpeed += deltaTime;

    glfwPollEvents();
    process_input(window, deltaTime);
    process_mouse_input(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Astrolabe->Update(deltaTime);
    Astrolabe->Render();
    Astrolabe->DrawUI();

    if (DEBUG) {
      ImGui::Begin("Debug menu");
      ImGui::Text("Absolute Time: %.2f", glfwGetTime());
      ImGui::Text("Time: %.2f", timeConsideringSpeed);
      ImGui::Text("FPS: %.3f", io.Framerate);
      ImGui::SliderFloat("Time Speed", &timeSpeed, 0.5f, 10.0f);
      if (ImGui::Button("Reset time speed")) {
        timeSpeed = 1.0f;
      }
      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  delete Astrolabe;

  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  Astrolabe->ChangeResolution(width, height);
}

void process_input(GLFWwindow *window, float dt) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  bool shift = false;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    shift = true;
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    Astrolabe->ProcessKeyAction(GLFW_KEY_W, dt, shift);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    Astrolabe->ProcessKeyAction(GLFW_KEY_A, dt, shift);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    Astrolabe->ProcessKeyAction(GLFW_KEY_S, dt, shift);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    Astrolabe->ProcessKeyAction(GLFW_KEY_D, dt, shift);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  Astrolabe->ProcessScrollAction(yoffset, glfwGetTime());
}

void process_mouse_input(GLFWwindow *window) {
  ImGuiIO &io = ImGui::GetIO();

  if (!io.WantCaptureMouse) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
      Astrolabe->ProcessMouseAction(
          GLFW_MOUSE_BUTTON_LEFT,
          glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT), xpos, ypos);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
      Astrolabe->ProcessMouseAction(
          GLFW_MOUSE_BUTTON_RIGHT,
          glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT), xpos, ypos);
    }
  }
}
