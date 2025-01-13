#pragma once
#include "cglm/cglm.h"

typedef struct {
	float x;
	float y;
}point;

typedef struct {
	float x;
	float y;
}edge;

typedef struct {
	vec2 size;
	vec2 location;
	point corners[4];
	edge edges[4];
	uint32_t texture;
} quad;

void init_quad(quad* q, vec2 size, vec2 location, uint32_t texture);