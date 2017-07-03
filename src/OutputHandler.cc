#include <OutputHandler.h>

#include <TNtuple.h>
#include <TFile.h>
#include <TH1.h>

#include <Reader.h>

OutputHandler* OutputHandler::fgOutputHandler = NULL;

OutputHandler* OutputHandler::GetInstance()
{
    if ( fgOutputHandler == NULL )
    {
	fgOutputHandler = new OutputHandler();
	
	return fgOutputHandler;
    }
    
    else 
    {
	return fgOutputHandler;
    }
}

OutputHandler::OutputHandler()
{

printf("-----------TODO: why do we do it here\n");
/*
    // Ntuple storing the number of photoelectrons
    f_EventNtuple = new TNtuple("event","Event Information","pid:energy:theta:phi:x0:y0:z0:normc:deltac:loseqec:lossc:nc:eid");

    f_EinNtuple = new TNtuple("ein","Event inside tank Information","Pid:Did:energy:x0:y0:z0:dx0:dy0:dz0");

    f_EoutNtuple = new TNtuple("eout","Event out tank Information","Pid:Did:energy:x0:y0:z0:dx0:dy0:dz0:status");

    f_CerenkovNtuple = new TNtuple("cntuple","cerenkov Information","time:nbounces");
*/
} 
 
void OutputHandler::OpenFile(char* fileName)
{
  mdevent = new MDEvent();

  fOutFile = new TFile(fileName, "RECREATE", "KM2A Water Cherenkov Detector");
  fOutFile->cd();

  mdtree = new TTree("mdtree", "Muon Detector");
  mdtree->SetMarkerStyle(4);
  mdtree->SetMarkerColor(2);
  mdtree->Branch("Event", "MDEvent", &mdevent, 64000, 2);
  mdtree->SetAutoSave(1500000000);

    f_EventNtuple = new TNtuple("event","Event Information","pid:energy:theta:phi:x0:y0:z0:normc:deltac:loseqec:lossc:nc:eid");

    f_EinNtuple = new TNtuple("ein","Event inside tank Information","Pid:Did:energy:x0:y0:z0:dx0:dy0:dz0:eid");


    f_EoutNtuple = new TNtuple("eout","Event out tank Information","Pid:Did:energy:x0:y0:z0:dx0:dy0:dz0:status:eid");


    f_CerenkovNtuple = new TNtuple("cntuple","cerenkov Information","time:nbounces");
}

void OutputHandler::CloseFile()
{
  fOutFile->Write();
  fOutFile->Close();
}

TNtuple* OutputHandler::GetEventNtuple()
{ return f_EventNtuple; }

TNtuple* OutputHandler::GetEinNtuple()
{ return f_EinNtuple; }

TNtuple* OutputHandler::GetEoutNtuple()
{ return f_EoutNtuple; }

TNtuple* OutputHandler::GetCerenkovNtuple()
{ return f_CerenkovNtuple; }

TTree* OutputHandler::GetMdtree()
{ return mdtree; }

MDEvent* OutputHandler::GetMdevent()
{ return mdevent; }

OutputHandler::~OutputHandler()
{
  delete f_EventNtuple;
  delete f_EinNtuple;
  delete f_EoutNtuple;
  delete f_CerenkovNtuple;
  delete mdtree;
  delete mdevent;
}

