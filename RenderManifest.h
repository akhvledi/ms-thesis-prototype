

#ifndef RENDER_MANIFEST
#define RENDER_MANIFEST

/* A struct to store elements that will be rendered. */


#include <string>
#include <map>
#include <list>

#include "flat_rect.h"
#include "textblock.h"


#include "atlas.h"

struct subRegion {

	bool active = true;

	rect2D* boundary;
	bool clipping;
	int padding = 16;

	int dispPix[2] = {0,0};
	GLfloat disp2d[2]={0,0};

	std::map<std::string,rect2D*> rectMap;
	std::map<std::string,textblock*> textMap;

	std::map<std::string, std::vector<textblock*>> textVectMap;
	std::map<std::string, std::vector<rect2D*>> rectVectMap;


	std::map<std::string, std::map< int, rect2D*>> rectMapMap;
	std::map<std::string, std::map< int, textblock*>> textMapMap;


	std::list<gl_line*> lines;
	//internal boundary as well, how far the contents reach

	subRegion() {
	}

};


struct render_manifest{

	int SCREEN_WIDTH = 1280;
	int SCREEN_HEIGHT = 720;

	float sx = 2.0 / SCREEN_WIDTH; 
	float sy = 2.0 / SCREEN_HEIGHT; 

	std::map<int,atlas*> freeFontMap;

	subRegion plainText;
	subRegion domTree;
	subRegion htmlDisplay;
	subRegion circOverlay;

	subRegion devOverlay;


	render_manifest() {

	}

};

#endif