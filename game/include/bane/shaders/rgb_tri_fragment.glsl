std::string rgb_tri_frag_shader_src = R"(
#version 460 core
out vec4 FragColour;

in vec3 colour;
in vec2 texCoords;

uniform sampler2D uitexture;

void main()
{
  FragColour = texture(uitexture, texCoords);// * vec4(1.0, 1.0, 1.0, 1.0);
 // FragColour = vec4(texCoords.x, texCoords.y, 0.0, 0.5);
}
)";
