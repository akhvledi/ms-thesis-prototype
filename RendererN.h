
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

//Using SDL and standard IO
#include <SDL2/SDL.h>
//#include <stdio.h>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "textRenderer.h"
#include "textblock.h"


#include "rect2dRenderer.h"
#include "flat_rect.h"



#include "./shader_utils/shader_utils.h"

//#include "../common/shader_utils.h"

#include "fileops.h"
#include <cstring>

#include "renderOps.h"


#include "RenderManifest.h"


/*
**********************************************************************
 End of inclusions
**********************************************************************
*/

std::string programName = "SDL_OpenGL Test";


//textdis textrender;

textRenderer t_render;
rect2dRenderer r_render;


class RendererN
{

	
	textblock *blook;

	public:
	//Default Constructor**********************************
	RendererN()
	{

	}

	float sx; 	
	float sy;



	bool Init()
	{

		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "Failed to init SDL\n";
			return false;
		}


		/*Temporary, get rid of it later*/
		sx = 2.0 / SCREEN_WIDTH; 
		sy = 2.0 / SCREEN_HEIGHT; 
		/*Temporary, get rid of it later*/
		Uint32 flags = 0;

		//flags |= SDL_WINDOW_RESIZABLE;
		flags |= SDL_WINDOW_OPENGL;

		// Create our window centered at defined resolution
		mainWindow = SDL_CreateWindow(
									programName.c_str(), 
									SDL_WINDOWPOS_CENTERED, 
									SDL_WINDOWPOS_CENTERED,
									SCREEN_WIDTH, 
									SCREEN_HEIGHT, 
									flags);



		// Check that everything worked out okay
		if (!mainWindow)
		{
			std::cout << "Unable to create window\n";
			CheckSDLError(__LINE__);
			return false;
		}

		// Create our opengl context and attach it to our window
		mainContext = SDL_GL_CreateContext(mainWindow);


		SetOpenGLAttributes();

		// This makes our buffer swap syncronized with the monitor's vertical refresh
		SDL_GL_SetSwapInterval(1);

		// Init GLEW
		// Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
		
		#ifndef __APPLE__
		glewExperimental = GL_TRUE;
		glewInit();
		#endif	

		
		if (!t_render.Init())
		{
			std::cout << "Unable to initialize text rendering\n";
			CheckSDLError(__LINE__);
			return false;
		}
		if (!r_render.Init())
		{
			std::cout << "Unable to initialize rect2D rendering\n";
			CheckSDLError(__LINE__);
			return false;
		}


		if (!init_resources())
		{
			std::cout << "Unable to initialize resources\n";
			CheckSDLError(__LINE__);
			return false;
		}

	 	return true;

	}




	

	void CheckSDLError(int line = -1)
	{
		std::string error = SDL_GetError();

		if (error != "")
		{
			std::cout << "SDL Error : " << error << std::endl;

			if (line != -1)
				std::cout << "\nLine : " << line << std::endl;

			SDL_ClearError();
		}
	}



	/* basically just a testing area for render features not yet fully implemented & packaged*/
	int init_resources() 
	{
		

		//std::strncpy(texta, strOne.c_str(), strOne.size()-1);

		//blook = new textblock(texta, 8, 100, 24);

		//t_render.update_textBlock(blook, sx, sy);

		
		return 1;
	}






	void display(render_manifest manifest) 
	{
				
		/* White background */
		glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//glScissor( 0, 0, 500, 720);
		//glEnable(GL_SCISSOR_TEST);
		
		//r_render.display_tri();
		/*
		r_render.enable();
	
		std::map<std::string, rect2D*>::iterator it;
		for (it = manifest.rectMap.begin(); it != manifest.rectMap.end(); ++it){
			if(it->second->active) {
				r_render.render_triStrip(it->second);
   				//r_render.render_lineLoop(it->second);
   			}
		}*/

		/*
		std::map<std::string, std::list<rect2D*>>::iterator im;
		for(im = manifest.rectListMap.begin(); im != manifest.rectListMap.end(); ++im) {
			if(!im->second.empty()) {
				for( auto const& i : im->second) {
					r_render.render_triStrip(i);
				}
			}
		}
	*/
		/*
		t_render.enableText();		

		//std::cout << "Is it here?" << std::endl;
		
		std::map<std::string, textblock*>::iterator j;
		for (j = manifest.textMap.begin(); j != manifest.textMap.end(); ++j){
			if(j->second->active) {
				t_render.renderTextblock(j->second, manifest.freeFontMap[j->second->font_scale]);
   			}
		}*/


		displayRegion(manifest.plainText, manifest.SCREEN_HEIGHT, manifest.freeFontMap);
		displayRegion(manifest.domTree, manifest.SCREEN_HEIGHT, manifest.freeFontMap);
		displayRegion(manifest.htmlDisplay, manifest.SCREEN_HEIGHT, manifest.freeFontMap);
		displayRegion(manifest.circOverlay, manifest.SCREEN_HEIGHT, manifest.freeFontMap);



		displayRegion(manifest.devOverlay, manifest.SCREEN_HEIGHT, manifest.freeFontMap);

		/*
		std::map<std::string, std::list<textblock*>>::iterator iq;
		for(iq = manifest.textListMap.begin(); iq != manifest.textListMap.end(); ++iq) {
			if(!iq->second.empty()) {
				for( auto const& i : iq->second) {
					t_render.renderTextblock(i);
				}
			}
		}
	*/

		//t_render.renderTextblock(blook);

		
		SDL_GL_SwapWindow(mainWindow);
	}


	void displayRegion(subRegion region, float SCREEN_H, std::map<int,atlas*> fontMap) {

		if(!region.active)
			return;

		
		GLfloat noDisp[2] ={0,0};

		r_render.enable();
		if(region.boundary->active)
			r_render.render_triStrip(region.boundary, noDisp);

		if(region.clipping)	{
			int x1 = region.boundary->pixelCoords[0][0];
			int y1 = region.boundary->pixelCoords[0][1];
			int x2 = region.boundary->pixelCoords[3][0];
			int y2 = region.boundary->pixelCoords[3][1];
			glScissor( x1, SCREEN_H-y2, x2-x1, y2-y1);
			glEnable(GL_SCISSOR_TEST);
		}
	
			
		std::map<std::string, rect2D*>::iterator it;
		for (it = region.rectMap.begin(); it != region.rectMap.end(); ++it){
			if(it->second->active) {
				r_render.render_triStrip(it->second, region.disp2d);
   				//r_render.render_lineLoop(it->second);
   			}
		}


		std::map<std::string, std::vector<rect2D*>>::iterator im;
		for(im = region.rectVectMap.begin(); im != region.rectVectMap.end(); ++im) {
			if(!im->second.empty()) {
				for( auto const& i : im->second) {
					r_render.render_triStrip(i, region.disp2d);
				}
			}
		}


		std::map<std::string, std::map< int, rect2D*>>::iterator imm;
		for(imm = region.rectMapMap.begin(); imm != region.rectMapMap.end(); ++imm) {
			if(!imm->second.empty()) {
				
				for( auto const& i : imm->second) {
					if(i.second->active)
						r_render.render_triStrip(i.second, region.disp2d);
				}

			}
		}




		t_render.enableText();		

		//std::cout << "Is it here?" << std::endl;
		
		std::map<std::string, textblock*>::iterator j;
		for (j = region.textMap.begin(); j != region.textMap.end(); ++j){
			if(j->second->active) {
				t_render.renderTextblock(j->second, fontMap[j->second->font_scale], region.disp2d);
   			}
		}

		
		std::map<std::string, std::vector<textblock*>>::iterator iq;
		for(iq = region.textVectMap.begin(); iq != region.textVectMap.end(); ++iq) {
			if(!iq->second.empty()) {
				for( auto const& i : iq->second) {
					t_render.renderTextblock(i, fontMap[i->font_scale], region.disp2d);
				}
			}
		}
		



		std::map<std::string, std::map< int, textblock*>>::iterator ig;
		for(ig = region.textMapMap.begin(); ig != region.textMapMap.end(); ++ig) {
			if(!ig->second.empty()) {
				
				for( auto const& i : ig->second) {
					t_render.renderTextblock(i.second, fontMap[i.second->font_scale], region.disp2d);
				}

			}
		}

		glDisable(GL_SCISSOR_TEST);

		if(region.lines.empty())
			return;

		r_render.enable();
		for( auto const& i : region.lines) {
			r_render.drawLine(i);
			//std::cout <<("any luck?") << std::endl;
		}

	}



	void free_resources() 
	{
		//glDeleteProgram(program);

		t_render.free_resources();
	}


	
//*********************************************************************************************************************************
//********************************************************************************************************************************

	bool generateFonts(render_manifest &manifest)
	{
		return t_render.initFont(manifest.freeFontMap, "lib/FreeSans.ttf");
	}



//Called by INIT()*************************************
	bool SetOpenGLAttributes()
	{
		// Set our OpenGL version.
		// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

		// Turn on double buffering with a 24bit Z buffer.
		// You may need to change this to 16 or 32 for your system
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		//SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,1);


		return true;
	}







	//********SDL WINDOW STUFF*****************************

	//The window we'll be rendering to
	SDL_Window* mainWindow;
	// Our opengl context handle
	SDL_GLContext mainContext;



	//Static array shit here
	std::string strOne = fileops::get_file_string("source_files/index.html");
	char *texta = new char[strOne.size()];



	

	void setScreenSize(int x, int y)
	{
		SCREEN_WIDTH =x;
		SCREEN_HEIGHT =y;
	}

	void setBackColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
	{
		bgColor[0]=r;
		bgColor[1]=g;
		bgColor[2]=b;
		bgColor[3]=a;
	}

private:

	GLclampf bgColor[4];
	std::map<std::string,rect2D> rectMap;


	//Screen dimensions
	int SCREEN_WIDTH = 1280;
	int SCREEN_HEIGHT = 720;



};