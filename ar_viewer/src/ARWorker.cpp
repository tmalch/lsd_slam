/*
 * ARWorker.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cloud
 */

#include "ARWorker.h"

#include "../../lsd_slam_core/src/IOWrapper/Timestamp.h"
#include "../../lsd_slam_core/src/IOWrapper/TimestampedObject.h"
#include "ARViewer.h"

#define DEBUG(s) std::cout << __FUNCTION__ << " " << s << std::endl;

ARWorker::ARWorker(ARViewer* viewer,lsd_slam::NotifyBuffer<TimestampedMat>* imageStream) {
	this->imageStream = imageStream;
	this->viewer = viewer;
	imageStream->setReceiver(this);
}

ARWorker::~ARWorker() {
}

void ARWorker::addPoseMsg(geometry_msgs::PoseStampedConstPtr msg){
	msg->pose.orientation;
	msg->pose.position;
	DEBUG("received Pose: "<< msg->pose.position.x << ":"<< msg->pose.position.y << ":"<< msg->pose.position.z)
}
void ARWorker::addFrameMsg(ar_viewer::keyframeMsgConstPtr msg){

}
void ARWorker::Loop(){

	while (true) {
		// wait for new image
		boost::unique_lock<boost::recursive_mutex> waitLock(imageStream->getMutex());
		while (!(imageStream->size() > 0)) {
			notifyCondition.wait(waitLock);
		}
		waitLock.unlock();
		// retrieve new image
		TimestampedMat image = imageStream->first();
		imageStream->popFront();

		cv::Mat img = image.data;
		lsd_slam::Timestamp imgTime = image.timestamp;


		//doSomeImportantWork(img,imgTime)
		DEBUG("received Img send to viewer")
		this->viewer->setImage(img); //sendTheResultToARViewer() //which runs in main Thread
	}
}


