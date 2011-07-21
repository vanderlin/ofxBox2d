#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 290, 900, OF_WINDOW);
	ofRunApp(new testApp());
	
}
