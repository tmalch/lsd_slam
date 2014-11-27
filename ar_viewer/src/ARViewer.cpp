/*
 * ARViewer.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cloud
 */

#include "ARViewer.h"
#define DEBUG(s) std::cout << __FUNCTION__ << " " << s << std::endl;

ARViewer::ARViewer() {
	current_img = cv::Mat(640,480,CV_8UC3, cv::Scalar(0,0,50));
}

ARViewer::~ARViewer() {
	// TODO Auto-generated destructor stub
}
void ARViewer::setImage(const cv::Mat& image){
	boost::lock_guard<boost::mutex> guard(dataMutex);
	cv::cvtColor(image,current_img,CV_GRAY2RGB);
}
void ARViewer::draw(){

	renderBackgroundGL();

	const float nbSteps = 200.0;

	glBegin(GL_QUAD_STRIP);
	for (float i=0; i<nbSteps; ++i)
	{
	  float ratio = i/nbSteps;
	  float angle = 21.0*ratio;
	  float c = cos(angle);
	  float s = sin(angle);
	  float r1 = 1.0 - 0.8*ratio;
	  float r2 = 0.8 - 0.8*ratio;
	  float alt = ratio - 0.5;
	  const float nor = .5;
	  const float up = sqrt(1.0-nor*nor);
	  glColor3f(1.0-ratio, 0.2f , ratio);
	  glNormal3f(nor*c, up, nor*s);
	  glVertex3f(r1*c, alt, r1*s);
	  glVertex3f(r2*c, alt+0.05, r2*s);
	}
	glEnd();

}
void ARViewer::init(){

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	setAnimationPeriod(1000/30);
	startAnimation();
}
void ARViewer::renderBackgroundGL()
{

	glBindTexture(GL_TEXTURE_2D, textureId);
	{
		boost::lock_guard<boost::mutex> guard(dataMutex);
		if(current_img.data != nullptr){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, current_img.cols, current_img.rows, 0,   GL_BGR , GL_UNSIGNED_BYTE, current_img.data);
		}
	}
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	startScreenCoordinatesSystem(true);

	// Draws the background quad
	glColor3f(1,1,1); //important
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);	glVertex2i(0,0);
	glTexCoord2f(0.0,   0.0);	glVertex2i(0,height());
	glTexCoord2f(1.0, 0.0);		glVertex2i(width(),height());
	glTexCoord2f(1.0, 1.0);	glVertex2i(width(),0);
	glEnd();

	stopScreenCoordinatesSystem();

	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

}

