#!/usr/bin/env python3
import os
from os.path import *
import re
import sys



seenFiles = set()
includePattern = re.compile(r'#include[ ]*"([^"]+)"')
currentDir = os.getcwd()

def resolvePath(path):
    global currentDir
    testPath = currentDir + "/" + path
    if exists(testPath):
        return realpath(testPath)
    for p in alternativePaths:
        testPath = p + "/" + path
        if exists(testPath):
            return realpath(testPath)
    print("path not found:", path, file=sys.stderr)
    sys.exit(1)


def readFile(path):
    global currentDir
    path = resolvePath(path)
    if path in seenFiles:
        return ""
    with open(path) as file:
        currentDirBackup = currentDir
        currentDir = dirname(path)
        seenFiles.add(path)
        src = file.read()
        src = re.sub(includePattern,lambda m: readFile(m.group(1)),src)
        currentDir = currentDirBackup
        return src
        


alternativePaths = set(sys.argv[2:])
print(readFile(sys.argv[1]))