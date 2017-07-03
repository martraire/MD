#ifndef _ROOT_Event
#define _ROOT_Event 1
/////////////////////////////////////////////////////////////////////////
// Event                                                               //
// Description of the event and hit parameters                         //
/////////////////////////////////////////////////////////////////////////
#include <TObject.h>
#include <TClonesArray.h>


// MDEVENT
// -------

class MDEvent : public TObject {

 private:
  Int_t ev_n, id; 
  Int_t ngamma, ne, npe;
  float phi, theta, x, y, energy;
  float epunch;

  Int_t npunch;

  TClonesArray * punch;
  TClonesArray * photon;

 public:
  MDEvent();
  virtual ~MDEvent();

  
  // Set Methods
  void SetPrimary(Int_t s_id, float s_theta, float s_phi, float s_x, float s_y, float s_energy)
  {
    id = s_id;
    theta = s_theta;
    phi = s_phi;
    x = s_x;
    y = s_y;
    energy = s_energy;
  };
 
  
  void SetNpe(Int_t s_npe)       
  { npe = s_npe ; };
  

  TClonesArray * Photon() const  { return photon; };
  TClonesArray * Punch() const  { return punch; };

  // To add a hit ...
  void AddPhoton(UInt_t p_t, UInt_t p_n);
  void AddPunch(Int_t s_id, float s_x, float s_y, float s_z, float s_px, float s_py, float s_pz, float s_energy);
  void Clear()
  {
    punch->Clear() ;  npunch = 0; ngamma = 0; ne = 0; epunch = 0;
    photon->Clear();   npe = 0;
  };

  ClassDef(MDEvent,1) 
    
 };

// PUNCH
// -----

 class MDPunch : public TObject {

 private:
   Int_t id; 
   float x, y, z; 
   float px, py, pz; 
   float energy;

 public:
   MDPunch() {};
   
   MDPunch(Int_t s_id, float s_x, float s_y, float s_z, float s_px, float s_py, float s_pz, float s_energy)
   {
     id = s_id;
     x = s_x;
     y = s_y;
     z = s_z;
     px = s_px;
     py = s_py;
     pz = s_pz;
     energy = s_energy;
   };
   
  virtual ~MDPunch() {} ;
   


 ClassDef(MDPunch,1) 

};


// PHOTON
// ------
class MDPhoton : public TObject {

 private:
   UInt_t t, n;

 public:
  MDPhoton() {};

  MDPhoton(UInt_t p_t, UInt_t p_n)
  {
    t = p_t;
    n = p_n;
  };

  virtual ~MDPhoton() {};


 ClassDef(MDPhoton,1) 

};

#endif
