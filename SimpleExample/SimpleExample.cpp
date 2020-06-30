// SimpleExample 
//
// This tutorial shows a simple example of 
// how to use OpenSG together with GLUT to create a little
// interactive scene viewer.
// taken from OpenSG Tutorials


// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <math.h>

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;
OSG::NodeRefPtr spherenode;


enum MENU_ITEMS

{

	WIREFRAME,

	SOLID,

	VERTEX,

};


//You will need to include also all the functions to setup GLUT :

// GLUT callback functions

//

// redraw the window

void display(void)

{

	//get the volume of the sphere node and all its children

	OSG::BoxVolume vol;

	spherenode->getWorldVolume(vol);


	//get minimum and maximum values

	OSG::Pnt3f min, max;

	vol.getBounds(min, max);

	std::cout << min << " " << max << std::endl;

	mgr->setHighlight(spherenode);
	mgr->redraw();

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

// setup the GLUT library which handles the windows for us

int setupGLUT(int *argc, char *argv[])

{

	glutInit(argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);



	int winid = glutCreateWindow("03SettingScene Tutorial");


	

	glutReshapeFunc(reshape);

	glutDisplayFunc(display);

	glutMouseFunc(mouse);

	glutMotionFunc(motion);

	glutKeyboardFunc(keyboard);

	createMenu();


	return winid;

	// initialise OpenSG
}

int main(int argc, char **argv)

{
	// initialise GLUT
	int winid = setupGLUT(&argc, argv);

	OSG::osgInit(argc, argv);

	// create a OSGGLUT window
	OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();


	OSG::GeometryRefPtr spheregeo = OSG::makeSphereGeo(2, 1);

	spherenode = OSG::Node::create();

	spherenode->setCore(spheregeo);

	OSG::commitChanges();

	gwin->setGlutId(winid);

	gwin->init();

	// create the SimpleSceneManager helper
	mgr = OSG::SimpleSceneManager::create();

	// tell the manager what to manage
	mgr->setWindow(gwin);

	mgr->setRoot(spherenode);

	// show the whole scene
	mgr->showAll();

	// GLUT main loop
	glutMainLoop();

	return 0;
}
