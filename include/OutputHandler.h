// ***************************************************************************
// * OutputHandler.h                                                         *
// *                                                                         *
// * Date : 15 July 2015                                                     * 
// * Author : D. Martraire                                                   * 
// *                                                                         *
// * Description :                                                           *
// * This file declare the element of the output file. This permit a better  *
// * readability.                                                            *
// ***************************************************************************

#ifndef _OutputHandler_
#define _OutputHandler_

#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <TTree.h>

#include <MDEvent.hh>

class OutputHandler
{
public:
    static OutputHandler* GetInstance();
    
    ~OutputHandler();
    
    void OpenFile(char*);
    void CloseFile();
    TFile* GetFile() { return fOutFile; }
    
    TNtuple* GetEventNtuple();
    TNtuple* GetEinNtuple();
    TNtuple* GetEoutNtuple();
    TNtuple* GetCerenkovNtuple();
    TTree* GetMdtree();
    MDEvent* GetMdevent();
    
private:
    OutputHandler();

    static OutputHandler* fgOutputHandler;
    
    TFile* fOutFile;

    // Number of pe
    TNtuple* f_EventNtuple;
    TNtuple* f_EinNtuple;
    TNtuple* f_EoutNtuple;
    TNtuple* f_CerenkovNtuple;
    TTree* mdtree; 
    MDEvent* mdevent;
};

#endif
