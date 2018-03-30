#include "puru2.h"

//---------------------------------------------------
void puru2::setup(ofImage src, vector<particle> p, float grid, float k){

	pA.clear();
	index_pB.clear();
	particles.clear();
	images.clear();
	springs.clear();
	type = 0;
	grid_size = grid;
	springiness = k;

	//copy particle
	for(int i = 0; i < p.size(); i++){
		particles.push_back(p[i]);
	}
	//check particles
	if(particles.size() == 0){
		return;
	}

	//copy image of particle
	unsigned char *src_data = src.getPixels();
	for(int i = 0; i < p.size(); i++){
		ofImage tmp_img;
		unsigned char *tmp_data;
		switch (src.type) {
            case OF_IMAGE_GRAYSCALE:
                tmp_img.allocate(grid_size, grid_size, OF_IMAGE_GRAYSCALE);
				tmp_data = tmp_img.getPixels();
				for(int k = 0; k < grid_size; k++){
					for(int j = 0; j < grid_size; j++){
						int index = (int)(p[i].pos.y + k)*src.getWidth() + (int)(p[i].pos.x + j);
						tmp_data[(int)(k*grid_size)+j] = src_data[index];
					}
				}
				tmp_img.update();
                break;
            case OF_IMAGE_COLOR:
				tmp_img.allocate(grid_size, grid_size, OF_IMAGE_COLOR);
				tmp_data = tmp_img.getPixels();
				for(int k = 0; k < grid_size; k++){
					for(int j = 0; j < grid_size; j++){
						int index = (int)(p[i].pos.y + k)*src.getWidth() + (int)(p[i].pos.x + j);
						tmp_data[((int)(k*grid_size)+j)*3] = src_data[index*3];
						tmp_data[((int)(k*grid_size)+j)*3 + 1] = src_data[index*3 + 1];	
						tmp_data[((int)(k*grid_size)+j)*3 + 2] = src_data[index*3 + 2];	
					}
				}
				tmp_img.update();
                break;
            case OF_IMAGE_COLOR_ALPHA:
				tmp_img.allocate(grid_size, grid_size, OF_IMAGE_COLOR_ALPHA);
				tmp_data = tmp_img.getPixels();
				for(int k = 0; k < grid_size; k++){
					for(int j = 0; j < grid_size; j++){
						int index = (int)(p[i].pos.y + k)*src.getWidth() + (int)(p[i].pos.x + j);
						tmp_data[((int)(k*grid_size)+j)*4] = src_data[index*4];
						tmp_data[((int)(k*grid_size)+j)*4 + 1] = src_data[index*4 + 1];	
						tmp_data[((int)(k*grid_size)+j)*4 + 2] = src_data[index*4 + 2];	
						tmp_data[((int)(k*grid_size)+j)*4 + 3] = src_data[index*4 + 3];	
					}
				}
				tmp_img.update();
				break;
		}
		images.push_back(tmp_img);
	}
	//push_back spring
	bool l_s , r_s, u_s, d_s;
	spring tmp_s;
		tmp_s.distance = grid_size;
		tmp_s.springiness = springiness;
		particle tmp_p;

	for(int i = 0; i < particles.size(); i++){
		//search springs
		l_s = r_s = u_s = d_s = false;
		for(int j = 0; j < springs.size(); j++){
			if((particles[i].pos.x - grid_size == springs[j].particleA->pos.x && particles[i].pos.x == springs[j].particleB->pos.x && particles[i].pos.y == springs[j].particleA->pos.y)
				|| (particles[i].pos.x - grid_size == springs[j].particleB->pos.x && particles[i].pos.x == springs[j].particleA->pos.x && particles[i].pos.y == springs[j].particleA->pos.y))
				l_s = true;
			if((particles[i].pos.x + grid_size == springs[j].particleA->pos.x && particles[i].pos.x == springs[j].particleB->pos.x && particles[i].pos.y == springs[j].particleA->pos.y)
				|| (particles[i].pos.x + grid_size == springs[j].particleB->pos.x && particles[i].pos.x == springs[j].particleA->pos.x && particles[i].pos.y == springs[j].particleA->pos.y))
				r_s = true;
			if((particles[i].pos.y - grid_size == springs[j].particleA->pos.y && particles[i].pos.y == springs[j].particleB->pos.y && particles[i].pos.x == springs[j].particleA->pos.x)
				|| (particles[i].pos.y - grid_size == springs[j].particleB->pos.y && particles[i].pos.y == springs[j].particleA->pos.y && particles[i].pos.x == springs[j].particleA->pos.x))
				u_s = true;
			if((particles[i].pos.y + grid_size == springs[j].particleA->pos.y && particles[i].pos.y == springs[j].particleB->pos.y && particles[i].pos.x == springs[j].particleA->pos.x)
				|| (particles[i].pos.y + grid_size == springs[j].particleB->pos.y && particles[i].pos.y == springs[j].particleA->pos.y && particles[i].pos.x == springs[j].particleA->pos.x))
				d_s = true;
		}
		//add springs
		//Ž©—R’[
		for(int k = 0; k < particles.size(); k++){
			if(!l_s){
				if(particles[k].pos.x == particles[i].pos.x - grid_size && particles[k].pos.y == particles[i].pos.y){
					tmp_s.particleA = &(particles[k]);
					tmp_s.particleB = &(particles[i]);
					springs.push_back(tmp_s);
					l_s = true;
				}
			}
			if(!r_s){
				if(particles[k].pos.x == particles[i].pos.x + grid_size && particles[k].pos.y == particles[i].pos.y){
					tmp_s.particleA = &(particles[i]);
					tmp_s.particleB = &(particles[k]);
					springs.push_back(tmp_s);
					r_s = true;
				}
			}
			if(!u_s){
				if(particles[k].pos.y == particles[i].pos.y - grid_size && particles[k].pos.x == particles[i].pos.x){
					tmp_s.particleA = &(particles[k]);
					tmp_s.particleB = &(particles[i]);
					springs.push_back(tmp_s);
					u_s = true;
				}
			}
			if(!d_s){
				if(particles[k].pos.y == particles[i].pos.y + grid_size && particles[k].pos.x == particles[i].pos.x){
					tmp_s.particleA = &(particles[i]);
					tmp_s.particleB = &(particles[k]);
					springs.push_back(tmp_s);
					d_s = true;
				}
			}
		}
		//ŒÅ’è’[		
		if(!l_s){
			tmp_p.setInitialCondition(particles[i].pos.x - grid_size, particles[i].pos.y, 0, 0);
			tmp_p.damping = 0.2F;
			pA.push_back(tmp_p);
			index_pB.push_back(i);
			l_s = true;
		}	
		if(!r_s){
			tmp_p.setInitialCondition(particles[i].pos.x + grid_size, particles[i].pos.y, 0, 0);
			tmp_p.damping = 0.2F;
			pA.push_back(tmp_p);
			index_pB.push_back(i);		
			//pA.push_back(particles[i]);
			//pB.push_back(tmp_p);
			r_s = true;
		}
		if(!u_s){
			tmp_p.setInitialCondition(particles[i].pos.x, particles[i].pos.y - grid_size, 0, 0);
			tmp_p.damping = 0.2F;
			pA.push_back(tmp_p);
			index_pB.push_back(i);
			u_s = true;
		}
		if(!d_s){
			tmp_p.setInitialCondition(particles[i].pos.x, particles[i].pos.y + grid_size, 0, 0);
			tmp_p.damping = 0.2F;
			pA.push_back(tmp_p);
			index_pB.push_back(i);
			//pA.push_back(particles[i]);
			//pB.push_back(tmp_p);
			d_s = true;
		}
		
	}
	
	if(!(pA.size() == index_pB.size()))
		return;

	for(int i = 0; i < pA.size(); i++){
		tmp_s.particleA = &(pA[i]);
		tmp_s.particleB = &(particles[index_pB[i]]);
		springs.push_back(tmp_s);
	}
	

}

//---------------------------------------------------
void puru2::update(){

}

//---------------------------------------------------
void puru2::clear(){

	pA.clear();
	index_pB.clear();
	particles.clear();
	images.clear();
	springs.clear();
	type = 0;
	springiness = 0.2f;
}
