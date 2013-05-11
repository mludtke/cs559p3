#version 400

layout (location = 0) out vec4 FragColor;

in vec3 LightIntensity;
in vec3 FrontColor;
in vec3 BackColor;

void main()
{
  if( gl_FrontFacing ) {
    FragColor = vec4(FrontColor, 1.0);
  }
 else {
    //FragColor = vec4(BackColor, 1.0);
    FragColor = mix( vec4(BackColor,1.0), vec4(FrontColor,1.0), 0.7 );
    //FragColor = mix( vec4(BackColor,1.0), vec4(FrontColor,1.0), 0.1 );
  }
}
