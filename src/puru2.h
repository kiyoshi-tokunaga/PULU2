#ifndef PURU2_H
#define PURU2_H

#include "ofMain.h"
#include "particle.h"
#include "spring.h"

typedef struct grid{
	particle p;
	ofImage patch;
}GRID;

class puru2 {
	private:
		vector<particle> pA;
		vector<int> index_pB;
        
	public:
        vector<particle> particles;
		vector<ofImage> images;
		vector<spring> springs;  

		int type;
		float grid_size;		
        float springiness;       // this is the k, springiness constant
        
		void setup(ofImage src, vector<particle> p, float grid, float k = 0.1f);
        void update();
		void clear();
        
};


#endif