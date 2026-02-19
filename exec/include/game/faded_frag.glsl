std::string faded_frag = R"(
#version 460 core
out vec4 FragColour;
void main()
{
  FragColour = vec4(0.0, 0.0, 0.0, 0.8);
}
)";
