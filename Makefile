# generic Makefile
#The MIT License (MIT)

#Copyright (c) 2018 Eric Bachard

#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:

#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.

UNAME_S = $(shell uname -s)

ifeq (${UNAME_S}, Linux)

CXX = g++
CXX_STANDARD = -std=c++11
INCLUDE_DIR = -I./beziercurves/include
SOURCES_DIR = ./beziercurves
BUILD_DIR = .
APPLICATION_NAME = Bezier2D
FILENAME = ${BUILD_DIR}/${APPLICATION_NAME}
CXX_FLAGS = -DLinux -Wall -O3  ${CXX_STANDARD}

ILU_CXXFLAGS = `pkg-config --cflags --libs IL ILU ILUT`

LDFLAGS = -ldl -lGL -lGLEW -lGLU -lglfw ${ILU_CXXFLAGS}
DEBUG_SUFFIX = _debug
CXX_FLAGS_DEBUG = -g -DDEBUG
OBJS = ${SOURCES_DIR}/*.cpp

first : clean ${FILENAME}

all : clean ${FILENAME} ${FILENAME}${DEBUG_SUFFIX}

${FILENAME}: ${OBJS}
	${CXX} ${INCLUDE_DIR} ${CXX_FLAGS}  -o $@ $^ ${LDFLAGS}

${FILENAME}${DEBUG_SUFFIX}: ${OBJS}
	${CXX} ${INCLUDE_DIR} ${CXX_FLAGS} ${CXX_FLAGS_DEBUG} -o $@ $^ ${LDFLAGS}

clean:
	${RM} *.o ${FILENAME} ${FILENAME}${DEBUG_SUFFIX}

endif
