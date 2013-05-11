/*********************************************
*	Constants
*
*	Constants used for colors and materials
*
*
*********************************************/


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
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;  
  GLfloat shiny;
};

/* list of materials */
const Materials materials[] = {
  { 
	glm::vec3(0.329412, 0.223529, 0.027451),
    glm::vec3(0.780392, 0.568627, 0.113725),
    glm::vec3(0.992157, 0.941176, 0.807843),
    27.8974f
  },
  { 
    glm::vec3(0.2125, 0.1275, 0.054),
    glm::vec3(0.714, 0.4284, 0.18144),
    glm::vec3(0.393548, 0.271906, 0.166721),
    25.6f
  },
  { 
    glm::vec3(0.25, 0.148, 0.06475),
    glm::vec3(0.4, 0.2368, 0.1036),
    glm::vec3(0.774597, 0.458561, 0.200621),
    76.8f
  },
  {
    glm::vec3(0.25, 0.25, 0.25),
    glm::vec3(0.4, 0.4, 0.4),
    glm::vec3(0.774597, 0.774597, 0.774597),
    76.8f
  },
  { 
    glm::vec3(0.19125, 0.0735, 0.0225),
    glm::vec3(0.7038, 0.27048, 0.0828),
    glm::vec3(0.256777, 0.137622, 0.086014),
    12.8f
  },
  { 
    glm::vec3(0.2295, 0.08825, 0.0275),
    glm::vec3(0.5508, 0.2118, 0.066),
    glm::vec3(0.580594, 0.223257, 0.0695701),
    51.2f
  },
  { 
    glm::vec3(0.24725, 0.21995, 0.0745),
    glm::vec3(0.75164, 0.60648, 0.22648),
    glm::vec3(0.628281, 0.555802, 0.366065),
    51.2f
  },
  { 
    glm::vec3(0.24725, 0.2245, 0.0645),
    glm::vec3(0.34615, 0.3143, 0.0903),
    glm::vec3(0.797357, 0.723991, 0.208006),
    83.2f
  },
  { 
    glm::vec3(0.105882, 0.058824, 0.113725),
    glm::vec3(0.427451, 0.470588, 0.541176),
    glm::vec3(0.333333, 0.333333, 0.521569),
    9.84615f
  },
  { 
    glm::vec3(0.19225, 0.19225, 0.19225),
    glm::vec3(0.50754, 0.50754, 0.50754),
    glm::vec3(0.508273, 0.508273, 0.508273),
    51.2f
  },
  { 
    glm::vec3(0.23125, 0.23125, 0.23125),
    glm::vec3(0.2775, 0.2775, 0.2775),
    glm::vec3(0.773911, 0.773911, 0.773911),
    89.6f
  },
  { 
    glm::vec3(0.0215, 0.1745, 0.0215),
    glm::vec3(0.07568, 0.61424, 0.07568),
    glm::vec3(0.633, 0.727811, 0.6335),
    76.8f
  },
  { 
    glm::vec3(0.135, 0.2225, 0.1575),
    glm::vec3(0.54, 0.89, 0.63),
    glm::vec3(0.316228, 0.316228, 0.316228),
    12.8f
  },
  { 
    glm::vec3(0.05375, 0.05, 0.06625),
    glm::vec3(0.18275, 0.17, 0.22525),
    glm::vec3(0.332741, 0.328634, 0.346435),
    38.4f
  },
  { 
    glm::vec3(0.25, 0.20725, 0.20725),
    glm::vec3(1.0, 0.829, 0.829),
    glm::vec3(0.296648, 0.296648, 0.296648),
    11.264f
  },
  { 
    glm::vec3(0.1745, 0.01175, 0.01175),
    glm::vec3(0.61424, 0.04136, 0.04136),
    glm::vec3(0.727811, 0.626959, 0.626959),
    76.8f
  },
  { 
    glm::vec3(0.1, 0.18725, 0.1745),
    glm::vec3(0.396, 0.74151, 0.69102),
    glm::vec3(0.297254, 0.30829, 0.306678),
    12.8f
  },
  { 
    glm::vec3(0.0, 0.0, 0.0),
    glm::vec3(0.01, 0.01, 0.01),
    glm::vec3(0.50, 0.50, 0.50),
    32.0f
  },
  { 
    glm::vec3(0.02, 0.02, 0.02),
    glm::vec3(0.01, 0.01, 0.01),
    glm::vec3(0.4, 0.4, 0.4),
    10.0f
  }
};