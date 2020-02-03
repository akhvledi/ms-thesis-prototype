
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
//#include <GL/freeglut.h>

//Using SDL and standard IO
//#include <SDL2/SDL.h>
#include <stdio.h>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "./shader_utils/shader_utils.h"


#include "flat_rect.h"


/*
struct triattributes 
	{ 	
		GLfloat coord2d[2]; 	
		GLfloat v_color[3]; 
	};

triattributes triangle_attributes[4];
*/

class rect2dRenderer
{


	public:

	
	triattributes *triangle_attributes[4];

	rect2dRenderer()
	{

	}

	bool Init()
	{
	
		glGenBuffers(1, &vbo_triangle); 	
		glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle); 	
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW); 	
	 	//glBufferData(GL_ARRAY_BUFFER, sizeof(recty), recty, GL_STATIC_DRAW); 	

		GLint link_ok = GL_FALSE; 

	 	GLuint vs, fs; 	
	 	if ((vs = create_shader("shaders/triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return false; 	
	 	if ((fs = create_shader("shaders/triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return false; 		

	 	triprogram = glCreateProgram(); 	
	 	glAttachShader(triprogram, vs); 	
	 	glAttachShader(triprogram, fs); 	

	 	glLinkProgram(triprogram); 	
	 	glGetProgramiv(triprogram, GL_LINK_STATUS, &link_ok); 	
	 	if (!link_ok) 
	 	{ 		
	 		std::cout << "glLinkProgram:"; 		
	 		print_log(triprogram); 		
	 		return false; 	
	 	} 		

	 	std::cout << "Did prog link?" << std::endl; 	

	 	const char* attribute_name; 	
	 	attribute_name = "coord2d"; 	
	 	attribute_coord2d = glGetAttribLocation(triprogram, attribute_name); 	
	 	if (attribute_coord2d == -1) { 		
	 		std::cout << "Could not bind attribute " << attribute_name << std::endl; 		
	 		return false; 	
	 	} 	

	 	attribute_name = "v_color"; 	
	 	attribute_v_colort = glGetAttribLocation(triprogram, attribute_name); 	
	 	if (attribute_v_colort == -1) { 		
	 		std::cout << "Could not bind attribute " << attribute_name << std::endl; 		
	 		return false; 	
	 	} 	


	 	uniform_transform2d = glGetUniformLocation(triprogram, "transform2d");
	 	if (uniform_transform2d == -1) { 		
	 		std::cout << "Could not bind uniform " << "transform2d" << std::endl; 		
	 		return false; 	
	 	} 	
	 	///attribute_name = 
	 	

	 	return true;

	}



	void enable()
	{
		glUseProgram(triprogram);
		glDisable(GL_BLEND);

		//glUniform1f(uniform_fade, 1.0);

		//glEnableVertexAttribArray(attribute_coord2d); 	
		//glEnableVertexAttribArray(attribute_v_colort); 	
		//glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle); 	

	}


	void disable()
	{
		//glDisableVertexAttribArray(attribute_coord2d); 	
		//glDisableVertexAttribArray(attribute_v_colort);		
	}


	void drawLine(gl_line *l)
	{
		glEnableVertexAttribArray(attribute_coord2d); 	
		glEnableVertexAttribArray(attribute_v_colort); 	
		glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle); 
		
		glVertexAttribPointer( 		
				attribute_coord2d,   // attribute 		
				2,                   // number of elements per vertex, here (x,y) 		
				GL_FLOAT,            // the type of each element 		
				GL_FALSE,            // take our values as-is 		
				sizeof(struct triattributes),  // next coord2d appears every 5 floats 		
				0                    // offset of first element 	
		); 	

		glVertexAttribPointer( 		
				attribute_v_colort,      // attribute 		
				4,                      // number of elements per vertex, here (r,g,b) 		
				GL_FLOAT,               // the type of each element 		
				GL_FALSE,               // take our values as-is 		
				sizeof(struct triattributes),  // stride 		
				//(GLvoid*) (2 * sizeof(GLfloat))     // offset of first element 		
				(GLvoid*) offsetof(struct triattributes, v_color)  // offset 	
		); 	

		triattributes lineRect[4]; 

		lineRect[0].coord2d[0]=l->p1[0];
		lineRect[0].coord2d[1]=l->p1[1];
		lineRect[1].coord2d[0]=l->p2[0];
		lineRect[1].coord2d[1]=l->p2[1];		
		
		for(int i = 0; i <4; i++)
		{
			lineRect[0].v_color[i]=0.0;
			lineRect[1].v_color[i]=0.0;
			lineRect[2].v_color[i]=0.0;
			lineRect[3].v_color[i]=1.0;		
		}


		glEnable(GL_LINE_SMOOTH);
		glBufferData(GL_ARRAY_BUFFER, sizeof lineRect, lineRect, GL_DYNAMIC_DRAW); 	
		glLineWidth(2.0f);
		glDrawArrays(GL_LINES, 0, 2); 	


		glDisableVertexAttribArray(attribute_coord2d); 	
		glDisableVertexAttribArray(attribute_v_colort); 
	

	}


	void render_triStrip(rect2D *rect, GLfloat disp[2])
	{

		glUniform2fv(uniform_transform2d, 1, disp);
		
		glEnableVertexAttribArray(attribute_coord2d); 	
		glEnableVertexAttribArray(attribute_v_colort); 	
		glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle); 
		
		glVertexAttribPointer( 		
				attribute_coord2d,   // attribute 		
				2,                   // number of elements per vertex, here (x,y) 		
				GL_FLOAT,            // the type of each element 		
				GL_FALSE,            // take our values as-is 		
				sizeof(struct triattributes),  // next coord2d appears every 5 floats 		
				0                    // offset of first element 	
		); 	

		glVertexAttribPointer( 		
				attribute_v_colort,      // attribute 		
				4,                      // number of elements per vertex, here (r,g,b) 		
				GL_FLOAT,               // the type of each element 		
				GL_FALSE,               // take our values as-is 		
				sizeof(struct triattributes),  // stride 		
				//(GLvoid*) (2 * sizeof(GLfloat))     // offset of first element 		
				(GLvoid*) offsetof(struct triattributes, v_color)  // offset 	
		); 	

		

		triattributes rectas[4] = {rect->triangle_attributes[0], 
									rect->triangle_attributes[1],
									rect->triangle_attributes[2],
									rect->triangle_attributes[3]};
		glBufferData(GL_ARRAY_BUFFER, sizeof rect->triangle_attributes, rect->triangle_attributes, GL_DYNAMIC_DRAW); 	
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 	
		
		
		if(rect->border) {
			//render_lineLoop(rect);
			triattributes lineloopRect[4]; 

			lineloopRect[0].coord2d[0]=rect->triangle_attributes[0].coord2d[0];
			lineloopRect[0].coord2d[1]=rect->triangle_attributes[0].coord2d[1];
			lineloopRect[1].coord2d[0]=rect->triangle_attributes[1].coord2d[0];
			lineloopRect[1].coord2d[1]=rect->triangle_attributes[1].coord2d[1];		
			lineloopRect[2].coord2d[0]=rect->triangle_attributes[3].coord2d[0];
			lineloopRect[2].coord2d[1]=rect->triangle_attributes[3].coord2d[1];
			lineloopRect[3].coord2d[0]=rect->triangle_attributes[2].coord2d[0];
			lineloopRect[3].coord2d[1]=rect->triangle_attributes[2].coord2d[1];


			for(int i = 0; i <4; i++)
			{
				lineloopRect[0].v_color[i]=rect->borderCol[i];
				lineloopRect[1].v_color[i]=rect->borderCol[i];
				lineloopRect[2].v_color[i]=rect->borderCol[i];
				lineloopRect[3].v_color[i]=rect->borderCol[i];		
			}


			glBufferData(GL_ARRAY_BUFFER, sizeof rect->triangle_attributes, lineloopRect, GL_DYNAMIC_DRAW); 	
			glLineWidth(1.75f);
			glDrawArrays(GL_LINE_LOOP, 0, 4); 	//delete lineloopRect;

		}
		glDisableVertexAttribArray(attribute_coord2d); 	
		glDisableVertexAttribArray(attribute_v_colort); 
		
	}

private:
	GLuint vbo_triangle, vbo_triangle_colors; 
	GLuint triprogram; 
	GLint attribute_coord2d, attribute_v_colort, attribute_v_colorat; 
	GLint uniform_transform2d; 


};