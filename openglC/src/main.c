#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "include/shader.h"
#include "include/texture.h"
#include "include/batch_renderer.h"
#include "include/quad.h"
#include "include/camera.h"
#include "include/maze.h"

typedef struct {
    int size[2];
    char* name;
} Settings;

int main(void) {
    GLFWwindow* window; 
    Settings s;
    s.name = "batch renderer demo\0";
    s.size[0] = 1600;
    s.size[1] = 900;
    
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(s.size[0], s.size[1], s.name, NULL, NULL);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(0);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW INIT ERROR");
    }
    glClearColor(0.16f, 0.16f, 0.16f, 1.0f);

    camera cam;
    init_camera(&cam);

    batch_renderer renderer;
    init_batch_renderer(&renderer);

    uint32_t texture1, texture2, texture3;
    texture1 = load_texture("resources/textures/white.png", GL_TEXTURE_2D, 0);

    texture2 = load_texture("resources/textures/black.png", GL_TEXTURE_2D, 1);
    
    texture3 = load_texture("resources/textures/green.png", GL_TEXTURE_2D, 2);

    uint32_t tex = glGetUniformLocation(renderer.shader_program, "textures");
    int samplers[3] = { 0, 1, 2 };
    glUniform1iv(tex, 3, samplers);

    float delta_time = 0.0f;
    float last_frame = 0.0f;
    float cam_speed = 200.0f;


    printf("\n");
    maze m;
    init_maze(&m, 7);
    print_maze(&m);
    printf("\n\n");
    //random_sort_walls(&m);
    //for (int it = 0; it < m.walls_size; it++) {
        //printf("%d ", m.walls[it]);
    //}
    generate_maze(&m);
    print_maze(&m);
    //print_disjoint_set(&(m.open_cells));
    printf("\n");

    
    bool solved = false;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.num_draw_calls = 0;
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            move_camera(&cam, (vec2) {0.0f, cam_speed * delta_time});
            calculate_view_projection_matrix(&cam);

        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            move_camera(&cam, (vec2) { 0.0f, -cam_speed * delta_time});
            calculate_view_projection_matrix(&cam);

        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            move_camera(&cam, (vec2) { -cam_speed * delta_time, 0.0f });
            calculate_view_projection_matrix(&cam);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            move_camera(&cam, (vec2) { cam_speed* delta_time, 0.0f });
            calculate_view_projection_matrix(&cam);

        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            printf("should be zoomin");
            zoom_camera(&cam, cam.zoom - .01);
            calculate_view_projection_matrix(&cam);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            printf("should be zoomin");
            zoom_camera(&cam, cam.zoom + .01);
            calculate_view_projection_matrix(&cam);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (!solved) {
                solve_maze(&m, m.size - 2, m.size * m.size - m.size + 1);
                solved = true;
            }
        }
        glUseProgram(renderer.shader_program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);

        glBindTextureUnit(0, texture1);
        glBindTextureUnit(1, texture2);
        glBindTextureUnit(2, texture3);
        uint32_t tex = glGetUniformLocation(renderer.shader_program, "textures");
        int samplers[2] = { 0, 1 };
        glUniform1iv(tex, 2, samplers);
        
        uint32_t projection = glGetUniformLocation(renderer.shader_program, "projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, (float*)cam.view_projection_matrix);

        int row, col;
        float offset = 60;
        float size = 80;
        float border_width = 10;
        /*
        for (col = 0; col < 10; col++) {
            for (row = 0; row < 10; row++) {
                quad q;
                init_quad(&q, (vec2) { size, size }, (vec2) { (col * size) + offset, (row * size) + 60.0 }, col % 2);
                add_quad(&renderer, q);
            }
        }
        */
        /*
        for (col = 0; col < 10; col++) {
            for (row = 0; row < 10; row++) {
                quad q;
                init_quad(&q, (vec2) { size, size }, (vec2) { (border_width * (col + 1)) + (col * size) + offset, (border_width * (row + 1)) + (row * size) + offset }, col % 2);
                add_quad(&renderer, q);
            }
        }
        */
        /*
        for (int i = 0; i < 10; i++) {
            quad q;
            init_quad(&q, (vec2) { size, size }, (vec2) { (border_width * (i + 1)) + (i * size) + offset, offset }, 0);
            add_quad(&renderer, q);
        }
        */
        render_maze(&renderer, &m, 10);
        draw_batch(&renderer);
        flush_renderer(&renderer);
        //printf("number of draw calls this frame: %d\n", renderer.num_draw_calls);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
