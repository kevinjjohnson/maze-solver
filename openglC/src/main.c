#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "include/shader.h"
#include "include/texture.h"
#include "include/batch_renderer.h"
#include "include/quad.h"
#include "include/camera.h"


typedef struct {
    int size[2];
    char* name;
} Settings;



int main(void) {
    GLFWwindow* window; 
    Settings s;
    s.name = "i hate bug\0";
    s.size[0] = 1600;
    s.size[1] = 900;
    
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(s.size[0], s.size[1], s.name, NULL, NULL);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW INIT ERROR");
    }
    glClearColor(0.16f, 0.16f, 0.16f, 1.0f);


    uint32_t vertexShader;
    vertexShader = create_shader("resources/shaders/firstVertex.shader", GL_VERTEX_SHADER);
    
    uint32_t fragmentShader;
    fragmentShader = create_shader("resources/shaders/firstFragment.shader", GL_FRAGMENT_SHADER);
     
    uint32_t shaderProgram;
    shaderProgram = create_shader_program(vertexShader, fragmentShader);


    //mat4 ortho;
    //glm_ortho(0.0f, 1600.0f, 0.0f, 900.0f, -1.0f, 1.0f, ortho);

    camera cam;
    init_camera(&cam);



    uint32_t projection = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, (float*)cam.view_projection_matrix);

    uint32_t texture1, texture2;
    texture1 = load_texture("resources/textures/home.png", GL_TEXTURE_2D, 0);

    texture2 = load_texture("resources/textures/bruno.png", GL_TEXTURE_2D, 1);
    uint32_t tex = glGetUniformLocation(shaderProgram, "textures");
    int samplers[2] = { 0, 1 };
    glUniform1iv(tex, 2, samplers);


    batch_renderer renderer;
    init_batch_renderer(&renderer);

    quad q1;
    init_quad(&q1, (vec2) { 100, 100 }, (vec2) { 100, 100 }, 0);
    quad q2;
    init_quad(&q2, (vec2) { 500, 500 }, (vec2) { 800, 450 }, 0);
    quad q3;
    init_quad(&q3, (vec2) { 200, 200 }, (vec2) {300, 400 }, 1);

    add_quad(&renderer, q1);
    add_quad(&renderer, q2);
    add_quad(&renderer, q3);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.num_draw_calls = 0;
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            move_camera(&cam, (vec2) {0.0f, 2.0f});
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            move_camera(&cam, (vec2) { 0.0f, -2.0f });
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            move_camera(&cam, (vec2) { -2.0f, 0.0f });
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            move_camera(&cam, (vec2) { 2.0f, 0.0f });
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            printf("should be zoomin");
            zoom_camera(&cam, cam.zoom - .01);
        }
        

        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindTextureUnit(0, texture1);
        glBindTextureUnit(1, texture2);
        uint32_t tex = glGetUniformLocation(shaderProgram, "textures");
        int samplers[2] = { 0, 1 };
        glUniform1iv(tex, 2, samplers);
        
        calculate_view_projection_matrix(&cam);
        projection = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, (float*)cam.view_projection_matrix);
        //add_quad(&renderer, q1);
        //add_quad(&renderer, q2);
        //add_quad(&renderer, q3);

        int row, col;
        float offset = 60;
        float size = 80;
        for (col = 0; col < 10; col++) {
            for (row = 0; row < 10; row++) {
                quad q;
                init_quad(&q, (vec2) { size, size }, (vec2) { (col * size) + offset, (row * size) + 60.0 }, col % 2);
                add_quad(&renderer, q);
            }
        }


        draw_batch(&renderer);
        flush_renderer(&renderer);
        printf("number of draw calls this frame: %d\n", renderer.num_draw_calls);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    
}