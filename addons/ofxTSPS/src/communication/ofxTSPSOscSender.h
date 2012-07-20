/***********************************************************************

 ofxTSPSOSCSender.h
 TSPSPeopleVision

 Copyright (c) 2009, LAB at Rockwell Group
 http://lab.rockwellgroup.com
 Created by Brett Renfer on 1/14/10.

 OSC Message is structured like this:

 address: /TSPS/person/ + ordered id (i.e. TSPS/person/0)

 // LEGACY
 
 argument 0: pid;
 argument 1: oid;
 argument 2: age;
 argument 3: centroid.x;
 argument 4: centroid.y;
 argument 5: velocity.x;
 argument 6: velocity.y;
 argument 7: boundingRect.x;
 argument 9: boundingRect.y;
 argument 9: boundingRect.width;
 argument 10: boundingRect.height;
 argument 11: opticalFlowVectorAccumulation.x;
 argument 12: opticalFlowVectorAccumulation.y;
 argument 13+ : contours (if enabled)

 // NEW
 
 argument 0: pid;
 argument 1: oid;
 argument 2: age;
 argument 3: centroid.x;
 argument 4: centroid.y;
 argument 5: velocity.x;
 argument 6: velocity.y;
 argument 7: depth;
 argument 8: boundingRect.x;
 argument 9: boundingRect.y;
 argument 10: boundingRect.width;
 argument 11: boundingRect.height;
 argument 12: haarRect.x;           - will be 0 if hasHaar == false
 argument 13: haarRect.y;           - will be 0 if hasHaar == false
 argument 14: haarRect.width;       - will be 0 if hasHaar == false
 argument 15: haarRect.height;      - will be 0 if hasHaar == false
 argument 12: opticalFlowVectorAccumulation.x;
 argument 13: opticalFlowVectorAccumulation.y;
 argument 14+ : contours (if enabled)
 
 //
 
 
 ***********************************************************************/

#pragma once

#include "ofxOsc.h"
#include "ofxTSPSPerson.h"

class ofxTSPSOscSender : public ofxOscSender
{
	public :

	//"old" variables are to check for reroute
	string	ip, oldip;
	int		port, oldport;
	bool useLegacy;

	ofxTSPSOscSender();
	ofxTSPSOscSender(string _ip, int _port);
	void setupSender(string _ip, int _port);
    
	void update();
	void personEntered ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
	void personMoved ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
	void personUpdated ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
	void personWillLeave ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
  void goLeft();
  void goRight();
  void goStraight();
  ofxOscMessage getOSCMessage( ofxTSPSPerson * p, string type, bool bUseLegacy, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
	void send ( ofxOscMessage m );
	void reroute(string _ip, int _port);

};

