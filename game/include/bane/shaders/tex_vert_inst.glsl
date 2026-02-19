std::string inst_shader = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

out vec3 position;
out vec2 texCoords;
out vec3 Normal;

uniform mat4[100] model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  Normal = mat3(transpose(inverse(model[gl_InstanceID]))) * aNormal;
  gl_Position = projection * view * model[gl_InstanceID] * vec4(aPos, 1.0);
  texCoords = aTexCoords;
}
)";
