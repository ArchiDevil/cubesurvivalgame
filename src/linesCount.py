import os

def getLines(currentLines, path):
	list = os.listdir(path)
	readLines = 0
	for elem in list:
		if os.path.isdir(os.path.join(path, elem)):
			readLines += getLines(readLines, os.path.join(path, elem))
		else:
			if ".cpp" in elem or ".h" in elem or ".txt" in elem or ".hpp" in elem:
				file = open(os.path.join(path, elem), "r")
				lines = file.readlines()
				readLines += len(lines)
				file.close()
	return readLines

print "Lines count: {0}".format(getLines(0, os.getcwd()))