std::string zombie_frag = R"(
#version 460 core
out vec4 FragColour;

uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform vec3 objectColour;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoords;

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

struct DirLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform Material material;
uniform float redness;

vec3 CalculateDirectionalLight(DirLight light, vec3 norm, vec3 viewDir)
{
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(norm, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
  vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, texCoords));
  //vec3 specular = spec * light.specular * vec3(texture(material.texture_specular1, texCoords));
  vec3 specular = vec3(0.0, 0.0, 0.0);

  return (ambient + diffuse);
}

vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 FragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
  vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, texCoords));
  vec3 specular = spec * light.specular * vec3(texture(material.texture_specular1, texCoords));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

void main()
{
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 result = CalculateDirectionalLight(dirLight, norm, viewDir);
 // for (int i = 0; i < NR_POINT_LIGHTS; i++)
 // {
 //   result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);
 // }
  result += vec3(redness, 0.0, 0.0);
  FragColour = vec4(result * objectColour, 1.0);
}
)";
