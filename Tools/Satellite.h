//File Satellite contains the definitions of Satellite functions
//File created @ 2009-10-21

#ifndef __SATELLITE_H
#define __SATELLITE_H

#include <stdio.h>
#include <iostream>

#include <osg/Node>
#include <osg/MatrixTransform>
//#include "disEarth.h"
//#include "const.h"
#include "Hud.h"
#include "HLBToFixXYZ.h"
#include "windows.h"

#define MAX_DEVICE_NUMBER 32
#define MAX_ACTIVE_DEVICE_NUMBER 1000
#define MAX_NodelsName 100
#define MAX_DeviceUnitage 50
using namespace std;

class Device
{
public:
	osg::Node* pNodeModel;
public:
	string strDiscription;
	TCHAR  TCHRNodelsName[MAX_NodelsName];
	TCHAR  TCHRDeviceUnitage[MAX_DeviceUnitage];
	osg::MatrixTransform* pMatrixTransform;
	osg::MatrixTransform* pTransformMatrix;
	bool bVisible;
	bool bHighLight;
	bool bVisibleOfTemperature;
	osgText::Text* pTextOfTemperature;
	osg::Geode* pNodeToShowTemperature;
	double DeviceParameter; 

public:
	Device();
	~Device(){};

	void InitialModel(const string strConfigFileName){};

//	void FixToFather(const osg::Group* pGroupFather);
	//void FixToFather(osg::Group* pGroupFather){};

	void HighLight();//this high light the model 
	void CancelHighLight();
void Device::DisplayTemperature(const double Temperature);
};

class DeviceWithAction :public Device,public osg::NodeCallback
{
public:
	osg::Vec3d vPosition;
	osg::Vec3d vEulerAttitude;
	osg::Vec3d vScale;
	osg::Quat qQuaternionAttitude;
	osg::Vec3d vSpeed;
	osg::Vec3d vAngularSpeed;
	osg::Matrix mFixMatrix;
	double CurrentDeviceParameter;
	TCHAR TCHRCurrentDeviceName[100];
	bool bRotateByQuaternion;
	bool bThrusterOnFire;
public:
	DeviceWithAction();
	virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);
};

class MainBody :public DeviceWithAction
{
public:
	Device clsDevices[MAX_DEVICE_NUMBER];
	DeviceWithAction clsActiveDevices[MAX_ACTIVE_DEVICE_NUMBER];
	unsigned char ucDeviceNumber;
	unsigned char ucActiveDeviceNumber;

public:
	MainBody(osg::Group *pRoot,osgViewer::Viewer *pViewer);
	bool ReadConfigFile(const char chrFileName[]);
	void AssembleSatellite(void);
	void UpdateSatelliteData(unsigned char ucId,double Data);
//	add messages to hud or scroll hud
};


#endif 