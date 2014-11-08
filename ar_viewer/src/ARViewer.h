/*
 * ARViewer.h
 *
 *  Created on: Nov 7, 2014
 *      Author: cloud
 */

#ifndef ARVIEWER_H_
#define ARVIEWER_H_

#include "QGLViewer/qglviewer.h"
#include "ar_viewer/keyframeMsg.h"
#include <opencv2/core/core.hpp>
#include "boost/thread.hpp"

class ARViewer: public QGLViewer {
public:
	ARViewer();
	virtual ~ARViewer();
	void setImage(const cv::Mat& image);
protected:
	virtual void draw();
	virtual void init();
private:
	void renderBackgroundGL();
	GLuint textureId;
	boost::mutex dataMutex; //protects current_img
	cv::Mat current_img;
};

#endif /* ARVIEWER_H_ */
