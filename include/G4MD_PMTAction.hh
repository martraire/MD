#ifndef G4MD_PMTAction_h
#define G4MD_PMTAction_h 1

#include <list.h>

#include <G4VSensitiveDetector.hh>
#include <G4MD_DetectorConstruction.hh>
#include <TNtuple.h>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

struct Hit
{
  G4int particle;
  
  G4double energy;
  G4double time;
  G4double x0;
  G4double y0;
  G4double z0;
  G4double cx0;
  G4double cy0;
  G4double cz0;
  
  G4int bounces;
  G4int xid;
  G4int yid;
};

class G4MD_PMTAction : public G4VSensitiveDetector
{
public:
    G4MD_PMTAction(G4String name, G4int pmtindex);
    virtual ~G4MD_PMTAction();

    void Initialize(G4HCofThisEvent* HCE);
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
    void EndOfEvent(G4HCofThisEvent* HCE);
      
private:
    G4MD_DetectorConstruction* fDetector;

    list<struct Hit> fHitList;  
    G4int fPMTIndex;
};

#endif





