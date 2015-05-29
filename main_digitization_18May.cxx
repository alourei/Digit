#include "TSystem.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TRandom.h"
#include "TClonesArray.h"
#include <iostream>
#include <iomanip>
#include "TGraph.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TH2F.h"
#include "TSpline.h"
#include <ctime>
#include "/user/perezlou/geant4_workdir/ProtonDetector/include/ProtonDetectorBeamInfo.hh"
#include "/user/perezlou/geant4_workdir/ProtonDetector/include/ProtonDetectorSimpleTrack.hh"
#include "main_digitization.h"

using namespace std;

#define DEBUG 4

//DetectorPadGeometry *CreateGeometry(Char_t *);
//Float_t Polya(Float_t param=3.2);
//void loadbar(unsigned int k, unsigned int n, unsigned int w = 72);

int main(int argc,char **argv){
  
  Char_t inputfile[256];
  Char_t outputfile[256];
  if(argc<3){
    cerr<<"Input file and output file missing!"<<endl;
    return -1;
  }
  else{
    strcpy(inputfile,argv[1]);
    strcpy(outputfile,argv[2]);
    
    Int_t start_time=time(NULL);
    Bool_t circular=kTRUE;
    //Bool_t circular=kFALSE;
    //Bool_t draw=kTRUE;
    Bool_t draw=atoi(argv[3]);
    TApplication theApp("cona",&argc,argv);
   TCanvas *c;
   TCanvas *c2;
   gROOT->SetStyle("Pub");
  if(draw){
    c=new TCanvas();
    c->Draw();
    c->Divide(1,2);
    c2=new TCanvas();
    c2->Draw();
    c2->Divide(2,1);
 }
  // c->Divide(1,3);

 

  //cout<<argc<<" "<<inputfile<<" "<<outputfile<<endl;
  
  Double_t PI=3.1415926535897932384626433;
  Double_t rad=180./PI;
  Double_t Y_length=85; //mm
  Double_t Z_length=150; //mm
  //Double_t W= 26.; //eV P-10
  Double_t W= 26.3704; //eV Ar iso 2.5%
  //Double_t W= 21.5; //eV Xe
  //Double_t W= 36.2; //eV Ne

  //P5-800 Gas Parameters
  // Double_t v_drift = 0.04165; // mm/ns P-5 800 torr
  //Double_t D_trans = 10.58e-4; //mm**2/ns P-5 800 torr 
  //Double_t D_long = 4.411e-4; //mm**2/ns P-5 800 torr 
   //Xe 5 atm parameters
  //Double_t v_drift = 7.02e-4; // mm/ns P-5 800 torr
  //Double_t D_trans = 9.74e-5; //mm**2/ns P-5 800 torr 
  //Double_t D_long = 1.96e-5; //mm**2/ns P-5 800 torr 
  //
  //Xe 2 atm parameters
  // Double_t v_drift = 9.51e-4; // mm/ns P-5 800 torr
  // Double_t D_trans = 3.2e-5; //mm**2/ns P-5 800 torr 
  // Double_t D_long = 2.56e-5; //mm**2/ns P-5 800 torr 
  // //

  //Ar-iso 2.5% 1 atm parameters
  Double_t v_drift = 3.25e-2; // mm/ns 
  Double_t D_long = 1.412e-4; //mm**2/ns 
  Double_t D_trans = 6.417e-4; //mm**2/ns  
  
  //Disable Difussion
  //Double_t D_long = 0; //mm**2/ns 
  //Double_t D_trans = 0; //mm**2/ns  

  Double_t sigma_trans;
  Double_t sigma_long;
  //gSystem->Load("~perezlou/geant4_workdir/ProtonDetector_build/libProton.sl");
  //gROOT->ProcessLine(".L DetectorPadGeometry.C+");
  TFile *fil=new TFile(inputfile);
  TFile *out_file=new TFile(outputfile,"recreate");
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

  
  // DetectorPadGeometry *theGeometry=CreateGeometry("All");
  // DetectorPadGeometry *theGeometry_protons=CreateGeometry("protons");
  // DetectorPadGeometry *theGeometry_betas=CreateGeometry("betas");


  //4mm Pads
   // TH2F *theGeometry=new TH2F("All","All",45,-90,90,15,-30,30);
   // TH2F *theGeometry_protons= new TH2F("protons","protons",45,-90,90,15,-30,30);
   // TH2F *theGeometry_betas= new TH2F("betas","betas",45,-90,90,15,-30,30);

  //2mm Pads
  // TH2F *theGeometry=new TH2F("All","All",90,-90,90,30,-30,30);
  // TH2F *theGeometry_protons= new TH2F("protons","protons",90,-90,90,30,-30,30);
  // TH2F *theGeometry_betas= new TH2F("betas","betas",90,-90,90,30,-30,30);
 
  
  //Circular Pads
  TH2Poly *theGeometry=CreateCircularPads("pads");
  TH2Poly *theGeometry_protons=CreateCircularPads("protons");
  TH2Poly *theGeometry_betas=CreateCircularPads("betas");
  TH2Poly *theGeometry_others=CreateCircularPads("others");
  //TH2Poly *theGeometry=CreateCircularPads_2("pads");
  //TH2Poly *theGeometry_protons=CreateCircularPads_2("protons");
  //TH2Poly *theGeometry_betas=CreateCircularPads_2("betas");
  //TH2Poly *theGeometry=CreateCircularPads_3("pads");
  //TH2Poly *theGeometry_protons=CreateCircularPads_3("protons");
  //TH2Poly *theGeometry_betas=CreateCircularPads_3("betas");
  theGeometry->SetMinimum(1);

  //Added Energy Resolutions
  Float_t ResolutionFactor=90.;
  Float_t FWHMFactor= 2.35482;
  // Add Resolution (Lolly's estimate) 90/sqrt(Ep(kev))%

  Double_t energy_total;
  Double_t Energy_deposited[3000];
  Double_t Energy_pad[3000];
  Double_t Energy_protons[3000];
  Double_t Energy_betas[3000];
  Double_t Energy_others[3000];
  Int_t Charge;
  Int_t padNumber;
  Int_t nbins=theGeometry->GetNumberOfBins();
  //Int_t nbins=theGeometry->GetSize();
  TTree *outTree =new TTree("theTree","Output Tree");

  outTree->Branch("nbins",&nbins,"nbins/I");
  outTree->Branch("Edeposited",Energy_deposited,"Ed[nbins]/D");
  outTree->Branch("Epad",Energy_pad,"Ep[nbins]/D");
  outTree->Branch("Eprotons",Energy_protons,"Ep[nbins]/D");
  outTree->Branch("Ebetas",Energy_betas,"Eb[nbins]/D");
  outTree->Branch("Eothers",Energy_others,"Eo[nbins]/D");
  outTree->Branch("Etot",&energy_total,"Et/D");
  outTree->Branch("Pad",&padNumber,"pad/I");
  outTree->Branch("Charge",&Charge,"ch/I");

  TGraph *g_parent=new TGraph();
  TGraph *g_beam=new TGraph();
  TGraph *g_betas=new TGraph();
  TGraph *g_protons=new TGraph();
  TGraph *g_protonsZ=new TGraph();
  TGraph *g_betasZ=new TGraph();
  TGraph *g_beamZ=new TGraph();
  g_beam->SetMarkerStyle(20);
  g_betas->SetMarkerStyle(20);
  g_protons->SetMarkerStyle(20);
  g_protonsZ->SetMarkerStyle(20);
  g_betasZ->SetMarkerStyle(24);
  g_beamZ->SetMarkerStyle(20);
  
  
  g_betas->SetMarkerColor(7);
  g_betasZ->SetMarkerColor(7);
  g_protons->SetMarkerColor(1);
  g_protonsZ->SetMarkerColor(1);
  g_beam->SetMarkerColor(6);
  g_beamZ->SetMarkerColor(6);
  g_parent->SetMarkerColor(3);
  g_betas->SetLineColor(7);
  g_betasZ->SetLineColor(7);
  g_protons->SetLineColor(3);
  g_protonsZ->SetLineColor(3);
  g_beam->SetLineColor(6);
  g_beamZ->SetLineColor(6);
  
  g_betas->SetLineWidth(2);
  g_protons->SetLineWidth(3);
  g_beam->SetLineWidth(4);

  TH1F *h1=new TH1F("h","Energy Deposited in central pad",200,1,2000);
  TH1F *h2=new TH1F("h2","Energy Deposited in lateral pads",200,1,2000);


  cout<<nevents<<" Events in the ROOT file"<<endl;
  for(Int_t i=0;i<nevents;i++){ //Event Loop	
 
    //cout<<"**************************************NEW EVENT********************************************************"<<endl;
   
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
    g_protonsZ->Set(0);
    g_betasZ->Set(0);
    g_beamZ->Set(0);
    g_parent->Set(0);
     for(Int_t k=0;k<nbins;k++){
      Energy_pad[k]=-10;
      Energy_protons[k]=-10;
      Energy_betas[k]=-10;
    }
    nb+=T->GetEntry(i);
    
    Int_t nentries=simpleTrackCA->GetEntriesFast();
    //Loop on the Clones Array
    //cout<<nentries<<" strides in the event"<<endl;
    for(Int_t j=0;j<nentries;j++){
       
      localTrack=(ProtonDetectorSimpleTrack*)simpleTrackCA->At(j);
      //if(localTrack->GetZPre()<-150 ||localTrack->GetZPre()>150 )continue;
      if(pow(localTrack->GetXPre(),2)+pow(localTrack->GetYPre(),2)>pow(120,2))continue;
      Int_t charge=localTrack->GetParticleCharge();
      Int_t Parent_TID=localTrack->GetParentTrackID();
      Int_t PID=localTrack->GetParticleID();
      Charge=charge;
      Double_t energy=1000*localTrack->GetEnergyStride(); //Energy deposited in stride in keV
      //if(DEBUG>2)
      //cout<<j<<" Charge "<<charge<<" Energy  "<<energy<<" KeV "<<energy/(W*1e-3)<<endl;
       energy_total+=energy;
      Double_t x_position=(localTrack->GetXPre()+localTrack->GetXPost())/2.;
      Double_t y_position=(localTrack->GetYPre()+localTrack->GetYPost())/2.;
      Double_t z_position=(localTrack->GetZPre()+localTrack->GetZPost())/2.;
      Double_t real_X_pos=localTrack->GetXPre();
      Double_t real_Z_pos=localTrack->GetZPre();
      Double_t real_Y_pos=localTrack->GetYPre();
      //if(DEBUG>2)
      //cout<<real_X_pos<<" "<<real_Y_pos<<" "<<real_Z_pos<<endl;
      if(!circular){      
       if(PID==2212){
	g_protons->SetPoint(n_points_protons,real_Z_pos,real_X_pos);
	n_points_protons++;	  
      }
       else if(PID==-11&&Parent_TID==1){
	g_betas->SetPoint(n_points_betas,real_Z_pos,real_X_pos);
	n_points_betas++;
      }
       else {
     g_beam->SetPoint(n_points_beam,real_Z_pos,real_X_pos);
     g_beamZ->SetPoint(n_points_beam,real_Z_pos,real_X_pos);
      n_points_beam++;
      }
      }
      else{
      if(PID==2212){
	g_protons->SetPoint(n_points_protons,real_X_pos,real_Y_pos);
	g_protonsZ->SetPoint(n_points_protons,real_Z_pos,real_Y_pos);
	n_points_protons++;	  
      }
       else if(PID==-11&&Parent_TID==1){
	g_betas->SetPoint(n_points_betas,real_X_pos,real_Y_pos);
	g_betasZ->SetPoint(n_points_betas,real_Z_pos,real_Y_pos);
	n_points_betas++;
      }
       else if (charge>1){
	g_beam->SetPoint(n_points_beam,real_X_pos,real_Y_pos);
	g_beamZ->SetPoint(n_points_beam,real_Z_pos,real_Y_pos);
	n_points_beam++;
      }

      }

      Double_t drift_distance;
      if(!circular)
	drift_distance=y_position+Y_length;
      else
	drift_distance=z_position+Z_length;
      sigma_trans=sqrt(drift_distance*2*D_trans/v_drift);
      sigma_long=sqrt(drift_distance*2*D_long/v_drift);
      //if(DEBUG>2)
      //cout<<sigma_trans<<" "<<sigma_long<<endl;
      Double_t drift_time=drift_distance/v_drift; //(ns)
      //Number of electrons per stride
      Int_t nelectrons=energy/(W*1e-3);
      Int_t realelectrons=gRandom->Poisson(nelectrons);
      //Int_t realelectrons =1;
      //if(DEBUG>2)
      //cout<<j<<" "<<nelectrons<<" "<<realelectrons<<" electrons produced in the step"<<endl;
      if(nelectrons==0)continue;
      total_electrons+=realelectrons;
      //Loop on the electrons
      for(Int_t h=0;h<realelectrons;h++){
	//loadbar(h,realelectrons-1);
	Double_t X_pos=0;
	Double_t Z_pos=0;
	Double_t Y_pos=0;
	if(!circular){
	X_pos=gRandom->Gaus(x_position,sigma_trans);
	Z_pos=gRandom->Gaus(z_position,sigma_trans);
	Y_pos=gRandom->Gaus(y_position,sigma_long);
	}
	else{
	X_pos=gRandom->Gaus(x_position,sigma_trans);
	Y_pos=gRandom->Gaus(y_position,sigma_trans);
	Z_pos=gRandom->Gaus(z_position,sigma_long);	
	}
	//cout<<X_pos<<" "<<Y_pos<<" "<<Z_pos<<endl;
	TVector3 electron_position(X_pos,Y_pos,Z_pos);
	
	Int_t pad_Number=theGeometry->FindBin(X_pos,Y_pos);
	//Int_t row_Number=theGeometry->GetRowNumber(electron_position);
	//Int_t column_Number=theGeometry->GetColumnNumber(electron_position);

	//if(DEBUG>2)
	  //cout<<X_pos<<" "<<Y_pos<<" "<<Z_pos<<" "<<pad_Number<<endl;
	//cout<< pad_Number<<" "<<row_Number<<" "<<column_Number<<endl;
	
	
	//Double_t gain=1.;
 	Double_t gain=1000*Polya();
	Double_t charge_deposited=gain;
	
	//cout<<"Charge deposited "<<gain<<endl;
	//Double_t charge_deposited=energy;
	//if(pad_Number>0)
	//  theGeometry->AddBinContent(pad_Number,charge_deposited);
	if(!circular){
	  theGeometry->Fill(Z_pos,X_pos,charge_deposited);
	  
	  if(PID==-11&&Parent_TID==1){
	    theGeometry_betas->Fill(Z_pos,X_pos,charge_deposited);
	  }
	  else if(PID=2212){
	    theGeometry_protons->Fill(Z_pos,X_pos,charge_deposited);
	  } 
	  else   
	    theGeometry_others->Fill(Z_pos,X_pos,charge_deposited);
	
	}
	else{
	  theGeometry->Fill(X_pos,Y_pos,charge_deposited);
	  if(pad_Number==1)
	    h1->Fill(charge_deposited);
	  else
	    h2->Fill(charge_deposited);	  
	  if(PID==-11&&Parent_TID==1){
	    theGeometry_betas->Fill(X_pos,Y_pos,charge_deposited);
	  }
	  else if(PID=2212){
	    theGeometry_protons->Fill(X_pos,Y_pos,charge_deposited);
	  } 
	  else   
	    theGeometry_others->Fill(X_pos,Y_pos,charge_deposited);
	  
	}	
        
      }//end of ELECTRON loop!

	if(draw){
	c->cd(1);
	//	if(n_points_protons>0){
	  TH1D *projX= theGeometry->ProjectionX("X"); 
	  TH1D *projY= theGeometry->ProjectionY("Y"); 
	  TSpline3 *spX=new TSpline3(projX);
	  TSpline3 *spY=new TSpline3(projY);
	  spX->SetLineColor(2);
	  spX->SetLineWidth(2);
	  spY->SetLineColor(2);
	  spY->SetLineWidth(2);
	theGeometry->Draw("colzl");
	//vp->Update();
	//vp=c->cd(2);
	//projX->Draw();
	//spX->Draw("same");
	//vp=c->cd(3);
	//projY->Draw();
	//spY->Draw("same");
	//vp=c->cd(1);
	  if(n_points_betas>0)
	g_betas->Draw("LP");
	if(n_points_protons>0)
	 g_protons->Draw("LP");
	if(n_points_beam>0)
	 g_beam->Draw("LP");	
	c->cd(2);
	  c->cd(2)->DrawFrame(-150,-90,150,90);
	if(n_points_protons>0)
	  g_protonsZ->Draw("LP");
	if(n_points_beam>0)
	  g_beamZ->Draw("LP");	
	if(n_points_betas>0)
	  g_betasZ->Draw("LP");
	c->Update();
	c2->cd(1);
	h1->Draw();
	c2->cd(2);
	h2->Draw();
	c2->Update();
	//c->WaitPrimitive();
	//}
	}   

 
    }//end of ARRAY LOOP!
 
	for(Int_t bin=1;bin<=nbins;bin++){
	  Float_t binContent=theGeometry->GetBinContent(bin);
	  Float_t E_protons=theGeometry_protons->GetBinContent(bin);
	  Float_t E_betas=theGeometry_betas->GetBinContent(bin);
	  Float_t E_others=theGeometry_others->GetBinContent(bin);
	  //Float_t noise=gRandom->Gaus(1000,200);
	  Float_t noise=0;
	  binContent+=noise;
	  E_protons+=noise;
	  E_betas+=noise;
	  E_others+=noise;
	  Float_t resolutionPercent=ResolutionFactor/sqrt(binContent);
	  Float_t totalResolution=(resolutionPercent/100)*binContent;
	  Float_t sigmaResolution=totalResolution/FWHMFactor;
	  /*
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
	  */
	  Energy_pad[bin-1]=binContent;
	  Energy_protons[bin-1]=E_protons;
	  Energy_betas[bin-1]=E_betas;
	  Energy_others[bin-1]=E_others;
	}

    	outTree->Fill();      
	//TVirtualPad *vp=c->cd(1);
	// if(draw){
	// c->cd(1);
	// if(n_points_protons>0){
	//   TH1D *projX= theGeometry->ProjectionX("X"); 
	//   TH1D *projY= theGeometry->ProjectionY("Y"); 
	//   TSpline3 *spX=new TSpline3(projX);
	//   TSpline3 *spY=new TSpline3(projY);
	//   spX->SetLineColor(2);
	//   spX->SetLineWidth(2);
	//   spY->SetLineColor(2);
	//   spY->SetLineWidth(2);
	// theGeometry->Draw("colzl");
	// //vp->Update();
	// //vp=c->cd(2);
	// //projX->Draw();
	// //spX->Draw("same");
	// //vp=c->cd(3);
	// //projY->Draw();
	// //spY->Draw("same");
	// //vp=c->cd(1);
	//   if(n_points_betas>0)
	// g_betas->Draw("LP");
	// if(n_points_protons>0)
	//  g_protons->Draw("LP");
	// if(n_points_beam>0)
	//  g_beam->Draw("LP");	
	// c->cd(2);
	// if(n_points_protons>0)
	//   c->cd(2)->DrawFrame(-150,-90,150,90);
	//   g_protonsZ->Draw("LP");
	//   g_betasZ->Draw("LP");
	// c->Update();
	// c->WaitPrimitive();
	// }
	// }
  }//end of EVENT LOOP!

  out_file->Write();
  delete out_file;
  Int_t end_time=time(NULL);
  cout<<endl;
  cout<<"Run time "<<(end_time-start_time)/60<<" min."<<endl;
  return 1;
  }
}



