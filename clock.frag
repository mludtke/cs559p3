#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform ivec2 size;

vec2 resolution = vec2(size.y, size.y); 

float R = resolution.y;		// Screen 'DPI'

// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// Switched to screen space distance by Trisomie21

float distanceToSegment( vec2 a, vec2 b, vec2 p, float t )
{
	vec2 pa = p - a;
	vec2 ba = b - a;
	float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
	float d = length( pa - ba*h );
	
	// Screen space distance
	return clamp((t-.5*d)*R, 0., 1.);
}

void main(void)
{
	// get time
	float secs = mod( floor(time),        60.0 );
	float mins = mod( floor(time/60.0),   60.0 )+5.;
	float hors = mod( floor(time/3600.0), 24.0 );
	

	vec2 uv = -1.0 + 2.0*gl_FragCoord.xy / (resolution.xy) ;
	uv.y *= resolution.x/resolution.x;


	
	float r = length( uv ); //radius
	float a = atan( uv.y, uv.x )+3.1415926;
    
	// background color
	vec3 nightColor = vec3( 0.2, 0.2, 0.2 ) + 0.1*uv.y;
	vec3 dayColor   = vec3( 0.5, 0.6, 0.7 ) + 0.2*uv.y;
	vec3 col = mix( nightColor, dayColor, smoothstep( 5.0, 7.0, hors ) - 
				                          smoothstep(19.0,21.0, hors ) );
	// inner watch body	
	col = mix( col, vec3(0.9-0.4*pow(r,4.0)), 1.0-smoothstep(0.94,0.95,r) );

	// hours & minute marks	
	float u, h, m;
	u = r * 6.2831 / 12.; //(radius * 2pi / 12(number of hours)
	h = fract(a*r/u +.5)-.5;
	h = .004-.5*abs(h*u);
	h *= (1.-step(r, .84))*step(r, .95);
	
	u = r * 6.2831 / 60.; //(radius * 2pi / 60(number of minutes
	m = fract(a*r/u +.5)-.5;
	m = .0025-.5*abs(m*u);
	m *= (1.-step(r, .89))*step(r, .95);
	col = mix( col, vec3(0.0), max(m*R, 0.)+max(h*R, 0.) );
	
	// seconds hand
	vec2 dir;
	dir = vec2( sin(6.2831*secs/60.0), cos(6.2831*secs/60.0) );
	float f = distanceToSegment( vec2(0.0), dir*0.9, uv, .005);
	col = mix( col, vec3(0.5,0.0,0.0), f );

	// minutes hand
	dir = vec2( sin(6.2831*mins/60.0), cos(6.2831*mins/60.0) );
	f = distanceToSegment( vec2(0.0), dir*0.7, uv, .01 );
	col = mix( col, vec3(0.0), f );

	// hours hand
	dir = vec2( sin(6.2831*hors/12.0), cos(6.2831*hors/12.0) );
	f = distanceToSegment( vec2(0.0), dir*0.4, uv, .015 );
	col = mix( col, vec3(0.0), f );

	// center mini circle	
	col = mix( col, vec3(0.5), clamp((.055-r)*R, 0., 1.) );
	col = mix( col, vec3(0.0), clamp((.004-.5*abs(.055-r))*R, 0., 1.) );

	// border of watch
	col = mix( col, vec3(0.0), clamp((.01-.5*abs(r-.95))*R, 0., 1.) );

	gl_FragColor = vec4( col,1.0 );
}
