#version 400

layout (location = 0) out vec4 FragColor;

in vec2 tc;
uniform sampler2D picture;

void main()
{
	//FragColor = vec4(tc, 0, 1);
	FragColor = texture(picture, tc);
}
