std::string simple_pixel_frag = R"(
#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int size;
uniform float width;
uniform float height;

// note here
// pixel shader, to determine pixel area of fragment coord (I think) = floor((input/width) * (resolution * size))

void main()
{
  float divisions = width / size;
  FragColor = texture(screenTexture, TexCoords);
  vec2 resolution = vec2(width, height);
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  vec2 texUV = floor(uv * divisions) / divisions;
  FragColor = texture(screenTexture, texUV).rgba;
}
)";
