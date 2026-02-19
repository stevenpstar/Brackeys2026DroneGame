std::string shadow_anim_vert = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

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

  gl_Position = lightSpaceMatrix * model * totalPosition;
}
)";
