#version 400

layout (location = 0) out vec4 FragColor;

in vec3 LightIntensity;
in vec3 FrontColor;
in vec3 BackColor;
in vec2 TexCoord;
uniform sampler2D picture;
uniform float time;

void main()
{
  const float scale = 15.0;
  float multiplier;
  multiplier = cos(time);
  multiplier = clamp(multiplier, 0.1, 0.9);
  bvec2 toDiscard = greaterThan( fract(TexCoord * scale ), vec2(0.9,0.9) * multiplier );
  if( all(toDiscard) )
		discard;
  if( gl_FrontFacing ) 
  {
    //FragColor = vec4(FrontColor, 1.0);
	FragColor = vec4(LightIntensity, 1.0);
	}
 else 
 {
    FragColor = mix( vec4(BackColor,1.0), vec4(FrontColor,1.0), 0.7 );
 }
}
