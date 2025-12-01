/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                                                                           *
 *                                                                           */
/*===========================================================================*/

#include "assignment.hh"

extern GLuint teapot_vbo;
extern GLuint teapot_ea;
extern glm::mat4 modelViewMatrix;
extern glm::mat4 projectionMatrix;

namespace
{
GLuint vs_a, fs_a, prog_a;
GLuint vs_b, fs_b, prog_b;
GLuint vs_c, fs_c, prog_c;

}


void task::drawScene(int scene, float runTime)
{
    // Create modelview matrix
    // First: Create transformation into eye-space
    // glm::lookAt needs the eye position, the center to look at and the up vector
    modelViewMatrix = glm::lookAt(eyePos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    // One round should take 10 seconds
    float angle = -2.f * M_PI * runTime / 10.f;

    glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix *= glm::scale(modelMatrix, glm::vec3(1.0f, 1.3f, 1.0f));

    modelViewMatrix = modelViewMatrix * modelMatrix;

    if (scene == 1)
    {
        glUseProgram(prog_a);
        setupShaderUniforms(prog_a, scene);
    }
    else if (scene == 2)
    {
        glUseProgram(prog_b);
        setupShaderUniforms(prog_b, scene);
    }
    else if (scene == 3)
    {
        glUseProgram(prog_c);
        setupShaderUniforms(prog_c, scene);
    }

    // Draw teapot
    glDrawElements(GL_TRIANGLES, g_numberOfTeapotTriangles * 3, GL_UNSIGNED_INT, (void*)0);
}

void task::initCustomResources()
{
    // create your ressources here, e.g. shaders, buffers,...

    // create the shaders:
    createShaderProgram(vs_a, fs_a, prog_a, std::string(CURR_DIR) + "/assignment06/shader_a.vsh", std::string(CURR_DIR) + "/assignment06/shader_a.fsh");
    createShaderProgram(vs_b, fs_b, prog_b, std::string(CURR_DIR) + "/assignment06/shader_b.vsh", std::string(CURR_DIR) + "/assignment06/shader_b.fsh");
    createShaderProgram(vs_c, fs_c, prog_c, std::string(CURR_DIR) + "/assignment06/shader_c.vsh", std::string(CURR_DIR) + "/assignment06/shader_c.fsh");
}

void task::deleteCustomResources()
{
    // don't forget to delete your OpenGL ressources (shaders, buffers, etc.)!

    glDeleteShader(vs_a);
    glDeleteShader(vs_b);
    glDeleteShader(vs_c);
    glDeleteShader(fs_a);
    glDeleteShader(fs_b);
    glDeleteShader(fs_c);
    glDeleteProgram(prog_a);
    glDeleteProgram(prog_b);
    glDeleteProgram(prog_c);
}
