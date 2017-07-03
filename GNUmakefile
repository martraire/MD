# --------------------------------------------------------------
# GNUmakefile						       -
#							       -
# Date : 15 June 2015					       -
# Modif : 2 Sept 2015 (Add rootdict: l.20,21 - new lines:23,25 -
# Author : Diane Martraire (IHEP)			       -	
# 							       -
# Description :						       -
# GNUmakefile is the file to compile MDCalib module.           -
# --------------------------------------------------------------

name := MDCalib
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: rootdict all   # Add rootdict 
all: rootdict lib bin  # Add rootdict

include rootdict.gmk  # Add line
include $(G4INSTALL)/config/binmake.gmk
include $(G4INSTALL)/config/architecture.gmk  # Add line

# ROOT support
# CPPFLAGS += -I$(shell root-config --incdir) 
# EXTRALIBS = $(shell root-config --glibs)


visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

