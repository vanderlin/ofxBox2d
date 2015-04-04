#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
    ofSetupOpenGL(1000, 500, OF_WINDOW);
    ofRunApp(new testApp());
}
