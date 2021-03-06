// *********************************************************************
// * G4MD_PrimaryGeneratorAction.hh                                    *
// *                                                                   *
// * Date : 15 June 2015                                               *
// * Author : Diane Martraire (IHEP)                                   *
// *                                                                   *
// * Description :                                                     *
// * This file generate permit to define the generator of particles    *
// * which will interact in the MD Detector.                           *
// *********************************************************************


#ifndef G4MD_PrimaryGeneratorAction_h
#define G4MD_PrimaryGeneratorAction_h 1

#include <G4VUserPrimaryGeneratorAction.hh>

#include <G4Event.hh>
#include <G4GeneralParticleSource.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <globals.hh>
#include <G4Point3D.hh>
#include <G4Vector3D.hh>
#include <G4Transform3D.hh>

#include <G4UnitsTable.hh>
#include <G4RunManager.hh>

#include <string>
using namespace std;

class G4ParticleGun;
class G4Event;
class G4MD_PrimaryGeneratorMessenger;

class G4MD_PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  G4MD_PrimaryGeneratorAction();    
  ~G4MD_PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event* anEvent);
  
  static G4int fParticleID;
  static G4double particle_ekin;
  static G4double sample_theta;
  static G4double sample_phi;
  static G4double xin;
  static G4double yin;
  static G4double zin;
  
private:
  G4ParticleGun* fParticleGun;	  
  G4GeneralParticleSource* fParticleGun2;

  void PrepareCores();
  G4MD_PrimaryGeneratorMessenger* gunMessenger;
  
  G4double InjectionAltitude;
  G4double particletime;
  G4double alpha;
  G4ParticleDefinition* particledef;
  
  G4ThreeVector Momentum;
  G4Point3D Position;
  G4Vector3D shift_core; 
  
  string fParticleName;
  
  G4double fParticleX;
  G4double fParticleY;
  G4double fParticleZ;
    
  G4double fParticlePX;
  G4double fParticlePY;
  G4double fParticlePZ;
  
  G4double fParticleEnergy;
  G4double fParticleTime;

  G4double fParticleTheta;
  //G4double fParticlePhi;
  
  
  G4int fMode;

  static G4int isample;
  static G4int nsample;

  G4double tx[5000];
  G4double ty[5000];
  G4double tz[5000];
  
  G4double tpx[5000];
  G4double tpy[5000];
  G4double tpz[5000];
  
  G4double sample_p;
  
  // Tank information;
  G4double fTankRadius;
  G4double fTankHalfHeight;

  G4double fTankPos_x;
  G4double fTankPos_y;
  G4double fTankPos_z;
  
  G4double fTankThickness;

  G4double sample_radius;
  G4double sample_halfheight;
};


#endif
