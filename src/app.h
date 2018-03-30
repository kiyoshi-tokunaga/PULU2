#pragma once

#include "ofMain.h"
#include "particle.h"
#include "spring.h"
#include "ofxUI.h"


typedef struct index2{
	int x;
	int y;
}INDEX;

typedef struct puru2{
	int type;
	particle p;
	ofImage i;
}PURU2;

class app : public ofBaseApp{

	private:
		vector< vector<PURU2> > manage; //gridä«óù
		//0:for init, 1:for in, 2for out:
		vector<particle> kotei;
		vector<INDEX> pB;
		vector<INDEX> obj;
		vector<spring> springs;
		ofImage src_img; int srcW, srcH;

		PURU2 puru;
		void puru_setup();

		//for visualization
		ofVec2f circle_point;
		INDEX p_point;
		float rad; float omega;

	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxUICanvas *gui;
		void setGUI();
		void guiEvent(ofxUIEventArgs &e);
		
		int MODE_TYPE; //0:Set, 1:Clear, 2:Move, 3:Pick
		bool drawPadding;

};
