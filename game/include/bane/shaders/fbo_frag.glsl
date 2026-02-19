std::string fbo_frag = R"(
#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int size;

// note here
// pixel shader, to determine pixel area of fragment coord (I think) = floor((input/width) * (resolution * size))

void main()
{
  FragColor = texture(screenTexture, TexCoords);
  vec2 resolution = vec2(1920.0, 1080.0);
  float divisionsX = resolution.x / size;
  float divisionsY = resolution.y / size;
  float xOffset = floor((gl_FragCoord.x / resolution.x) * divisionsX);
  float yOffset = floor((gl_FragCoord.y / resolution.y) * divisionsY);
  float xStart = xOffset/divisionsX * resolution.x;
  float yStart = yOffset/divisionsY * resolution.y;
  float averageR = 0.0;
  float averageG = 0.0;
  float averageB = 0.0;
  float pixelIncrX = 1/resolution.x;
  float pixelIncrY = 1/resolution.y;
  for (int i = 0; i < size; ++i)
  {
    float xPix = (xStart / resolution.x) + (i * pixelIncrX);
    float yPix = 0.0;
    for (int j = 0; j < size; ++j) {
      yPix = (yStart / resolution.y) + (i * pixelIncrY);
      vec4 colour = texture(screenTexture, vec2(xPix, yPix));
      averageR += colour.r;
      averageG += colour.g;
      averageB += colour.b;
    }
  }

  vec3 averagedColour = (vec3(averageR, averageG, averageB)) / (size * size);
  FragColor = vec4(averagedColour.r, averagedColour.g, averagedColour.b, 1.0);
}
)";
