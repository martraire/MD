// **********************************************************************
// * G4MD_EventAction.cc                                                *
// *                                                                    *
// * Date : 15 June 2015                                                *
// * Author : Diane Martraire (IHEP)                                    *
// *                                                                    *
// * Description :                                                      *
// * This file manage every aspects linked to the event (print info, ..)*
// **********************************************************************


#include <G4MD_EventAction.hh>
#include <G4MD_RunAction.hh> 
#include <G4MD_TrackingAction.hh>
#include <G4MD_SteppingAction.hh>
#include <G4MD_PrimaryGeneratorAction.hh>

#include <G4Event.hh>
#include <G4Timer.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>

#include <TRandom.h>
#include <TNtuple.h>
#include <OutputHandler.h>

#include <cmath>
#include <Randomize.hh>
#include <iomanip> 

int G4MD_EventAction::eventid = 0;
int G4MD_EventAction::fgPrimaryID = 0;


G4MD_EventAction::G4MD_EventAction()
{
  fTimer = new G4Timer();
}


G4MD_EventAction::~G4MD_EventAction()
{
  delete fTimer;
}


void G4MD_EventAction::BeginOfEventAction(const G4Event* evt)
{  
  fgPrimaryID = evt->GetPrimaryVertex()->GetPrimary()->GetPDGcode();
  fTimer->Start();
  
  G4int fEventID = evt->GetEventID();
  G4int f1 = evt->GetPrimaryVertex(fEventID)->GetNumberOfParticle();
  G4int f2 = f1-1;
  
  G4double fT = evt->GetPrimaryVertex(fEventID)->GetT0();
  G4int fParticleID = evt->GetPrimaryVertex(fEventID)->GetPrimary(f2)->GetPDGcode();
  fgPrimaryID = fParticleID;
  
  G4double fPx = evt->GetPrimaryVertex(fEventID)->GetPrimary(f2)->GetPx();
  G4double fPy = evt->GetPrimaryVertex(fEventID)->GetPrimary(f2)->GetPy();
  G4double fPz = evt->GetPrimaryVertex(fEventID)->GetPrimary(f2)->GetPz();
  G4double fNx = fPx / sqrt(fPx*fPx + fPy*fPy + fPz*fPz);
  G4double fNy = fPy / sqrt(fPx*fPx + fPy*fPy + fPz*fPz);
  G4double fNz = fPz / sqrt(fPx*fPx + fPy*fPy + fPz*fPz);
  
  G4cout << "\n---> Begin of event: " << fEventID << G4endl;
}
  

void G4MD_EventAction::EndOfEventAction(const G4Event* evt)
{
  G4int fEventID = evt->GetEventID();
  G4int f1 = evt->GetPrimaryVertex(fEventID)->GetNumberOfParticle();
  G4int f2 = f1-1;
  G4double fX0 = evt->GetPrimaryVertex(fEventID)->GetX0();
  G4double fY0 = evt->GetPrimaryVertex(fEventID)->GetY0();
  G4double fZ0 = evt->GetPrimaryVertex(fEventID)->GetZ0();
  G4double fT = evt->GetPrimaryVertex(fEventID)->GetT0();
  G4int fParticleID = evt->GetPrimaryVertex(fEventID)->GetPrimary(f2)->GetPDGcode();
  
  G4double fPx = evt->GetPrimaryVertex(fEventID)->GetPrimary(f2)->GetPx();
  G4double fPy = evt->GetPrimaryVertex(fEventID)->GetPrimary(f2)->GetPy();
  G4double fPz = evt->GetPrimaryVertex(fEventID)->GetPrimary(f2)->GetPz();
  G4double fPtot = sqrt(fPx*fPx + fPy*fPy + fPz*fPz);
  G4double fNx = fPx / fPtot;
  G4double fNy = fPy / fPtot;
  G4double fNz = fPz / fPtot;

  G4double pi = 3.1415926;
  G4double theta = acos(fNz * -1.)*180. / pi;
  G4double phi = atan(fNy/fNx)*180. / pi; 
  G4float Var[12];

  OutputHandler* output = OutputHandler::GetInstance();
  MDEvent *mdevent = output->GetMdevent();
  mdevent->SetPrimary(fParticleID, theta, phi, fX0, fY0, fPtot);
  TTree *mdtree = output->GetMdtree();
  mdtree->Fill();
  mdevent->Clear();

  TNtuple* nt = output->GetEventNtuple();
  Var[0] = fParticleID;
  Var[1] = fPtot;
  Var[2] = theta;
  Var[3] = phi;
  Var[4] = fX0 ;
  Var[5] = fY0 ;
  Var[6] = fZ0 ;
  Var[7] = G4MD_TrackingAction::fgNumC;
  Var[8] = G4MD_TrackingAction::fgNumCdelta;
  Var[9] = G4MD_TrackingAction::fLoseLightQE;
  Var[10] = G4MD_TrackingAction::fLoseLight;
  Var[11] = G4MD_TrackingAction::nph;
  Var[12] = eventid;
  nt->Fill(Var);

  fTimer->Stop();

  eventid++;
  printf("%d %d %d\n",99,99,99); 
  printf("%d %d %f %f %f\n",eventid, G4MD_TrackingAction::nph, fX0, fY0, sqrt(fX0*fX0+fY0*fY0)); 
    
  G4MD_TrackingAction::fgNumC = 0;
  G4MD_TrackingAction::fgNumCdelta = 0;
  G4MD_TrackingAction::fgNbounces = 0;
  G4MD_TrackingAction::fLoseLight = 0;
  G4MD_TrackingAction::fLoseLightQE = 0;
  G4MD_TrackingAction::fLoseLightEnergy = 0;
  G4MD_TrackingAction::nph = 0;

}
  


