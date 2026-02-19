std::string fbo_frag_simple = R"(
#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
  FragColor = texture(screenTexture, TexCoords);
}
)";
