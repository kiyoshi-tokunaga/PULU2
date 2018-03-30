#include "app.h"

int GRID_SIZE = 15;
int DRAW_SIZE = 15;
int TYPE = 1;
//MODE_TYPE
int N_MODE = 4;
int MODE_TYPE_SET = 0;
int MODE_TYPE_CLEAR = 1;
int MODE_TYPE_MOVE = 2;
int MODE_TYPE_PICK = 3;

int WinW = 640, WinH = 480;
int CPW = 300, CPH = 480;
int MANW, MANH;
//keisu
float M = 0.02F;
float SPRINGINESS = 0.02F;
//gui
std::string WIN = "Control Panel";
bool MOUSE_ON = false;
bool UPDATE = false;
bool PICK = false;
bool GRID = true;

//--------------------------------------------------------------
void app::setup(){
	ofSetWindowShape(WinW + CPW, WinH);
	ofBackground(125, 125, 125);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	if(src_img.loadImage("src.png") == false){
		cout << "Can't load image." << endl;
		exit();
	}

	//resize src_img
    float ratioMin, ratioW = WinW / src_img.getWidth(), ratioH = WinH / src_img.getHeight();
    if (ratioW < ratioH) 
        ratioMin = ratioW;
    else
        ratioMin = ratioH;
	srcW = ratioMin*src_img.getWidth();
	srcH = ratioMin*src_img.getHeight();
	src_img.resize(srcW, srcH);

	//set manage size
	MANW = srcW/GRID_SIZE;
	MANH = srcH/GRID_SIZE;
	
	//動的確保
	manage.resize(MANW);
	for(int i = 0; i < MANW; i++)
		manage[i].resize(MANH);


	//init manage
	unsigned char *src_data = src_img.getPixels();
	for(int i = 0; i < MANW; i++){
		for(int j = 0; j < MANH; j++){
			//set type
			manage[i][j].type = 0;

			//set particle
			manage[i][j].p.setInitialCondition((float)i*GRID_SIZE, (float)j*GRID_SIZE, 0, 0);
			manage[i][j].p.damping = 0.2F;

			//set image
			unsigned char *tmp_data;
			switch (src_img.type) {
			  case OF_IMAGE_GRAYSCALE:
			     manage[i][j].i.allocate(GRID_SIZE, GRID_SIZE, OF_IMAGE_GRAYSCALE);
					tmp_data = manage[i][j].i.getPixels();
					for(int k = 0; k < GRID_SIZE; k++){
						for(int l = 0; l < GRID_SIZE; l++){
							int index = (int)(manage[i][j].p.pos.y + k)*src_img.getWidth() + (int)(manage[i][j].p.pos.x + l);
							tmp_data[(int)(k*GRID_SIZE)+l] = src_data[index];
						}
					}
					manage[i][j].i.update();
					break;
			  case OF_IMAGE_COLOR:
					manage[i][j].i.allocate(GRID_SIZE, GRID_SIZE, OF_IMAGE_COLOR);
					tmp_data = manage[i][j].i.getPixels();
					for(int k = 0; k < GRID_SIZE; k++){
						for(int l = 0; l < GRID_SIZE; l++){
							int index = (int)(manage[i][j].p.pos.y + k)*src_img.getWidth() + (int)(manage[i][j].p.pos.x + l);
							tmp_data[((int)(k*GRID_SIZE)+l)*3] = src_data[index*3];
							tmp_data[((int)(k*GRID_SIZE)+l)*3 + 1] = src_data[index*3 + 1];	
							tmp_data[((int)(k*GRID_SIZE)+l)*3 + 2] = src_data[index*3 + 2];	
						}
					}
					manage[i][j].i.update();
				 break;
			 case OF_IMAGE_COLOR_ALPHA:
					manage[i][j].i.allocate(GRID_SIZE, GRID_SIZE, OF_IMAGE_COLOR_ALPHA);
					tmp_data = manage[i][j].i.getPixels();
					for(int k = 0; k < GRID_SIZE; k++){
						for(int l = 0; l < GRID_SIZE; l++){
							int index = (int)(manage[i][j].p.pos.y + k)*src_img.getWidth() + (int)(manage[i][j].p.pos.x + l);
							tmp_data[((int)(k*GRID_SIZE)+l)*4] = src_data[index*4];
							tmp_data[((int)(k*GRID_SIZE)+l)*4 + 1] = src_data[index*4 + 1];	
							tmp_data[((int)(k*GRID_SIZE)+l)*4 + 2] = src_data[index*4 + 2];	
							tmp_data[((int)(k*GRID_SIZE)+l)*4 + 3] = src_data[index*4 + 3];	
						}
					}
					manage[i][j].i.update();
					break;
			}
		}
	}

	setGUI();

}

//--------------------------------------------------------------
void app::update(){

	rad += 0.01;
	
	for(int i = 0; i < obj.size(); i++){
		manage[obj[i].x][obj[i].y].p.resetForce();
	}
	
	if(!MOUSE_ON){
		for(int i = 0; i < springs.size(); i++)
			springs[i].update();

		for(int i = 0; i < obj.size(); i++){
			manage[obj[i].x][obj[i].y].p.bounceOffWalls();
			manage[obj[i].x][obj[i].y].p.addDampingForce();
			manage[obj[i].x][obj[i].y].p.update();
		}
	}


}

//--------------------------------------------------------------
void app::draw(){

	if(MOUSE_ON == true && MODE_TYPE != MODE_TYPE_PICK){
		ofFill();
		ofCircle(mouseX, mouseY, DRAW_SIZE);
	}else{
		src_img.draw(0, 0);

		for(int i = 0; i < obj.size(); i++){
			manage[obj[i].x][obj[i].y].i.draw(manage[obj[i].x][obj[i].y].p.pos.x, manage[obj[i].x][obj[i].y].p.pos.y);
		}
		if(GRID){
			for(int i = 0; i <springs.size(); i++)
				springs[i].draw();
		}
	}

	ofSetRectMode(OF_RECTMODE_CORNER);


}

//--------------------------------------------------------------
void app::exit(){

	//clear
	obj.clear();
	kotei.clear();
	pB.clear();
	springs.clear();
	for(int i = 0; i < manage.size(); i++)
		manage[i].clear();
	manage.clear();


	gui->saveSettings("GUI/guiSettings.xml"); 
	delete gui;
}

//--------------------------------------------------------------
void app::guiEvent(ofxUIEventArgs &e){

	string name = e.widget->getName();
	int kind = e.widget->getKind();

	//slider
	if(name == "MASS VALUE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;     
		M = (float)slider->getScaledValue();
    }
    else if(name == "SPRINGINESS VALUE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        SPRINGINESS = (float)slider->getScaledValue();
    }
	//Check Mode
	if(kind == OFX_UI_WIDGET_TOGGLE){
		ofxUIRadio *radio = (ofxUIRadio *) e.widget;
		//cout << radio->getName() << endl;
		if(radio->getName() == "Set"){
			MODE_TYPE = MODE_TYPE_SET;
		}else if(radio->getName() == "Clear"){
			MODE_TYPE = MODE_TYPE_CLEAR;
		}else if(radio->getName() == "Move"){
			MODE_TYPE = MODE_TYPE_MOVE;
		}else if(radio->getName() == "Pick"){
			MODE_TYPE = MODE_TYPE_PICK;
		}
	}

}

//--------------------------------------------------------------
void app::keyPressed(int key){

	if(key == 's'){
		if(obj.size() > 0)
			manage[obj[0].x][obj[0].y].p.pos.set(mouseX, mouseY);
	}

	if(key == 'g'){
		if(GRID == true){
			GRID = false;
		}else{
			GRID = true;
		}
	}

	if(key == 'c'){
		for(int i = 0; i < MANW; i++){
			for(int j = 0; j < MANH; j++){
				//set type
				manage[i][j].type = 0;
				manage[i][j].p.damping = 0.02F;
			}
		}
		obj.clear();
		kotei.clear();
		pB.clear();
		springs.clear();
	}

    switch (key) {
        case 'p':
            gui->setDrawWidgetPadding(true);
            break;
        case 'P':
            gui->setDrawWidgetPadding(false);
            break;

        default:
            break;
    }
	

}

//--------------------------------------------------------------
void app::keyReleased(int key){

}

//--------------------------------------------------------------
void app::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void app::mouseDragged(int x, int y, int button){

	//problem
	if(MOUSE_ON == true && x <= srcW && y <= srcH){
		if(MODE_TYPE == MODE_TYPE_SET){
			for(int i = (int)(x-DRAW_SIZE)/GRID_SIZE; i < (int)(x+DRAW_SIZE)/GRID_SIZE; i++){
				for(int j = (int)(y-DRAW_SIZE)/GRID_SIZE; j < (int)(y+DRAW_SIZE)/GRID_SIZE; j++){
					if(0 < i && i < MANW-1 && 0 < j && j < MANH-1){
							manage[i][j].type = 1;
							manage[i][j].p.damping = M;
					}
				}
			}
			UPDATE = true;
		}else if(MODE_TYPE == MODE_TYPE_CLEAR){
			for(int i = (int)(x-DRAW_SIZE)/GRID_SIZE; i < (int)(x+DRAW_SIZE)/GRID_SIZE; i++){
				for(int j = (int)(y-DRAW_SIZE)/GRID_SIZE; j < (int)(y+DRAW_SIZE)/GRID_SIZE; j++){
					if(0 < i && i < MANW-1 && 0 < j && j < MANH-1)
							manage[i][j].type = 0;
				}
			}
			UPDATE = true;
		}

	//pick
	if(MODE_TYPE == MODE_TYPE_PICK && PICK && manage[p_point.x][p_point.y].type == 1)
			manage[p_point.x][p_point.y].p.pos = ofVec2f((float)x, (float)y);

	}


}

//--------------------------------------------------------------
void app::mousePressed(int x, int y, int button){



	MOUSE_ON = true;
	p_point.x = (int)(mouseX/GRID_SIZE);
	p_point.y = (int)(mouseY/GRID_SIZE);

	//PICK UP
	if(MODE_TYPE == MODE_TYPE_PICK && p_point.x < MANW && p_point.x > 0 && p_point.y < MANH && p_point.y > 0){
		PICK = true;
	}

	//塗りつぶしON
	ofSetBackgroundAuto(!MOUSE_ON);
}

//--------------------------------------------------------------
void app::mouseReleased(int x, int y, int button){



	MOUSE_ON = false;
	//塗りつぶしOFF
	ofSetBackgroundAuto(!MOUSE_ON);
	//update PURU2
	if(UPDATE == true){
		puru_setup();
		UPDATE = false;
	}

	if(PICK == true)
		PICK = false;

}

//--------------------------------------------------------------
void app::windowResized(int w, int h){

}

//--------------------------------------------------------------
void app::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void app::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------
void app::puru_setup(){

	//init
	obj.clear();
	kotei.clear();
	pB.clear();
	springs.clear();

	//input particle index
	for(int i = 0; i < MANW; i++){
		for(int j = 0; j < MANH; j++){
			if(manage[i][j].type == 1){
				INDEX tmp_index = {i, j};
				obj.push_back(tmp_index);
			}
		}
	}

	//set spring
	spring tmp_s;
		tmp_s.distance = GRID_SIZE;
		tmp_s.springiness = SPRINGINESS;
		particle tmp_p;
			tmp_p.damping = 0.2F;

	for(int i = 0; i < obj.size(); i ++){
		int x = obj[i].x;
		int y = obj[i].y;
		if(x > 0 && y > 0 && x < MANW - 1 && y < MANH - 1){
			//外周以外
			//up spring
			if(manage[x][y-1].type == 1){
				tmp_s.particleA = &(manage[x][y-1].p);
				tmp_s.particleB = &(manage[x][y].p);
				springs.push_back(tmp_s);
			}else{
					//固定端
				tmp_p.setInitialCondition(manage[x][y-1].p.pos.x , manage[x][y-1].p.pos.y, 0, 0);
				kotei.push_back(tmp_p);
				pB.push_back(obj[i]);
			}
			//left spring
			if(manage[x-1][y].type == 1){
				tmp_s.particleA = &(manage[x-1][y].p);
				tmp_s.particleB = &(manage[x][y].p);
				springs.push_back(tmp_s);
			}else{
					//固定端
				tmp_p.setInitialCondition(manage[x-1][y].p.pos.x , manage[x-1][y].p.pos.y, 0, 0);
				kotei.push_back(tmp_p);
				pB.push_back(obj[i]);
			}
			//right spring
			if(manage[x+1][y].type == 0){
					//固定端
				tmp_p.setInitialCondition(manage[x+1][y].p.pos.x , manage[x+1][y].p.pos.y, 0, 0);
				kotei.push_back(tmp_p);
				pB.push_back(obj[i]);
			}
			//down spring
			if(manage[x][y+1].type == 0){
					//固定端
				tmp_p.setInitialCondition(manage[x][y+1].p.pos.x , manage[x][y+1].p.pos.y, 0, 0);
				kotei.push_back(tmp_p);
				pB.push_back(obj[i]);
			}
		}
	}

	if(!(kotei.size() == pB.size()))
		return;

	for(int i = 0; i < kotei.size(); i++){
		tmp_s.particleA = &(kotei[i]);
		tmp_s.particleB = &(manage[pB[i].x][pB[i].y].p);
		springs.push_back(tmp_s);
	}

}

//--------------------------------------------
void app::setGUI(){

	//gui setup
	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = CPW-xInit;
    vector<string> names; 
	names.push_back("Set");
	names.push_back("Clear");
	names.push_back("Move");
	names.push_back("Pick");

    gui = new ofxUICanvas(WinW,0,length+xInit,CPH);
	gui->setColorBack(ofColor(125, 125, 125));
    gui->addWidgetDown(new ofxUILabel(WIN, OFX_UI_FONT_LARGE)); 

	//botton
	gui->addSpacer(length-xInit, 2); 	
    gui->addRadio("MODE", names, OFX_UI_ORIENTATION_HORIZONTAL, dim, dim);

	//slider
	gui->addSpacer(length-xInit, 2); 
    gui->addSlider("MASS VALUE", 0.0, 1.0, 0.01, length-xInit, dim);
    gui->addSlider("SPRINGINESS VALUE", 0.0, 0.4, 0.01,length-xInit, dim);

	//2DPad
	//gui->addSpacer(length-xInit, 2);
	//gui->addWidgetDown(new ofxUILabel("2D Force", OFX_UI_FONT_MEDIUM)); 
	//gui->add2DPad("Force", ofPoint(0, 3.0), ofPoint(0, 3.0), ofPoint(100,0), 100, 100, 100.0F, 0);

	//display fps
	gui->addSpacer(length-xInit, 2);
    gui->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM));

	//display manual
	gui->addLabel("manual_image","read file is\n'PURU2/bin/data/src.png'");
	gui->addLabel("manual_set", "set: set springs");
	gui->addLabel("manual_clear", "clear: clear springs");
	gui->addLabel("manual_move", "move: not implemented");
	gui->addLabel("manual_pick", "pick: press left-side mouse button\n-> dragg -> release mouse button");
	gui->addLabel("manual_g", "press 'g' key:\ndisplay/not display springs");
	gui->addLabel("manual_c", "press 'c' key: clear all springs");

	//xml
    ofAddListener(gui->newGUIEvent, this, &app::guiEvent); 
    gui->loadSettings("GUI/guiSettings.xml"); 


}



