std::string drone_frag = R"(
#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec3 viewFragPos;
    vec3 viewNormal;
    vec3 Tangent;
} fs_in;

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;
  float intensity;

 // vec3 ambient;
  vec3 diffuse;
  //vec3 specular;
};

uniform sampler2D diffuseTexture;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;
uniform sampler2D emissiveMap;
uniform samplerCube reflectionMap;

uniform vec3 lightPos;
uniform float ambientStrength;
uniform float batteryPercentage;
uniform vec3 viewPos;
uniform vec3 objectColour;
uniform vec3 sunColour;
uniform vec3 emissiveColour;
uniform float sunIntensity;
uniform bool lightRim;
uniform bool reflective;
uniform bool hasNormalMap;
uniform bool hasSpecularMap;
uniform bool hasEmissiveMap;

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 2.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
      for (int y = -1; y <= 1; ++y) {
        float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
      }
    }
    shadow /= 10.0;
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : ambientStrength;
 //   float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    if(projCoords.z > 1.0)
        shadow = ambientStrength;

    return shadow;
}

vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 FragPos, vec3 viewDir, float intensity, inout float shadow)
{
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, norm);
 // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  //vec3 ambient = light.ambient * vec3(texture(diffuseTexture, fs_in.TexCoords));
  vec3 diffuse = diff * light.diffuse * vec3(texture(diffuseTexture, fs_in.TexCoords));

  vec3 specular = vec3(0.0, 0.0, 0.0);
  if (hasSpecularMap) {
//    specular = diff * 100 * vec3(texture(specularMap, fs_in.TexCoords));
    specular = diff * texture(specularMap, fs_in.TexCoords).rgb;
  }

 // if (distance <= 10.0) {
    //diffuse = vec3(1.0, 0.0, 0.0);
    shadow -= attenuation;
    if (shadow < 0.0) {
      shadow = 0.0;
    }
  //}

 // ambient *= attenuation;
 // diffuse *= attenuation;
 // specular *= attenuation;

  //return (ambient + diffuse + specular);
  return (diffuse + specular) * intensity;
}

void main()
{           
    float reflectionAmount = 0.1;
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb * objectColour;
  //  vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
  //  normal = normalize(normal * 2.0 - 1.0);
    vec3 normal = normalize(fs_in.Normal);
    if (hasNormalMap) {
      normal = texture(normalMap, fs_in.TexCoords).rgb;
      normal = normalize(normal * 2.0 - 1.0);
    }
    vec3 viewNorm = normalize(fs_in.viewNormal);
    // ambient
    vec3 ambient = ambientStrength * sunColour * (sunIntensity);
    // diffuse
    vec3 alightPos = vec3(lightPos.xyz);
//    alightPos.x *= 1000.0;
//    alightPos.y *= 1000.0;
//    alightPos.z *= 1000.0;
    vec3 lightDir = normalize(alightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * sunColour * (sunIntensity);
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if (hasSpecularMap) {
      spec = 1.0;
    }
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 1.0);
    vec3 specular = spec * sunColour * (sunIntensity);    
    if (hasSpecularMap) {
      specular = diff * texture(specularMap, fs_in.TexCoords).rgb * sunColour * (sunIntensity);    
      //specular = diff * texture(specularMap, fs_in.TexCoords).rgb;
    }
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);   
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
     // if (pointLights[i]) {
        diffuse += CalculatePointLight(pointLights[i], normal, fs_in.FragPos, viewDir, pointLights[i].intensity, shadow);
      //}
      //diffuse += vec3(1.0, 0.0, 0.0);
    }

    if (lightRim) {
      vec3 eye = normalize(-fs_in.viewFragPos.xyz);
      vec3 n = normalize(fs_in.Normal);
      vec3 coltest = vec3(0.5, 0.2, 0.0);
      float rimIntensity = 0.1;
      float rim = 1.0 - max(dot(viewDir, n), 0.0);
      rim = smoothstep(0.6, 1.0, rim);
      rim *= rimIntensity;
      coltest = vec3(rim * coltest.r, rim * coltest.g, rim * coltest.b);
      diffuse *= coltest;
      ambient += coltest;
    }
    vec4 reflectiveColour = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 ref = vec3(1.0, 1.0, 1.0);
    if (reflective)
    {
      float ratio = 1.00 / 1.52;
      vec3 I = normalize(fs_in.FragPos - viewPos);
      vec3 R = reflect(I, normalize(fs_in.Normal));
      vec3 RF = refract(I, normalize(fs_in.Normal), ratio);
      ref = texture(reflectionMap, RF).rgb;
      reflectiveColour = vec4(texture(reflectionMap, R).rgb, 1.0);
    } else {
      reflectionAmount = 0.0;
    }
    vec3 emission = texture(emissiveMap, fs_in.TexCoords).rgb;
    float batteryBarSize = 1346 - 1139;
    float batteryEndPosition = 1139 + (batteryPercentage * batteryBarSize);
    if ((fs_in.TexCoords.y * 2048) > 1139 && (fs_in.TexCoords.y * 2048) < batteryEndPosition
    && fs_in.TexCoords.x * 2048 > 32 && fs_in.TexCoords.x * 2048 < 78) {
      emission = emissiveColour;
      //color = vec3(0.0, 0.0, 0.0);
    } 
   if ((fs_in.TexCoords.y * 2048) > batteryEndPosition && (fs_in.TexCoords.y * 2048) < 1346
    && fs_in.TexCoords.x * 2048 > 32 && fs_in.TexCoords.x * 2048 < 78) {
      emission = vec3(0.0, 0.0, 0.0);
      color = vec3(0.0, 0.0, 0.0);
    } 

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color * sunIntensity;
    if (hasEmissiveMap) {
      lighting = (ambient + (1.0 - shadow) * (diffuse + specular + emission)) * color * sunIntensity;
    }

    FragColor = mix(vec4(lighting, 1.0), reflectiveColour, reflectionAmount);
}
)";
