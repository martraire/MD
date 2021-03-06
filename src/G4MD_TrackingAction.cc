// ********************************************************************
// * G4MD_TrackingAction.cc                                           *
// *                                                                  *
// * Date : 15 July 2015                                              *
// * Author : Diane Martraire (IHEP)                                  *
// *                                                                  *
// * Description :                                                    *
// * This file manage the tracking of particle. We use some tricks to *
// * speed up the simulation (especially on the Cherenkov).           *
// ********************************************************************

#include <G4MD_TrackingAction.hh>
#include <G4TrackingManager.hh>
#include <G4Track.hh>
#include <G4TrackStatus.hh>

#include <Reader.h>

using namespace CLHEP;

//G4int G4MD_TrackingAction::fPrintCout = 0;

G4int G4MD_TrackingAction::fLoseLightEnergy = 0;
G4int G4MD_TrackingAction::fLoseLightQE = 0;

G4int G4MD_TrackingAction::fgNumC = 0;
G4int G4MD_TrackingAction::fgNumCdelta = 0;
G4int G4MD_TrackingAction::fgNbounces = 0;
G4int G4MD_TrackingAction::fLoseLight = 0;
G4int G4MD_TrackingAction::nph = 0;


G4MD_TrackingAction::G4MD_TrackingAction()
{
  bool validate = true;
  
  // Fetch parameters form control file
  // ----------------------------------
  Reader controlReader("xml/control.xml", validate);
  Branch tB = controlReader.GetTopBranch().GetChild("controlParameters");
  
  // Fetch detector parameters from file
  // -----------------------------------
  G4String detectorFile;
  tB.GetChild("detectorFile").GetData(detectorFile);
  Reader reader(detectorFile, validate);  
  Branch branch = reader.GetTopBranch().GetChild("pmtProperty");
  
  G4double min_cerenkov_energy;
  G4double max_cerenkov_energy;
  
  branch.GetChild("min_cerenkov_energy").GetData(min_cerenkov_energy);
  fMinCerenkovEnergy = min_cerenkov_energy*eV;
  
  branch.GetChild("max_cerenkov_energy").GetData(max_cerenkov_energy);
  fMaxCerenkovEnergy = max_cerenkov_energy*eV;
  
  G4double collection_efficiency;
  
  branch.GetChild("collection_efficiency").GetData(collection_efficiency);
  fCollectionEfficiency = collection_efficiency;
  
  vector<G4double> cerenkov_energy;
  vector<G4double> QE;
  
  branch.GetChild("cerenkov_energy").GetData(cerenkov_energy);
  branch.GetChild("QE").GetData(QE);
  
  vector<double>::iterator It;
  for (It  = cerenkov_energy.begin(); It != cerenkov_energy.end(); It++)
    (*It) = (*It)*eV;

  fEnergyVector = cerenkov_energy;
  fQEVector = QE;
}


G4double G4MD_TrackingAction::GetQE(G4double energy)
{
  if (fEnergyVector.size() == 0 || fQEVector.size() == 0)
    {
      G4cout << "Quantum efficiency was not set! Assuming 0 efficiency" << G4endl;  
      return 0.0;
    }
  
  for (G4int i=0; i < fEnergyVector.size() - 1; i++ )
    {  
      if (energy >= fEnergyVector[i] && energy <  fEnergyVector[i+1])
	{
	  G4double slope = (fQEVector[i+1] - fQEVector[i]) / (fEnergyVector[i+1] - fEnergyVector[i]);
	  G4double intercept = fQEVector[i] - fEnergyVector[i] * slope;
	  
	  return slope*energy + intercept;
	} 
    }
  
  return 0.0;
}


G4MD_TrackingAction::~G4MD_TrackingAction()
{;}


void G4MD_TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  G4Track* track;
  G4int code = aTrack->GetDefinition()->GetPDGEncoding();
  
  G4int tmp_trackid =  aTrack->GetTrackID();
  if (code == 0) // optical photon
    {
      G4double energy = aTrack->GetKineticEnergy();
      
      // Kill Cherenkov that fall outside of defined energy range
      // ---------------------------------------------------------
      if (energy < fMinCerenkovEnergy || energy > fMaxCerenkovEnergy)	
	{ 
	  track = const_cast<G4Track*> (aTrack);
	  track->SetTrackStatus( fStopAndKill );
	  
	  // G4cout << "StopAndKill due to out of energy range Cherenkov trackid "
	  //        << code,tmp_trackid << G4endl;
	  fLoseLightEnergy++;
	}
    
      // Randomly kill photons according to quantum & collection efficiency to speed up simu
      // -----------------------------------------------------------------------------------
      G4double ran = RandFlat::shoot(0.0,1.0);
      
      if ( ran > GetQE(energy)*fCollectionEfficiency )
	//if ( ran > GetQE(energy)*fCollectionEfficiency*0.01)
	{             
	  track = const_cast<G4Track*> (aTrack);
	  track->SetTrackStatus( fStopAndKill );
	  fLoseLightQE++;
	}
    }
  
  BeforeTracking(aTrack);
}


void G4MD_TrackingAction::BeforeTracking(const G4Track* aTrack)
{
  fgNbounces = 0;
  fLoseLight = 0;
  
  G4int code = aTrack->GetDefinition()->GetPDGEncoding();
  G4int tmp_trackid = aTrack->GetTrackID();
  G4int tmp_parentid = aTrack->GetParentID();
  
  if (code == 0) // optical photon
    {
      G4int parent = aTrack->GetParentID();  
      
      // Count number of Cherenkov photons
      // ---------------------------------
      if (parent != 1)  // Produced by delta-ray (or decay electron)
	{
	  // G4cout << " fgNumCdelta = " << fgNumCdelta << G4endl;
	  fgNumCdelta++;
	    
	  // Kill delta-ray photon
	  //track = const_cast<G4Track*> (aTrack);
	  //track->SetTrackStatus(fStopAndKill);
	}

	else // Produced by injected particle
	{
	  // G4cout << "Cherenkov from mother = " << fgNumC << G4endl;
	  fgNumC++;
	}
    }
  // G4cout << "Leaving beforetracking" << G4endl;
}


void G4MD_TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  G4int code = aTrack->GetDefinition()->GetPDGEncoding();
  G4int tmp_trackid = aTrack->GetTrackID();
  G4int tmp_parentid = aTrack->GetParentID(); 
  // G4cout << "Aftertracking" << G4endl;
}
