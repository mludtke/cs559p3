#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_normal;
layout (location = 3) in vec2 texture_coordinates;

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform mat3 projection_matrix;

out vec3 LightIntensity;
out vec3 FrontColor;
out vec3 BackColor;
out vec2 TexCoord;

struct LightInfo {
  vec4 position;  // Light position in eye coords.
  vec3 la;        // Ambient light intensity  
  vec3 ld;        // Diffuse light intensity
  vec3 ls;        // Specular light intensity
}; uniform LightInfo light;

struct MaterialInfo {
  vec3 ka;  // Ambient reflectivity
  vec3 kd;  // Diffuse reflectivity
  vec3 ks;  // Specular reflectivity
  float shininess;  // Specular shininess factor
}; uniform MaterialInfo material;

void getEyeSpace( out vec3 norm, out vec4 position )
{
  norm = normalize( normal_matrix * vertex_normal);
  position = modelview_matrix * vec4(vertex_position,1.0);
}

vec3 phongModel( vec4 position, vec3 norm )
{
  vec3 s = normalize(vec3(light.position - position));
  vec3 v = normalize(-position.xyz);
  vec3 r = reflect( -s, norm );
  vec3 ambient = light.la * material.ka;
  float sDotN = max( dot(s,norm), 0.0 );
  vec3 diffuse = light.ld * material.kd * sDotN;
  vec3 spec = vec3(0.0);
  if( sDotN > 0.0 )
    spec = light.ls * material.ks *
    pow( max( dot(r,v), 0.0 ), material.shininess );
  return ambient + diffuse + spec;
}

//const vec3 light_position = vec3(0.0, 0.0, 0.0);

void main()
{
  vec3 eyeNorm;
  vec4 eyePosition;
  TexCoord = texture_coordinates;
  // Get the position and normal in eye space
  getEyeSpace(eyeNorm, eyePosition);
  // Evaluate the lighting equation.
  LightIntensity = phongModel( eyePosition, eyeNorm );  
  vec3 tnorm = normalize(normal_matrix * vertex_normal);
  vec4 eyeCoords = modelview_matrix * vec4(vertex_position, 1.0);
  FrontColor = phongModel( eyeCoords, tnorm );
  BackColor  = phongModel( eyeCoords, -tnorm );
  gl_Position = mvp * vec4(vertex_position, 1.0);
}

