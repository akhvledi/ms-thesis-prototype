
#ifndef RENDEROPS_H
#define RENDEROPS_H

#include <string>
#include <map>
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

#include "flat_rect.h"
#include "textblock.h"
#include "RenderManifest.h"

#include "atlas.h"

//#include "textRenderer.h"

#include "mapStatic.h"

class renderOps {


public:

/* General operations */

	static float yPtoC( int ypix, float sy);

	static float yPixToVector(int ypix, float sy);

	static float xPtoC(int xpix, float sx);

	static int yCtoP(float ycoord, float sy);

	static int xCtoP(float xcoord, float sx);

	static int midValue(int x1, int x2);

	static bool withinRect(int xIn,  int yIn, rect2D *dest, int disp[2]);

	//static std::array<float, 4> getColor(std::string name);

	static void setColor(float colorDest[4], std::string name);



/*rect2D -geometric modifiers*/

	static void setLinePix(gl_line *line, int x1, int y1, int x2, int y2, float sx, float sy);

	static void setRectPixels(rect2D *dest, int sourceCoords[4][2]);
	
	static void setRectCorners(rect2D *dest, int x1, int y1, int x2, int y2);

	static void updateRectCoords(rect2D *dest, float sx, float sy);

	static void setRectColor(rect2D *dest, float cola[4]);

	static void setRectColors(rect2D *dest, float colA[4],float colB[4],float colC[4],float colD[4]);

	static void setBorderColor(rect2D *dest, float cola[4]);

	static void setTextPerim( textblock* src, rect2D *dest,  int padd );


/*Textblock modifiers */

	/* 'Wrapper' function, calls update_textmapping in a more convenient way, needs to retrive the appropriate atlas "*/
	static void updateTextblock(textblock *block, render_manifest manifest);

	static int update_textmapping(textblock *block, atlas *a, float sx, float sy);

	static void setTextColor(textblock *block, float colorIn[4]);

	
/*Manifest modifiers */

	static void clearManifest(render_manifest &manifest);

	static void clearRegion(subRegion &region);

	static void insertTextblock(render_manifest &manifest,std::string key, const char* text, int x, int y, int f_scale);


	static void UpdateDisplace2d(subRegion &region, float sy, float sx);

	static void processText(render_manifest &manifest, subRegion &region, std::string key, const char *inText, int fscale, int lineH, int x, int y);

	
	
//private: 

	//static std::map<std::string, float[4]> rgbaColors;

	//static const int x = 1;

 	static std::map<std::string, std::array<float, 4>>rgbaColors;

 	

	/*static std::array<float, 4> getColor(std::string name) {
		return rgbaColors[name];
	}*/

};

#endif