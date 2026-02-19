std::string tex_frag_nolit_shader_src = R"(
#version 460 core
out vec4 FragColour;

uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform vec3 objectColour;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoords;
in vec4 weightColour;

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_diffuse2;
  sampler2D texture_diffuse3;
  sampler2D texture_specular1;
  sampler2D texture_specular2;
  sampler2D texture_specular3;
  vec3 ambient;
  float shininess;
};

uniform Material material;

void main()
{
  FragColour = texture(material.texture_diffuse1, texCoords);
  // FragColour = vec4(0.f, 0.f, 0.f, 1.0);
  //FragColour = weightColour;
}
)";
