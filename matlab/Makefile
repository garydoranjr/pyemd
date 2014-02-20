# This Makefile is used under Linux

MATLABDIR ?= /Applications/MATLAB_R2010b.app
#CXX = gcc-4.0
CXX = g++
#CXX = g++-4.1
CFLAGS = -m64 -Wall -O3 -fPIC -I$(MATLABDIR)/extern/include -I../c_emd
COMMON = ../c_emd/emd.c ../c_emd/emd.h

MEX = $(MATLABDIR)/bin/mex
MEX_OPTION = CC\#$(CXX) CXX\#$(CXX) CFLAGS\#"$(CFLAGS)" CXXFLAGS\#"$(CFLAGS)"
# comment the following line if you use MATLAB on 32-bit computer
MEX_OPTION += -largeArrayDims
MEX_EXT = $(shell $(MATLABDIR)/bin/mexext)

OCTAVEDIR ?= /usr/include/octave
OCTAVE_MEX = env CC=$(CXX) mkoctfile
OCTAVE_MEX_OPTION = --mex
OCTAVE_MEX_EXT = mex
OCTAVE_CFLAGS = -Wall -O3 -fPIC -I$(OCTAVEDIR)

all:	matlab

matlab:	binary

binary: c_emd.$(MEX_EXT)

c_emd.$(MEX_EXT): $(COMMON)
	$(MEX) $(MEX_OPTION) c_emd.c
clean:
	rm -f *~ *.o *$(MEX_EXT) *.obj
