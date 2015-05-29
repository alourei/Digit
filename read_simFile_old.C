// #include <iostream>
// #include "TFile.h"
// #include "TTree.h"
// #include "TH1F.h"
// #include "TH2F.h"
// #include "TClonesArray.h"
// #include "/Users/david/geant4/ActarSim2011_w_decay/include/ActarSimPrimaryInfo.hh"
// #include "/Users/david/geant4/ActarSim2011_w_decay/include/ActarSimSimpleTrack.hh"
// #include "/Users/david/geant4/ActarSim2011_w_decay/include/ActarSimSilHit.hh"
// #include "/Users/david/geant4/ActarSim2011_w_decay/include/ActarSimSciHit.hh"
// #include "/Users/david/geant4/ActarSim2011_w_decay/src/ActarSimDict.h"
void read_simFile(){
  Double_t PI=3.1415926535897932384626433;
  Double_t rad=180./PI;
  Double_t Y_length=128; //mm
  Double_t W= 26.; //eV P-10
  Double_t v_drift = 0.04165; // mm/ns P-5 800 torr
  Double_t D_trans = 10.58e-4; //mm**2/ns P-5 800 torr 
  Double_t sigma_trans;
  gSystem->Load("~perezlou/geant4_workdir/ProtonDetector_build/libProton.sl");
  TFile *fil=new TFile("simFile_Decay.root");
  //TFile *out_file=new TFile("cona.root","recreate");
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
  Int_t nb=0;
  Int_t nentries;
  Int_t total_electrons=0;
  TH2Poly *hpads=newPads();
  TH2Poly *hpads_total=newPads("total");
  TH2Poly *hpads_protons=newPads("protons");
  TH2Poly *hpads_betas=newPads("betas");
  TH2Poly *hpads_all=newPads("all");
  hpads->SetMinimum(1);
  TGraph *g_betas=new TGraph();
  TGraph *g_protons=new TGraph();
  TGraph *g_beam=new TGraph();
  TGraph *g_vertex=new TGraph();
  TGraph *g_all=new TGraph();

  Int_t numberOfProtonDecays=0;
  Int_t decayFlag=0;


 
  g_betas->SetMarkerStyle(20);
  g_protons->SetMarkerStyle(20);
  g_beam->SetMarkerStyle(20);
  g_vertex->SetMarkerStyle(20);
  g_all->SetMarkerStyle(20);

  g_betas->SetMarkerColor(7);
  g_protons->SetMarkerColor(3);
  g_beam->SetMarkerColor(6);
  g_vertex->SetMarkerSize(2.);
  g_all->SetMarkerColor(0);

  g_betas->SetLineColor(7);
  g_protons->SetLineColor(3);
  g_beam->SetLineColor(6);
  g_all->SetLineColor(0);

  g_betas->SetLineWidth(2);
  g_protons->SetLineWidth(3);
  g_beam->SetLineWidth(4);
  g_all->SetLineWidth(1);

  //Added Energy Resolutions
  Float_t ResolutionFactor=90.;
  Float_t FWHMFactor= 2.35482;

  // Add Resolution (Lolly's estimate) 90/sqrt(Ep(kev))%


  const Int_t nHistogramPads=hpads->GetNumberOfBins();

  TH1F **hE=new TH1F*[nHistogramPads];
  TH1F **hEp=new TH1F*[nHistogramPads];
  TH1F **hEbeta=new TH1F*[nHistogramPads];
  Char_t hname[256];
  Char_t htitle[256];

  for(Int_t h;h<(nHistogramPads+1);h++){
    sprintf(hname,"histo_%d",h);
    sprintf(htitle,"Energy Deposit Pad %d",h);
    hE[h]=new TH1F(hname,htitle,500,0,2500);
    sprintf(hname,"histoP_%d",h);
    sprintf(htitle,"Energy Deposit Pad protons %d",h);
    hEp[h]=new TH1F(hname,htitle,500,0,2500);
    sprintf(hname,"histob_%d",h);
    sprintf(htitle,"Energy Deposit Pad betas %d",h);
    hEbeta[h]=new TH1F(hname,htitle,500,0,2500);
  }

  TH1F *h_inner=new TH1F("h_inner","Energy deposit in inner pads",500,0,2500);
  TH1F *h_outer=new TH1F("h_outer","Energy deposit in outer pads",500,0,2500);
  TH1F *h_inner_gated=new TH1F("h_inner_g","Energy deposit in inner pads, gated on outer",500,0,2500);
  TH1F *h_total=new TH1F("h_total","Sum Energy deposit in all pads",500,0,2500);
  TH2F *h_2D=new TH2F("h_total2D","Energy deposit in all pads",500,0,2500,500,0,2500);

  Double_t energy_protons;
  Double_t energy_betas;
  Double_t energy_total;
  Int_t padNumber;

  //TTree *outTree =new TTree("theTree","Output Tree");

  //outTree->Branch("Eprotons",&energy_protons,"Ep/D");
  //outTree->Branch("Ebetas",&energy_betas,"Eb/D");
  //outTree->Branch("Etot",&energy_total,"Et/D");
  //outTree->Branch("Pad",&padNumber,"pad/I");



  hpads->SetMarkerStyle(20);
  TCanvas *c=new TCanvas;
  gStyle->SetOptStat(0);
  c->Update();
  //c->WaitPrimitive();
  Int_t event_number;
  cout<<"Event number :"<<flush;
  //cin>>event_number;
  cout<<nevents<<" Events in the ROOT file"<<endl;
  for(Int_t i=0;i<nevents;i++){ //Event Loop	


    loadbar(i,nevents-1);

    Int_t n_points_betas=0,n_points_protons=0,n_points_beam=0;
    simpleTrackCA->Clear();
	hpads->Reset();
	nb+=T->GetEntry(i);

	energy_protons=0;
	energy_betas=0;
	energy_total=0;
 

	//nb+=tracksT->GetEntry(i);
	Double_t X_vertex=beamInfo->GetXInitial();
	Double_t Z_vertex=beamInfo->GetZInitial();
	g_betas->Set(0);
	g_protons->Set(0);
	g_beam->Set(0);
	g_vertex->Set(0);
	g_all->Set(0);

	g_vertex->SetPoint(0,Z_vertex,X_vertex);
	//T->Show(i);
	Int_t nentries=simpleTrackCA->GetEntriesFast();
	//cout<<i<<" "<<nentries<<" strides in the ClonesArray"<<endl;
	Int_t flag=0;
	for(Int_t j=0;j<nentries;j++){
	  localTrack=(ProtonDetectorSimpleTrack*)simpleTrackCA->At(j);
	  if(localTrack->GetZPre()<-128 ||localTrack->GetZPre()>128 )continue;
	  Int_t charge=localTrack->GetParticleCharge();
	  Double_t energy=1000*localTrack->GetEnergyStride();
	  energy_total+=energy;
	  Double_t x_position=(localTrack->GetXPre()+localTrack->GetXPost())/2.;
	  Double_t y_position=(localTrack->GetYPre()+localTrack->GetYPost())/2.;
	  Double_t z_position=(localTrack->GetZPre()+localTrack->GetZPost())/2.;
	  Int_t PID=localTrack->GetParticleID();
	  if(PID== 1000120230 &&flag==0){
	    //cout<<localTrack->GetProcessName()<<"TrackID:"<<localTrack->GetTrackID()<<endl;
	      flag=1;
	      }
	  if(PID==2212){
	    numberOfProtonDecays++;
	    energy_protons+=energy;
	    //decayFlag=1;
	  }
	  else if(PID==-11)
	    energy_betas+=energy;

	  Int_t Parent_TID=localTrack->GetParentTrackID();
	  Double_t drift_distance=y_position+Y_length;
	  sigma_trans=sqrt(drift_distance*2*D_trans/v_drift);
	  Double_t drift_time=drift_distance/v_drift;
	  //  cout<<j<<" "<<charge<<" "<<energy<<endl;
	  Int_t nelectrons=energy/(W*1e-6);
	  Int_t realelectrons=gRandom->Poisson(nelectrons);
	  total_electrons+=realelectrons;
	  //cout<<energy*1000<<"keV "<<realelectrons<<endl;
	  //getchar();
	  //for(Int_t h=0;h<realelectrons;h++){
	  Double_t X_pos=gRandom->Gaus(x_position,sigma_trans);
	  Double_t Z_pos=gRandom->Gaus(z_position,sigma_trans);
	  //cout<<Z_pos<<" "<<X_pos<<endl;
	  //if(h%50000==0)
	  //cout<<"."<<flush;
	  //cout<<"Z:"<<z_position<<" X: "<<x_position<<endl;
	  //hpads->Fill(z_position,x_position,energy);
	  //Double_t gain=1000*Polya();
	  Double_t gain=1;
	  //hpads->Fill(Z_pos,X_pos,gain);
	  hpads->Fill(Z_pos,X_pos,energy);
	  hpads_all->Fill(Z_pos,X_pos,energy_total);
	  hpads_betas->Fill(Z_pos,X_pos,energy_betas);
	  hpads_protons->Fill(Z_pos,X_pos,energy_protons);
	  hpads_total->Fill(Z_pos,X_pos);
	  //}
	  Double_t real_X_pos=localTrack->GetXPre();
	  Double_t real_Z_pos=localTrack->GetZPre();
	  g_all->SetPoint(j,real_Z_pos,real_X_pos);
	  if(PID==-11&&Parent_TID==1){
	    g_betas->SetPoint(n_points_betas,real_Z_pos,real_X_pos);
	    n_points_betas++;
	  }
	  //else if(PID==2212/&&Parent_TID==2){
	  else if(PID==2212){
	  g_protons->SetPoint(n_points_protons,real_Z_pos,real_X_pos);
	  n_points_protons++;	  
	  }
	  else if(PID== 1000160310){
	  g_beam->SetPoint(n_points_beam,real_Z_pos,real_X_pos);
	  n_points_beam++;	  
	  }
	  //cout<<h<<" "<<X_pos<<" "<<Z_pos<<endl;
	  }
	  
	
	//cout<<"Total number of electrons "<<total_electrons<<endl;
	//Char_t c;
	//cout<<endl;
	// if(n_points_protons){
	//   hpads->Draw("zcol");
	//   hpads_protons->Draw("zcol");
	// hpads->Draw("same");
	// g_all->Draw("P");
	// if(n_points_betas>0)
	// g_betas->Draw("LP");
	// if(n_points_protons>0)
	//   g_protons->Draw("LP");
	// if(n_points_beam>0)
	//   g_beam->Draw("LP");
	// g_vertex->Draw("P");
	// c->Update();
	//hpads->Print();
	  //cout<<hpads->GetBinContent(bin)<<endl;
	//}
	//c->WaitPrimitive();
	Int_t nbinsPadPlane=hpads->GetNumberOfBins();
	//cout<<nbinsPadPlane<<endl;
	Float_t inner_Energy=0;
	Float_t outer_Energy=0;
	Float_t total_Energy=0;
	for(Int_t bin=1;bin<=nbinsPadPlane;bin++){
	  Float_t binContent=hpads->GetBinContent(bin);
	  Float_t binContentbetas=hpads_betas->GetBinContent(bin);
	  Float_t binContentprotons=hpads_protons->GetBinContent(bin);
	  Float_t resolutionPercent=ResolutionFactor/sqrt(binContent);
	  Float_t totalResolution=(resolutionPercent/100)*binContent;
	  Float_t sigmaResolution=totalResolution/FWHMFactor;
	  if(binContent>0)
	  binContent=gRandom->Gaus(binContent,sigmaResolution);
	  total_Energy+=binContent;
	  //if(binContent<0)
	  //binContent=0;
	  if(bin==5 || bin==6)
	    inner_Energy+=binContent;
	  else
	    outer_Energy+=binContent;
	  hE[bin]->Fill(binContent);
	  hEp[bin]->Fill(binContent);
	  hEbeta[bin]->Fill(binContentbetas);
	  hEbeta[bin]->Fill(binContentprotons);
	}
	padNumber=bin;
	h_total->Fill(inner_Energy+outer_Energy);
	h_2D->Fill(inner_Energy,outer_Energy);
	//outTree->Fill();
	if(inner_Energy)
	  h_inner->Fill(inner_Energy);
	if(outer_Energy<10)
	  h_inner_gated->Fill(inner_Energy);
	if(outer_Energy)
	  h_outer->Fill(outer_Energy);
	  //else
	  //continue;
	decayFlag=0;
  }//End Of Event Loop
  hE[5]->Draw();
  hpads_total->Draw("zcol");
  hpads_total->Draw("same");
  cout<<endl;
  cout<<"Proton decays "<<numberOfProtonDecays<<" "<<numberOfProtonDecays/nentries<<endl;

}


TH2Poly *pads(Char_t *name="Boxes"){
  //TCanvas *ch2p2 = new TCanvas("ch2p2","ch2p2",600,400);
   TH2Poly *h2p = new TH2Poly();
   h2p->SetName(name);
   h2p->SetTitle("Pad Plane in ProtonDetector");
   h2p->SetLineWidth(2);
   h2p->SetLineColor(2);
   gStyle->SetPalette(1);
   Int_t i,j;
   Int_t nx = 6;
   Int_t x1,y1,x2,y2;
   Int_t dx=40,dy=20;
   x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = -120.;
      y2 = y1+dy;
      x2=x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }
   x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = -100.;
      y2 = y1+dy;
      x2=x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }
    x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = -80.;
      y2 = y1+dy;
      x2=x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }
    x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = -60.;
      y2 = y1+dy;
      x2=x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }
   x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = -40.;
      y2 = y1+dy;
      x2=x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }
  Int_t dx=20,dy=40;
  nx=12;
  x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = -20.; 
      y2 = y1+dy;
      x2=x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y1+dy;
      x1 = x2;
      x2 = x2+dx;
   }
  nx=6;
  Int_t dx=40,dy=20;
  x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = 20.;
      y2 = y1+dy;
      x2 = x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }
  x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = 40.;
      y2 = y1+dy;
      x2 = x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }
  x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = 60.;
      y2 = y1+dy;
      x2 = x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }
  x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = 80.;
      y2 = y1+dy;
      x2 = x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }
  x1=-120;
   for (i = 0; i<nx; i++) {
      y1 = 100.;
      y2 = y1+dy;
      x2 = x1+dx;
      //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }

   return h2p;
   //h2p->Draw();
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


/*TH2Poly *pads2(Float_t x0,Float_t y0,Int_t rows_x,Int_t rows_y,Float_t *size_x, Float_t *size_y){
for(Int_t i=0;i<row)

}
*/

TH2Poly *newPads(Char_t *hname="hpoly"){
  TH2Poly *pads2=new TH2Poly();
  pads2->SetName(hname);
  pads2->SetTitle("New  pad plane");
  pads2->AddBin(-50,-30,-40,30);
  pads2->AddBin(-40,-30,40,-20);
  pads2->AddBin(40,-30,50,30);
  pads2->AddBin(-40,20,40,30);
  pads2->AddBin(-40,-20,0,20);
  pads2->AddBin(0,-20,40,20);
  return pads2;
  
}
void plot_tracks(){

   gSystem->Load("~perezlou/geant4_workdir/ProtonDetector_build/libProton.sl");
  TFile *fil=new TFile("simFile.root");
   TTree *tracksT= (TTree*)fil->Get("The_Tracks_Tree");
  TBranch *branch_tracks=tracksT->GetBranch("trackData");
  ProtonDetectorTrack *thetrack=new ProtonDetectorTrack;
  branch_tracks->SetAddress(&thetrack);
  branch_tracks->SetAutoDelete(kTRUE);
  //Int_t nentries=tracksT->GetEntriesFast();
  Int_t nentries=2;
  cout<<nentries<<endl;
  for(Int_t i=0;i<nentries;i++){

    tracksT->GetEntry(i);
    tracksT->Show(i);

    //cout<<thetrack->GetXCoord()<<" "<<thetrack->GetZCoord()<<endl;
  }
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
