#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "include/shader.h"
#include "include/texture.h"
#include "include/collison.h"

typedef struct {
    int size[2];
    char* name;
} Settings;

float verts[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
};
uint32_t indicies[] = {
    0, 1, 3,
    1, 2, 3
};


int main(void)
{
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
    vertexShader = createShader("resources/shaders/firstVertex.shader", GL_VERTEX_SHADER);
    

    uint32_t fragmentShader;
    fragmentShader = createShader("resources/shaders/firstFragment.shader", GL_FRAGMENT_SHADER);
     
    uint32_t shaderProgram;
    shaderProgram = createShaderProgram(vertexShader, fragmentShader);
    
    
    uint32_t vbo, vao, ebo;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    mat4 test;
    glm_mat4_identity(test);
    vec3 translation = { 800.0f, 450.0f, 0.0 };
    glm_translate(test, translation);
    vec3 axis = { 0.0, 0.0, 1.0 };
    //glm_rotate(test, glm_rad(90), axis);
    vec3 scale = { 700, 700, 1.0f };
    glm_scale(test, scale);

    uint32_t loc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)test);

    mat4 ortho;
    glm_ortho(0.0f, 1600.0f, 0.0f, 900.0f, -1.0f, 1.0f, ortho);

    uint32_t projection = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, (float*)ortho);

    vec2 testAxis = { 0.0f, 1.0f };
    point polygon1[4];
    polygon1[0].x = 1;
    polygon1[0].y = 1;
    polygon1[1].x = 3;
    polygon1[1].y = 1;
    polygon1[2].x = 3;
    polygon1[2].y = 4;
    polygon1[3].x = 1;
    polygon1[3].y = 4;

    point polygon2[4];
    polygon2[0].x = 2;
    polygon2[0].y = 3;
    polygon2[1].x = 9;
    polygon2[1].y = 3;
    polygon2[2].x = 9;
    polygon2[2].y = 6;
    polygon2[3].x = 2;
    polygon2[3].y = 6;

    //float min, max;
    //projectPolygon(testAxis, &points, 4, &min, &max);
    //printf("min is: %f\n", min);
    //printf("max is: %f\n", max);
    vec2 translationVec;
    bool collided = isColliding(polygon1, 4, polygon2, 4, &translationVec);
    if (collided) {
        printf("polygons collided!\n translation vector is: (%f, %f)\n", translationVec[0], translationVec[1]);
    }
    else {
        printf("polygons didnt collide!\n");
    }



    uint32_t texture;
    texture = loadTexture("resources/textures/test.png", GL_TEXTURE_2D);
    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}