#pragma once

#include <glad/glad.h>
/// make sure inclusion of glad.h does not come after glfw

#include <GLFW/glfw3.h>

#include <glm/fwd.hpp>
#include <iostream>

namespace common
{
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/);

GLFWwindow* createWindow(int windowWidth, int windowHeight, const char* name);

void error_callback(int error, const char* description);

bool init(GLFWwindow* window);

void resizeCallback(GLFWwindow*, int newWidth, int newHeight);

void checkCompileErrors(GLuint shader);

void checkLinkErrors(GLuint program);

void printMat4(glm::mat4 const& m, const char* name);

}
