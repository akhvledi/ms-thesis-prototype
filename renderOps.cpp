

#include "renderOps.h"



std::map<std::string, std::array<float,4>> renderOps::rgbaColors =
 create_map<std::string, std::array<float,4>>("white", {1.0, 1.0, 1.0, 1.0});

/*const std::map<std::string, std::array<float, 4>> renderOps::rgbaColors { 
		{"white",std::array<float, 4>{1.0, 1.0, 1.0, 1.0}},
		{"black",std::array<float, 4>{0.0, 0.0, 0.0, 1.0}},
		{"orange",std::array<float, 4>{0.7, 0.5, 0.15, 1.0}}

	};*/
	
/**********************************************
**********General Funcs**************
**************************************************/

float renderOps::yPtoC( int ypix, float sy)	{
		return (1-(ypix*sy));
}

float renderOps::yPixToVector(int ypix, float sy) {
	return -1*ypix*sy;
}

float renderOps::xPtoC(int xpix, float sx) {
		return (-1 +(xpix*sx));
}

int renderOps::yCtoP(float ycoord, float sy) {
		return ((1-ycoord)/sy);
}

int renderOps::xCtoP(float xcoord, float sx) {
		return ((xcoord+1)/sx);
}

int renderOps::midValue(int x1, int x2) {
	int diff = x2 -x1;
	return x1 + (diff/2);
}

bool renderOps::withinRect(int xIn,  int yIn, rect2D *dest, int disp[2]) {
		int x1 = dest->pixelCoords[0][0];
		int y1 = dest->pixelCoords[0][1] +disp[1];
		int x2 = dest->pixelCoords[3][0];
		int y2 = dest->pixelCoords[3][1] +disp[1];

		std::cout << "x1, y1: "<< x1 << ", " << y1 << std::endl;
		std::cout << " x2, y2: " << x2 << ", " << y2 << std::endl;
		
		if(xIn >= x1 && xIn <= x2 && yIn >= y1 && yIn <= y2) {
			return true;
			std::cout << "A Hit " << std::endl;
		}
		return false;
}


/*std::array<float, 4> renderOps::getColor(std::string name)  {
	return renderOps::rgbaColors[name];
}*/


void renderOps::setColor(float colorDest[4], std::string name) {
	for(int i = 0; i <4; i++)
		colorDest[i] = rgbaColors[name][i];
}


/***************************************************
**********Rect2D - geomtric modification**************

****************************************************/

void renderOps::setLinePix(gl_line *line, int x1, int y1, int x2, int y2, float sx, float sy) {
	line->p1[0] = -1 + (x1 * sx);
	line->p1[1] = 1 - (y1 * sy);

	line->p2[0] = -1 + (x2 * sx);
	line->p2[1] = 1 - (y2 * sy);
}

void renderOps::setRectPixels(rect2D *dest, int sourceCoords[4][2]) {
		for(int i = 0; i <4; i++)	{
			dest->pixelCoords[i][0] = sourceCoords[i][0];
			dest->pixelCoords[i][1] = sourceCoords[i][1];
		}
	}

void renderOps::setRectCorners(rect2D *dest, int x1, int y1, int x2, int y2) {
		dest->pixelCoords[0][0] = x1;
		dest->pixelCoords[0][1] = y1;
		dest->pixelCoords[1][0] = x1;
		dest->pixelCoords[1][1] = y2;
		dest->pixelCoords[2][0] = x2;
		dest->pixelCoords[2][1] = y1;
		dest->pixelCoords[3][0] = x2;
		dest->pixelCoords[3][1] = y2;
}


void renderOps::updateRectCoords(rect2D *dest, float sx, float sy) {		
	for(int i = 0; i < 4; i++)	{
		dest->triangle_attributes[i].coord2d[0] = -1 + (dest->pixelCoords[i][0]*sx);
		dest->triangle_attributes[i].coord2d[1] =  1 - (dest->pixelCoords[i][1]*sy);
		}
}

void renderOps::setRectColor(rect2D *dest, float colA[4]) {
	for(int i = 0; i <4; i++) {
			dest->triangle_attributes[0].v_color[i]=colA[i];
			dest->triangle_attributes[1].v_color[i]=colA[i];
			dest->triangle_attributes[2].v_color[i]=colA[i];
			dest->triangle_attributes[3].v_color[i]=colA[i];		
		}
}

void renderOps::setBorderColor(rect2D *dest, float colA[4]) {
	for(int i = 0; i <4; i++) {
			dest->borderCol[i]=colA[i];	
	}
}

void renderOps::setRectColors(rect2D *dest, float colA[4],float colB[4],float colC[4],float colD[4])	{
	for(int i = 0; i <4; i++) {
			dest->triangle_attributes[0].v_color[i]=colA[i];
			dest->triangle_attributes[1].v_color[i]=colB[i];
			dest->triangle_attributes[2].v_color[i]=colC[i];
			dest->triangle_attributes[3].v_color[i]=colD[i];		
	}
}


void renderOps::setTextPerim(textblock* src, rect2D *dest,  int padd ) {
		int c_scale = src->font_scale;
		int x1 = src->start_xpix-padd -2;
		int y1 = src->start_ypix;
		int x2 = src->final_xpix+padd +2;
		int y2 = src->final_ypix+(padd) +4;


		int testCoords[4][2] = {{x1,y1},{x1, y2},{x2,y1},{x2,y2}};	

		//std::cout << "Perimeter rectangle: "<< testCoords[0][0] << " " <<testCoords[3][1]<< std::endl;
		//need to fix how rect works 
		setRectPixels(dest, testCoords);
		//updateRectCoords(dest, 1600, 900);
		//need to update after
}



/***************************************************
**********textblock modification**************
***************************************************/


	/*Textblock-specific operations*/
void renderOps::updateTextblock(textblock *block, render_manifest manifest) {
		block->block_size = update_textmapping(block, manifest.freeFontMap[block->font_scale], manifest.sx, manifest.sy);
}


	/* Maps the const char array associated with a given textblock */
int renderOps::update_textmapping(textblock *block, atlas *a, float sx, float sy) {
	const uint8_t *p;

	block->final_ypix= block->start_ypix + block->font_scale;
	block->final_xpix = block->start_xpix;

	int c = 0;
	float x = renderOps::xPtoC(block->start_xpix, sx);
	float y = renderOps::yPtoC(block->start_ypix + block->font_scale, sy);

	//keeping track of original values		
	//float init_xc = x;	
	
	// Loop through all characters 
	for (p = (const uint8_t *)block->text; *p; p++) {
		//newline character
		if(*p =='\n') {
			x = renderOps::xPtoC(block->start_xpix, sx);
			block->final_ypix += block->font_scale;
			y = renderOps::yPtoC(block->final_ypix, sy);
		}
		else if(*p == '\t')	{
			x = renderOps::xPtoC( ( renderOps::xCtoP(x,sx) + (block->font_scale*4) ), sx);				
		}
		
		// Calculate the vertex and texture coordinates 
		float x2 = x + a->c[*p].bl * sx;
		float y2 = -y - a->c[*p].bt * sy;
		float w = a->c[*p].bw * sx;
		float h = a->c[*p].bh * sy;

		// Advance the cursor to the start of the next character 
		x += a->c[*p].ax * sx;
		y += a->c[*p].ay * sy;


		if(((x+1)/sx)>block->final_xpix) {
			block->final_xpix = renderOps::xCtoP(x,sx);
		}
			
		// Skip glyphs that have no pixels 						
		if (!w || !h) {				
			continue;
		}			

		block->coords[c++] = (point) {
		x2, -y2, a->c[*p].tx, a->c[*p].ty};

		block->coords[c++] = (point) {
		x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty};

		block->coords[c++] = (point) {
		x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h};

		block->coords[c++] = (point) {
		x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty};

		block->coords[c++] = (point) {
		x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h};

		block->coords[c++] = (point) {
		x2 + w, -y2 - h, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h};
			
	}
		
	return c;
}


void renderOps::setTextColor(textblock *block, float colorIn[4]) {
	block->rgbA[0] = colorIn[0];
	block->rgbA[1] = colorIn[1];
	block->rgbA[2] = colorIn[2];
	block->rgbA[3] = colorIn[3];
}


/***************************************************
**********Manifest modification**************
***************************************************/

void renderOps::clearManifest(render_manifest &manifest) {
		clearRegion(manifest.plainText);
		clearRegion(manifest.domTree);
		clearRegion(manifest.htmlDisplay);
		clearRegion(manifest.circOverlay);
		clearRegion(manifest.devOverlay);

		//manifest.rectMap.clear();
		//manifest.textMap.clear();
	}


void renderOps::clearRegion(subRegion &region) {
		region.rectMap.clear();
		region.textMap.clear();
		region.textVectMap.clear();
		region.rectVectMap.clear();
	}


void renderOps::UpdateDisplace2d(subRegion &region, float sy, float sx) {

	region.disp2d[1] = yPixToVector(region.dispPix[1], sy);
}
	/*void insertTextblock(render_manifest &manifest, std::string key, const char* text, int x, int y, int f_scale)
	{
		manifest.textMap.insert( std::make_pair( key, new textblock(text, x, y, f_scale)) );
	}*/



void renderOps::processText(render_manifest &manifest, subRegion &region, std::string key, const char *inText, int fscale, int lineH, int x, int y) {
	const uint8_t *p;
	int c_index = 0;
	int c_offset = 0;

	int t_index = 0;



	std::vector<textblock*> texVec;
	//std::vector<rect2D*> recVec;

	for (p = (const uint8_t *)inText; *p; p++) {

		if(*p =='\n' && c_index>0) {

			int size = c_index - c_offset;
			char *textb = new char[size];
			strncpy(textb, inText+c_index, size);

			int height = y + ((fscale*lineH) * t_index);

			texVec.push_back(new textblock(textb, x, height, fscale));
			texVec[t_index]->active = true;
			updateTextblock(texVec[t_index], manifest);

			c_offset += size;
			t_index++;
		}

		c_index++;
	}

	std::string label1 = key + "Text";
	//std::string label2 = key + "Border";
	region.textVectMap.insert( std::make_pair( label1,  texVec));
	//region.rectVectMap.insert( std::make_pair( label2,  recVec));
}






