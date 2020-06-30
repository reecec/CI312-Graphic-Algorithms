#include "MyVertex.h"


	OSG::Pnt3f vertex;
	std::vector<MyFace*> faces;

	
	MyVertex::~MyVertex()
	{
	};
	
	

	MyVertex::MyVertex(OSG::Pnt3f thevertex)
	{
		vertex = thevertex;
	}

	OSG::Pnt3f MyVertex::getVertex()
	{
		return vertex;
	}

	void MyVertex::setVertex(OSG::Pnt3f thevertex)
	{
		vertex = thevertex;
	}

	std::vector<MyFace*> MyVertex::getFaces()
	{
		return faces;
	}

	int MyVertex::getFacesLength()
	{
		return faces.size();
	}

	void MyVertex::addFace(MyFace* face)
	{
		faces.push_back(face);
	}

