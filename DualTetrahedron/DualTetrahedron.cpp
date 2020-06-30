// Geometry
//
// This tutorial supports learning
// about assembling a scene in a scene graph
// using transformation cores

// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <math.h>
#include <OpenSG/OSGTriangleIterator.h>

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;
//OSG::NodeRefPtr createCube();
OSG::NodeRefPtr createTetrahedron(float);
OSG::NodeRefPtr createDual(OSG::NodeRefPtr nodegeo);
OSG::NodeRefPtr createCube();

//OSG::Vec3f getNorms(OSG::Pnt3f a, OSG::Pnt3f b, OSG::Pnt3f c);
int setupGLUT(int *argc, char *argv[]);

enum MENU_ITEMS

{
	WIREFRAME,
	SOLID,
	VERTEX,
};


int main(int argc, char **argv)
{

	// initialise OpenSG
	OSG::osgInit(argc, argv);

	// initialise GLUT
	int winid = setupGLUT(&argc, argv);

	{
		// create a OSGGLUT window
		OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
		gwin->setGlutId(winid);
		gwin->init();
		
		OSG::NodeRefPtr scene = OSG::Node::create();
		OSG::GroupRefPtr group_core = OSG::Group::create();
		scene->setCore(group_core);

		//value for sides of tetrahedron to be passed in
		float s = 1;

		//create a tetrahedron
		OSG::NodeRefPtr tetra = createTetrahedron(s);
		scene->addChild(tetra);

		OSG::NodeRefPtr dual = createDual(tetra);
		scene->addChild(dual);

		//commit all changes to OpenSG 
		OSG::commitChanges();

		// create the SimpleSceneManager helper
		mgr = OSG::SimpleSceneManager::create();

		// tell the manager what to manage
		mgr->setWindow(gwin);
		mgr->setRoot(scene);

		// show the whole scene
		mgr->showAll();
	}

	// GLUT main loop
	glutMainLoop();

	return 0;
}

//
// GLUT callback functions
//

// redraw the window
void display(void)
{
	mgr->redraw();
}

// react to size changes
void reshape(int w, int h)
{
	mgr->resize(w, h);
	glutPostRedisplay();
}

// react to mouse button presses
void mouse(int button, int state, int x, int y)
{
	if (!state)
		mgr->mouseButtonPress(button, x, y);

	glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{
	mgr->mouseMove(x, y);
	glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 'e':
	{
		// clean up global variables
		mgr = NULL;

		OSG::osgExit();
		exit(0);
	}
	break;
	case 's':
	{
		mgr->setStatistics(!mgr->getStatistics());
	}
	break;
	}
}

void menu(int item)

{

	switch (item)

	{

	case VERTEX: {

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	}

				 break;

	case WIREFRAME:
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	break;

	case SOLID:
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	break;


	default:

	{            }

	break;

	}



	glutPostRedisplay();



	return;

}

void createMenu() {

	glutCreateMenu(menu);

	// Add menu items

	glutAddMenuEntry("Show vertices", VERTEX);

	glutAddMenuEntry("Show wireframe", WIREFRAME);

	glutAddMenuEntry("Show solid", SOLID);



	// Associate a mouse button with menu

	glutAttachMenu(GLUT_RIGHT_BUTTON);



	return;

}


// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	int winid = glutCreateWindow("02 Geometry Tutorial");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	createMenu();

	return winid;
}

OSG::NodeRefPtr createDual(OSG::NodeRefPtr nodegeo)
{

	//create a node to hold the geometry
	OSG::NodeRefPtr dualthnode = OSG::Node::create();

	//used to iterate over OpenGL triangle primitives
	OSG::TriangleIterator triangleIterator;

	//create a geometry
	OSG::GeometryRefPtr thgeo = OSG::Geometry::create();

	//reference geometry core to hold passed in geomotry core (tetrahedron geometry)
	OSG::GeometryRefPtr geocore = dynamic_cast<OSG::Geometry *>(nodegeo->getCore());

	//The primitive types.
	//OpenGL provides us with several different types of shapes that we can draw 
	//(e.g. GL_LINES, GL_POLYGON, GL_QUADS, GL_TRIANGLES)
	//we need to specify the type of geometry we want to use
	//lets start by using only triangles (although different
	//types can be freely mixed)
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();

	//adding OpenGl triangle primitive to type
	type->addValue(GL_TRIANGLES);

	// The primitive lengths.
	//These define the number of vertices to be passed to OpenGL for each 
	//primitive Thus there have to be at least as many entries as in the types
	//property.in the case of the cube we are using 12 triangles which each  
	//have 3 vertices (12 X 3 = 36) 
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();

	lens->addValue(12); //4 triangles which have 3 vertices (4 * 3 = 12)

	//vector to store centre of points for dual geometry
	std::vector<OSG::Pnt3f> dualPoints;

	//iterate over triangle primitives of geometry core passed in (tetrahedron)
	//code for loop shown in lecture slides
	for (triangleIterator = geocore->beginTriangles();
		triangleIterator != geocore->endTriangles();
		++triangleIterator)
	{
		std::cout << "Triangle " << triangleIterator.getIndex() << ":" << std::endl;
		std::cout << triangleIterator.getPosition(0) <<  std::endl;
		std::cout << triangleIterator.getPosition(1) <<  std::endl;
		std::cout << triangleIterator.getPosition(2) <<  std::endl;

		//storing 3 points of trinagle primitive currently iterating
		OSG::Pnt3f p0 = triangleIterator.getPosition(0);
		OSG::Pnt3f p1 = triangleIterator.getPosition(1);
		OSG::Pnt3f p2 = triangleIterator.getPosition(2);

		//variables to store centre co-ordinates of triangle primitive iterating over 
		//by applying formula to assign the variable value
		float xdual = (p0.x() + p1.x() + p2.x()) / 3;
		float ydual = (p0.y() + p1.y() + p2.y()) / 3;
		float zdual = (p0.z() + p1.z() + p2.z()) / 3;

		std::cout << "Points of dual " << OSG::Pnt3f(xdual, ydual, zdual) << std::endl;

		//adding Pnt3f variable (centre point of triangle primitive) to end of dual points 
		//vector storing centre points for dual geometry
		dualPoints.push_back(OSG::Pnt3f(xdual, ydual, zdual));
	
	}

	//used to iterate over dual points reference pointer list
	std::vector<OSG::Pnt3f>::iterator myIntVectorIterator;

	//used to store final centre points for indexing dual geometry
	OSG::GeoPnt3fPropertyRefPtr  finaldualPoints = OSG::GeoPnt3fProperty::create();
	
	//iterating over dual points vector containing 
	//centre points so can be added to finaldualpoints pointer
	//to add points for indexing the dual geometry
	for (myIntVectorIterator = dualPoints.begin();
		myIntVectorIterator != dualPoints.end();
		myIntVectorIterator++) {
		finaldualPoints->addValue(*myIntVectorIterator);
	}

	// The indices.
	// in order not to replicate the same positions all the time, 
	// use index number of the position in relation to index of pnts reference pointer list
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//POINTS OF FACES
	//face 1: front of tetrahedron
	//face 1: left of dual tetrahedron
	indices->addValue(0);
	indices->addValue(3);
	indices->addValue(1);

	//face 2: left of tetrahedron
	//face 2: right of dual tetrahedron
	indices->addValue(2);
	indices->addValue(3);
	indices->addValue(0);
	
	//face 3: right of tetrahedron
	//face 3: front of dual tetrahedron
	indices->addValue(1);
	indices->addValue(3);
	indices->addValue(2);

	//face 4: bottom of tetrahedron
	//face 4: top of dual tetrahedron
	indices->addValue(1);
	indices->addValue(2);
	indices->addValue(0);
	
	// Put it all together into a Geometry NodeCore.
	thgeo->setTypes(type);
	thgeo->setLengths(lens);
	thgeo->setProperty(finaldualPoints, OSG::Geometry::PositionsIndex);
	thgeo->setIndex(indices, OSG::Geometry::PositionsIndex);

	dualthnode->setCore(thgeo);

	return dualthnode;
}

OSG::NodeRefPtr createTetrahedron(float length)
{	
	//create a node to hold the geometry
	OSG::NodeRefPtr thnode = OSG::Node::create();

	//create a geometry
	OSG::GeometryRefPtr thgeo = OSG::Geometry::create();

	//The primitive types.
	//OpenGL provides us with several different types of shapes that we can draw 
	//(e.g. GL_LINES, GL_POLYGON, GL_QUADS, GL_TRIANGLES)
	//we need to specify the type of geometry we want to use
	//lets start by using only triangles (although different types can be freely mixed)
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();

	//MODIFY HERE 
	type->addValue(GL_TRIANGLES);

	//The primitive lengths.
	//These define the number of vertices to be passed to OpenGL for each primitive. 
	//Thus there have to be at least as many entries as in the types property.
	//in the case of the cube we are using 12 triangles which each have 3 vertices (12 X 3 = 36) 
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	//MODIFY HERE 
	lens->addValue(12); //4 triangles which have 3 vertices (4 * 3 = 12)

	float s, h, H;

	//side
	s = length;

	//lateral height
	h = (sqrt(3) * s) / 2; 

	//space height
	H = (sqrt(6) * s) / 3;

	//variables to store co-ordinates of four points of tetrahedron
	OSG::Pnt3f p0, p1, p2, p3;

	// The vertices.
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
	//MODIFY HERE with positions of your geometry
	p0 = OSG::Pnt3f(0.0, 0.0, 0.0);
	pnts->addValue(OSG::Pnt3f(p0)); //point 0 - origin
	p1 = OSG::Pnt3f(s, 0.0, 0.0);
	pnts->addValue(OSG::Pnt3f(p1)); //point 1 - x/right
	p2 = OSG::Pnt3f(s / 2, 0.0, h);
	pnts->addValue(OSG::Pnt3f(p2)); //point 2 - z/forward
	p3 = OSG::Pnt3f(s / 2, H, h/3);
	pnts->addValue(OSG::Pnt3f(p3)); //point 3 - y/top vertex

	//print to console point co-ordinates
	std::cout << "Point 0: " << p0 << std::endl;
	std::cout << "Point 1: " << p1 << std::endl;
	std::cout << "Point 2: " << p2 << std::endl;
	std::cout << "Point 3: " << p3 << std::endl;

	//vectors for front face
	OSG::Vec3f FFVA = p0-p1;
	OSG::Vec3f FFVB = p3 - p1;

	//vectors for left face
	OSG::Vec3f LFVA = p2-p0;
	OSG::Vec3f LFVB = p3 - p0;

	//vectors for right face
	OSG::Vec3f RFVA = p1-p2;
	OSG::Vec3f RFVB = p3-p2;

	//vectors for bottom face
	OSG::Vec3f BFVA = p1-p0;
	OSG::Vec3f BFVB = p2-p0;
	
	//cross product of faces to get face normals
	OSG::GeoVec3fPropertyRefPtr  norms = OSG::GeoVec3fProperty::create();
	norms->push_back(OSG::Vec3f(FFVA.cross(FFVB))); //front
	norms->push_back(OSG::Vec3f(LFVA.cross(LFVB))); //left
	norms->push_back(OSG::Vec3f(RFVA.cross(LFVB))); //right
	norms->push_back(OSG::Vec3f(BFVA.cross(BFVB))); //bottom

	//Calculate distances of edges of tetrahedron by using varation of pyhtagerous thereom, Magnitude of edge vectors
	float squared = 2; //power of value to pass in for pow function
	float mag_VecFFVA = std::sqrt((pow(FFVA.x(), squared)) + pow(FFVA.y(), squared) + pow(FFVA.z(), squared));
	float mag_VecFFVB = std::sqrt((pow(FFVB.x(), squared)) + pow(FFVB.y(), squared) + pow(FFVB.z(), squared));
	float mag_VecLFVA = std::sqrt((pow(LFVA.x(), squared)) + pow(LFVA.y(), squared) + pow(LFVA.z(), squared));
	float mag_VecLFVB = std::sqrt((pow(LFVB.x(), squared)) + pow(LFVB.y(), squared) + pow(LFVB.z(), squared));
	float mag_VecRFVA = std::sqrt((pow(RFVA.x(), squared)) + pow(RFVA.y(), squared) + pow(RFVA.z(), squared));
	float mag_VecRFVB = std::sqrt((pow(RFVB.x(), squared)) + pow(RFVB.y(), squared) + pow(RFVB.z(), squared));
	float mag_VecBFVA = std::sqrt((pow(BFVA.x(), squared)) + pow(BFVA.y(), squared) + pow(BFVA.z(), squared));
	float mag_VecBFVB = std::sqrt((pow(BFVB.x(), squared)) + pow(BFVB.y(), squared) + pow(BFVB.z(), squared));

	//printing to console vector magnitudes
	std::cout << "Tetrahedron edge vector magnitudes" <<  std::endl;	
	std::cout << "Vector FFVA magnitude: " << mag_VecFFVA << std::endl;
	std::cout << "Vector FFVB magnitude: " << mag_VecFFVB << std::endl;
	std::cout << "Vector LFVA magnitude: " << mag_VecLFVA << std::endl;
	std::cout << "Vector LFVB magnitude: " << mag_VecLFVB << std::endl;
	std::cout << "Vector RFVA magnitude: " << mag_VecRFVA << std::endl;
	std::cout << "Vector RFVB magnitude: " << mag_VecRFVB << std::endl;
	std::cout << "Vector BFVA magnitude: " << mag_VecBFVA << std::endl;
	std::cout << "Vector BFVB magnitude: " << mag_VecBFVB << std::endl;
											
	// The colours.   
	// GeoColor3fProperty stores all color values that will be used
	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();

	colors->addValue(OSG::Color3f(0, 0, 1)); //blue - front face
	colors->addValue(OSG::Color3f(1, 0, 0)); //red - left face
	colors->addValue(OSG::Color3f(0, 1, 0)); //green - right face
	colors->addValue(OSG::Color3f(1, 0.5, 0)); //orange - bottom face

	// The indices.
	// in order not to replicate the same positions all the time, 
	// use index number of the position in relation to index of pnts reference pointer list
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//POINTS OF FACES
	//face 1: front
	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(3);
	indices->addValue(1);

	//face 2: left
	//face 2: triangle 2
	indices->addValue(2);
	indices->addValue(3);
	indices->addValue(0);

	
	//face 3: right 
	//face 3: triangle 3
	indices->addValue(1);
	indices->addValue(3);
	indices->addValue(2);

	
	//face 4: bottom
	//face 4: triangle 4
	indices->addValue(1);
	indices->addValue(2);
	indices->addValue(0);
	
	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//FACE NORMALS
	//face 1: front 
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);

	
	//face 2: left
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);

	
	//face 3: right
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);

	
	//face 4: bottom
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	

	// Put it all together into a Geometry NodeCore.
	thgeo->setTypes(type);
	thgeo->setLengths(lens);

	thgeo->setProperty(pnts, OSG::Geometry::PositionsIndex);
	thgeo->setIndex(indices, OSG::Geometry::PositionsIndex);

	thgeo->setProperty(norms, OSG::Geometry::NormalsIndex);
	thgeo->setIndex(indicesnormpos, OSG::Geometry::NormalsIndex);

	thgeo->setProperty(colors, OSG::Geometry::ColorsIndex);
	thgeo->setIndex(indicesnormpos, OSG::Geometry::ColorsIndex);

	thnode->setCore(thgeo);

	return thnode;
}

OSG::NodeRefPtr createCube() {
	//create a node to hold the geometry
	OSG::NodeRefPtr geonode = OSG::Node::create();

	//create a geometry
	OSG::GeometryRefPtr geo = OSG::Geometry::create();

	//The primitive types.
	//OpenGL provides us with several different types of shapes that we can draw 
	//(e.g. GL_LINES, GL_POLYGON, GL_QUADS, GL_TRIANGLES)
	//we need to specify the type of geometry we want to use
	//lets start by using only triangles (although different types can be freely mixed)
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	//MODIFY HERE 
	type->addValue(GL_TRIANGLES);

	//The primitive lengths.
	//These define the number of vertices to be passed to OpenGL for each primitive. 
	//Thus there have to be at least as many entries as in the types property.
	//in the case of the cube we are using 12 triangles which each have 3 vertices (12 X 3 = 36) 
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	//MODIFY HERE 
	lens->addValue(36);


	// The vertices.
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
	//MODIFY HERE with positions of your geometry
	pnts->addValue(OSG::Pnt3f(-0.5, 0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(0.5, 0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(0.5, -0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(-0.5, -0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(-0.5, 0.5, -0.5));
	pnts->addValue(OSG::Pnt3f(0.5, 0.5, -0.5));
	pnts->addValue(OSG::Pnt3f(0.5, -0.5, -0.5));
	pnts->addValue(OSG::Pnt3f(-0.5, -0.5, -0.5));

	// The normals.
	//These are used for lighting calculations and have to point away from the
	//surface. Normals are standard vectors. 
	OSG::GeoVec3fPropertyRefPtr  norms = OSG::GeoVec3fProperty::create();
	norms->push_back(OSG::Vec3f(0, 0, 1));
	norms->push_back(OSG::Vec3f(1, 0, 0));
	norms->push_back(OSG::Vec3f(0, 0, -1));
	norms->push_back(OSG::Vec3f(-1, 0, 0));
	norms->push_back(OSG::Vec3f(0, 1, 0));
	norms->push_back(OSG::Vec3f(0, -1, 0));

	// The colours.   
	// GeoColor3fProperty stores all color values that will be used
	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));

	// The indices.
	// in order not to replicate the same positions all the time, 
	// use index number of the position 
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//face 1: front 
	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(2);
	indices->addValue(1);
	//face 1 - triangle 2
	indices->addValue(0);
	indices->addValue(3);
	indices->addValue(2);


	//face 2: right 
	//face 2 - triangle 1
	indices->addValue(1);
	indices->addValue(2);
	indices->addValue(6);
	//face 3 - triangle 2
	indices->addValue(1);
	indices->addValue(6);
	indices->addValue(5);


	//face 3: back
	//face 3 - triangle 1
	indices->addValue(5);
	indices->addValue(6);
	indices->addValue(7);
	//face 3 - triangle 2
	indices->addValue(5);
	indices->addValue(7);
	indices->addValue(4);


	//face 4: left
	//face 4 - triangle 1
	indices->addValue(4);
	indices->addValue(7);
	indices->addValue(3);
	//face 4 - triangle 2
	indices->addValue(4);
	indices->addValue(3);
	indices->addValue(0);

	//face 5: top
	//face 5 - triangle 1
	indices->addValue(4);
	indices->addValue(1);
	indices->addValue(5);
	//face 5 - triangle 2
	indices->addValue(4);
	indices->addValue(0);
	indices->addValue(1);

	//face 6: bottom
	//face 6 - triangle 1
	indices->addValue(2);
	indices->addValue(3);
	indices->addValue(7);
	//face 6 - triangle 2
	indices->addValue(2);
	indices->addValue(7);
	indices->addValue(6);


	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//face 1: front 
	//face 1 - triangle 1
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	//face 1 - triangle 2
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);


	//face 2: right 
	//face 2 - triangle 1
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	//face 3 - triangle 2
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);


	//face 3: back
	//face 3 - triangle 1
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	//face 3 - triangle 2
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);


	//face 4: left
	//face 4 - triangle 1
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	//face 4 - triangle 2
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);

	//face 5: top
	//face 5 - triangle 1
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	//face 5 - triangle 2
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);

	//face 6: bottom
	//face 6 - triangle 1
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	//face 6 - triangle 2
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);

	// Put it all together into a Geometry NodeCore.
	geo->setTypes(type);
	geo->setLengths(lens);

	geo->setProperty(pnts, OSG::Geometry::PositionsIndex);
	geo->setIndex(indices, OSG::Geometry::PositionsIndex);

	geo->setProperty(norms, OSG::Geometry::NormalsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::NormalsIndex);

	geo->setProperty(colors, OSG::Geometry::ColorsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::ColorsIndex);

	// if you were not using any indexing you will simply use:
	//geo->setTypes    (type);
	//geo->setLengths  (lens);
	//geo->setPositions (pnts);
	//geo->setNormals   (norms);
	//geo->setColors    (colors);        

	geonode->setCore(geo);

	return geonode;

}