#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 theColor;
out vec2 texCoord;

uniform mat4 transform;
uniform mat4 projection;

void main(){
	gl_Position = projection * transform * vec4(aPos.x, aPos.y, 0.0, 1.0);
	theColor = aColor;
	texCoord = aTexCoord;
}