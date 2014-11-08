/*
 * ARViewer.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cloud
 */

#include "ARViewer.h"

ARViewer::ARViewer() {
	// TODO Auto-generated constructor stub

}

ARViewer::~ARViewer() {
	// TODO Auto-generated destructor stub
}
void ARViewer::setImage(const cv::Mat& image){
	boost::lock_guard<boost::mutex> guard(dataMutex);
	current_img = image;
}
void ARViewer::draw(){
	renderBackgroundGL();
}
void ARViewer::init(){
	static bool textureGenerated = false;
	if (!textureGenerated)
	{
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		textureGenerated = true;
	}
}
void ARViewer::renderBackgroundGL()//https://github.com/DineshKannan/OpenCV-OpenGL-AugmentedReality/blob/master/Hand-OpenGL.cpp
{
	init();
	GLint polygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, polygonMode);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Copy the image to the texture.
	glBindTexture(GL_TEXTURE_2D, textureId);
	{
		boost::lock_guard<boost::mutex> guard(dataMutex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, current_img.size().width, current_img.size().height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, current_img.data);
	}
	// Draw the image.
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	// Clear the depth buffer so the texture forms the background.
	glClear(GL_DEPTH_BUFFER_BIT);
	// Restore the polygon mode state.
	glPolygonMode(GL_FRONT, polygonMode[0]);
	glPolygonMode(GL_BACK, polygonMode[1]);
}

