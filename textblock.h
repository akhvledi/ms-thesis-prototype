#ifndef TEXTBLOCK
#define TEXTBLOCK
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


//#include "atlas.h"
//#include "point.h"


//Used in atlas generation, 
//#include "./include/freetype2/ft2build.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define MAXWIDTH 1024




//OpenGL 'point' cordinate, need six per character
struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};



/*
The textblock struct stores the basic attributes of a text entity, 
thus allowing for them to be modified in real time as appropriate.


*/

struct textblock 
{
	//Inputs
	const char *text;

	int start_xpix;
	int start_ypix;
	//These are determined through rendering
	int final_xpix;
	int final_ypix;

	int line_height;	

	int font_scale;


	bool active = false;

	//red green blue alpha, changing this does not necessitate update
	GLfloat rgbA[4]= { 0, 0, 0, 1 };

	//Updated internally
	int block_size;

	point coords[6 * 1000];

	textblock(  std::string s, int x, int y, int fs) {
		//text = t; const char *t, 
		text = s.c_str();
		//new char[s.size()];
		//std::strcpy(text, s.c_str());

		start_xpix = x;
		start_ypix = y;
		final_xpix = x;
		final_ypix = y;
		
		font_scale = fs;
	}

	void setColor(float colorIn[4])
	{
		rgbA[0] = colorIn[0];
		rgbA[1] = colorIn[1];
		rgbA[2] = colorIn[2];
		rgbA[3] = colorIn[3];
	}


	~textblock() {
		delete text;
			}

};

#endif