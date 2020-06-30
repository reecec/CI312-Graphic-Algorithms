#pragma once 
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>


class MyFace {

	OSG::Pnt3f faceCentre;

public:
	MyFace(OSG::Pnt3f centre);
	~MyFace();
	OSG::Pnt3f getFaceCentre();
	void setFaceCentre(OSG::Pnt3f centre);
};