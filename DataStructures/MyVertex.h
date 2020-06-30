//MyVertex.h
#pragma once 
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include "MyFace.h"


class MyVertex {

	OSG::Pnt3f vertex;
	std::vector<MyFace*> faces;

public:

	MyVertex(OSG::Pnt3f thevertex);
	~MyVertex();
	OSG::Pnt3f getVertex();
	void setVertex(OSG::Pnt3f thevertex);

	std::vector<MyFace*> getFaces();
	int getFacesLength();
	void addFace(MyFace* face);
};