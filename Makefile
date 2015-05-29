# Makefile for the ROOT test programs.
# This Makefile shows nicely how to compile and link applications
# using the ROOT libraries on all supported platforms.
#
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

include Makefile.arch

#------------------------------------------------------------------------------

IDO       = main_digitization.$(ObjSuf) DetectorPadGeometry.$(ObjSuf) MyDict.$(ObjSuf)
IDS       = main_digitization.$(SrcSuf) DetectorPadGeometry.$(SrcSuf) MyDict.$(SrcSuf)
IDX        = Digit$(ExeSuf)

HEADERS		= DetectorPadGeometry.h 
OBJS          = $(IDO) 

PROGRAMS      = $(IDX)


#------------------------------------------------------------------------------
LDFLAGS += -L/user/perezlou/geant4_workdir/ProtonDetector_build/
LIBS    += -lProton
#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

$(IDX):      $(IDO)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		@echo "$@ done"

clean:
		@rm -f $(OBJS)  $(PROGRAMS) $(EVENTSO) *~  core

.SUFFIXES: .$(SrcSuf)


MyDict.cxx: $(HEADERS) LinkDef.h
	rootcint -f $@ -c $(CXXFLAGS) -I./ -p $^
###

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
