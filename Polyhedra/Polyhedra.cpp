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
OSG::NodeRefPtr createTetrahedron();
OSG::NodeRefPtr createDual(OSG::NodeRefPtr nodegeo);

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

		//create a tetrahedra

		OSG::NodeRefPtr scene = OSG::Node::create();
		OSG::GroupRefPtr group_core = OSG::Group::create();
		scene->setCore(group_core);

		//create a tetrahedron
		OSG::NodeRefPtr tetra = createTetrahedron();
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

	OSG::NodeRefPtr dualthnode = OSG::Node::create();

	OSG::TriangleIterator triangleIterator;

	OSG::GeometryRefPtr thgeo = OSG::Geometry::create();

	OSG::GeometryRefPtr geocore = dynamic_cast<OSG::Geometry *>(nodegeo->getCore());

	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();

	type->addValue(GL_TRIANGLES);

	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(12);

	std::vector<OSG::Pnt3f> dualPoints;

	for (triangleIterator = geocore->beginTriangles();
		triangleIterator != geocore->endTriangles();
		++triangleIterator)
	{
		std::cout << "Triangle " << triangleIterator.getIndex() << ":" << std::endl;
		std::cout << triangleIterator.getPosition(0) << " " << triangleIterator.getNormal(0) << std::endl;
		std::cout << triangleIterator.getPosition(1) << " " << triangleIterator.getNormal(1) << std::endl;
		std::cout << triangleIterator.getPosition(2) << " " << triangleIterator.getNormal(2) << std::endl;

		OSG::Pnt3f p0 = triangleIterator.getPosition(0);
		OSG::Pnt3f p1 = triangleIterator.getPosition(1);
		OSG::Pnt3f p2 = triangleIterator.getPosition(2);

		float xdual = (p0.x() + p1.x() + p2.x()) / 3;
		float ydual = (p0.y() + p1.y() + p2.y()) / 3;
		float zdual = (p0.z() + p1.z() + p2.z()) / 3;

		dualPoints.push_back(OSG::Pnt3f(xdual, ydual, zdual));
	
	}

	std::vector<OSG::Pnt3f>::iterator myIntVectorIterator;

	OSG::GeoPnt3fPropertyRefPtr  finaldualPoints = OSG::GeoPnt3fProperty::create();

	for (myIntVectorIterator = dualPoints.begin();
		myIntVectorIterator != dualPoints.end();
		myIntVectorIterator++) {
		finaldualPoints->addValue(*myIntVectorIterator);
	}

	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//POINTS OF FACES
	//face 1: front
	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(3);
	indices->addValue(1);

	//face 4: left
	//face 4 - triangle 1
	indices->addValue(2);
	indices->addValue(3);
	indices->addValue(0);

	//face 2: right 
	//face 2 - triangle 1
	indices->addValue(1);
	indices->addValue(3);
	indices->addValue(2);

	//face 3: bottom
	//face 3 - triangle 1
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


OSG::NodeRefPtr createTetrahedron()
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

	float length = 1;

	s = length;

	h = (sqrt(3) * s) / 2; 

	H = (sqrt(6) * s) / 3;

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


	OSG::Vec3f a = p0-p1;
	OSG::Vec3f a1 = p3 - p1;

	OSG::Vec3f b = p2-p0;
	OSG::Vec3f b1 = p3 - p0;

	OSG::Vec3f c = p1-p2;
	OSG::Vec3f c1 = p3-p2;


	OSG::Vec3f d = p1-p0;
	OSG::Vec3f d1 = p2-p0;
	
	OSG::GeoVec3fPropertyRefPtr  norms = OSG::GeoVec3fProperty::create();
	norms->push_back(OSG::Vec3f(a.cross(a1))); //back	
	norms->push_back(OSG::Vec3f(b.cross(b1))); //left
	norms->push_back(OSG::Vec3f(c.cross(c1))); //right
	norms->push_back(OSG::Vec3f(d.cross(d1))); //bottom
	
											
	// The colours.   
	// GeoColor3fProperty stores all color values that will be used
	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();

	colors->addValue(OSG::Color3f(0, 0, 1)); //blue
	colors->addValue(OSG::Color3f(1, 0, 0)); //red
	colors->addValue(OSG::Color3f(0, 1, 0)); //green
	colors->addValue(OSG::Color3f(1, 0.5, 0)); //orange

	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//POINTS OF FACES
	//face 1: front
	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(3);
	indices->addValue(1);

	//face 4: left
	//face 4 - triangle 1
	indices->addValue(2);
	indices->addValue(3);
	indices->addValue(0);


	//face 2: right 
	//face 2 - triangle 1
	indices->addValue(1);
	indices->addValue(3);
	indices->addValue(2);


	//face 3: bottom
	//face 3 - triangle 1
	indices->addValue(1);
	indices->addValue(2);
	indices->addValue(0);


	

	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//FACE NORMALS
	//face 1: front 
	//face 1 - triangle 1
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);


	//face 2: right 
	//face 2 - triangle 1
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);


	//face 3: back
	//face 3 - triangle 1
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);


	//face 4: left
	//face 4 - triangle 1
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

