std::string cube_map_frag = R"(
#version 460 core
out vec4 FragColour;

in vec3 TexCoords;
uniform samplerCube skybox;

void main()
{
  FragColour = texture(skybox, TexCoords);
}
)";
