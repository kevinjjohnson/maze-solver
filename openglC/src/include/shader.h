#pragma once

uint32_t createShader(char* filepath, GLenum type);
uint32_t createShaderProgram(uint32_t vertexShader, uint32_t fragmentShader);