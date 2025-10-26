#include "common.hh"

#include "constants.hh"

#include <glm/mat4x4.hpp>
#include <iostream>

/**
 * Can be useful if your hardware supports ARB_debug_output.
 * If you have compile problems with this, just comment it out.
 */
void APIENTRY common::glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::string s = "";
    std::string t = "";
    std::string lvl = "";

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        s = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        s = "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        s = "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        s = "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        s = "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        s = "Other";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        t = "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        t = "Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        t = "Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        t = "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        t = "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        t = "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        t = "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        t = "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        t = "Other";
        break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        lvl = "High";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        lvl = "Medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        lvl = "Low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        lvl = "Info";
        return;
    }

    std::cerr << "[Error][" << s << "][" << t << "][" << lvl << "][" << id << "]: " << message << std::endl;
}


/**
 * Creates a window and returns it. Returns nullptr on failure
 */
GLFWwindow* common::createWindow(int windowWidth, int windowHeight, const char* name)
{
    //    glfwWindowHint(GLFW_SAMPLES, 8);

    // request an OpenGL debug context for:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OGL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OGL_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, name, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "[Error] Failed to open a GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowTitle(window, name);

    return window;
}

/**
 * GLFW Errors if we did something stupid
 */
void common::error_callback(int error, const char* description) { std::cout << "[Error] " << error << " occured: " << description << std::endl; }

/**
 * Init function. Returns false if something went wrong. If there is an issue here, the rest will not work
 */
bool common::init(GLFWwindow* window)
{
    if (!window)
    {
        {
            std::cerr << "Unable to get OpenGL " << OGL_MAJOR << "." << OGL_MINOR << " Core Debug Context. Trying again with Compat." << std::endl;
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            window = glfwCreateWindow(1, 1, "GLFW Window", nullptr, nullptr);

            if (!window)
            {
                std::cerr << "Unable to create a GLFW window" << std::endl;
                glfwTerminate();
                return -1;
            }
        }
        std::cerr << "[Error] Creation of window failed. Terminating." << std::endl;
        glfwTerminate();
        return -1;
    }


    if (!gladLoadGL())
    {
        std::cerr << "[Error] Init of glad failed. Terminating." << std::endl;
        glfwTerminate();
        return false;
    }


    /// register callback for error stuff
    glfwSetErrorCallback(error_callback);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    //glDebugMessageCallback(glDebugOutput, nullptr);
    //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // report opengl version
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);


    // report context flags
    GLint flags, profile;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    std::string context = "Unknown";
    switch (profile)
    {
    case GL_CONTEXT_CORE_PROFILE_BIT:
        context = "Core";
        break;
    case GL_CONTEXT_COMPATIBILITY_PROFILE_BIT:
        context = "Compatibility";
        break;
    }
    context += ", Debug (with callback)";

    if (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
        context += ", Forward Compatible";

    auto const* gpuName = glGetString(GL_RENDERER);
    std::cout << "Loaded OpenGL " << major << "." << minor << " Context [" << context << "]"
              << " on " << gpuName << std::endl;


    return true;
}

/**
 * GLFW Window Resize Callback.
 */
void common::resizeCallback(GLFWwindow*, int newWidth, int newHeight)
{
    // define the part of the screen OpenGL should draw to (in pixels):
    glViewport(0, 0, newWidth, newHeight);
}

/**
 * check shaderprogram compile errors
 */
void common::checkCompileErrors(GLuint shader)
{
    constexpr auto length = 512;
    GLint success;
    char infoLog[length];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, length, nullptr, infoLog);
        std::cout << "[Error][Shader][Compile] " << infoLog << std::endl;
        exit(-1);
    }
}

/**
 * check shaderprogram linker error
 */
void common::checkLinkErrors(GLuint program)
{
    constexpr auto length = 512;
    GLint success;
    char infoLog[length];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, length, nullptr, infoLog);
        std::cout << "[Error][Shader][Link] " << infoLog << std::endl;
        exit(-1);
    }
}

void common::printMat4(glm::mat4 const& m, const char* name)
{
    std::cout << name << ":\n"
              << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << "\n"
              << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << "\n"
              << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << "\n"
              << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << "\n"
              << std::endl;
}
