/*
 * ARWorker.h
 *
 *  Created on: Nov 7, 2014
 *      Author: cloud
 */

#ifndef ARWORKER_H_
#define ARWORKER_H_

#include "../../lsd_slam_core/src/IOWrapper/NotifyBuffer.h"
#include "../../lsd_slam_core/src/IOWrapper/ROS/ROSImageStreamThread.h"
#include "geometry_msgs/PoseStamped.h"
#include "ar_viewer/keyframeMsg.h"

class ARViewer;

class ARWorker: public lsd_slam::Notifiable {
public:
	ARWorker(ARViewer* viewer,lsd_slam::ROSImageStreamThread* imageStream);
	virtual ~ARWorker();
	void Loop();
	void addPoseMsg(geometry_msgs::PoseStampedConstPtr msg);
	void addFrameMsg(ar_viewer::keyframeMsgConstPtr msg); // has more information about frame eg camToWorld matrix
private:
	lsd_slam::ROSImageStreamThread* imageStream;
	ARViewer* viewer;
};

//keyframeMsg_()
//: id(0)
//, time(0.0)
//, isKeyframe(false)
//, camToWorld()
//, fx(0.0)
//, fy(0.0)
//, cx(0.0)
//, cy(0.0)
//, height(0)
//, width(0)
//, pointcloud()
//{
//  camToWorld.assign(0.0);
//}

#endif /* ARWORKER_H_ */
