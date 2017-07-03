// **********************************************************************
// * G4MD_SteppingAction.cc                                             *
// *                                                                    *
// * Date : 15 June 2015                                                *
// * Author : Diane Martraire (IHEP)                                    *
// *                                                                    *
// * Description :                                                      *
// * This file manage the parameters inside a step of a run.            *
// **********************************************************************


#include <G4MD_SteppingAction.hh>
#include <G4MD_TrackingAction.hh>
#include <G4MD_EventAction.hh>
#include <G4MD_RunAction.hh>
#include <G4StepPoint.hh>
#include <G4VPhysicalVolume.hh>
#include <G4TrackStatus.hh>

#include <G4TouchableHandle.hh>
#include <G4Step.hh>
#include <G4ios.hh>

#include <G4MD_PrimaryGeneratorAction.hh>
#include <OutputHandler.h>
#include <TNtuple.h>

G4MD_SteppingAction::G4MD_SteppingAction()
{;}


G4MD_SteppingAction::~G4MD_SteppingAction()
{;}


void G4MD_SteppingAction::UserSteppingAction(const G4Step* theStep)
{
  // ________________________________________________________________
  // 
  // N.B : Read the information of the track ending of stepping
  // Useful information for checking, 
  // Primary particle with tmp_trackparentid=0 & tmp_trackid=1
  
  // Different status of tracking :
  // ------------------------------
  // tmp_status = 0  -> Alive
  // tmp_status = 1  -> StopButAlive
  // tmp_status = 2  -> StopAndKill
  // tmp_status = 3  -> KillTrackAndSecondaries
  // tmp_status = 4  -> Suspend
  // tmp_status = 5  -> PostponeToNextEvent
  // ________________________________________________________________
  
  
  G4int PrintCout = 1;
  const G4VProcess* theProcess = theStep->GetPostStepPoint()->GetProcessDefinedStep();
  
  G4int tmp_trackid = theStep->GetTrack()->GetTrackID();
  G4int tmp_trackparentid = theStep->GetTrack()->GetParentID();
  G4int code = theStep->GetTrack()->GetDefinition()->GetPDGEncoding();
  G4int tmp_status = theStep->GetTrack()->GetTrackStatus();
  G4double tmp_globaltime = theStep->GetTrack()->GetGlobalTime();
  G4double tmp_kineticenergy = theStep->GetPreStepPoint()->GetKineticEnergy();
  G4double tmp_energy = theStep->GetPreStepPoint()->GetTotalEnergy();
  G4int primaryid = G4MD_PrimaryGeneratorAction::fParticleID;

  G4Track* theTrack = theStep->GetTrack();
  G4String particleType = theTrack->GetDefinition()->GetParticleType();

  
  if (particleType == "nucleus") theTrack->SetTrackStatus(fStopAndKill);
  

  if (theProcess!=0)
    {     
      // Do not track neutrinos (12: nu_e / 14: nu_mu / 16: nu_tau)
      // ----------------------------------------------------------
      if (code == 12 || code == 14 || code == 16 || 
	  code == -12 || code == -14 || code == -16)
	{
	  G4Track* theTrack = theStep->GetTrack();
	  theTrack->SetTrackStatus(fStopAndKill);
	}
      
      
      G4ThreeVector outposition = theStep->GetPostStepPoint()->GetPosition();
      G4double x0 = outposition.x(); 
      G4double y0 = outposition.y(); 
      G4double z0 = outposition.z(); 
      G4ThreeVector direction = theStep->GetPostStepPoint()->GetMomentumDirection();
      G4double dx0 = direction.x();
      G4double dy0 = direction.y();
      G4double dz0 = direction.z();
      
      if (theStep->GetTrack()->GetNextVolume() != 0) 
	{
	  if(PrintCout == 1) 
	    G4cout << "--->  Douane :: PreStepVol = " <<  theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() << " & PostStepPoint = " << theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() << G4endl;
	  
	  
	  // Initial conditions before entering in the tank
	  // ----------------------------------------------
	  if((theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()=="top_tyvek"  ||
	      theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()=="side_tyvek" ||
	      theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()=="bottom_tyvek")
	     && (theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="SoilVol" ||
		 theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="top" ||
		 theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="side" ||
		 theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="bottom")
	     && code!=0)
	    {
	      if(PrintCout == 1) 
		{
		  G4cout << "Enter into tank : PreStepPoint = " << theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() << " and PostStepPoint = " << theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() << G4endl;
		  G4cout << "Enter into tank with E = " << tmp_energy << " and position = " << x0 << " , " << y0 << " , " << z0 << ". Particle code = " << code << ", ParentID = " << tmp_trackparentid << G4endl;
		}
	      
	      OutputHandler* output = OutputHandler::GetInstance();
	      MDEvent *mdevent = output->GetMdevent();
	      mdevent->AddPunch(code, x0, y0, z0, dx0, dy0, dz0, tmp_energy);
	      
	      TNtuple* einntuple = output->GetEinNtuple();
	      float par[9];
	      par[0] = (G4float) code*1.;
	      par[1] = (G4float) tmp_trackparentid*1.;
	      par[2] = (G4float) tmp_energy;
	      par[3] = (G4float) x0;
	      par[4] = (G4float) y0;
	      par[5] = (G4float) z0;
	      par[6] = (G4float) dx0;
	      par[7] = (G4float) dy0;
	      par[8] = (G4float) dz0;
	      par[9] = (G4float) G4MD_EventAction::eventid*1.;
	      einntuple->Fill(par);
	    }
	}
      
      // Final conditions before leaving the tank
      // ----------------------------------------
      if ((theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "top_tyvek" ||
	   theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "side_tyvek"||
	   theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "bottom_tyvek")
	  && (theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "top"   ||
	      theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "side"  ||
	      theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "bottom"||
	      theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "SoilVol"))
	{
	  OutputHandler* output = OutputHandler::GetInstance();
	  TNtuple* eoutntuple = output->GetEoutNtuple();
	  float parout[9];
	  parout[0] = (G4float) code * 1.;
	  parout[1] = (G4float) tmp_trackparentid * 1.;
	  parout[2] = (G4float) tmp_energy;
	  parout[3] = (G4float) x0;
	  parout[4] = (G4float) y0;
	  parout[5] = (G4float) z0;
	  parout[6] = (G4float) dx0;
	  parout[7] = (G4float) dy0;
	  parout[8] = (G4float) dz0;
	  parout[9] = (G4float) tmp_status*1.;
	  parout[10] = (G4float) G4MD_EventAction::eventid * 1.;
	  
	  eoutntuple->Fill(parout);
	  
	  if(PrintCout == 1) 
	    {
	      G4cout << " Out of tank with E = " << tmp_energy << ". Position = " << x0 << " , " << y0 << " , " << z0 << ". PArticle code = " << code << ", ParentID = " << tmp_trackparentid << G4endl;
	    }
	}
      
      
      // Follow Optical photons (code = 0)
      // ---------------------------------
      if (code == 0) 
	{
	  // Records absorption light (absorbed in water/tyvek -> Status2: StopAndKill)
	  // ------------------------
	  // Photons absorbed in the water or Tyvek (
	  if (tmp_status == 2 && 
	      theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() != "pmt")
	    G4MD_TrackingAction::fLoseLight++;
	  
	  // Records number of interaction with photocathode 
	  // -----------------------------------------------
	  if (theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "pmt" &&
	      theStep->GetPreStepPoint()->GetMaterial()->GetName() == "Pyrex")
	    {
	      G4MD_TrackingAction::nph++;
	      
	      if(PrintCout == 1) 
		G4cout << "Interact with PMT, nphoton = " << G4MD_TrackingAction::nph << G4endl;
	      
	      G4Track* theTrack = theStep->GetTrack();
	      theTrack->SetTrackStatus(fStopAndKill);
	    }
	  
	  // Record the number of bounces did by photons on the sides of the tank
	  // ---------------------------------------------------------------------
	  if (theStep->GetTrack()->GetNextVolume() != 0)
	    {
	      if (theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "top_tyvek" )
		G4MD_TrackingAction::fgNbounces++;
	      
	      if(theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "bottom_tyvek")
		G4MD_TrackingAction::fgNbounces++;
	      
	      if (theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "side_tyvek")
		G4MD_TrackingAction::fgNbounces++;
	    }
	}
      
    }
}

