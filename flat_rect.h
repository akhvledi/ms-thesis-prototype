
//This class handles attributes and motions of a virtual
// rectangle, no rendering involved

#ifndef FLAT_RECT
#define FLAT_RECT
// C++ Headers
#include <string>
#include <iostream>

#include <math.h>


#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>



#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct gl_line
	{
		GLfloat p1[2];
		GLfloat p2[2];

		gl_line(int x1, int y1, int x2, int y2, float sx, float sy)
		{
			p1[0] = -1 + (x1 * sx);
			p1[1] = 1 - (y1 * sy);
			p2[0] = -1 + (x2 * sx);
			p2[1] = 1 - (y2 * sy);
		}
	};

struct triattributes 
	{ 	
		GLfloat coord2d[2]; 	
		GLfloat v_color[4]; 

		~triattributes() {
			}
	};



struct rect2D
{
	triattributes triangle_attributes[4];

	int pixelCoords[4][2];

	bool active = false;

	bool border = false;
	GLfloat borderCol[4] = {0.0, 0.0, 0.0, 1.0};


	rect2D()	{ 

	}
	
	
};

#endif

