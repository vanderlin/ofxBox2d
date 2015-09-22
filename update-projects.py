# this script copies the project files in empty-example to all the other examples

import glob, re, shutil, fileinput, os
from os.path import join

def copy(src, dst):
	if os.path.isfile(src):
		shutil.copy(src, dst)

def toUpper(pattern):
	return pattern.group(1).upper()
	
def replaceInFile(filename, pattern, replacement):
	if os.path.isfile(filename):
		for line in fileinput.FileInput(filename, inplace=1):
			print re.sub(pattern, replacement, line),

sourceProjectName = "EmptyExample"
emptyDir = "example-empty"

# windows code::blocks
sourceCbp = join(emptyDir, "EmptyExample.cbp")
sourceWorkspace = join(emptyDir, "EmptyExample.workspace")

# windows vs2010
sourceSln = join(emptyDir, "EmptyExample.sln")
sourceVcxproj = join(emptyDir, "EmptyExample.vcxproj")
sourceVcxprojFilters = join(emptyDir, "EmptyExample.vcxproj.filters")
sourceVcxprojUser = join(emptyDir, "EmptyExample.vcxproj.user")

# xcode osx
sourceXcconfig = join(emptyDir, "Project.xcconfig")
sourcePlist = join(emptyDir, "openFrameworks-Info.plist")
sourceXcodeproj = join(emptyDir, "ofApp.xcodeproj")

# linux
sourceMakefile = join(emptyDir, "Makefile")
sourceConfigs = join(emptyDir, "config.make")
sourceAddons = join(emptyDir, "addons.make")

examples = glob.glob("example*")
for example in examples:
	if example != emptyDir:
		sansExample = re.sub("^example-", "", example)
		upper = re.sub("^([a-z])", toUpper, sansExample)
		targetProjectName = upper + "Example"
		
		#linux
		targetDir = "{0}/".format(example, targetProjectName)
		copy(sourceMakefile, targetDir)
		copy(sourceConfigs, targetDir)
		copy(sourceAddons, targetDir)

		# windows code::blocks
		targetCbp = "{0}/{1}.cbp".format(example, targetProjectName)
		targetWorkspace = "{0}/{1}.workspace".format(example, targetProjectName)
		copy(sourceCbp, targetCbp)
		copy(sourceWorkspace, targetWorkspace)
		replaceInFile(targetCbp, sourceProjectName, targetProjectName)
		replaceInFile(targetWorkspace, sourceProjectName, targetProjectName)

		# windows vs2010
		targetSln = "{0}/{1}.sln".format(example, targetProjectName)
		targetVcxproj = "{0}/{1}.vcxproj".format(example, targetProjectName)
		targetVcxprojFilters = "{0}/{1}.vcxproj.filters".format(example, targetProjectName)
		targetVcxprojUser = "{0}/{1}.vcxproj.user".format(example, targetProjectName)
		copy(sourceSln, targetSln)
		copy(sourceVcxproj, targetVcxproj)
		copy(sourceVcxprojFilters, targetVcxprojFilters)
		copy(sourceVcxprojUser, targetVcxprojUser)
		replaceInFile(targetSln, sourceProjectName, targetProjectName)
		replaceInFile(targetVcxproj, sourceProjectName, targetProjectName)

		# xcode osx
		targetXcodeproj = "{0}/ofApp.xcodeproj".format(example)
		copy(sourceXcconfig, example)
		copy(sourcePlist, example)
		try: shutil.rmtree(targetXcodeproj)
		except: pass
		try: shutil.copytree(sourceXcodeproj, targetXcodeproj)
		except: pass
		
		print "Copied into " + targetProjectName + "."
