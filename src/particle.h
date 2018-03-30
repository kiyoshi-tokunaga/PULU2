#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"
//#define OF_ADDON_USING_OFXVECTORMATH
//#include "ofAddons.h"

class particle
{
    public:
        ofVec2f pos;
        ofVec2f vel;   // speed
        ofVec2f frc;   // frc is also know as acceleration (newton says "f=ma")
                        
        particle();
                virtual ~particle(){};

        void resetForce(); //reset Force
                void addForce(float x, float y);
                void addRepulsionForce(float x, float y, float radius, float scale);
                void addAttractionForce(float x, float y, float radius, float scale);
                
                void addRepulsionForce(particle &p, float radius, float scale);
                void addAttractionForce(particle &p, float radius, float scale);
                void addClockwiseForce(particle &p, float radius, float scale);
                void addCounterClockwiseForce(particle &p, float radius, float scale);
        
                //抵抗力（摩擦）を加える
                void addDampingForce();
				//initialize
                void setInitialCondition(float px, float py, float vx, float vy);
        void update();
        void draw();
        
                void bounceOffWalls(); //バウンドする動き
        
                bool  bFixed;
        
                float damping; //抵抗力

    protected:
    private:
};

#endif // PARTICLE_H