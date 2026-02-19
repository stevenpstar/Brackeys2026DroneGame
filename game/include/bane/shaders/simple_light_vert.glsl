std::string simple_light_vert = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 5) in vec3 aTangent;

//out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec3 viewFragPos;
    vec3 viewNormal;
    vec3 Tangent;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform sampler2D heightMap;
uniform sampler2D heightMap2;
uniform bool hasHeightMap;
uniform bool hasHeightMap2;
uniform float heightIntensity;
uniform float height2Intensity;

void main()
{
   vec3 position = aPos;
   if (hasHeightMap) {
      vec4 heightData = texture(heightMap, aTexCoords);
      float height = texture2D(heightMap, aTexCoords).r;
      position = vec3(aPos.x, aPos.y + height * heightIntensity, aPos.z);
    }

   if (hasHeightMap2) {
      vec4 heightData = texture(heightMap2, aTexCoords);
      float height = texture2D(heightMap2, aTexCoords).r;
      position += vec3(0.0, position.y + height * height2Intensity, 0.0);
    }

    vs_out.FragPos = vec3(model * vec4(position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    vs_out.viewNormal = vec3(model * view * vec4(aNormal, 1.0));
    vs_out.viewFragPos = vec3(model * view * vec4(position, 1.0));
    //vs_out.Tangent = aTangent * model;

    gl_Position = projection * view * model * vec4(position, 1.0);
   
}
)";
