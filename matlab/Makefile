MATLABDIR ?= /Applications/MATLAB_R2010b.app
CXX = gcc
CFLAGS = -m64 -Wall -O3 -fPIC -I$(MATLABDIR)/extern/include -I../c_emd
COMMON = ../c_emd/emd.c ../c_emd/emd.h
MAIN = c_emd.c

MEX = $(MATLABDIR)/bin/mex
MEX_OPTION = CC\#$(CXX) CXX\#$(CXX) CFLAGS\#"$(CFLAGS)" CXXFLAGS\#"$(CFLAGS)"
# comment the following line if you use MATLAB on 32-bit computer
MEX_OPTION += -largeArrayDims
MEX_EXT = $(shell $(MATLABDIR)/bin/mexext)

all: matlab

matlab: c_emd.$(MEX_EXT)

c_emd.$(MEX_EXT): $(COMMON) $(MAIN)
	$(MEX) $(MEX_OPTION) $(MAIN)
clean:
	rm -f *~ *.o *$(MEX_EXT) *.obj
