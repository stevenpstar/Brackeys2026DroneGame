std::string ray_vertex = R"(
#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = model * view * projection * vec4(aPos, 1.0);
}
)";
