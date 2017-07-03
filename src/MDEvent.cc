#include <MDEvent.hh>

ClassImp(MDEvent)
ClassImp(MDPunch)
ClassImp(MDPhoton)

MDEvent::MDEvent()
{
  npe  =  0;
  ngamma = 0;
  ne = 0;
  epunch = 0;
  npunch = 0;
  punch = new TClonesArray("MDPunch", 100);
  npe = 0;
  photon = new TClonesArray("MDPhoton", 100);
}

MDEvent::~MDEvent()  {
  punch->Clear(); 
  delete  punch;
  photon->Clear();
  delete photon;
}

void MDEvent::AddPunch(Int_t s_id, float s_x, float s_y, float s_z, float s_px, float s_py, float s_pz, float s_energy)
{
  epunch += s_energy;
  new((*punch)[npunch++]) MDPunch(s_id, s_x, s_y, s_z, s_px, s_py, s_pz, s_energy);
  switch(s_id)
    {
    case 22:
      ngamma++;
      break;
    case -11:
    case 11:
      ne++;
      break;
    }
}

void MDEvent::AddPhoton(UInt_t p_t, UInt_t p_n)
{
  new((*photon)[npe++]) MDPhoton(p_t, p_n);
}

