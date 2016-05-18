# MSW - My Scons Wrapper
#
# @author Victor Garcia <vichor@gmail.com>
# @date 2011-05-29

# Imports from standard python library
import os, fnmatch, shutil, sys, re

# Functions

def getPackageName():
    '''
    Returns the name for the package to be built extracted from the name of
    the directory.
    '''
    return os.path.basename(os.getcwd())

def getPackages():
    '''
    Get the list of all the packages
    '''
    return _mswGetDirectories()
    
def getModules():
    '''
    Get the list of all the modules found in the package.
    '''
    return [ "." ] + _mswGetDirectories()

def getSources(avoid = None):
    '''
    Get the list of all the source files found in current directory applying
    recursive search in subdirectories but avoiding subdirectory "avoid" 
    '''
    # Ensure the parameter starts with ./
    if (not re.match("./", avoid)):
        avoid = "./" + avoid
    # Find files to avoid
    _avoid = []
    for _f in _mswLocateFiles("*.cpp",avoid):
        _avoid.append(_f)

    _filelist = []
    for _f in _mswLocateFiles("*.cpp","."):
        if ( (_avoid != None) and (_avoid.count(_f) != 0) ):
            continue
        _filelist.append(_f)
    return _filelist


def getHeadersDeployInfo():
    '''
    Get the information for header files deployment. This information is
    contained in a dictionary of the form
            <deployment location> : [list of files]
    For example:
            os/thread : [ CondThread.h, MutexThread.h, Thread.h ]
    '''
    _result = dict()
    for _h in _mswLocateFiles("*.h","."):
        _f = os.path.basename(_h)
        _d = os.path.dirname(_h)
        if _d not in _result:
            _result[_d] = [_f]
        else:
            _result[_d].append(_f)
    return _result


# Local Functions

def _mswGetDirectories(root=os.curdir):
    '''
    Gets a list of all directories found in the given one
    This is not done recursively. 
    '''
    _dirs = [ ]
    for _d in os.listdir(root):
        if os.path.isdir(_d) == True:
            _dirs.append(_d)
    return _dirs

def _mswLocateFiles(pattern, root=os.curdir):
    '''
    Locate all files matching supplied filename pattern in and below
    supplied root directory. 
    Yields a list so it can be used as an iterator.
    '''

    #for path, dirs, files in os.walk(os.path.abspath(root)):
    for path, dirs, files in os.walk(root):
        for filename in fnmatch.filter(files, pattern):
            yield os.path.join(path, filename)

