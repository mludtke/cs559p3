#pragma once
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Colors */
const glm::vec3 RED     (1.0f, 0.0f, 0.0f);
const glm::vec3 GREEN   (0.0f, 1.0f, 0.0f);
const glm::vec3 BLUE    (0.0f, 0.0f, 1.0f);
const glm::vec3 YELLOW  (1.0f, 1.0f, 0.0f);
const glm::vec3 CYAN    (0.0f, 1.0f, 1.0f);
const glm::vec3 MAGENTA (1.0f, 0.0f, 1.0f);
const glm::vec3 BLACK   (0.0f, 0.0f, 0.0f);
const glm::vec3 GRAY    (0.5f, 0.5f, 0.5f);
const glm::vec3 WHITE   (1.0f, 1.0f, 1.0f);

/* Material Types */
#define MAT_BRASS              0
#define MAT_BRONZE             1
#define MAT_POLISHED_BRASS     2
#define MAT_CHROME             3
#define MAT_COPPER             4
#define MAT_POLISHED_COPPER    5
#define MAT_GOLD               6
#define MAT_POLISHED_GOLD      7
#define MAT_PEWTER             8
#define MAT_SILVER             9
#define MAT_POLISHED_SILVER   10
#define MAT_EMERALD           11
#define MAT_JADE              12
#define MAT_OBSIDIAN          13
#define MAT_PEARL             14
#define MAT_RUBY              15
#define MAT_TURQUOISE         16
#define MAT_BLACK_PLASTIC     17
#define MAT_BLACK_RUBBER      18

/* material properties structure */
struct Materials{   
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;  
  GLfloat shiny;
};

/* list of materials */
const Materials materials[] = {
  { 
	  glm::vec4(0.329412, 0.223529, 0.027451, 1.0),
    glm::vec4(0.780392, 0.568627, 0.113725, 1.0),
    glm::vec4(0.992157, 0.941176, 0.807843, 1.0),
    27.8974f
  },
  { 
    glm::vec4(0.2125, 0.1275, 0.054, 1.0),
    glm::vec4(0.714, 0.4284, 0.18144, 1.0),
    glm::vec4(0.393548, 0.271906, 0.166721, 1.0),
    25.6f
  },
  { 
    glm::vec4(0.25, 0.148, 0.06475, 1.0),
    glm::vec4(0.4, 0.2368, 0.1036, 1.0),
    glm::vec4(0.774597, 0.458561, 0.200621, 1.0),
    76.8f
  },
  {
    glm::vec4(0.25, 0.25, 0.25, 1.0),
    glm::vec4(0.4, 0.4, 0.4, 1.0),
    glm::vec4(0.774597, 0.774597, 0.774597, 1.0),
    76.8f
  },
  { 
    glm::vec4(0.19125, 0.0735, 0.0225, 1.0),
    glm::vec4(0.7038, 0.27048, 0.0828, 1.0),
    glm::vec4(0.256777, 0.137622, 0.086014, 1.0),
    12.8f
  },
  { 
    glm::vec4(0.2295, 0.08825, 0.0275, 1.0),
    glm::vec4(0.5508, 0.2118, 0.066, 1.0),
    glm::vec4(0.580594, 0.223257, 0.0695701, 1.0),
    51.2f
  },
  { 
    glm::vec4(0.24725, 0.21995, 0.0745, 1.0),
    glm::vec4(0.75164, 0.60648, 0.22648, 1.0),
    glm::vec4(0.628281, 0.555802, 0.366065, 1.0),
    51.2f
  },
  { 
    glm::vec4(0.24725, 0.2245, 0.0645, 1.0),
    glm::vec4(0.34615, 0.3143, 0.0903, 1.0),
    glm::vec4(0.797357, 0.723991, 0.208006, 1.0),
    83.2f
  },
  { 
    glm::vec4(0.105882, 0.058824, 0.113725, 1.0),
    glm::vec4(0.427451, 0.470588, 0.541176, 1.0),
    glm::vec4(0.333333, 0.333333, 0.521569, 1.0),
    9.84615f
  },
  { 
    glm::vec4(0.19225, 0.19225, 0.19225, 1.0),
    glm::vec4(0.50754, 0.50754, 0.50754, 1.0),
    glm::vec4(0.508273, 0.508273, 0.508273, 1.0),
    51.2f
  },
  { 
    glm::vec4(0.23125, 0.23125, 0.23125, 1.0),
    glm::vec4(0.2775, 0.2775, 0.2775, 1.0),
    glm::vec4(0.773911, 0.773911, 0.773911, 1.0),
    89.6f
  },
  { 
    glm::vec4(0.0215, 0.1745, 0.0215, 0.55),
    glm::vec4(0.07568, 0.61424, 0.07568, 0.55),
    glm::vec4(0.633, 0.727811, 0.633, 0.55),
    76.8f
  },
  { 
    glm::vec4(0.135, 0.2225, 0.1575, 0.95),
    glm::vec4(0.54, 0.89, 0.63, 0.95),
    glm::vec4(0.316228, 0.316228, 0.316228, 0.95),
    12.8f
  },
  { 
    glm::vec4(0.05375, 0.05, 0.06625, 0.82),
    glm::vec4(0.18275, 0.17, 0.22525, 0.82),
    glm::vec4(0.332741, 0.328634, 0.346435, 0.82),
    38.4f
  },
  { 
    glm::vec4(0.25, 0.20725, 0.20725, 0.922),
    glm::vec4(1.0, 0.829, 0.829, 0.922),
    glm::vec4(0.296648, 0.296648, 0.296648, 0.922),
    11.264f
  },
  { 
    glm::vec4(0.1745, 0.01175, 0.01175, 0.55),
    glm::vec4(0.61424, 0.04136, 0.04136, 0.55),
    glm::vec4(0.727811, 0.626959, 0.626959, 0.55),
    76.8f
  },
  { 
    glm::vec4(0.1, 0.18725, 0.1745, 0.8),
    glm::vec4(0.396, 0.74151, 0.69102, 0.8),
    glm::vec4(0.297254, 0.30829, 0.306678, 0.8),
    12.8f
  },
  { 
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    glm::vec4(0.01, 0.01, 0.01, 1.0),
    glm::vec4(0.50, 0.50, 0.50, 1.0),
    32.0f
  },
  { 
    glm::vec4(0.02, 0.02, 0.02, 1.0),
    glm::vec4(0.01, 0.01, 0.01, 1.0),
    glm::vec4(0.4, 0.4, 0.4, 1.0),
    10.0f
  }
};