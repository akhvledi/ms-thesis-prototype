
#include <string>
#include <iostream>
#include <map>
#include <list>


#include <math.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>




enum FontWeight {normal, bold, bolder, lighter, one, two, three, four, five, six, seven, eight, nine, initial, inherit};
enum DisplayStyle {none, in_line, block, inline_block};



std::list<std::string> hiddenNodes = {"area", "base", "basefront", "datalist", 
										"head", "link", "meta",	"noembed", "noframes",
										"param", "script", "source", "style", 
										"template", "track", "title"};

std::list<std::string> ElementList = {"h1", "h2", "h3", "h4", "h5", "h6"};

struct styleProperties { 
	float margin_block_start=0;
	float margin_block_end=0;
	float font_size=1;
	FontWeight font_weight;
	DisplayStyle display;
};

struct renderNode{
	
	styleProperties style;
	std::string content;

	std::list<renderNode*> children;

	int nIndex;

	renderNode(){

	}

};


class renderTree {
private:	
	std::map<std::string, styleProperties> stylesDefault;
	renderNode* r_root;
	float eletag_rgba[4] ={0.8, 1.0, 1.0, 0.0};
	
public:
	renderTree() {

		styleProperties html;
		html.display = block;

		styleProperties body;
		body.display = block;

		styleProperties p;
		p.display = block;
		p.margin_block_start = 1;
		p.margin_block_end = 1;


		styleProperties h1;
		h1.margin_block_start = 0.67;
		h1.margin_block_end = 0.67;
		h1.font_size = 2.00;
		h1.font_weight = bold;

		styleProperties h2;
		h2.margin_block_start = 0.83;
		h2.margin_block_end = 0.83;
		h2.font_size = 1.50;

		stylesDefault.insert(std::make_pair("html", html));
		stylesDefault.insert(std::make_pair("body", body));
		stylesDefault.insert(std::make_pair("h1", h1));
		stylesDefault.insert(std::make_pair("h2", h2));

	}

	void buildRenderTree(domNode* d_node) {

		r_root = processDnode(d_node);

	}

	renderNode* processDnode(domNode* d_node)
	{
		std::string p = "p";
		std::string h1= "h1";

		renderNode *new_rnode = new renderNode();

		if(!d_node->type==Element){
			std::cout << "Null tree" << std::endl;
			return new_rnode;
		}

		new_rnode->nIndex = d_node->nIndex;
	
		if(p.compare(d_node->value)==0) {
			new_rnode->style.margin_block_start = 1;
			new_rnode->style.margin_block_end = 1;
		}

		if(h1.compare(d_node->value)==0) {
			new_rnode->style.margin_block_start = 0.67;
			new_rnode->style.margin_block_end = 0.67;
			new_rnode->style.font_size = 2.00;
		}


		domElement *de_node = dynamic_cast<domElement*>(d_node);
		if(!de_node->children.empty()) {		
			std::cout << "Element " <<  d_node->value << "  - contains "<< de_node->children.size()<< " children." << std::endl;
			for ( auto const& i : de_node->children) {
				bool hidden = false;
				for ( auto const& i : hiddenNodes) {
					if(i.compare(d_node->value)==0)
						hidden = true;
				}

				if(i->type==Text && !hidden)
					new_rnode->content = i->value;

				if(i->type==Element && !hidden){
					new_rnode->children.push_back(processDnode(i));
				}

			}
		}

		return new_rnode;
	}


	void attachRTree(int f_scale, render_manifest &manifest, subRegion &region, std::string name) {
		std::vector<textblock*> texVec;
		int h = (manifest.htmlDisplay.boundary->pixelCoords[0][1] +manifest.htmlDisplay.padding );
		std::cout << h <<std::endl;


		std::map<int,textblock*> texMap;
		std::map<int,rect2D*> recMap;
		region.textMapMap.insert( std::make_pair( name,  texMap));
		region.rectMapMap.insert( std::make_pair(name,  recMap));


		printNode(r_root, h, f_scale, manifest, region, name);
		
		region.textVectMap.insert( std::make_pair( name,  texVec));		
	}



	void printNode( renderNode* node, int &height, int f_scale, render_manifest &manifest, subRegion &region, std::string key) {
		int base_x = manifest.htmlDisplay.boundary->pixelCoords[0][0] +  manifest.htmlDisplay.padding;
	

		if(!node->content.empty()) {
			//char *textb = new char[node->content.size()];
			//std::strcpy(textb, node->content.c_str());

			int n = node->nIndex;

			std::cout << "current height: " << height << std::endl;


			textblock* newtb = new textblock(node->content, base_x + (node->style.margin_block_start*f_scale), height, f_scale * node->style.font_size);
			newtb->active= true;
			renderOps::updateTextblock(newtb, manifest);
			
			//texVec.push_back(newtb);			
			region.textMapMap[key].insert( std::make_pair( n,  newtb));


			
			region.rectMapMap[key].insert( std::make_pair(n,  new rect2D()));

			//region.rectMapMap[key][n]->active = true;
			renderOps::setTextPerim(region.textMapMap[key][n],region.rectMapMap[key][n], 0);
			renderOps::updateRectCoords(region.rectMapMap[key][n], manifest.sx, manifest.sy);	
			renderOps::setRectColor(region.rectMapMap[key][n], eletag_rgba);

			height = height +  f_scale *node->style.font_size *2;	
				
		}

		if(!node->children.empty())
		{
			for ( auto const& i : node->children) {
				///std::cout << h << std::endl;
				printNode(i, height, f_scale, manifest, region, key);
			}
		}


	}

/*

		 {
			for ( auto const& i : hiddenNodes) {
				if(i.compare(d_node->value)==0){
					std::cout << d_node->value << " is a hidden element" << std::endl;
					return;
				}
			}

			domElement *de_node = dynamic_cast<domElement*>(d_node);
			if(!de_node->children.empty()) {		
				std::cout << "Element " <<  d_node->value << "  - contains "<< de_node->children.size()<< " children." << std::endl;		
				for ( auto const& i : de_node->children) {
					renderNode *new_rnode = new renderNode();
					std::cout << "Here ?" << std::endl;

					new_rnode->parent = r_node;


					buildTree(i, new_rnode);
					std::cout << "Here ?" << std::endl;
					r_node->children.push_back(new_rnode);

				}

		}



	}*/


};




