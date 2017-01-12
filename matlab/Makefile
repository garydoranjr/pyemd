MATLABDIR ?= /Applications/MATLAB_R2014b.app
CXX = gcc
CFLAGS = -m64 -Wall -O3 -fPIC -I$(MATLABDIR)/extern/include -I../c_emd
COMMON = ../c_emd/emd.c
HEADER = ../c_emd/emd.h
MAIN = c_emd.c

MEX = $(MATLABDIR)/bin/mex
MEX_OPTION = CFLAGS="$(CFLAGS)" CC=$(CXX)
# comment the following for 32-bit systems
MEX_OPTION += -largeArrayDims
MEX_EXT = $(shell $(MATLABDIR)/bin/mexext)

all: matlab

matlab: c_emd.$(MEX_EXT)

c_emd.$(MEX_EXT): c_emd.o
	$(CXX) -Wl,-twolevel_namespace -undefined error -arch x86_64 -bundle \
		-Wl,-exported_symbols_list,"$(MATLABDIR)/extern/lib/maci64/mexFunction.map" \
		c_emd.o emd.o -O -L"$(MATLABDIR)/bin/maci64" \
		-lmx -lmex -lmat -lc++ -o c_emd.$(MEX_EXT)

c_emd.o: $(COMMON) $(HEADER) $(MAIN)
	$(CXX) $(CFLAGS) -c -DMX_COMPAT_32 -DMATLAB_MEX_FILE -I"$(MATLABDIR)/extern/include" \
		-I"$(MATLABDIR)/simulink/include"  -fno-common -arch x86_64 -fexceptions \
		-O2 -fwrapv -flto $(COMMON) $(MAIN)

clean:
	rm -f *~ *.o *$(MEX_EXT) *.obj
