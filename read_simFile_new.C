#include "TSystem.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TRandom.h"
#include "TClonesArray.h"
#include "DetectorPadGeometry.C"
#include <iostream>
#include <iomanip>
#include "TGraph.h"
#include "TPad.h"
#include <ctime>
//#include "/user/perezlou/geant4_workdir/ProtonDetector/include/ProtonDetectorBeamInfo.hh"
//#include "/user/perezlou/geant4_workdir/ProtonDetector/include/ProtonDetectorSimpleTrack.hh"
using namespace std;
//DetectorPadGeometry *CreateGeometry(Char_t *);
//Float_t Polya(Float_t param=3.2);
//void loadbar(unsigned int k, unsigned int n, unsigned int w = 72);

void read_simFile_new(){

  Int_t start_time=time(NULL);
  TCanvas *c=new TCanvas();
  //c->Divide(2,1);
     
  Double_t PI=3.1415926535897932384626433;
  Double_t rad=180./PI;
  Double_t Y_length=60; //mm
  Double_t W= 26.; //eV P-10
  Double_t v_drift = 0.04165; // mm/ns P-5 800 torr
  Double_t D_trans = 10.58e-4; //mm**2/ns P-5 800 torr 
  Double_t D_long = 4.411e-4; //mm**2/ns P-5 800 torr 
  Double_t sigma_trans;
  Double_t sigma_long;
  gSystem->Load("~perezlou/geant4_workdir/ProtonDetector_build/libProton.sl");
  //gROOT->ProcessLine(".L DetectorPadGeometry.C+");
  TFile *fil=new TFile("simFile.root");
  TFile *out_file=new TFile("cona.root","recreate");
  TTree *T=(TTree*)fil->Get("The_Event_Tree");
  TClonesArray *simpleTrackCA=new TClonesArray("ProtonDetectorSimpleTrack",10);
  TBranch *branchTrack=T->GetBranch("simpleTrackData");
  branchTrack->SetAddress(&simpleTrackCA);
  branchTrack->SetAutoDelete(kTRUE);
  TBranch *branchData=T->GetBranch("beamInfo");
  ProtonDetectorBeamInfo *beamInfo=new ProtonDetectorBeamInfo;
  branchData->SetAddress(&beamInfo);
  ProtonDetectorSimpleTrack *localTrack=new ProtonDetectorSimpleTrack;
  Int_t nevents= T->GetEntries();
  //Int_t nevents=10000;
  Int_t nb=0;
  Int_t nentries;
  Int_t total_electrons=0;

  DetectorPadGeometry *theGeometry=CreateGeometry("All");
  DetectorPadGeometry *theGeometry_protons=CreateGeometry("protons");
  DetectorPadGeometry *theGeometry_betas=CreateGeometry("betas");
  theGeometry->SetMinimum(1);

  //Added Energy Resolutions
  Float_t ResolutionFactor=90.;
  Float_t FWHMFactor= 2.35482;
  // Add Resolution (Lolly's estimate) 90/sqrt(Ep(kev))%

  Double_t energy_total;
  Double_t Energy_pad[100];
  Double_t Energy_protons[100];
  Double_t Energy_betas[100];
  Int_t Charge;
  Int_t padNumber;
  Int_t nbins=theGeometry->GetNumberOfBins();
  TTree *outTree =new TTree("theTree","Output Tree");

  outTree->Branch("nbins",&nbins,"nbins/I");
  outTree->Branch("Epad",Energy_pad,"Ep[nbins]/D");
  outTree->Branch("Eprotons",Energy_protons,"Ep[nbins]/D");
  outTree->Branch("Ebetas",Energy_betas,"Eb[nbins]/D");
  outTree->Branch("Etot",&energy_total,"Et/D");
  outTree->Branch("Pad",&padNumber,"pad/I");
  outTree->Branch("Charge",&Charge,"ch/I");

  TGraph *g_beam=new TGraph();
  TGraph *g_betas=new TGraph();
  TGraph *g_protons=new TGraph();
  g_beam->SetMarkerStyle(20);
  g_betas->SetMarkerStyle(20);
  g_protons->SetMarkerStyle(20);
  
  
  g_betas->SetMarkerColor(7);
  g_protons->SetMarkerColor(3);
  g_beam->SetMarkerColor(6);
  
  g_betas->SetLineColor(7);
  g_protons->SetLineColor(3);
  g_beam->SetLineColor(6);
  
  g_betas->SetLineWidth(2);
  g_protons->SetLineWidth(3);
  g_beam->SetLineWidth(4);


  cout<<nevents<<" Events in the ROOT file"<<endl;
  for(Int_t i=0;i<nevents;i++){ //Event Loop	
    
    loadbar(i,nevents-1);
    Int_t n_points_beam=0;
    Int_t n_points_betas=0;
    Int_t n_points_protons=0;
    simpleTrackCA->Clear();
    theGeometry->Reset("");
    theGeometry_protons->Reset("");
    theGeometry_betas->Reset("");
    g_beam->Set(0);
    g_betas->Set(0);
    g_protons->Set(0);
    for(Int_t k=0;k<nbins;k++){
      Energy_pad[k]=0;
      Energy_protons[k]=0;
      Energy_betas[k]=0;
    }
    nb+=T->GetEntry(i);
    
    Int_t nentries=simpleTrackCA->GetEntriesFast();
    //Loop on the Clones Array
    //cout<<nentries<<" strides in the event"<<endl;
    for(Int_t j=0;j<nentries;j++){
      
      localTrack=(ProtonDetectorSimpleTrack*)simpleTrackCA->At(j);
      //if(localTrack->GetZPre()<-128 ||localTrack->GetZPre()>128 )continue;
      Int_t charge=localTrack->GetParticleCharge();
      Int_t Parent_TID=localTrack->GetParentTrackID();
      Int_t PID=localTrack->GetParticleID();
      Charge=charge;
      Double_t energy=1000*localTrack->GetEnergyStride(); //Energy deposited in stride in keV
      //cout<<j<<"Energy  "<<energy<<" KeV "<<energy/(W*1e-3)<<endl;
       energy_total+=energy;
      Double_t x_position=(localTrack->GetXPre()+localTrack->GetXPost())/2.;
      Double_t y_position=(localTrack->GetYPre()+localTrack->GetYPost())/2.;
      Double_t z_position=(localTrack->GetZPre()+localTrack->GetZPost())/2.;
      Double_t real_X_pos=localTrack->GetXPre();
      Double_t real_Z_pos=localTrack->GetZPre();

       if(PID==2212){
	g_protons->SetPoint(n_points_protons,real_Z_pos,real_X_pos);
	n_points_protons++;	  
      }
       else if(PID==-11&&Parent_TID==1){
	g_betas->SetPoint(n_points_betas,real_Z_pos,real_X_pos);
	n_points_betas++;
      }
      else{
     g_beam->SetPoint(n_points_beam,real_Z_pos,real_X_pos);
      n_points_beam++;
      }
      Double_t drift_distance=y_position+Y_length;
      sigma_trans=sqrt(drift_distance*2*D_trans/v_drift);
      sigma_long=sqrt(drift_distance*2*D_long/v_drift);
      Double_t drift_time=drift_distance/v_drift; //(ns)
      //Number of electrons per stride
      Int_t nelectrons=energy/(W*1e-3);
      Int_t realelectrons=gRandom->Poisson(nelectrons);
      //cout<<j<<" "<<nelectrons<<" "<<realelectrons<<" electrons produced in the step"<<endl;
      if(nelectrons==0)continue;
      total_electrons+=realelectrons;
      //Loop on the electrons
      for(Int_t h=0;h<realelectrons;h++){
	//loadbar(h,realelectrons-1);
	Double_t X_pos=gRandom->Gaus(x_position,sigma_trans);
	Double_t Z_pos=gRandom->Gaus(z_position,sigma_trans);
	Double_t Y_pos=gRandom->Gaus(y_position,sigma_long);
	//cout<<X_pos<<" "<<Y_pos<<" "<<Z_pos<<endl;
	TVector3 electron_position(X_pos,Y_pos,Z_pos);
	
	//Int_t pad_Number=theGeometry->CalculatePad(electron_position);
	//Int_t row_Number=theGeometry->GetRowNumber(electron_position);
	//Int_t column_Number=theGeometry->GetColumnNumber(electron_position);

	//cout<< pad_Number<<" "<<row_Number<<" "<<column_Number<<endl;

	
	//Double_t gain=1.;
 	Double_t gain=1000*Polya();
	Double_t charge_deposited=gain;

	//cout<<"Charge deposited "<<gain<<endl;
	//Double_t charge_deposited=energy;
	//if(pad_Number>0)
	//  theGeometry->AddBinContent(pad_Number,charge_deposited);
	theGeometry->Fill(Z_pos,X_pos,charge_deposited);

	if(PID==-11&&Parent_TID==1){
	  theGeometry_betas->Fill(Z_pos,X_pos,charge_deposited);
	}
	else if(PID=2212){
	  theGeometry_protons->Fill(Z_pos,X_pos,charge_deposited);
	}    

        
	}//end of ELECTRON loop!
 
    }//end of ARRAY LOOP!
 
	for(Int_t bin=1;bin<=nbins;bin++){
	  Float_t binContent=theGeometry->GetBinContent(bin);
	  Float_t resolutionPercent=ResolutionFactor/sqrt(binContent);
	  Float_t totalResolution=(resolutionPercent/100)*binContent;
	  Float_t sigmaResolution=totalResolution/FWHMFactor;
	  if(binContent>0)
	    binContent=gRandom->Gaus(binContent,sigmaResolution);
	  else
	    binContent=0;
	  Float_t E_protons=theGeometry_protons->GetBinContent(bin);
	  if(E_protons>0)
	    E_protons=gRandom->Gaus(E_protons,sigmaResolution);
	  else
	    E_protons=0;
	  Float_t E_betas=theGeometry_betas->GetBinContent(bin);
	  if(E_betas>0)
	    E_betas=gRandom->Gaus(E_betas,sigmaResolution);
	  else
	    E_betas=0;
	  Energy_pad[bin-1]=binContent/1000.;
	  Energy_protons[bin-1]=E_protons/1000.;
	  Energy_betas[bin-1]=E_betas/1000.;
	}

    	outTree->Fill();      
     //TVirtualPad *vp=c->cd(1);
	//if(n_points_protons>0){
	  
	//theGeometry->Draw("colzl");
	  //vp->Update();
	  //vp=c->cd(2);
	  //theGeometry2->Draw("colzl");
	  //if(n_points_betas>0)
	//g_betas->Draw("LP");
	//if(n_points_protons>0)
	//  g_protons->Draw("LP");
	//if(n_points_beam>0)
	//  g_beam->Draw("LP");
	//
	//c->Update();
	//c->WaitPrimitive();
	  //}
	
  }//end of EVENT LOOP!

  out_file->Write();
  delete out_file;
  Int_t end_time=time(NULL);
  cout<<endl;
  cout<<"Run time "<<(end_time-start_time)/60<<" min."<<endl;

}



DetectorPadGeometry *CreateGeometry2(Char_t *name="pads"){

  DetectorPadGeometry *theGeometry=new DetectorPadGeometry(5);
  theGeometry->SetName(name);
  Int_t nColumns[]={3,9,9,9,3};
  theGeometry->SetNumberOfColumns(nColumns);
  Int_t padSizeX[]={60,20,20,20,60};
  Int_t padSizeY[]={10,10,20,10,10};
  theGeometry->SetPadSizesX(padSizeX);
  theGeometry->SetPadSizesY(padSizeY);
  theGeometry->ConstructGeometry();

  return theGeometry;
}

DetectorPadGeometry *CreateGeometry3(Char_t *name="pads"){

  DetectorPadGeometry *theGeometry=new DetectorPadGeometry(3);
  theGeometry->SetName(name);
  Int_t nColumns[]={3,9,3};
  theGeometry->SetNumberOfColumns(nColumns);
  Int_t padSizeX[]={60,20,60};
  Int_t padSizeY[]={15,30,15};
  theGeometry->SetPadSizesX(padSizeX);
  theGeometry->SetPadSizesY(padSizeY);
  theGeometry->ConstructGeometry();

  return theGeometry;
}


DetectorPadGeometry *CreateGeometry(Char_t *name="pads"){

  DetectorPadGeometry *theGeometry=new DetectorPadGeometry(3);
  theGeometry->SetName(name);
  Int_t nColumns[]={3,9,3};
  theGeometry->SetNumberOfColumns(nColumns);
  Int_t padSizeX[]={60,20,60};
  Int_t padSizeY[]={10,40,10};
  theGeometry->SetPadSizesX(padSizeX);
  theGeometry->SetPadSizesY(padSizeY);
  theGeometry->ConstructGeometry();

  return theGeometry;
}

static  inline void loadbar(unsigned int k, unsigned int n, unsigned int w = 72)
{
  if ( (k != n) && (k % (n/100) != 0) ) return;
  
  float ratio  =  k/(float)n;
  int   c      =  ratio * w;
  
  cout << setw(3) << (int)(ratio*100) << "% [";
  for ( k=0; k<c; k++) cout << "=";
  for ( k=c; k<w; k++) cout << " ";
  cout << "]\r" << flush;
}


Float_t Polya(Float_t param=3.2){ //Bellazzini et al NIMA 581 (2007) 246
  
  static Short_t firstcall=0;
  static Float_t integral[1000];
  
  Int_t check=0;
  Int_t i=0;
  Long_t rseed;
  Float_t f,buff[1000];
  Float_t lambda;
  Float_t step=0.01;
  Float_t shift=0.005;
  Float_t pran=0;
  
  if(firstcall==0){
    for(i=0;i<1000;i++){
      lambda=i*step+shift;
      buff[i]=pow(param,param)/TMath::Gamma(param)*pow(lambda,param-1)*exp(-param*lambda);
      if(i>0)
	integral[i]=integral[i-1]+buff[i];
      else
	integral[i]=buff[i];
    }
    firstcall=1;
  }
  while(check==0){
    
    f=gRandom->Rndm();
    if(f>0.0001 && f<0.9999) check=1;
  }
  
  i=0;
  while(i<1000){
    if(f<integral[i]/integral[999]){
      pran=i*step+shift;
      break;
    }
    i++;
  }
  return pran;
}
