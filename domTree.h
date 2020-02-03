#include <string>
#include <iostream>
#include <map>

#include <math.h>
#include <list>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>

//#include "domnode.h"
#include "RenderManifest.h"
#include "renderOps.h"

/*TinyXML ****/
#include "lib/tinyxml2.h"


enum NodeType {Element, Comment, Text};

struct domNode {
	NodeType type;
	std::string value;
	int lineNum;
	int depth;

	int nIndex;

	domNode(NodeType t, std::string v, int l, int d, int n) {
		type = t;
		value = v;
		lineNum = l;
		depth = d;
		nIndex = n;
	}

	virtual ~domNode() {}
	
};


struct domElement : public domNode {
	//std::string name;
	std::map<std::string, std::string> attributes;
	std::list<domNode*> children;

	domElement(NodeType t, std::string v, int l,int d, int n) : domNode(t, v, l, d, n) {
	}

	
};



class domTree {

public:

	domTree(){

	}


	
	bool treeFromFile(const char* fileN)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError xmlResult = doc.LoadFile(fileN);

		pRoot = doc.FirstChild();

		std::cout << "Processing nodes" << std::endl;
		int index = 0;
		root = processNode(pRoot, 0, index); 



		return true;
	}


	domNode* processNode(tinyxml2::XMLNode* xNode, int depth, int &ind)
	{
		std::string outstring ="";

		

		if(xNode->ToComment()) {
			//std::cout << "A comment node" << std::endl; 
			outstring = std::string(xNode->Value());
			return new domNode(Comment, xNode->Value(),  xNode->GetLineNum(), depth, ind);		
		}
		else if(xNode->ToText()) {
			
			outstring = std::string(xNode->Value());
			//std::cout << " A Text node: " << outstring << std::endl; 
			return new domNode(Text,  xNode->Value(),  xNode->GetLineNum(), depth, ind);
		}
		else if(xNode->ToElement())
		{
			outstring = std::string(xNode->Value());
			//std::cout << " An Element node: " << outstring << std::endl; 
			domElement* out = new domElement(Element, xNode->Value(),  xNode->GetLineNum(), depth, ind);
			

			if(xNode->NoChildren())
				return out;
							

			tinyxml2::XMLNode* cNode = xNode->FirstChild();

			ind++;
			out->children.push_back(processNode(cNode, depth+1, ind));
			while(cNode->NextSibling())
			{
				cNode = cNode->NextSibling();
				ind++;
				out->children.push_back(processNode(cNode, depth+1, ind));
			}
			

			return out;
		}
	}


	void listNodes(domNode* dNode, std::vector<domNode*> &nodeList)
	{
		
		//std::string out = ().c_str();

		//std::cout << "Depth is " << dNode->depth << " ";
		
		if(dNode->type == Comment)
		{
			std::string breh = std::string(dNode->value);
			//std::cout << "Comment: " + breh << std::endl;
			nodeList.push_back( dNode );
		}
		else if(dNode->type == Text)
		{
			std::string breh = std::string(dNode->value);
			//std::cout <<  "  Text: " + breh << std::endl;
			nodeList.push_back( dNode );	
		}		
		else if(dNode->type == Element)
		{
			std::string breh = std::string(dNode->value);
			//std::cout << " Element: <" + breh+">" << std::endl;
			
			nodeList.push_back( dNode );
			//std::cout << "THIS FAR4?" << std::endl;
			domElement *deNode = dynamic_cast<domElement*>(dNode);
		
			if(deNode && !deNode->children.empty())
			{
				for( auto const& i : deNode->children) {
					listNodes(i, nodeList);
				}
			}	

		}	
	}

	/*

		[string] i_span : index span (just a string containing however many spaces per indent)
		[int] f_scale : font scale (needed to do line by line properly)
		[int] y_offset : Y pixel position offset for text display

	*/
	void  attachTree(int f_scale, render_manifest &manifest) {
		
		x = manifest.domTree.boundary->pixelCoords[0][0] +  manifest.domTree.padding + f_scale;
		y = manifest.domTree.boundary->pixelCoords[0][1] +  manifest.domTree.padding ;

		std::cout << "Building list of nodes" << std::endl;
		listNodes(root, nodeList);


		std::vector<textblock*> texVec;
		std::vector<rect2D*> recVec;


		if(!nodeList.empty()) {	
			int k =0;
			for( auto const& i : nodeList)	{

				int indenta = 0;
				std::string out = "";

				
				for(int j =0; j < i->depth; j++) {			
					indenta+=1;
				}

				//std::string key = "node" + k;
				int height = y + ((f_scale*1.5) * k);						

				
				if(i->type == Comment){			
					out = "Comment: "+ i->value +" ";
					//char *textb = new char[out.size()];
					//std::strcpy(textb, out.c_str());

					texVec.push_back(new textblock(out, x + (indenta*f_scale), height, f_scale));
					texVec[k]->active = true;
					texVec[k]->setColor(txt_rgba);
					renderOps::updateTextblock(texVec[k], manifest);


					recVec.push_back( new rect2D() );
					recVec[k]->active = true;
					renderOps::setRectColor(recVec[k], txtbg_rgba);
					renderOps::setTextPerim(texVec[k],recVec[k], 0);
					renderOps::updateRectCoords(recVec[k], manifest.sx, manifest.sy);	
				
					k++;
				}
				else if(i->type == Text) {
					//std::cout << i->value.c_str() << std::endl;
					out = "Text: "+ i->value +" ";


					//std::cout << "Text Processing" << std::endl;

					//char *textb = new char[out.size()];
					//std::strcpy(textb, out.c_str());

					texVec.push_back(new textblock(out, x + (indenta*f_scale), height, f_scale));
					texVec[k]->active = true;
					texVec[k]->setColor(txt_rgba);
					renderOps::updateTextblock(texVec[k], manifest);


					recVec.push_back( new rect2D() );
					recVec[k]->active = true;
					renderOps::setRectColor(recVec[k], txtbg_rgba);
					renderOps::setTextPerim(texVec[k],recVec[k], 0);
					renderOps::updateRectCoords(recVec[k], manifest.sx, manifest.sy);	
				
					k++;		
				}
				else if(i->type == Element) {
					//sprintf(textbuff, "Element: %s ", i->value.c_str());
					out = i->value;

					//char *textb = new char[out.size() ];
					//std::strcpy(textb, out.c_str());

					//std::cout << "Element Processing" << std::endl;
					
					texVec.push_back(new textblock(out, x + (indenta*f_scale), height, f_scale));
					texVec[k]->active = true;
					texVec[k]->setColor(eletag_rgba);
					renderOps::updateTextblock(texVec[k], manifest);


					recVec.push_back( new rect2D() );
					recVec[k]->active = true;
					renderOps::setRectColor(recVec[k], elebg_rgba);
					renderOps::setTextPerim(texVec[k],recVec[k], 0);
					renderOps::updateRectCoords(recVec[k], manifest.sx, manifest.sy);	
				

					k++;

				}
			
			}

			manifest.domTree.textVectMap.insert( std::make_pair( "nodeText",  texVec));
			manifest.domTree.rectVectMap.insert( std::make_pair( "nodeBorder",  recVec));
		}
	}

	
	std::list<int> lineNodes(int line, render_manifest manifest)
	{
		std::list<int> out;

		if(nodeList.empty())
			return out;
	
		int k =0;
		for( auto const& i : nodeList) {
			if(i->lineNum == line) {
				out.push_back(k);
			}
			k++;
		}

		return out;
	}




	int X(){
		return x;
	}

	int Y(){
		return y;
	}

	int nodeLineNum( int index)
	{
		return nodeList[index]->lineNum;
	}

	domNode* getRoot()
	{
		return root;
	}

private:

	tinyxml2::XMLNode* pRoot;
	domNode* root;
	std::vector<domNode*> nodeList;
	int x;
	int y;

	float eletag_rgba[4] ={0.9, 1.0, 1.0, 1.0};
	float elebg_rgba[4] ={0.1, 0.3, 0.7, 1.0};

	

	float txt_rgba[4] = {0.0, 0.0, 0.0, 1.0};
	float txtbg_rgba[4] = {1.0, 1.0, 0.8, 1.0};


};