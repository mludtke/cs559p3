/*
 colorful wavy effect obtained from http://glsl.heroku.com/e#8384.0

 edited by Faiz and Morgan to work with our shader class
*/


#version 400

layout (location = 0) out vec4 FragColor;

uniform ivec2 size;
uniform float time;

const float Pi = 3.14159;

void main()
{
	vec2 resolution = vec2(512, 512); 
	vec2 p=(2.0*gl_FragCoord.xy-resolution)/max(resolution.x,resolution.y);
	for(int i=1;i<100;i++)
	{
		vec2 newp=p; //the new position of the coordinates
		newp.x+=1.5/float(i)*sin(float(i)*p.y+time/400.0+0.3*float(i))+400./20.0;		 //move x pos
		newp.y+=0.05/float(i)*sin(float(i)*p.x+time/1.0+0.3*float(i+10))-400./20.0+15.0; //move y pos
		p=newp;
	}
	vec3 col=vec3((0.5*sin(3.0*p.x)+0.5) * 0.75, (0.5*sin(3.0*p.y)+0.5) * 0.75, sin(p.x+p.y) * 0.75); // color is coordinated to position
																									  // multiplied by 0.75 to darken
	gl_FragColor=vec4(col, 1.0);
}