std::string anim_vert_shader = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;
layout (location = 5) in vec3 Tangents;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec3 viewFragPos;
    vec3 viewNormal;
    vec3 Tangent;
} vs_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBonesMatrices[100];

void main()
{
  vec4 totalPosition = vec4(0.0);
  for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
  {
    if (boneIds[i] < 0)
      continue;

    if (boneIds[i] >= MAX_BONES - 1)
    {
      totalPosition = vec4(aPos, 1.0);
      break;
    }

    vec4 localPos = finalBonesMatrices[boneIds[i]] * vec4(aPos, 1.0);
    totalPosition += localPos * weights[i];
  }
  vs_out.FragPos = vec3(model * totalPosition);
  vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
  vec4 localNormal = totalPosition * vec4(aNormal, 0.0);
  vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
  vs_out.TexCoords = aTexCoords; 
  vs_out.viewNormal = vec3(model * view * vec4(aNormal, 1.0));
  vs_out.viewFragPos = vec3(model * view * totalPosition);

  gl_Position = projection * view * model * totalPosition;

}
)";
