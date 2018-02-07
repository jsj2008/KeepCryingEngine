#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 model_view;
uniform mat4 transform;
uniform vec4 ourColor;

void main()
{
	gl_Position = projection * model_view * transform * vec4(position, 1.0f);
	gl_FrontColor = ourColor;
}