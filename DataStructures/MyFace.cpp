#include "MyFace.h"

	OSG::Pnt3f faceCentre;

	MyFace::~MyFace()
	{

	}
	
	
	MyFace::MyFace(OSG::Pnt3f centre)
	{
		faceCentre = centre;
	}

	OSG::Pnt3f MyFace::getFaceCentre()
	{
		return faceCentre;
	}

	void MyFace::setFaceCentre(OSG::Pnt3f centre)
	{
		faceCentre = centre;
	}

