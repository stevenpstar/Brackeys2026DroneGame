std::string rgb_tri_frag = R"(
#version 460 core
out vec4 FragColour;

uniform vec3 colour;
uniform float alpha;

void main()
{
  //FragColour = texture(uitexture, texCoords);// * vec4(1.0, 1.0, 1.0, 1.0);
  FragColour = vec4(colour.rgb, alpha);
}
)";
