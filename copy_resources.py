import os
import shutil

def get_path():
    return os.path.dirname(os.path.realpath(__file__)) 

def copy():
    print ("copy")
    path = os.path.join(get_path(), "build")
    dirs = ["bin/Debug", "bin/Release", "src/game", "src/MeshEditor"]
    for name in dirs:
        try:
            destination = os.path.join(path, name, "resources")
            source = os.path.join(get_path(), "src/resources")
            shutil.copytree(source, destination)
            
            destination = os.path.join(path, name, "saves")
            source = os.path.join(get_path(), "src/saves")
            shutil.copytree(source, destination)
            
            destination = os.path.join(path, name)
            shutil.copy2("src/settings.ini", os.path.join(destination, "settings.ini"))
        except Exception as e:
            print "copy(): " + str(e[0])
            continue
    
def copy_resources():
    print ("copy_resources")
    clean()
    copy()
    
def clean():
    print ("clean")
    path = os.path.join(get_path(), "build")
    dirs = ["bin/Debug", "bin/Release", "src/game", "src/MeshEditor"]
    for name in dirs:
        try:
            shutil.rmtree(os.path.join(path, name, "resources"), ignore_errors = True)
            shutil.rmtree(os.path.join(path, name, "saves"), ignore_errors = True)
            os.remove(os.path.join(path, name, "settings.ini"))
        except:
            continue

if __name__ == "__main__":
    copy_resources()