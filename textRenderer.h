
// C++ Headers
#include <string>
#include <iostream>

#include <math.h>


#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include <map>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
//#include <GL/freeglut.h>
#include <stdio.h>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <ft2build.h>
#include FT_FREETYPE_H

//#include "../common/shader_utils.h"
#include "./shader_utils/shader_utils.h"

#include "textblock.h"
#include "atlas.h"
/*
**********************************************************************
 End of inclusions
**********************************************************************
*/

	/**
	 * The atlas struct holds a texture that contains the visible US-ASCII characters
	 * of a certain font rendered with a certain character height.
	 * It also contains an array that contains all the information necessary to
	 * generate the appropriate vertex and texture coordinates for each character.
	 *
	 * After the constructor is run, you don't need to use any FreeType functions anymore.
	 */ 
/*
struct atlas {

		GLuint tex;		// texture object
		
		unsigned int w;			// width of texture in pixels
		unsigned int h;			// height of texture in pixels

		struct {
			float ax;	// advance.x
			float ay;	// advance.y
			float bw;	// bitmap.width;
			float bh;	// bitmap.height;
			float bl;	// bitmap_left;
			float bt;	// bitmap_top;
			float tx;	// x offset of glyph in texture coordinates
			float ty;	// y offset of glyph in texture coordinates
		} c[128];		// character information

		 atlas(FT_Face face, GLint uniform_tex, int height) {
			FT_Set_Pixel_Sizes(face, 0, height);
			FT_GlyphSlot g = face->glyph;

			unsigned int roww = 0;
			unsigned int rowh = 0;
			 w = 0;
			 h = 0;

			 memset(c, 0, sizeof c);

			// Find minimum size for a texture holding all visible ASCII characters 
			for (int i = 32; i < 128; i++) {
				if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
					fprintf(stderr, "Loading character %c failed!\n", i);
					continue;
				}
				if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
					w = std::max(w, roww);
					h += rowh;
					roww = 0;
					rowh = 0;
				}
				roww += g->bitmap.width + 1;
				rowh = std::max(rowh, (unsigned int) g->bitmap.rows);
			}

			w = std::max(w, roww);
			h += rowh;

			//Create a texture that will be used to hold all ASCII glyphs 
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glUniform1i(uniform_tex, 0);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

			// We require 1 byte alignment when uploading texture data 
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// Clamping to edges is important to prevent artifacts when scaling 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Linear filtering usually looks best for text 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Paste all glyph bitmaps into the texture, remembering the offset 
			int ox = 0;
			int oy = 0;

			rowh = 0;

			for (int i = 32; i < 128; i++) {
				if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
					fprintf(stderr, "Loading character %c failed!\n", i);
					continue;
				}
				if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
					oy += rowh;
					rowh = 0;
					ox = 0;
				}
				glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
				c[i].ax = g->advance.x >> 6;
				c[i].ay = g->advance.y >> 6;
				c[i].bw = g->bitmap.width;
				c[i].bh = g->bitmap.rows;
				c[i].bl = g->bitmap_left;
				c[i].bt = g->bitmap_top;
				c[i].tx = ox / (float)w;
				c[i].ty = oy / (float)h;
				rowh = std::max(rowh, (unsigned int)g->bitmap.rows);
				ox += g->bitmap.width + 1;
			}

			fprintf(stderr, "Generated a %d x %d (%d kb) texture atlas\n", w, h, w * h / 1024);
		}

		~atlas() {
				glDeleteTextures(1, &tex);
			}
};

*/


/*
**********************************************************************
 Official text render class
**********************************************************************
*/

class textRenderer
{
	
	FT_Library ft;
	FT_Face face;

	const char *fontfilename = "lib/FreeSans.ttf";
	std::map<int,atlas*> freeFontMap;
	
	GLuint program;
	GLint attribute_coord;
	GLint uniform_tex;
	GLint uniform_color;
	GLint uniform_transform2d; 

	GLuint vbo;


public:

	//Default Constructor**********************************
	textRenderer() {

	}


	void localErrorReport(const char* error) {
		fprintf(stderr, "Error in textRenderer: \n");
		fprintf(stderr, error);
	}


	bool Init() {
	
		program = create_program("shaders/text.v.glsl", "shaders/text.f.glsl");
		if(program == 0)
			return 0;

		attribute_coord = get_attrib(program, "coord");
		uniform_tex = get_uniform(program, "tex");
		uniform_color = get_uniform(program, "color");

		if(attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1)
			return 0;		

		uniform_transform2d = glGetUniformLocation(program, "transform2d");
	 	if (uniform_transform2d == -1) { 		
	 		std::cout << "Could not bind uniform " << "transform2d" << std::endl; 		
	 		return false; 	
	 	} 	
		// Create the vertex buffer object
		glGenBuffers(1, &vbo);
		return 1;
	}



	bool initFont(std::map<int,atlas*> &fontMap, const char* fontFilename) {
		if (FT_Init_FreeType(&ft)) {
			localErrorReport("Could not init freetype library\n");
			return false;
		}

		if (FT_New_Face(ft, fontfilename, 0, &face)) {
			localErrorReport("Could not open font ");
			fprintf(stderr, "Could not open font %s\n", fontfilename);
			return 0;
		}

		fontMap.insert(std::make_pair( 12, new atlas(face, uniform_tex, 12)));
		fontMap.insert(std::make_pair( 16, new atlas(face, uniform_tex, 16)));
		fontMap.insert(std::make_pair( 20, new atlas(face, uniform_tex, 20)));
		fontMap.insert(std::make_pair( 24, new atlas(face, uniform_tex, 24)));
		fontMap.insert(std::make_pair( 32, new atlas(face, uniform_tex, 32)));
		fontMap.insert(std::make_pair( 48, new atlas(face, uniform_tex, 48)));

		return true;
	}



	void enableText() {
		glUseProgram(program);
		/* Enable blending, necessary for our alpha texture */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


	void free_resources() {
		glDeleteProgram(program);
	}


	void renderTextblock(textblock *b, atlas *a, GLfloat disp[2]) {
		
		glUniform2fv(uniform_transform2d, 1, disp);

		glUniform4fv(uniform_color, 1, b->rgbA);

		glEnableVertexAttribArray(attribute_coord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);


		/* Repeat this to make sure the correct atlas is being used*/
		glBindTexture(GL_TEXTURE_2D, a->tex);
		glUniform1i(uniform_tex, 0);

		
		/* Draw all the character on the screen in one go */
		glBufferData(GL_ARRAY_BUFFER, sizeof b->coords, b->coords, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, b->block_size);

		glDisableVertexAttribArray(attribute_coord);

	}


};