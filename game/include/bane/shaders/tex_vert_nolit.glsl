std::string tex_vert_nolit_shader_src = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

out vec3 position;
out vec2 texCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBonesMatrices[100];

void main()
{
  vec4 totalPosition = vec4(0.0);
  for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
  {
    if (boneIds[i] == -1)
      continue;

    if (boneIds[i] >= MAX_BONES)
    {
      totalPosition = vec4(aPos, 1.0);
      break;
    }

    vec4 localPos = finalBonesMatrices[boneIds[i]] * vec4(aPos, 1.0);
    totalPosition += localPos * weights[i];
  }
  vec4 localNormal = totalPosition * vec4(aNormal, 0.0);
  Normal = mat3(transpose(inverse(model))) * aNormal;
  //Normal = (model * localNormal).xyz;
  //Normal = aNormal;
  gl_Position = projection * view * model * totalPosition;
  texCoords = aTexCoords;
}
)";
