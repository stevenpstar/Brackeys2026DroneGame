std::string ui_frag_tint = R"(
#version 460 core
out vec4 FragColour;

in vec3 colour;
in vec2 texCoords;

uniform sampler2D uitexture;
uniform vec3 tint;
uniform float alpha;

void main()
{
  FragColour = texture(uitexture, texCoords) * vec4(tint.rgb, alpha);
}
)";
