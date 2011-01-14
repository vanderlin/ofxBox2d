# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to, avoiding a bug in XCode 1.5
all.Debug: \
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Debug/libBox2D.dylib\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/Debug/HelloWorld\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/Debug/Testbed

all.Release: \
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Release/libBox2D.dylib\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/Release/HelloWorld\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/Release/Testbed

all.MinSizeRel: \
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/MinSizeRel/libBox2D.dylib\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/MinSizeRel/HelloWorld\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/MinSizeRel/Testbed

all.RelWithDebInfo: \
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/RelWithDebInfo/libBox2D.dylib\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/RelWithDebInfo/HelloWorld\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/RelWithDebInfo/Testbed

# For each target create a dummy rule so the target does not have to exist
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Debug/libBox2D.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/MinSizeRel/libBox2D.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/RelWithDebInfo/libBox2D.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Release/libBox2D.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/freeglut/Debug/libfreeglut_static.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/glui/Debug/libglui.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/freeglut/MinSizeRel/libfreeglut_static.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/glui/MinSizeRel/libglui.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/freeglut/RelWithDebInfo/libfreeglut_static.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/glui/RelWithDebInfo/libglui.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/freeglut/Release/libfreeglut_static.a:
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/glui/Release/libglui.a:


# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Debug/libBox2D.dylib:
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Debug/libBox2D.dylib


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/Debug/HelloWorld:\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Debug/libBox2D.a
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/Debug/HelloWorld


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/Debug/Testbed:\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Debug/libBox2D.a\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/freeglut/Debug/libfreeglut_static.a\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/glui/Debug/libglui.a
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/Debug/Testbed


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Release/libBox2D.dylib:
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Release/libBox2D.dylib


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/Release/HelloWorld:\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Release/libBox2D.a
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/Release/HelloWorld


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/Release/Testbed:\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/Release/libBox2D.a\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/freeglut/Release/libfreeglut_static.a\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/glui/Release/libglui.a
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/Release/Testbed


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/MinSizeRel/libBox2D.dylib:
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/MinSizeRel/libBox2D.dylib


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/MinSizeRel/HelloWorld:\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/MinSizeRel/libBox2D.a
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/MinSizeRel/HelloWorld


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/MinSizeRel/Testbed:\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/MinSizeRel/libBox2D.a\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/freeglut/MinSizeRel/libfreeglut_static.a\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/glui/MinSizeRel/libglui.a
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/MinSizeRel/Testbed


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/RelWithDebInfo/libBox2D.dylib:
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/RelWithDebInfo/libBox2D.dylib


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/RelWithDebInfo/HelloWorld:\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/RelWithDebInfo/libBox2D.a
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/HelloWorld/RelWithDebInfo/HelloWorld


/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/RelWithDebInfo/Testbed:\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Box2D/RelWithDebInfo/libBox2D.a\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/freeglut/RelWithDebInfo/libfreeglut_static.a\
	/Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/glui/RelWithDebInfo/libglui.a
	/bin/rm -f /Users/todd/Desktop/Box2D_v2.1.2/Box2D/Build/Testbed/RelWithDebInfo/Testbed


