#version 400

layout (location = 0) out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

struct LightInfo {
  vec4 position;
  vec3 intensity;
}; uniform LightInfo light;

struct MaterialInfo {
  vec3 ka;  // Ambient reflectivity
  vec3 kd;  // Diffuse reflectivity  
}; uniform MaterialInfo material;

const int levels = 5;
const float scaleFactor = 1.0 / levels;

vec3 toonShade( )
{
  vec3 s = normalize( light.position.xyz - Position.xyz );
  float cosine = max( 0.0, dot( s, Normal ) );
  vec3 diffuse = material.kd * floor( cosine * levels ) * scaleFactor;
  return light.intensity * (material.ka + diffuse);
}

void main() {
  FragColor = vec4(toonShade(), 1.0);
}