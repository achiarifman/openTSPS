/*
 *  ofxTSPSGuiManager.cpp
 *  TSPSPeopleVision
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 
 Portions of this software are distributed under the GPL and the MIT license.
 
 The MIT License
 
 Copyright (c) 2010 Rockwell Group LLC
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 */

#include "ofxTSPSGuiManager.h"
#include "ofxTSPSSettings.h"

enum{
	PARAM_INT, PARAM_FLOAT, PARAM_BOOL
};

ofxTSPSGuiManager::ofxTSPSGuiManager() {
	//JG TODO add drawing event
	//ofAddListener(ofEvents().draw, this, &ofxTSPSGuiManager::draw);
	
    bEventsEnabled = false;
}


ofxTSPSGuiManager::~ofxTSPSGuiManager(){
    disableEvents();
}

void ofxTSPSGuiManager::enableEvents(){
    if (bEventsEnabled) return;
    bEventsEnabled = true;
	ofAddListener(ofEvents().update, this, &ofxTSPSGuiManager::update);
	ofAddListener(ofEvents().mousePressed, this, &ofxTSPSGuiManager::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxTSPSGuiManager::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxTSPSGuiManager::mouseReleased);
	ofAddListener(ofEvents().keyPressed, this, &ofxTSPSGuiManager::keyPressed);
    quadGui.enableEvents();
}

void ofxTSPSGuiManager::disableEvents(){
    if (!bEventsEnabled) return;
    bEventsEnabled = false;
	ofRemoveListener(ofEvents().update, this, &ofxTSPSGuiManager::update);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxTSPSGuiManager::mousePressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxTSPSGuiManager::mouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxTSPSGuiManager::mouseReleased);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxTSPSGuiManager::keyPressed);
    quadGui.disableEvents();
}

void ofxTSPSGuiManager::setup(){
    enableGui = true;
	
	//create + setup layout of panel
	panel.setup("settings", 10, 10, 330, 680);
	panel.loadFont("fonts/times.ttf", 10);
	
	//panel layout	
	panel.setPosition(10, 40);
	panel.setShowText(false);
	panel.setDimensions(330, 650); //yikes... autospacing is not working so well
	panel.setCollapsible(false);
	panel.setDraggable(false);
	panel.setBackgroundColor(255,255,255);
	panel.setOutlineColor(255,255,255,0);
	panel.setTextColor(148,129,85);
	panel.setSaveColor(58, 187, 147);
	panel.setLoadColor(180, 87, 128);
	panel.setSaveAsColor(238, 53, 35);
	panel.setSaveSelectedColor(80, 253, 190);
	panel.setRestoreColor(34, 151, 210);
	panel.setRestoreSelectedColor(116, 191, 228);
	panel.setDrawBarOnTop(false);
    
    minimizeButton = new guiTypeButton();
	minimizeButton->setup("minimize", buttonWidth*1.5, buttonHeight);
    minimizeButton->setPosition(10,10);
    minimizeButton->setFont(&panel.guiTTFFont);
    minimizeButton->setBackgroundColor(205,198,63);
    minimizeButton->setBackgroundSelectColor(205,198,63);
    
    maximizeButton = new guiTypeButton();
	maximizeButton->setup("maximize", buttonWidth*1.5, buttonHeight);
    maximizeButton->setPosition(10,10);
    maximizeButton->disable();
    maximizeButton->setFont(&panel.guiTTFFont);
    maximizeButton->setBackgroundSelectColor(0,168,156);
    maximizeButton->setBackgroundColor(0,168,156);
    
	ofAddListener(minimizeButton->buttonPressed, this, &ofxTSPSGuiManager::minimize);
	ofAddListener(maximizeButton->buttonPressed, this, &ofxTSPSGuiManager::maximize);
    
    //panel.addButton("maximize");
    //disableElement("maximize");
    
	guiTypePanel * videoPanel = panel.addPanel("video", 1, false);
	videoPanel->setDrawLock( false );	
	videoPanel->setBackgroundColor(174,139,138);
	videoPanel->setBackgroundSelectColor(174,139,138);
	
	guiTypePanel * backgroundPanel = panel.addPanel("background", 1, false);
	backgroundPanel->setDrawLock( false );	
	backgroundPanel->setBackgroundColor(213,105,68);
	backgroundPanel->setBackgroundSelectColor(213,105,68);
	
	guiTypePanel * differencingPanel = panel.addPanel("differencing", 1, false);
	differencingPanel->setDrawLock( false );	
	differencingPanel->setBackgroundColor(113,171,154);
	differencingPanel->setBackgroundSelectColor(113,171,154);
	
	guiTypePanel * sensingPanel = panel.addPanel("sensing", 1, false);
	sensingPanel->setDrawLock( false );
	sensingPanel->setBackgroundColor(191,120,0);
	sensingPanel->setBackgroundSelectColor(191,120,0);
	
	guiTypePanel * fishesPanel = panel.addPanel("fishes", 1, false);
	fishesPanel->setDrawLock( false );
	fishesPanel->setBackgroundColor(34,151,210);
	fishesPanel->setBackgroundSelectColor(34,151,210);
	
	guiTypePanel * communicationPanel = panel.addPanel("communication", 1, false);
	communicationPanel->setDrawLock( false );
	communicationPanel->setBackgroundColor(180,87,128);
	communicationPanel->setBackgroundSelectColor(180,87,128);
    
    guiTypePanel * websocketPanel = panel.addPanel("ws://", 1, false);
	websocketPanel->setDrawLock( false );
	websocketPanel->setBackgroundColor(180,87,128);
	websocketPanel->setBackgroundSelectColor(180,87,128);
	
#ifdef USE_CUSTOM_GUI
	guiTypePanel * customPanel = panel.addPanel("custom", 1, false);
	customPanel->setDrawLock( false );
	customPanel->setBackgroundColor(218,173,90);
	customPanel->setBackgroundSelectColor(191,120,0);
#endif		
	
	// video settings		
	panel.setWhichPanel("video");	
	//James G: Added video settings
	guiTypeGroup * videoSettingsGroup = panel.addGroup("camera settings");
	videoSettingsGroup->setBackgroundColor(148,129,85);
	videoSettingsGroup->setBackgroundSelectColor(148,129,85);
	videoSettingsGroup->seBaseColor(244,136,136);
	videoSettingsGroup->setShowText(false);
	panel.addButton("open video settings");
    panel.addToggle("use kinect", "USE_KINECT", true);
    
    // video files    
	guiTypeGroup * videoFilesGroup = panel.addGroup("videoFiles");
	videoFilesGroup->setBackgroundColor(148,129,85);
	videoFilesGroup->setBackgroundSelectColor(148,129,85);
	videoFilesGroup->seBaseColor(58,187,147);
	videoFilesGroup->setShowText(false);
    
	//TODO: use the button class for this maybe?
	panel.addToggle("use video file instead of live camera", "USE_VIDEO_FILE", false);
	panel.addTextField("video directory (inside data folder)", "VIDEO_FILE_DIR", "videos", 200, 20);
	videoFiles = new simpleFileLister();	
	int numVideoFiles = videoFiles->listDir(ofToDataPath("videos", true));
	ofLog(OF_LOG_VERBOSE, "num video files found: " + numVideoFiles);
	panel.addFileLister("video files:", videoFiles, 240, 100);
	panel.addToggle("reload directory", "VIDEO_FILE_RELOAD", true);

    // amplification
	guiTypeGroup * amplificationGroup = panel.addGroup("amplification");
	amplificationGroup->setBackgroundColor(148,129,85);
	amplificationGroup->setBackgroundSelectColor(148,129,85);
	amplificationGroup->seBaseColor(244,136,136);
	amplificationGroup->setShowText(false);
	panel.addToggle("use amplification (video gain)", "USE_AMPLIFICATION", false);
	panel.addSlider("amplification amount:", "AMPLIFICATION_AMOUNT", 1, 1, 100, true);
	
	// ZACK BOKA: choose whether or not to display the Adjusted View in color
    // BR: disabling this for now
	/*guiTypeGroup* adjustedViewColorGroup = panel.addGroup("adjustedViewColor");
	adjustedViewColorGroup->setBackgroundColor(148,129,85);
	adjustedViewColorGroup->setBackgroundSelectColor(148,129,85);
	adjustedViewColorGroup->seBaseColor(244,136,136);
	adjustedViewColorGroup->setShowText(false);
	panel.addToggle("show Adjusted View in color", "ADJUSTED_VIEW_COLOR", false);*/
	
	//background settings
	
	panel.setWhichPanel("background");
	guiTypeGroup * backgroundGroup = panel.addGroup("background");
	backgroundGroup->setBackgroundColor(148,129,85);
	backgroundGroup->setBackgroundSelectColor(148,129,85);
	backgroundGroup->seBaseColor(58,187,147);
	backgroundGroup->setShowText(false);
	backgroundGroup->setActive(true);
	//TODO: use the button class for this maybe?
	panel.addToggle("capture background", "LEARN_BACKGROUND", false);
	panel.addToggle("blank out background", "BLACK_BACKGROUND", false);
	
	guiTypeGroup * relearnGroup = panel.addGroup("background relearn");
	relearnGroup->setBackgroundColor(148,129,85);
	relearnGroup->setBackgroundSelectColor(148,129,85);
	relearnGroup->seBaseColor(180,87,128);
	relearnGroup->setShowText(false);
	panel.addToggle("progressive background recapture", "RELEARN", false);
	panel.addSlider("recapture rate :", "RELEARN_BACKGROUND", .1f, 0.0f, 1000.0f, false);
	
	//differencing settings
	
	panel.setWhichPanel("differencing");
	guiTypeGroup * thresholdGroup = panel.addGroup("threshold group");	
	thresholdGroup->setBackgroundColor(148,129,85);
	thresholdGroup->setBackgroundSelectColor(148,129,85);
	thresholdGroup->seBaseColor(255,136,37);
	thresholdGroup->setShowText(false);
	thresholdGroup->setActive(true);
	panel.addSlider("threshold:", "THRESHOLD", 40, 0, 255, false);
	vector<string> multi;
	multi.push_back("light on dark");
	multi.push_back("dark on light");
	multi.push_back("absolute difference");
	multi.push_back("no differencing");
	panel.addMultiToggle("type of differencing:", "BLOB_TYPE", 0, multi);
	
	guiTypeGroup * highpassGroup = panel.addGroup("highpass");
	highpassGroup->setBackgroundColor(148,129,85);
	highpassGroup->setBackgroundSelectColor(148,129,85);
	highpassGroup->seBaseColor(58,187,147);
	highpassGroup->setShowText(false);
	panel.addToggle("use highpass", "USE_HIGHPASS", true);
	panel.addSlider("highpass blur:", "HIGHPASS_BLUR", 1, 1, 200, true);
	panel.addSlider("highpass noise:", "HIGHPASS_NOISE", 1, 1, 30, true);
	
	guiTypeGroup * smoothingGroup = panel.addGroup("smoothing");		
	smoothingGroup->setBackgroundColor(148,129,85);
	smoothingGroup->setBackgroundSelectColor(148,129,85);
	smoothingGroup->seBaseColor(34,151,210);
	smoothingGroup->setShowText(false);
	panel.addToggle("use shape smoothing", "USE_SMOOTHING", false);
	panel.addSlider("smooth amount:", "SMOOTH_AMOUNT", 0, 0, 15, false);		
	
  guiTypeGroup * edgeGroup = panel.addGroup("edge");
	edgeGroup->setBackgroundColor(148,129,85);
	edgeGroup->setBackgroundSelectColor(148,129,85);
	edgeGroup->seBaseColor(238,218,0);
	edgeGroup->setShowText(false);
	//edgeGroup->setActive(true);
	
	//JG 12/8/09 GUI-REDUX Removing this feature
	panel.addToggle("edge detection", "EDGE_DETECTION", true);
	panel.addSlider("threshold1 for hysteresis:", "EDGE_THRES1", 70, 0, 255, true);
	panel.addSlider("threshold2 for hysteresis:", "EDGE_THRES2", 200, 0, 255, true);
	panel.addSlider("dilatation:", "DILATATION", 2, 0, 10, true);
	
	//sensing
	
	panel.setWhichPanel("sensing");
	//TODO optionally disable people?
	
	guiTypeGroup * blobGroup = panel.addGroup("blobs");
	blobGroup->setBackgroundColor(148,129,85);
	blobGroup->setBackgroundSelectColor(148,129,85);
	blobGroup->seBaseColor(238,218,0);
	blobGroup->setShowText(false);
	//blobGroup->setActive(true);
	
	//JG 12/8/09 GUI-REDUX Removing this feature
	panel.addToggle("track blobs", "TRACK_BLOBS", true);
	panel.addSlider("minimum blob size (% of view):", "MIN_BLOB", 1.f, 0.01f, 2.0f, false);
	panel.addSlider("maximum blob size (% of view):", "MAX_BLOB", .50f, 0.1f, 100.f, false);
	panel.addToggle("ignore nested blobs", "FIND_HOLES", false);
	
	guiTypeGroup * optionsGroup = panel.addGroup("options");
	optionsGroup->setBackgroundColor(148,129,85);
	optionsGroup->setBackgroundSelectColor(148,129,85);
	optionsGroup->seBaseColor(58,187,147);
	optionsGroup->setShowText(false);
	panel.addToggle("track and send contours", "SEND_OSC_CONTOURS", false);
	
	guiTypeGroup * opticalGroup = panel.addGroup("optical flow");
	opticalGroup->setBackgroundColor(148,129,85);
	opticalGroup->setBackgroundSelectColor(148,129,85);
	opticalGroup->seBaseColor(34,151,210);
	opticalGroup->setShowText(false);
	//optical flow
	panel.addToggle("track and send optical flow in blobs", "SENSE_OPTICAL_FLOW", false);
	panel.addSlider("filter vectors smaller than:", "MIN_OPTICAL_FLOW", 0, 0, 5.0, false);
	panel.addSlider("clamp vectors: larger than", "MAX_OPTICAL_FLOW", 10, 5.0, 200, false);
	
	guiTypeGroup * haarGroup = panel.addGroup("haar tracking");
	haarGroup->setBackgroundColor(148,129,85);
	haarGroup->setBackgroundSelectColor(148,129,85);
	haarGroup->seBaseColor(238,53,35);
	haarGroup->setShowText(false);
	
	// haar
	panel.addToggle("detect and send features in blobs", "SENSE_HAAR", false);
	//	ofEnableDataPath();
	//	ofSetDataPathRoot("data/");
	haarFiles = new simpleFileLister();	
	int numHaar = haarFiles->listDir(ofToDataPath("haar", true));
	ofLog(OF_LOG_VERBOSE, "haar files found " + numHaar);
	panel.addFileLister("types of features:", haarFiles, 240, 100);
	panel.addSlider("expand detection area:", "HAAR_PADDING", 0.0f, 0.0f, 200.0f, false);
	
	//JG GUI-REDUX: removing this feature
	//gui.addToggle("send haar center as blob center", &settings.bUseHaarAsCenter);
	//JG 1/21/10 disabled this feature to simplify the interface
	//	panel.addSlider("min. checkable haar size (%)", "MIN_HAAR", .1f, 0.0001f, 1.0f, false);
	//	panel.addSlider("max. checkable haar size (%)", "MAX_HAAR", .5f, 0.0001f, 1.0f, false);
	

	//sensing
	
	panel.setWhichPanel("fishes");
	
	guiTypeGroup * generalGroup = panel.addGroup("generaloptions");
	generalGroup->setBackgroundColor(148,129,85);
	generalGroup->setBackgroundSelectColor(148,129,85);
	generalGroup->seBaseColor(34,151,210);
	generalGroup->setShowText(false);
	
	panel.addToggle("track arms", "TRACK_ARMS", true);
	panel.addSlider("minimum arm size (% of view):", "MIN_ARM", 1.f, 0.5f, 3.0f, false);
	//panel.addSlider("maximum arm size (% of view):", "MAX_ARM", .50f, 0.5f, 100.f, false);
	//panel.addToggle("ignore nested blobs", "FIND_HOLES", false);
/*	
	guiTypeGroup * optionsGroup = panel.addGroup("options");
	optionsGroup->setBackgroundColor(148,129,85);
	optionsGroup->setBackgroundSelectColor(148,129,85);
	optionsGroup->seBaseColor(58,187,147);
	optionsGroup->setShowText(false);
	panel.addToggle("track and send contours", "SEND_OSC_CONTOURS", false);
*/	
	guiTypeGroup * leftrightGroup = panel.addGroup("leftright");
	leftrightGroup->setBackgroundColor(148,129,85);
	leftrightGroup->setBackgroundSelectColor(148,129,85);
	leftrightGroup->seBaseColor(238,218,0);
	leftrightGroup->setShowText(false);
	//optical flow
	panel.addToggle("track and send right or left direction", "SENSE_LEFTRIGHT", true);
	panel.addSlider("left angle threshold:", "LEFT_ANGLE_THRES", -1*PI/8, -PI/2, 0.0f, false);
	panel.addSlider("right angle threshold:", "RIGHT_ANGLE_THRES", 1*PI/8, 0.0f, PI/2,  false);
	panel.addSlider("horizontal left threshold:", "LEFT_HORIZ_THRES", -3*PI/8, -PI/2, 0.0f, false);
	panel.addSlider("horizontal right threshold:", "RIGHT_HORIZ_THRES", 3*PI/8, 0.0f, PI/2, false);
/*	
	guiTypeGroup * haarGroup = panel.addGroup("haar tracking");
	haarGroup->setBackgroundColor(148,129,85);
	haarGroup->setBackgroundSelectColor(148,129,85);
	haarGroup->seBaseColor(238,53,35);
	haarGroup->setShowText(false);
	
	// haar
	panel.addToggle("detect and send features in blobs", "SENSE_HAAR", false);
	//	ofEnableDataPath();
	//	ofSetDataPathRoot("data/");
	haarFiles = new simpleFileLister();	
	int numHaar = haarFiles->listDir(ofToDataPath("haar", true));
	ofLog(OF_LOG_VERBOSE, "haar files found " + numHaar);
	panel.addFileLister("types of features:", haarFiles, 240, 100);
	panel.addSlider("expand detection area:", "HAAR_PADDING", 0.0f, 0.0f, 200.0f, false);
	
	//JG GUI-REDUX: removing this feature
	//gui.addToggle("send haar center as blob center", &settings.bUseHaarAsCenter);
	//JG 1/21/10 disabled this feature to simplify the interface
	//	panel.addSlider("min. checkable haar size (%)", "MIN_HAAR", .1f, 0.0001f, 1.0f, false);
	//	panel.addSlider("max. checkable haar size (%)", "MAX_HAAR", .5f, 0.0001f, 1.0f, false);
*/
	//communication
	
	panel.setWhichPanel("communication");
	panel.setWhichColumn(0);
	
	guiTypeGroup * oscGroup = panel.addGroup("OSC");
	oscGroup->setBackgroundColor(148,129,85);
	oscGroup->setBackgroundSelectColor(148,129,85);
	oscGroup->seBaseColor(238,53,35);
	oscGroup->setShowText(false);
	panel.addToggle("send OSC", "SEND_OSC", false);
	panel.addTextField("receiver IP address (OSC host) :", "OSC_HOST", "127.0.0.1", 200, 20);
	panel.addTextField("receiver port (OSC port) :", "OSC_PORT", "12000", 200, 20);
	panel.addToggle("use legacy OSC", "LEGACY_OSC", false);
	
	guiTypeGroup * tuioGroup = panel.addGroup("TUIO");
	tuioGroup->setBackgroundColor(148,129,85);
	tuioGroup->setBackgroundSelectColor(148,129,85);
	tuioGroup->seBaseColor(207,202,65);
	tuioGroup->setShowText(false);
	panel.addToggle("send TUIO", "SEND_TUIO", false);
	panel.addTextField("receiver IP address (TUIO host):", "TUIO_HOST", "127.0.0.1", 200, 20);
	panel.addTextField("receiver port (TUIO port):", "TUIO_PORT", "3333", 200, 20);
	
	guiTypeGroup * tcpGroup = panel.addGroup("TCP");
	tcpGroup->setBackgroundColor(148,129,85);
	tcpGroup->setBackgroundSelectColor(148,129,85);
	tcpGroup->seBaseColor(238,53,35);
	tcpGroup->setShowText(false);
	panel.addToggle("send TCP", "SEND_TCP", false);
	panel.addTextField("broadcast port (TCP port):", "TCP_PORT", "8888", 200, 20);
    
    panel.setWhichPanel("ws://");
	panel.setWhichColumn(0);
    
	guiTypeGroup * wssGroup = panel.addGroup("WebSocket Server");
	wssGroup->setBackgroundColor(148,129,85);
	wssGroup->setBackgroundSelectColor(148,129,85);
	wssGroup->seBaseColor(238,53,35);
	panel.addToggle("send over WebSocket server", "SEND_WSS", false);
	panel.addTextField("webSocket port:", "WSS_PORT", "7681", 200, 20);
    panel.addButton("open debug URL");
    
    guiTypeGroup * wsGroup = panel.addGroup("WebSocket Client");
	wsGroup->setBackgroundColor(148,129,85);
	wsGroup->setBackgroundSelectColor(148,129,85);
	wsGroup->seBaseColor(238,53,35);
	panel.addToggle("send over WebSocket client", "SEND_WS", false);
	panel.addTextField("webSocket host:", "WS_HOST", "localhost", 200, 20);
	panel.addTextField("webSocket port:", "WS_PORT", "7681", 200, 20);
	panel.addToggle("use SSL", "WS_USESSL", false);
	panel.addTextField("webSocket channel (optional):", "WS_CHANNEL", "", 200, 20);
	
    panel.setValueB("SEND_WSS", false);
    panel.setValueB("SEND_WS", false);
    
	//JG TODO: Optionally change config file through the UI
	//this would be a big help for setting up multiple install sites and having those setting
	//included in repositories
	//ofSetDataPathRoot("data");
	loadSettings("settings/settings.xml");
	
	//listen for save / load / reload / saveas events from GUI + pass to quadgui
	
	ofAddListener(panel.getSaveButton()->buttonPressed, this, &ofxTSPSGuiManager::saveEventCatcher);
	ofAddListener(panel.getRestoreButton()->buttonPressed, this, &ofxTSPSGuiManager::reloadEventCatcher);
	ofAddListener(panel.getLoadButton()->buttonPressed, this, &ofxTSPSGuiManager::loadEventCatcher);
	ofAddListener(panel.getSaveAsButton()->buttonPressed, this, &ofxTSPSGuiManager::saveAsEventCatcher);
	
	//set active panel to be differencing
	panel.setSelectedPanel("sensing");
	ofEventArgs nullArgs;
	update(nullArgs);
    enableEvents();
}

void ofxTSPSGuiManager::addSlider(string name, int* value, int min, int max)
{
	ofxTSPSGUICustomParam p;
	string key = "CUSTOM" + ofToString(params.size());
	
	panel.setWhichPanel("custom");
	panel.setWhichColumn(0);	
	panel.addSlider(name, key, *value, min, max, true);
	
	p.key = key;
	p.type = PARAM_INT;
	p.i = value;
	params.push_back(p);
}

void ofxTSPSGuiManager::addSlider(string name, float* value, float min, float max)
{
	ofxTSPSGUICustomParam p;
	string key = "CUSTOM" + ofToString(params.size());	
	panel.setWhichPanel("custom");
	panel.setWhichColumn(0);
	panel.addSlider(name, key, *value, min, max, false);
	
	p.key = key;
	p.type = PARAM_FLOAT;
	p.f = value;
	params.push_back(p);
}

void ofxTSPSGuiManager::addToggle(string name, bool* value)
{
	ofxTSPSGUICustomParam p;	
	string key = "CUSTOM" + ofToString(params.size());
	
	panel.setWhichPanel("custom");
	panel.setWhichColumn(0);
	panel.addToggle(name, key, *value);
	
	p.key = key;
	p.type = PARAM_BOOL;
	p.b = value;
	params.push_back(p);
}

/***************************************************************
	GET + SET PANEL INFO / SELECTED PANEL
***************************************************************/

int ofxTSPSGuiManager::getSelectedPanel(){
	return panel.getSelectedPanel();
}

void ofxTSPSGuiManager::setSelectedPanel( int index ){
	panel.setWhichPanel(index);
}

void ofxTSPSGuiManager::setSelectedPanel( string name ){
	panel.setWhichPanel(name);
}

void ofxTSPSGuiManager::update(ofEventArgs &e){
    update();
}

void ofxTSPSGuiManager::update(){
	if(!enableGui){
		//if the gui is not shown no need to propagate values
		return;
	}
	
	panel.update();
	
    // camera
    
	//settings.cameraIndex = panel.getValueF("CAMERA_INDEX");
	settings.bUseKinect  = panel.getValueB("USE_KINECT");    
        
    // video files
    bool bIsNewDirectory = false;
    settings.bUseVideoFile = panel.getValueB("USE_VIDEO_FILE");
    if ( settings.videoDirectory != panel.getValueS("VIDEO_FILE_DIR") ){
        settings.videoDirectory = panel.getValueS("VIDEO_FILE_DIR");    
        bIsNewDirectory = true;
    }
    if ( panel.getValueB("VIDEO_FILE_RELOAD") || bIsNewDirectory){
        int numVideos = 0;
        if ( bIsNewDirectory ) numVideos = videoFiles->listDir(ofToDataPath(settings.videoDirectory, true));
        else numVideos = videoFiles->refreshDir();
        panel.setValueB("VIDEO_FILE_RELOAD", false);
        
        if ( numVideos == 0 ){
            panel.setValueB("USE_VIDEO_FILE", false);     
            ofLog( OF_LOG_WARNING, "No videos found, switching to camera input" );
        }
    }
    if(videoFiles->getSelectedName() != ""){
		settings.videoFile = videoFiles->getSelectedPath();
	}
    
	panel.setGroupActive("video", "videoFiles", settings.bUseVideoFile);    
    
    // threshold
	
	settings.threshold = panel.getValueF("THRESHOLD");
	settings.bSmooth = panel.getValueB("USE_SMOOTHING");
	settings.smooth = panel.getValueF("SMOOTH_AMOUNT");
	panel.setGroupActive("differencing", "smoothing", settings.bSmooth);
	
	settings.bHighpass = panel.getValueI("USE_HIGHPASS");
	settings.highpassBlur =  panel.getValueI("HIGHPASS_BLUR");
	settings.highpassNoise = panel.getValueI("HIGHPASS_NOISE");
	panel.setGroupActive("differencing", "highpass", settings.bHighpass);
	
  settings.bEdgeDetection = panel.getValueI("EDGE_DETECTION");
	settings.edgeThres1 = panel.getValueI("EDGE_THRES1");
	settings.edgeThres2 = panel.getValueI("EDGE_THRES2");
	settings.dilatation = panel.getValueI("DILATATION");
	panel.setGroupActive("differencing", "edge", settings.bEdgeDetection);
	
	settings.bAmplify = panel.getValueB("USE_AMPLIFICATION");
	settings.highpassAmp = panel.getValueI("AMPLIFICATION_AMOUNT");
	panel.setGroupActive("video", "amplification", settings.bAmplify);

	if(panel.getButtonPressed("open video settings") && settings.getVideoGrabber() != NULL){
		ofVideoGrabber * grab = dynamic_cast<ofVideoGrabber *>(settings.getVideoGrabber());
        grab->videoSettings();
	}
    
    if (panel.getButtonPressed("open debug URL")){
        ofLaunchBrowser( "http://localhost:"+panel.getValueS("WSS_PORT"));
    }
	
	//settings.bAdjustedViewInColor = panel.getValueB("ADJUSTED_VIEW_COLOR");
	//panel.setGroupActive("video", "adjustedViewColor", settings.bAdjustedViewInColor);
	
	settings.bLearnBackground = panel.getValueB("LEARN_BACKGROUND");
	if(settings.bLearnBackground){ 
		panel.setValueB("LEARN_BACKGROUND", false);
	}
    settings.bBlankBackground = panel.getValueB("BLACK_BACKGROUND");
	if(settings.bLearnBackground){ 
		panel.setValueB("BLACK_BACKGROUND", false);
	}
	
	//panel.setValueB("LEARN_BACKGROUND", settings.bLearnBackground);
	//JG 12/8/09 GUI-REDUX Removing this feature
	//gui.addToggle("smart learn background", &settings.bSmartLearnBackground);
	
	settings.bTrackBlobs = panel.getValueB("TRACK_BLOBS");
	panel.setGroupActive("sensing", "blobs", settings.bTrackBlobs);
	settings.minBlob = panel.getValueF("MIN_BLOB")/100.0f; //scale var to be right for tracker
	settings.maxBlob = panel.getValueF("MAX_BLOB")/100.0f;	//scale var to be right for tracker
	settings.bLearnBackgroundProgressive = panel.getValueB("RELEARN");
	settings.fLearnRate = panel.getValueF("RELEARN_BACKGROUND");
	panel.setGroupActive("background", "background relearn", settings.bLearnBackgroundProgressive);

	settings.bFindHoles = !(panel.getValueB("FIND_HOLES"));
	settings.bTrackOpticalFlow = panel.getValueB("SENSE_OPTICAL_FLOW");
	panel.setGroupActive("sensing", "optical flow", settings.bTrackOpticalFlow);
	
	//JG 12/8/09 GUI-REDUX:
	settings.minOpticalFlow = panel.getValueF("MIN_OPTICAL_FLOW");
	settings.maxOpticalFlow = panel.getValueF("MAX_OPTICAL_FLOW");
	settings.trackType = panel.getValueI("BLOB_TYPE");
	settings.bDetectHaar = panel.getValueB("SENSE_HAAR");
	panel.setGroupActive("sensing", "haar tracking", settings.bDetectHaar);

	if(haarFiles->getSelectedName() != ""){
		settings.haarFile = haarFiles->getSelectedName();
	}
	settings.haarAreaPadding = panel.getValueF("HAAR_PADDING");
	//JG GUI-REDUX: removing this feature
	//gui.addToggle("send haar center as blob center", &settings.bUseHaarAsCenter);
	//JG 1/21/10 disabled this feature to simplify the interface
//	settings->minHaarArea = panel.getValueF("MIN_HAAR");
//	settings->maxHaarArea = panel.getValueF("MAX_HAAR");

  //update fishes
	settings.bTrackArms = panel.getValueB("TRACK_ARMS");
	panel.setGroupActive("fishes", "generaloptions", settings.bTrackArms);
	settings.minArm = panel.getValueF("MIN_ARM")/100.0f; //scale var to be right for tracker
	//settings.maxArm = panel.getValueF("MAX_ARM")/100.0f;	//scale var to be right for tracker
	settings.bSenseLeftRight = (panel.getValueB("SENSE_LEFTRIGHT"));
	settings.leftAngleThres = panel.getValueF("LEFT_ANGLE_THRES");
	settings.rightAngleThres = panel.getValueF("RIGHT_ANGLE_THRES");
	settings.leftHorizThres = panel.getValueF("LEFT_HORIZ_THRES");
	settings.rightHorizThres = panel.getValueF("RIGHT_HORIZ_THRES");
  	
	//update osc stuff
	settings.bSendOsc = panel.getValueB("SEND_OSC");
	settings.bSendTuio = panel.getValueB("SEND_TUIO");
	settings.bSendTcp = panel.getValueB("SEND_TCP");
    settings.bSendWebSocketServer = panel.getValueB("SEND_WSS");
    settings.bSendWebSocketClient = panel.getValueB("SEND_WS");
    
	settings.oscHost = panel.getValueS("OSC_HOST", 0, "localhost");
	settings.oscPort = (int) atoi(panel.getValueS("OSC_PORT", 0, "12000").c_str());
	settings.bUseLegacyOsc = panel.getValueB("LEGACY_OSC");
	settings.tuioHost = panel.getValueS("TUIO_HOST", 0, "localhost");
	settings.tuioPort = (int) atoi(panel.getValueS("TUIO_PORT", 0, "3333").c_str());
	settings.tcpPort = (int) atoi(panel.getValueS("TCP_PORT", 0, "8888").c_str());
    settings.webSocketServerPort = (int) atoi(panel.getValueS("WSS_PORT", 0, "7681").c_str());
    settings.webSocketPort = (int) atoi(panel.getValueS("WS_PORT", 0, "7681").c_str());
    settings.webSocketHost = panel.getValueS("WS_HOST", 0, "localhost");
    settings.webSocketUseSSL = panel.getValueB("WS_USESSL");
    settings.webSocketChannel = panel.getValueS("WS_CHANNEL", 0, "localhost");
    
	settings.bSendOscContours = panel.getValueB("SEND_OSC_CONTOURS");
	panel.setGroupActive("sensing", "options", settings.bSendOscContours);
	
	panel.setGroupActive("communication", "OSC", settings.bSendOsc);
	panel.setGroupActive("communication", "TUIO", settings.bSendTuio);
	panel.setGroupActive("communication", "TCP", settings.bSendTcp);
	
	// UPDATE GUI QUADS HERE
	// because this returns a pointer to the actual points that get updated,
	// you store it in an array so it doesn't get updated when it draws
	ofPoint * scaledPoints = quadGui.getScaledQuadPoints(cameraWidth,cameraHeight);
	for (int i=0; i<4; i++){
		settings.quadWarpScaled[i] = scaledPoints[i];
	}
	
	//modify custom parameters
	vector<ofxTSPSGUICustomParam>::iterator it;
	for(it = params.begin(); it != params.end(); it++){
		ofxTSPSGUICustomParam p = *it;
		switch (p.type) {
			case PARAM_INT:
				*p.i = panel.getValueI(p.key);
				break;
			case PARAM_FLOAT:
				*p.f = panel.getValueF(p.key);
				break;
			case PARAM_BOOL:
				*p.b = panel.getValueB(p.key);
				break;
			default:
				printf("ofxTSPSGUIManager: WARNING undefined parameter type encountered\n");
				break;
		}
	}
	
	//get xml
	settings.currentXmlFile = panel.getCurrentXMLFile();
	
	//get current panel
	settings.lastCurrentPanel = settings.currentPanel;
	settings.currentPanel = panel.getSelectedPanel();
}

//BR: Added some messiness here to setup, draw, and update the gui quad...

void ofxTSPSGuiManager::setupQuadGui ( int _cameraWidth, int _cameraHeight )
{
	cameraWidth = _cameraWidth;
	cameraHeight = _cameraHeight;
	
	// give the gui quad a starting setting
	
    quadGui.setup("QUAD");
	ofPoint quadSrc[4];
	quadSrc[0].set(0, 0);
	quadSrc[1].set(cameraWidth, 0);
	quadSrc[2].set(cameraWidth, cameraHeight);
	quadSrc[3].set(0, cameraHeight);
	quadGui.setQuadPoints(quadSrc);
	
	// give the gui quad a default setting
	settings.quadWarpOriginal[0].set(0, 0);
	settings.quadWarpOriginal[1].set(cameraWidth, 0);
	settings.quadWarpOriginal[2].set(cameraWidth, cameraHeight);
	settings.quadWarpOriginal[3].set(0, cameraHeight);
	
	//BR TO DO: add this into the normal settings file
	quadGui.width = cameraWidth;
	quadGui.height = cameraHeight;
	quadGui.disableAppEvents();
	
	quadGuiSetup = true;
};

void ofxTSPSGuiManager::drawQuadGui(){
	quadGui.draw();
};

void ofxTSPSGuiManager::drawQuadGui( int x, int y, int width, int height ){
	quadGui.x = x;
	quadGui.y = y;
	quadGui.setScale((float) width/quadGui.width, (float) height/quadGui.height );
	drawQuadGui();
};


// ZACK BOKA: Added so the quadGui instance can know when image warping is allowed to occur
//            (i.e., the image can only get warped when in Camera View).
void ofxTSPSGuiManager::changeGuiCameraView(bool bCameraView) {
	quadGui.bCameraView = bCameraView;
};

/***************************************************************
    GET + SET SPECIFIC VALUES FROM THE GUI
***************************************************************/

//----------------------------------------------------------
bool ofxTSPSGuiManager::getValueB( string name ){
    return panel.getValueB(name);
}

//----------------------------------------------------------
int ofxTSPSGuiManager::getValueI( string name ){
    return panel.getValueI(name);
}

//----------------------------------------------------------
float ofxTSPSGuiManager::getValueF( string name ){
    return panel.getValueF(name);    
}

//----------------------------------------------------------
string ofxTSPSGuiManager::getValueS( string name ){
    return panel.getValueS(name);    
}

//----------------------------------------------------------
void ofxTSPSGuiManager::setValueB( string name, bool val ){
    panel.setValueB(name, val);    
}

//----------------------------------------------------------
void ofxTSPSGuiManager::setValueI( string name, int val ){
    panel.setValueI(name, val);    
}

//----------------------------------------------------------
void ofxTSPSGuiManager::setValueF( string name, float val ){
    panel.setValueF(name, val);    
}

//----------------------------------------------------------
void ofxTSPSGuiManager::setValueS( string name, string val ){
    panel.setValueS(name, val);    
}

/***************************************************************
 ENABLE / DISABLE ELEMENTS
***************************************************************/

//----------------------------------------------------------
    void ofxTSPSGuiManager::enableElement( string name ){
        guiBaseObject * el = panel.getElement( name );
        if (el != NULL){
            el->enable();
        }
    };

//----------------------------------------------------------
    void ofxTSPSGuiManager::disableElement( string name ){
        guiBaseObject * el = panel.getElement( name );
        if (el != NULL){
            el->disable();
        }
    };

/***************************************************************
 GET EVENTS FROM GUI BUTTONS
 ***************************************************************/

void ofxTSPSGuiManager::saveEventCatcher( string & buttonName){
	settings.currentXmlFile = panel.getCurrentXMLFile();
	quadGui.saveToFile(settings.currentXmlFile);
};

void ofxTSPSGuiManager::reloadEventCatcher( string & buttonName){\
	quadGui.readFromFile(settings.currentXmlFile);
};

void ofxTSPSGuiManager::loadEventCatcher( string & buttonName){
	settings.currentXmlFile = panel.getCurrentXMLFile();
	quadGui.readFromFile(settings.currentXmlFile);
};

void ofxTSPSGuiManager::saveAsEventCatcher( string & buttonName){
	settings.currentXmlFile = panel.getCurrentXMLFile();
	quadGui.readFromFile(settings.currentXmlFile);
};

/***************************************************************
 GET + SET PANEL INFO / SELECTED PANEL
 ***************************************************************/

//forward to gui
void ofxTSPSGuiManager::mousePressed(ofMouseEventArgs &e)
{
	if(enableGui) panel.mousePressed(e.x, e.y, e.button);
	if(quadGuiSetup){
	}
}

void ofxTSPSGuiManager::mouseDragged(ofMouseEventArgs &e)
{
	if(enableGui) panel.mouseDragged(e.x, e.y, e.button);
}

void ofxTSPSGuiManager::mouseReleased(ofMouseEventArgs &e)
{
	if(enableGui){
        panel.mouseReleased();
        if (maximizeButton->enabled) maximizeButton->checkHit(e.x, e.y, e.button);
        else if (minimizeButton->enabled) minimizeButton->checkHit(e.x, e.y, e.button);      
        else {            
            map<std::string, guiTypeButton*>::iterator it;
            for( it=customButtons.begin(); it!=customButtons.end(); it++ ){
                if ( it->second->enabled){
                    it->second->checkHit( e.x, e.y, e.button );                    
                }
            }
        }
    }
}

void ofxTSPSGuiManager::loadSettings( string xmlFile ){
	panel.loadSettings(xmlFile);	
	if (quadGuiSetup) quadGui.readFromFile(xmlFile);	
    //update();
};

void ofxTSPSGuiManager::keyPressed(ofKeyEventArgs &e)
{
	if(enableGui) panel.keyPressed(e.key);
};

void ofxTSPSGuiManager::keyReleased(ofKeyEventArgs &e)
{
	if(enableGui) panel.keyReleased(e.key);
};

void ofxTSPSGuiManager::draw() {
	if(enableGui){
		panel.draw();
        maximizeButton->render();
        minimizeButton->render();
        
        // draw custom buttons
        map<std::string, guiTypeButton*>::iterator it;
        for( it=customButtons.begin(); it!=customButtons.end(); it++ ){
            it->second->render();
        }
	}
}

/***************************************************************
    CUSTOM BUTTONS
***************************************************************/

//----------------------------------------------------------
guiTypeButton * ofxTSPSGuiManager::addButton( string name, ofRectangle dimensions, ofColor color, ofColor bgcolor ){
    guiTypeButton * btn = new guiTypeButton();
    
    // if you didn't pass a rectangle, just use the default button settings
    if ( dimensions.x == dimensions.y == dimensions.width == dimensions.height){
        btn->setup( name, buttonWidth*1.5, buttonHeight);
        btn->setPosition(10,10);    
    } else {
        btn->setup( name, dimensions.width, dimensions.height);
        btn->setPosition(dimensions.x, dimensions.y);
    }
    btn->setFont(&panel.guiTTFFont);    
    
    if ( color != NULL){
        btn->setBackgroundSelectColor(color.r, color.g, color.b);        
    } else {
        btn->setBackgroundSelectColor(0,168,156);        
    }
    if ( bgcolor != NULL){
        btn->setBackgroundSelectColor(bgcolor.r, bgcolor.g, bgcolor.b);        
    } else {
        btn->setBackgroundColor(0,168,156); 
    }
    customButtons.insert( pair<std::string, guiTypeButton*>( name, btn) );
    return btn;
}

//----------------------------------------------------------
guiTypeButton * ofxTSPSGuiManager::getButton( string name ){
    if ( customButtons.find( name ) != customButtons.end() ){
        return customButtons[name];
    } else {
        return NULL;
    }    
}

/***************************************************************
    MINIMIZE + MAXIMIZE THE APP
***************************************************************/

//----------------------------------------------------------
void ofxTSPSGuiManager::minimize(string & button){
    settings.bMinimized = true;
    ofSetWindowShape(100,40);
    minimizeButton->disable();
    maximizeButton->enable();
};

//----------------------------------------------------------
void ofxTSPSGuiManager::maximize(string & button){
    settings.bMinimized = false;  
    ofSetWindowShape(1024,768); 
    minimizeButton->enable();
    maximizeButton->disable();
};


/***************************************************************
    GET SETTINGS OBJECT (NO LONGER SINGLETON!)
 ***************************************************************/

//----------------------------------------------------------
ofxTSPSSettings * ofxTSPSGuiManager::getSettings(){
    return &settings;
};
