/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// C++ Headers
#include <string>
#include <iostream>

#include <math.h>

//***********for file reading
#include <fstream>
#include <streambuf>
//#include <string>
#include <cerrno>

//***********end

// OpenGl / glew headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>





//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

#define GLM_FORCE_RADIANS 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>


#include "./shader_utils/shader_utils.h"
//#include <ft2build.h>
//#include FT_FREETYPE_H  


//#include "Renderer.h"
//#include "EventHandler.h"
#include "RendererN.h"


//#include "EventManager.h"
//#include "GameState.h"


#include "flat_rect.h"

//#include "basicButton.h"

#include "RenderManifest.h"


#include "renderOps.h"

//#include "domnode.h"
#include "domTree.h"
#include "renderTree.h"
//#define PROGRAM_NAME "Stutorial"
#include <list>
//std::string programName = "SDL_OpenGL Test";


#define FPS 60;
/*
int map_Sx = 22;
int map_Sy = 16;

//Map distance (from camera)
float map_D = -10.0;

//Map position relative to origin
float map_X = 0;
float map_Y = 0;
*/

//The window we'll be rendering to
//SDL_Window* mainWindow;

// Our opengl context handle
//SDL_GLContext mainContext;

/*
float X_brd = 8.0;

void RunGame();
void drawSquare();

*/

// Handles OpenGl drawing
//Renderer renderer;

RendererN renderer;

render_manifest r_manifest;

//GameState::Instance();
// Handles all our SDL2 event ( quit, keydown, ++ )

//EventManager eventManager;

//GameState gameState;
//(gameState);

domTree d_tree;
renderTree* r_tree = new renderTree();



//char *texta;



float rgba_White[] = {1.0, 1.0, 1.0, 1.0};
float rgba_Black[] = {0.0, 0.0, 0.0, 1.0};

float rgba_Navy[] = {0.55, 0.3, 0.7, 1.0};

float rgba_Green[] = {0.4, 0.8, 0.5, 0.8};

float rgba_Maroon[] = {0.6, 0.1, 0.4, 1.0};
float rgba_Orange[] = {0.7, 0.5, 0.15, 1.0};

int plainY;
int domY;
int htmlY;

bool loadHTML(const char* fileN)
{
	renderOps::clearRegion(r_manifest.plainText);
	renderOps::clearRegion(r_manifest.domTree);
	renderOps::clearRegion(r_manifest.htmlDisplay);
	

	//Plain html text display
	std::string fstring = fileops::get_file_string(fileN);
	//char *texta = new char[fstring.size()];
	//std::cout << "LOADING FILE" << std::endl << fstring <<std::endl;
	//std::strcpy(texta, fstring.c_str());

	int x = r_manifest.plainText.boundary->pixelCoords[0][0] +  r_manifest.plainText.padding;
	int y = r_manifest.plainText.boundary->pixelCoords[0][1] +  r_manifest.plainText.padding;

	//std::cout << "y: " << y << std::endl;

	//renderOps::processText(r_manifest, r_manifest.plainText, "htmlPlain", texta, 16, 1.5, x, y);

	r_manifest.plainText.textMap.insert( std::make_pair( "htmlPlain", new textblock(fstring, x, y, 16)) );
	r_manifest.plainText.textMap["htmlPlain"]->active = true;
	renderOps::updateTextblock(r_manifest.plainText.textMap["htmlPlain"], r_manifest);

	r_manifest.plainText.rectMap.insert( std::make_pair( "plaintx_hl", new rect2D() ) );
	renderOps::setRectColor(r_manifest.plainText.rectMap["plaintx_hl"], rgba_Green);



	//DOM TREE display
	if(!d_tree.treeFromFile(fileN)) {
		std::cerr<< "Unable to create DOM tree. " <<std::endl;
		return false;
	}

	d_tree.attachTree(16, r_manifest);

	std::vector<rect2D*> nSquares;
	r_manifest.domTree.rectVectMap.insert( std::make_pair( "nodeSquares",  nSquares));

	//Rendered HTML display
	r_tree->buildRenderTree(d_tree.getRoot());
	r_tree->attachRTree(16, r_manifest, r_manifest.htmlDisplay, "nodeText");
	

}

/*
void setText_Perimeter(std::string tbkey, std::string rctkey, int padd)
{

	int c_scale = r_manifest.textMap[tbkey]->font_scale;
	int x1 = r_manifest.textMap[tbkey]->start_xpix-padd;
	int y1 = r_manifest.textMap[tbkey]->start_ypix-(padd+c_scale);
	int x2 = r_manifest.textMap[tbkey]->final_xpix+padd;
	int y2 = r_manifest.textMap[tbkey]->final_ypix+(padd);

	int testCoords[4][2] = {{x1,y1},{x1, y2},{x2,y1},{x2,y2}};	

	//need to fix how rect works 
	renderOps::setRectPixels(r_manifest.rectMap[rctkey], testCoords);
	renderOps::updateRectCoords(r_manifest.rectMap[rctkey], r_manifest.sx, r_manifest.sy);	
}*/

/*
bool buildDomTree(domElement* root, const char* filename);

domNode* processNode(tinyxml2::XMLNode* xNode, int depth);

void printNode(domNode* dNode, std::list<domNode*> &nodeList);
/* 
 Setting up shop, creating our initial map -key pairings
*/
bool Init()
{
	if(!renderer.Init()) {
		std::cerr<< "Renderer initialization faliure. " <<std::endl;
		return false;
	}

	renderer.setBackColor(0.690,0.769,0.871,1);
	renderer.generateFonts(r_manifest);

	r_manifest.plainText.clipping=true;
	r_manifest.plainText.boundary=new rect2D();
	r_manifest.plainText.boundary->active=true;
	r_manifest.plainText.boundary->border=true;
	renderOps::setRectColor(r_manifest.plainText.boundary, rgba_White);
	renderOps::setRectCorners(r_manifest.plainText.boundary, 392, 24, 808, 266);
	renderOps::updateRectCoords(r_manifest.plainText.boundary, r_manifest.sx,  r_manifest.sy);	



	r_manifest.domTree.clipping=true;
	r_manifest.domTree.boundary = new rect2D();
	r_manifest.domTree.boundary->active=true;
	r_manifest.domTree.boundary->border=true;
	renderOps::setRectColor(r_manifest.domTree.boundary, rgba_White);
	renderOps::setRectCorners(r_manifest.domTree.boundary, 840, 24, 1256, 266);
	renderOps::updateRectCoords(r_manifest.domTree.boundary, r_manifest.sx,  r_manifest.sy);


	//html display init
	r_manifest.htmlDisplay.clipping=true;
	r_manifest.htmlDisplay.boundary = new rect2D();
	r_manifest.htmlDisplay.boundary->active=true;
	r_manifest.htmlDisplay.boundary->border=true;
	r_manifest.htmlDisplay.padding = 0;
	renderOps::setRectColor(r_manifest.htmlDisplay.boundary, rgba_White);
	renderOps::setRectCorners(r_manifest.htmlDisplay.boundary, 392, 296, 1256, 704);
	renderOps::updateRectCoords(r_manifest.htmlDisplay.boundary, r_manifest.sx,  r_manifest.sy);



	//Chage this first part to an init required for all regions
	r_manifest.circOverlay.boundary = new rect2D();


	//Plain title
	r_manifest.circOverlay.rectMap.insert( std::make_pair( "plainTitle", new rect2D() ));
	r_manifest.circOverlay.rectMap["plainTitle"]->active = true;
	renderOps::setRectColor(r_manifest.circOverlay.rectMap["plainTitle"], rgba_Navy);
	renderOps::setRectCorners(r_manifest.circOverlay.rectMap["plainTitle"], 392, 2, 640, 22);
	renderOps::updateRectCoords(r_manifest.circOverlay.rectMap["plainTitle"], r_manifest.sx,  r_manifest.sy);

	std::string title  = "Plain text Document view";
	r_manifest.circOverlay.textMap.insert( std::make_pair( "plainTitle", new textblock(title, 400, 4, 16)) );
	r_manifest.circOverlay.textMap["plainTitle"]->setColor(rgba_White);
	r_manifest.circOverlay.textMap["plainTitle"]->active = true;
	renderOps::updateTextblock(r_manifest.circOverlay.textMap["plainTitle"], r_manifest);


	//dom title
	r_manifest.circOverlay.rectMap.insert( std::make_pair( "domTitle", new rect2D() ));
	r_manifest.circOverlay.rectMap["domTitle"]->active = true;
	renderOps::setRectColor(r_manifest.circOverlay.rectMap["domTitle"], rgba_Navy);
	renderOps::setRectCorners(r_manifest.circOverlay.rectMap["domTitle"], 840, 2, 1000, 22);
	renderOps::updateRectCoords(r_manifest.circOverlay.rectMap["domTitle"], r_manifest.sx,  r_manifest.sy);

	std::string title2  = "DOM Tree layout ";
	r_manifest.circOverlay.textMap.insert( std::make_pair( "domTitle", new textblock(title2, 848, 4, 16)) );
	r_manifest.circOverlay.textMap["domTitle"]->setColor(rgba_White);
	r_manifest.circOverlay.textMap["domTitle"]->active = true;
	renderOps::updateTextblock(r_manifest.circOverlay.textMap["domTitle"], r_manifest);


	//html title
	r_manifest.circOverlay.rectMap.insert( std::make_pair( "htmlTitle", new rect2D() ));
	r_manifest.circOverlay.rectMap["htmlTitle"]->active = true;
	renderOps::setRectColor(r_manifest.circOverlay.rectMap["htmlTitle"], rgba_Navy);
	renderOps::setRectCorners(r_manifest.circOverlay.rectMap["htmlTitle"], 392, 274, 700, 294);
	renderOps::updateRectCoords(r_manifest.circOverlay.rectMap["htmlTitle"], r_manifest.sx,  r_manifest.sy);

	std::string title3  = "Html Render (Browser) view";
	r_manifest.circOverlay.textMap.insert( std::make_pair( "htmlTitle", new textblock(title3, 400, 276, 16)) );
	r_manifest.circOverlay.textMap["htmlTitle"]->setColor(rgba_White);
	r_manifest.circOverlay.textMap["htmlTitle"]->active = true;
	renderOps::updateTextblock(r_manifest.circOverlay.textMap["htmlTitle"], r_manifest);







	//devOverlay init
	r_manifest.devOverlay.boundary = new rect2D();
	r_manifest.devOverlay.textMap.insert( std::make_pair( "coordsTex", new textblock("X:9999\nY:9999", 20, 20, 20)) );
	r_manifest.devOverlay.textMap["coordsTex"]->active = true;
	r_manifest.devOverlay.textMap["coordsTex"]->setColor(rgba_Maroon);
	renderOps::updateTextblock(r_manifest.devOverlay.textMap["coordsTex"], r_manifest);

	r_manifest.devOverlay.textMap.insert( std::make_pair( "statusTex", new textblock("Status: ", 20, 80, 20)) );
	r_manifest.devOverlay.textMap["statusTex"]->active = true;
	r_manifest.devOverlay.textMap["statusTex"]->setColor(rgba_Maroon);
	renderOps::updateTextblock(r_manifest.devOverlay.textMap["statusTex"], r_manifest);

	return true;

}


/*
void setTBstring(std::string inputString, std::string mapKey)
{
	char* texnew = new char[inputString.size()];
	std::strncpy(texnew, inputString.c_str(), inputString.size()-1);
	r_manifest.textMap[mapKey]->text = texnew;
	//renderer.updateTextBlock(r_manifest.textMap[mapKey]);
	renderOps::updateTextblock(r_manifest.textMap[mapKey], r_manifest);
}*/

void setStatus(std::string msg)
{
	char *textb = new char[msg.size() ];
	std::strcpy(textb, msg.c_str());
	r_manifest.devOverlay.textMap["statusTex"]->text = textb;
	renderOps::updateTextblock(r_manifest.devOverlay.textMap["statusTex"], r_manifest);


}


void clearRegionSelection() {
	renderOps::setBorderColor(r_manifest.plainText.boundary, rgba_Black);
	renderOps::setBorderColor(r_manifest.domTree.boundary, rgba_Black);
	renderOps::setBorderColor(r_manifest.htmlDisplay.boundary, rgba_Black);
}


void clearLineSelection() {
	
	//plaintext
	r_manifest.plainText.rectMap["plaintx_hl"]->active = false;
	r_manifest.plainText.rectMap["plaintx_hl"]->border = false;
	//Dom tree
	r_manifest.domTree.rectVectMap["nodeSquares"].clear();

	for ( auto const& i : r_manifest.domTree.rectVectMap["nodeBorder"]) {
		i->border=false;
	}

	//htmlDisplay

	for ( auto const& i : r_manifest.htmlDisplay.rectMapMap["nodeText"]) {
		i.second->active = false;
		i.second->border = false;
	}



	//Circ overlay
	r_manifest.circOverlay.lines.clear();
}



void selectedPlain(int x, int y) {
	//clear other region elements
	//clear overall overlay
	renderOps::setBorderColor(r_manifest.plainText.boundary, rgba_Orange);

	int f_scale = r_manifest.plainText.textMap["htmlPlain"]->font_scale;
	int init_y = r_manifest.plainText.textMap["htmlPlain"]->start_ypix + r_manifest.plainText.dispPix[1];
	int fin_y = r_manifest.plainText.textMap["htmlPlain"]->final_ypix + r_manifest.plainText.dispPix[1];

	int x1 = r_manifest.plainText.textMap["htmlPlain"]->start_xpix;				
	int x2 = r_manifest.plainText.textMap["htmlPlain"]->final_xpix;
				
	
	for(int i = init_y; i < fin_y ; i+= f_scale ) {
		int i2 = i+ f_scale;
		//std::cout << "Clicked at y = "<< y << ", y1 y2: "<< i << " " << i2 << std::endl;						
		if(y>=i && y<= i2) {
			//std::cout << i <<" & " << i2 << std::endl;
			//std::cout<< "located btw: " << i << " and " << i2 << std::endl;
			//int testCoords[4][2] = {{x1,i},{x1, i2},{x2,i},{x2,i2}};
			renderOps::setRectCorners(r_manifest.plainText.rectMap["plaintx_hl"], x1, i - r_manifest.plainText.dispPix[1] , x2, i2 + 2 -r_manifest.plainText.dispPix[1]);
			//renderOps::setRectPixels(r_manifest.plainText.rectMap["plaintx_hl"], testCoords);
			renderOps::updateRectCoords(r_manifest.plainText.rectMap["plaintx_hl"], r_manifest.sx, r_manifest.sy);	
			r_manifest.plainText.rectMap["plaintx_hl"]->active = true;
			r_manifest.plainText.rectMap["plaintx_hl"]->border = true;

			int line_no = ( (i -init_y)/f_scale) + 1;												
			setStatus( "Status: Selected plain text line # " + std::to_string(line_no));


			std::list<int> nodeIndicies = d_tree.lineNodes(line_no, r_manifest);

			if(!nodeIndicies.empty()) {
				int k =0;
				for( auto const& i : nodeIndicies) {

					int u1 = r_manifest.domTree.boundary->pixelCoords[0][0] +r_manifest.domTree.padding;
					int v1 = r_manifest.domTree.rectVectMap["nodeBorder"][i]->pixelCoords[0][1] ;

					r_manifest.domTree.rectVectMap["nodeSquares"].push_back(new rect2D());

					renderOps::setRectCorners(r_manifest.domTree.rectVectMap["nodeSquares"][k], u1, v1+4, u1+10, v1+14);
					renderOps::setRectColor(r_manifest.domTree.rectVectMap["nodeSquares"][k], rgba_Maroon);
					renderOps::updateRectCoords(r_manifest.domTree.rectVectMap["nodeSquares"][k], r_manifest.sx,  r_manifest.sy);


					//circ-overlay
					int c1 = r_manifest.plainText.rectMap["plaintx_hl"]->pixelCoords[3][0];
					int d1 = renderOps::midValue(r_manifest.plainText.rectMap["plaintx_hl"]->pixelCoords[0][1], r_manifest.plainText.rectMap["plaintx_hl"]->pixelCoords[3][1]);
		
					r_manifest.circOverlay.lines.push_back(new gl_line(c1,d1+r_manifest.plainText.dispPix[1] ,u1, v1+9 + r_manifest.domTree.dispPix[1],  r_manifest.sx, r_manifest.sy  ));



					//html disp & line
					if( !(r_manifest.htmlDisplay.rectMapMap["nodeText"].find(i) == r_manifest.htmlDisplay.rectMapMap["nodeText"].end()) ) {
				
						r_manifest.htmlDisplay.rectMapMap["nodeText"][i]->active = true;
						r_manifest.htmlDisplay.rectMapMap["nodeText"][i]->border = true;				

						int htmlX = renderOps::midValue(r_manifest.htmlDisplay.rectMapMap["nodeText"][i]->pixelCoords[0][0], r_manifest.htmlDisplay.rectMapMap["nodeText"][i]->pixelCoords[3][0]);
						int htmlY = r_manifest.htmlDisplay.rectMapMap["nodeText"][i]->pixelCoords[0][1] + r_manifest.htmlDisplay.dispPix[1]; 

						r_manifest.circOverlay.lines.push_back(new gl_line(htmlX, htmlY, u1, v1+9+r_manifest.domTree.dispPix[1],  r_manifest.sx, r_manifest.sy  ));
					}




					k++;
				}
				return;
			}
		}
		
	}
}
	






/*

*/
void selectedNode(int x, int y)
{
	renderOps::setBorderColor(r_manifest.domTree.boundary, rgba_Orange);


	if(r_manifest.domTree.rectVectMap["nodeBorder"].empty())
		return;

	int k = 0;
	for ( auto const& i : r_manifest.domTree.rectVectMap["nodeBorder"]) {
		if(renderOps::withinRect(x, y, i, r_manifest.domTree.dispPix)) {
			//DomTree display part
			std::string out =  "Status: Selected node "; 
			//out+= r_manifest.domTree.textVectMap["nodeText"][k]->text;
			setStatus(out);
			i->border =true;


			int x1 = r_manifest.domTree.boundary->pixelCoords[0][0] +r_manifest.domTree.padding;
			int y1 = i->pixelCoords[0][1] ;

			r_manifest.domTree.rectVectMap["nodeSquares"].push_back(new rect2D());
			renderOps::setRectColor(r_manifest.domTree.rectVectMap["nodeSquares"][0], rgba_Maroon);
			renderOps::setRectCorners(r_manifest.domTree.rectVectMap["nodeSquares"][0], x1, y1+4, x1+10, y1+14 );
			renderOps::updateRectCoords(r_manifest.domTree.rectVectMap["nodeSquares"][0], r_manifest.sx,  r_manifest.sy);

			//PlainText display part

			int f_scale = r_manifest.plainText.textMap["htmlPlain"]->font_scale;
		 	int line_no = d_tree.nodeLineNum(k);
			
			int u1 = r_manifest.plainText.textMap["htmlPlain"]->start_xpix;				
			int u2 = r_manifest.plainText.textMap["htmlPlain"]->final_xpix;
			int v1 = r_manifest.plainText.textMap["htmlPlain"]->start_ypix - f_scale +(f_scale * line_no) ;

			renderOps::setRectCorners(r_manifest.plainText.rectMap["plaintx_hl"], u1, v1, u2, v1+f_scale +2);
			renderOps::updateRectCoords(r_manifest.plainText.rectMap["plaintx_hl"], r_manifest.sx, r_manifest.sy);	
			r_manifest.plainText.rectMap["plaintx_hl"]->active = true;


			//circOvleray
			int c1 = r_manifest.plainText.rectMap["plaintx_hl"]->pixelCoords[3][0];
			int d1 = renderOps::midValue(r_manifest.plainText.rectMap["plaintx_hl"]->pixelCoords[0][1], r_manifest.plainText.rectMap["plaintx_hl"]->pixelCoords[3][1]);
		

			r_manifest.circOverlay.lines.push_back(new gl_line(c1,d1+r_manifest.plainText.dispPix[1],x1, y1+9+r_manifest.domTree.dispPix[1],  r_manifest.sx, r_manifest.sy  ));
		

			//To html element
			if( !(r_manifest.htmlDisplay.rectMapMap["nodeText"].find(k) == r_manifest.htmlDisplay.rectMapMap["nodeText"].end()) ) {
				
				r_manifest.htmlDisplay.rectMapMap["nodeText"][k]->active = true;
				r_manifest.htmlDisplay.rectMapMap["nodeText"][k]->border = true;				

				int htmlX = renderOps::midValue(r_manifest.htmlDisplay.rectMapMap["nodeText"][k]->pixelCoords[0][0], r_manifest.htmlDisplay.rectMapMap["nodeText"][k]->pixelCoords[3][0]);
				int htmlY = r_manifest.htmlDisplay.rectMapMap["nodeText"][k]->pixelCoords[0][1] + r_manifest.htmlDisplay.dispPix[1]; 

				r_manifest.circOverlay.lines.push_back(new gl_line(htmlX, htmlY, x1, y1+9+r_manifest.domTree.dispPix[1],  r_manifest.sx, r_manifest.sy  ));
			}

			return;
		}
		k++;
	}
}





void selectedHtml(int x, int y){

	renderOps::setBorderColor(r_manifest.htmlDisplay.boundary, rgba_Orange);



	if(r_manifest.htmlDisplay.rectMapMap["nodeText"].empty())
		return;

	std::cout << "Here?" << std::endl;
	int k = 0;
	for ( auto const& i : r_manifest.htmlDisplay.rectMapMap["nodeText"]) {

		std::cout << "xIn, yIn: " << x << " , " << y << std::endl;
		if(renderOps::withinRect(x, y, i.second, r_manifest.htmlDisplay.dispPix)) {

			i.second->active = true;
			i.second->border = true;


			//squares
			int u1 = r_manifest.domTree.boundary->pixelCoords[0][0] +r_manifest.domTree.padding;
			int v1 = r_manifest.domTree.rectVectMap["nodeBorder"][i.first]->pixelCoords[0][1] ;

			r_manifest.domTree.rectVectMap["nodeSquares"].push_back(new rect2D());

			renderOps::setRectCorners(r_manifest.domTree.rectVectMap["nodeSquares"][0], u1, v1+4, u1+10, v1+14);
			renderOps::setRectColor(r_manifest.domTree.rectVectMap["nodeSquares"][0], rgba_Maroon);
			renderOps::updateRectCoords(r_manifest.domTree.rectVectMap["nodeSquares"][0], r_manifest.sx,  r_manifest.sy);

			//circ overlay pt 1
			int htmlX = renderOps::midValue(i.second->pixelCoords[0][0], i.second->pixelCoords[3][0]);
			int htmlY = i.second->pixelCoords[0][1] + r_manifest.htmlDisplay.dispPix[1]; 

			
			int domSqY = v1+9+r_manifest.domTree.dispPix[1];

			r_manifest.circOverlay.lines.push_back(new gl_line(htmlX, htmlY, u1, domSqY,  r_manifest.sx, r_manifest.sy  ));


			//htmplain
			int f_scale = r_manifest.plainText.textMap["htmlPlain"]->font_scale;
		 	int line_no = d_tree.nodeLineNum(i.first);
			
			int x1 = r_manifest.plainText.textMap["htmlPlain"]->start_xpix;				
			int x2 = r_manifest.plainText.textMap["htmlPlain"]->final_xpix;
			int y1 = r_manifest.plainText.textMap["htmlPlain"]->start_ypix - f_scale +(f_scale * line_no) ;

			renderOps::setRectCorners(r_manifest.plainText.rectMap["plaintx_hl"], x1, y1, x2, y1+f_scale +2);
			renderOps::updateRectCoords(r_manifest.plainText.rectMap["plaintx_hl"], r_manifest.sx, r_manifest.sy);	
			r_manifest.plainText.rectMap["plaintx_hl"]->active = true;


			//circOvleray pt2
			int c1 = r_manifest.plainText.rectMap["plaintx_hl"]->pixelCoords[3][0];
			int d1 = renderOps::midValue(r_manifest.plainText.rectMap["plaintx_hl"]->pixelCoords[0][1], r_manifest.plainText.rectMap["plaintx_hl"]->pixelCoords[3][1]);
		
			r_manifest.circOverlay.lines.push_back(new gl_line(c1,d1+r_manifest.plainText.dispPix[1],u1, domSqY,  r_manifest.sx, r_manifest.sy  ));
			

		}
	}

}

















void ShiftView(bool up, char select){

	int incr = 10;
	if(up)
		incr = incr *-1;

	switch(select) {
	case 'p':
		//if(incr <0 && r_manifest.plainText.dispPix[1] <=0 ){	}
		//else{
			r_manifest.plainText.dispPix[1]+=incr;
			renderOps::UpdateDisplace2d(r_manifest.plainText, r_manifest.sy, r_manifest.sx);
			clearLineSelection();			
		//}
		break;
	case 'd':
		//if(incr <0 && r_manifest.domTree.dispPix[1] <=0 ){	}
		//else{
			r_manifest.domTree.dispPix[1]+=incr;
			renderOps::UpdateDisplace2d(r_manifest.domTree, r_manifest.sy, r_manifest.sx);
			clearLineSelection();
		//}
		break;
	case 'h':
		//if(incr <0 && r_manifest.htmlDisplay.dispPix[1]  <=0 ){	}
		//else{
			r_manifest.htmlDisplay.dispPix[1] += incr;
			renderOps::UpdateDisplace2d(r_manifest.htmlDisplay, r_manifest.sy, r_manifest.sx);
			clearLineSelection();
		//}
		break;
	default: 
		break;
	}


}



/*
	Animation update loop


*/
void Update()
{

	//Framerate code
	//#define FPS 40
	Uint32 waittime = 1000.0f/(float)FPS;
	Uint32 framestarttime = 0;
	Sint32 delaytime;

	
	bool loop = true;

	char select = 'x';


	char textbuff [100];
	while (loop) {	

		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			
			//|| gameState.Exit
			if (event.type == SDL_QUIT )
				loop = false;
		
			if( event.type == SDL_MOUSEMOTION ) { 
				//Get the mouse offsets 
				//x = event.motion.x; 
				//y = event.motion.y; 
				//renderer.pushMouseMove(event.motion.x, event.motion.y);

				int x, y; 
				SDL_GetMouseState( &x, &y );

				if(r_manifest.devOverlay.active) {
					sprintf(textbuff, "X: %d\nY: %d", x, y);
					r_manifest.devOverlay.textMap["coordsTex"]->text = textbuff;
					renderOps::updateTextblock(r_manifest.devOverlay.textMap["coordsTex"], r_manifest);
				}
			}
			
			if(event.type == SDL_MOUSEBUTTONDOWN) {
				int x, y; 
				SDL_GetMouseState( &x, &y );
				int rect[2] = {0,0};
				if(renderOps::withinRect(x, y, r_manifest.plainText.boundary, rect)) {
					clearRegionSelection();
					clearLineSelection();
					setStatus("Status: Plain text area selected.");
					select = 'p';
					selectedPlain(x,y);
				}
				else if(renderOps::withinRect(x, y, r_manifest.domTree.boundary, rect)) {
					clearRegionSelection();
					clearLineSelection();
					setStatus("Status: DOM Tree area selected.");
					select = 'd';
					selectedNode(x,y);
				}
				else if(renderOps::withinRect(x, y, r_manifest.htmlDisplay.boundary, rect)) {
					clearRegionSelection();
					clearLineSelection();
					setStatus("Status: Html display area selected.");
					select='h';
					selectedHtml(x,y);
				}
				else {
					setStatus("Status:");
					select='x';
				}

			}

	
			
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					loop = false;
				case SDLK_SPACE:
					if(r_manifest.devOverlay.active)
						r_manifest.devOverlay.active = false;
					else
						r_manifest.devOverlay.active = true;
					break;
				case SDLK_UP:
					ShiftView(true, select);
					break;
				case SDLK_DOWN:
					ShiftView(false, select);
					break;
				default:
					break;				
				}


			}
			
		
		}
		//renderer.logic();
		//renderer.display();
	
		renderer.display(r_manifest);

    	delaytime = waittime - (SDL_GetTicks() - framestarttime);
   	 	if(delaytime > 0)
        {
        	SDL_Delay((Uint32)delaytime);
        }
    	framestarttime = SDL_GetTicks();


		// Swap our back buffer to the front
		// This is the same as :
		// 		SDL_RenderPresent(&renderer);
	}
	

}






int main(int argc, char *argv[])
{
	//gameState.screen = 'm';
	//gameState.Exit = false;


	if(!Init()) {
		std::cerr<< "Unable to initialize. " <<std::endl;
		return 0;
	}


	if(!loadHTML("source_files/index2.html")) {
		std::cerr<< "Unable to load file. " <<std::endl;
		return 0;
	}

	std::cout << argv <<std::endl;	

	//d_tree.attachTree("    ", 20, 100, 770, r_manifest);
	//renderer.updateManifestText(r_manifest);

	//domElement* root; 	


	
	//r_manifest.rectMap.insert( std::make_pair( "domtree_hl", new rect2D() ) );
	//renderOps::setRectColor(r_manifest.rectMap["domtree_hl"], rgba_Green);

	float red[4];
	renderOps::setColor(red, "white");

	Update();
	
	//delete texta;
	renderOps::clearManifest(r_manifest);
	delete r_tree;
	return 0;
}

/*

bool buildDomTree(domElement* root, const char* filename)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError xmlResult = doc.LoadFile( filename);
	//tinyxml2::XMLCheckResult(xmlResult); (need to insert macro def)

	tinyxml2::XMLNode* pRoot = doc.FirstChild()->ToElement();

	if (pRoot == nullptr || !pRoot) 
		{ std::cerr<< "Unable to read xml file. " <<std::endl; 	
			return false;}


	//root = new domElement();
	
	//root->name = pRoot->Value();
	//root->lineNum = pRoot->GetLineNum();

	tinyxml2::XMLNode* test = doc.FirstChild();

	if(test->ToElement())
	{
		std::cout<< "Node is a comment" << std::endl;
	}

	if(!pRoot->NoChildren())
	{
		tinyxml2::XMLNode* test = pRoot->FirstChild();
		std::cout << test->Value() << std::endl;


		while(test->NextSibling())
		{
			test = test->NextSibling();
			std::cout << test->Value() << std::endl;
		}
	}
	//std::cout << pRoot->Value() << std::endl;
	

	return true;
}


domNode* processNode(tinyxml2::XMLNode* xNode, int depth)
{
	if(xNode->ToComment()) {
		//std::cout << "A comment node" << std::endl; 
		return new domNode(Comment, xNode->Value(),  xNode->GetLineNum(), depth);		
	}
	else if(xNode->ToText()) {
		//std::cout << "A Text node" << std::endl; 
		return new domNode(Text, xNode->Value(),  xNode->GetLineNum(), depth);
	}
	else if(xNode->ToElement())
	{
		domElement* out = new domElement(Element, xNode->Value(),  xNode->GetLineNum(), depth);
		//std::cout << "An Element node: " << xNode->Value() << std::endl; 


		if(xNode->NoChildren())
			return out;

		tinyxml2::XMLNode* cNode = xNode->FirstChild();

		out->children.push_back(processNode(cNode, depth+1));
		while(cNode->NextSibling())
		{
			cNode = cNode->NextSibling();
			out->children.push_back(processNode(cNode, depth+1));
		}
		return out;
	}
}


void printNode(domNode* dNode, std::list<domNode*>& nodeList)
{
	std::string breh = dNode->value;
	std::string out;

	if(dNode->type == Comment)
	{
		//std::cout << "Comment: " + breh << std::endl;
				//return out.c_str();
		nodeList.push_back( dNode );

	}
	else if(dNode->type == Text)
	{
		//std::cout << depth + "Text: " + breh << std::endl;
		nodeList.push_back( dNode );	
	}		
	else if(dNode->type == Element)
	{
		//std::cout << depth + "Element: " + breh << std::endl;
		nodeList.push_back( dNode );
		//std::cout << "THIS FAR4?" << std::endl;
		domElement *deNode = dynamic_cast<domElement*>(dNode);
		
		if(deNode && !deNode->children.empty())
		{
			for( auto const& i : deNode->children)
			{
				printNode(i, nodeList);
			}
		}	

	}
}






/*
void RunGame()
{


	//Framerate code
	#define FPS 40
	Uint32 waittime = 1000.0f/FPS;
	Uint32 framestarttime = 0;
	Sint32 delaytime;

	bool loop = true;

	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;

			//drawSquare();
	
			
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				
				case SDLK_s:
					if(map_Y <= .8)
					{
						map_Y += .02;
					}
					//glTranslatef(0, 0, -0.5);
					//glRotatef(10, 0, 0, 0);
					break;
				case SDLK_w:
					if(map_Y >= -.8)
					{
						map_Y -= .02;
					}
					//glTranslatef(0, 0, 0.5);
					//glRotatef(-10, 0, 0, 0);
					break;
				case SDLK_d:
					if(map_X <= .8)
					{
						map_X += .02;
					}
					//glTranslatef(-0.5, 0, 0);
					//glRotatef(10, 0, 0, 0);
					break;
				case SDLK_a:
					if(map_X >= -.8)
					{
						map_X -= .02;
					}
					//glTranslatef(0.5, 0, 0);
					//glRotatef(-10, 0, 0, 0);
					break;
				case SDLK_t:
					if(map_D <= .8)
					{
						map_D += .02;
					}
					//glRotatef(10, 0, 0, 0);
					break;
				case SDLK_g:
					if(map_D >= -.8)
					{
						map_D -= .02;
					}
					//glRotatef(-10, 0, 0, 0);
					break;
				
				
				case SDLK_b:
					// Cover with blue and update
					glClearColor(0.0, 0.0, 1.0, 1.0);
					glClear(GL_COLOR_BUFFER_BIT);
					SDL_GL_SwapWindow(mainWindow);
					break;
				
				default:
					break;
				
				}
			}
		//glClearColor(1.0, 1.0, 1.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);
		
		}

		//draw via OpenGL, Framerate independant. 	
		//drawSquare();
		display();
		//SDL_GL_SwapWindow(mainWindow);

		//framerate regulation
    	delaytime = waittime - (SDL_GetTicks() - framestarttime);
   	 	if(delaytime > 0)
        	SDL_Delay((Uint32)delaytime);
    	framestarttime = SDL_GetTicks();


		// Swap our back buffer to the front
		// This is the same as :
		// 		SDL_RenderPresent(&renderer);
	}
}
*/

/*

void drawSquare()
{



	glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Draw ground texture


	//Draw grid
    //	Set Grid color to black
	glColor3f(0.0, 0.0, 0.0);
  	glBegin(GL_LINES);

  	int x_lim = map_Sx;
  	int y_lim = map_Sy;
  	
  	for (GLfloat i = -x_lim; i <=x_lim; i += 2) 
  	{
    	//Vertical Lines
    	glVertex3f((i+map_X), map_D, (y_lim+map_Y)); glVertex3f((i+map_X), map_D, (-y_lim+map_Y));
    	
    	//Horizontal Lines
    	//glVertex3f((5+map_X), map_D, (i+map_Y)); glVertex3f((-5+map_X), map_D, (i+map_Y));
  	}  	
  	
  	for (GLfloat i = -y_lim; i <=y_lim; i += 2) 
  	{
    	//Vertical Lines
    	//glVertex3f((i+map_X), map_D, (5+map_Y)); glVertex3f((i+map_X), map_D, (-5+map_Y));
    	
    	//Horizontal Lines
    	glVertex3f((x_lim+map_X), map_D, (i+map_Y)); glVertex3f((-x_lim+map_X), map_D, (i+map_Y));
  	}  	
  	glEnd();

 	 // Draw the tetrahedron.  It is a four sided figure, so when defining it
  	//	with a triangle strip we have to repeat the last two vertices.
  	/*
  	glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.8, 0.2, 0); glVertex3f(0, 2, 0);
    glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);
    glColor3f(0, 1, 0); glVertex3f(1, 0, 1);
    glColor3f(0, 0, 1); glVertex3f(0, 0, -1.4);
    glColor3f(0.8, 0.2, 0); glVertex3f(0, 2, 0);
    glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);

	glEnd();
	*/

/*
	glBegin(GL_TRIANGLE_STRIP);
    //glColor3f(0.8, 0.2, 0); glVertex3f(0, 2, 0);
    glColor3f(1, 0, 0); glVertex3f(-0.05, 0, 0.05);
    glColor3f(0, 1, 0); glVertex3f(0.05, 0, 0.05);
    glColor3f(0, 0, 1); glVertex3f(-0.05, 0, -0.05);
    glColor3f(0, 0, 1); glVertex3f(0.05, 0, -0.05);
    //glColor3f(0.8, 0.2, 0); glVertex3f(0, 2, 0);
   // glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);

	glEnd();

	//8, 4.5


	//Top Line area
	glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0, 0, 0.3); glVertex3f(-X_brd, 0, -4.5); //top left
    glColor3f(0, 0, 0.3); glVertex3f(X_brd, 0, -4.5); //top right
    glColor3f(0,0.08,0.8); glVertex3f(-X_brd, 0, -3.75);
    glColor3f(0,0.08,0.8); glVertex3f(X_brd, 0, -3.75);
    glEnd();


    //Bottom Line area
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0,0.08,0.8); glVertex3f(-X_brd, 0, 3.75);
    glColor3f(0,0.08,0.8); glVertex3f(X_brd, 0, 3.75);
    glColor3f(0, 0, 0.3); glVertex3f(-X_brd, 0, 4.5);
    glColor3f(0, 0, 0.3); glVertex3f(X_brd, 0, 4.5);

    glEnd();

    

    //Left side
	glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0, 0, 1); glVertex3f(-X_brd, 0, 4.5);
    glColor3f(0.5,0,0.5); glVertex3f(-5.5, 0, 4.5);
    glColor3f(0, 0, 1); glVertex3f(-X_brd, 0, -4.5);
    glColor3f(0.5, 0, 0.5); glVertex3f(-5.5, 0, -4.5);
    glEnd();

    //Right side
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1, 0, 0); glVertex3f(X_brd, 0, 4.5);
    glColor3f(0, 0,1); glVertex3f(5.5, 0, 4.5);
    glColor3f(0.5, 0, 0.5); glVertex3f(X_brd, 0, -4.5);
    glColor3f(0, 0, 1); glVertex3f(5.5, 0, -4.5);
    glEnd();





  	
    glFlush();
  }

*/
