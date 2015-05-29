//void plot_energies_circ(Char_t *filename="digiFile_1M_Cl31.root"){
//void plot_energies_circ(Char_t *filename="digi_23Al_tube.root"){
//void plot_energies_circ(Char_t *filename=" digiFile_Chain.root"){
  void plot_energies_circ(Char_t *filename="sigiFile_10M.root"){
  //ofstream * out=new ofstream("Efficiencies_Apr102014.txt",ios::app);
  gStyle->SetLabelSize(0.05,"XY");
  gStyle->SetTitleSize(0.05,"XY");

  TFile *file=new TFile(filename);

  TTree *T=(TTree*)file->Get("theTree");

  if(!T)
    return;

  Double_t E_protons[100];
  Double_t E_pad[100];
  Double_t E_betas[100];

  Double_t sum=0;
  Double_t betas_sum=0;
  Double_t protons_sum=0;
  Double_t sum_outer=0;
  Double_t sum_all=0;
  Double_t sum_all_protons=0;
  Double_t sum_all_betas=0;

  Double_t electron_charge=1.60217657e-4; //fC

  Int_t nbins=3000;
  Int_t initialX=0;
  //Int_t finalX=6000;
  Int_t finalX=12000;


  TH1F *histo=new TH1F("h_all","Charge deposit Central pad;Charge [fC];Counts",nbins/8,initialX,finalX);
  TH1F *histo_protons=new TH1F("h_protons","Charge Deposit Central pad;Charge [fC];Counts",nbins/8,initialX,finalX);
  TH1F *histo_betas=new TH1F("h_betas","Charge Deposit Central pad;Charge [fC];Counts",nbins/8,initialX,finalX);

  TH1F *histo2=new TH1F("h_all2","Charge deposit Outer pads;Charge [fC];Counts",nbins,initialX,finalX);
  TH1F *histo_protons2=new TH1F("h_protons2","Charge Deposit Outer pads;Charge [fC];Counts",nbins,initialX,finalX);
  TH1F *histo_betas2=new TH1F("h_betas2","Charge Deposit Outer pads;Charge [fC];Counts",nbins,initialX,finalX);

  TH1F *histo_sum=new TH1F("h_sum","Charge Sum central  pad;Charge [fC];Counts",nbins/8,initialX,finalX);
  TH1F *histo_sum_all=new TH1F("h_sum_all","Charge Sum all  pads;Charge [fC];Counts",nbins/8,initialX,finalX);
  TH1F *histo_sum_all_protons=new TH1F("h_sum_all_p","Charge Sum all  pads;Charge [fC];Counts",nbins/8,initialX,finalX);
 TH1F *histo_sum_all_betas=new TH1F("h_sum_all_b","Charge Sum all  pads;Charge [fC];Counts",nbins/8,initialX,finalX);
  TH1F *histo_sum_betas=new TH1F("h_sum_b"," Betas Charge Sum central  pads;Charge [fC];Counts",nbins/8,initialX,finalX);
  TH1F *histo_sum_protons=new TH1F("h_sum_p"," Proton Charge Sum central  pads;Charge [fC];Counts",nbins/8,initialX,finalX);
  TH1F *histo_antic=new TH1F("h_sum_ac","Charge Sum central  pad antic;Charge [fC];Counts",nbins/8,initialX,finalX);
  TH1F *histo_antic_betas=new TH1F("h_sum_ac_b","Charge Sum central  pad antic;Charge [fC];Counts",nbins/8,initialX,finalX);
  TH1F *histo_antic_protons=new TH1F("h_sum_ac_p","Charge Sum central  pad antic;Charge [fC];Counts",nbins/8,initialX,finalX);

  TH1F *histo_sum_outer=new TH1F("h_sum_out","Charge Sum outer pads;Charge [fC];Counts",300,0,8000);

  Bool_t out_all=kFALSE;
  Bool_t out_beta=kFALSE;
  Bool_t out_proton=kFALSE;

  Int_t threshold=40000;
  
  histo_sum_all_protons->SetLineColor(2);
  histo_sum_protons->SetLineColor(2);
  histo_antic_protons->SetLineColor(2);
  histo_protons->SetLineColor(2);
  histo_betas->SetLineColor(3);
  histo_sum_betas->SetLineColor(3);
  histo_sum_all_betas->SetLineColor(3);
  histo_antic_betas->SetLineColor(3);

  T->SetBranchAddress("Epad",E_pad);
  T->SetBranchAddress("Eprotons",E_protons); 
  T->SetBranchAddress("Ebetas",E_betas);

  Int_t n_events=T->GetEntriesFast();

  cout<<n_events<<endl;

  Bool_t electrons=kTRUE;

  for(Int_t i=0;i<n_events;i++){
    
    T->GetEntry(i);
      sum=0;
      betas_sum=0;
      protons_sum=0;
      sum_outer=0;
      sum_all=0;
      sum_all_protons=0;
      sum_all_betas=0;
      out_all=kFALSE;
      out_beta=kFALSE;
      out_proton=kFALSE;
      for(Int_t j=0;j<5;j++){
	if(!electrons){
	  E_pad[j]*=1000;
	  E_betas[j]*=1000;
	  E_protons[j]*=1000;
	}
	if(E_pad[j]>threshold)
	  sum_all+=E_pad[j];
	if(E_protons[j]>threshold)
	  sum_all_protons+=E_protons[j];
	if(E_betas[j]>threshold)
	  sum_all_betas+=E_betas[j];
	
	if(j==0){
	  if(E_pad[j]>threshold)
	    histo->Fill(E_pad[j]*electron_charge);   
	  if(E_protons[j]>threshold)
	    histo_protons->Fill(E_protons[j]*electron_charge);   
	  if(E_betas[j]>threshold)
	    histo_betas->Fill(E_betas[j]*electron_charge);   
	  sum+=E_pad[j];
	  betas_sum+=E_betas[j];
	  protons_sum+=E_protons[j];
	}
	else {
	  if(E_pad[j]>threshold){
	    histo2->Fill(E_pad[j]*electron_charge);   
	    sum_outer+=E_pad[j]*electron_charge;
	    if(!out_all)
	      out_all=kTRUE;
	  }
	  if(E_protons[j]>threshold){
	    histo_protons2->Fill(E_protons[j]*electron_charge);   
	    if(!out_proton)
	      out_proton=kTRUE;
	  }
	  if(E_betas[j]>threshold){
	    histo_betas2->Fill(E_betas[j]*electron_charge);   
	    if(!out_beta)
	      out_beta=kTRUE; 
	  }
	}
      }
      if(sum>threshold){
	histo_sum->Fill(sum*electron_charge);  
	if(!out_all)
	  histo_antic->Fill(sum*electron_charge);
      }
      if(sum_outer>threshold){
	histo_sum_outer->Fill(sum_outer*electron_charge);  
      }
      if(betas_sum>threshold){
	  histo_sum_betas->Fill(betas_sum*electron_charge);  
	if(!out_beta)
	  histo_antic_betas->Fill(betas_sum*electron_charge);
      }
      if(protons_sum>threshold){
	histo_sum_protons->Fill(protons_sum*electron_charge);  
	if(!out_proton)
	  histo_antic_protons->Fill(protons_sum*electron_charge);
      }
      if(sum_all>threshold)
	histo_sum_all->Fill(sum_all*electron_charge);
      if(sum_all_protons>0)
	histo_sum_all_protons->Fill(sum_all_protons*electron_charge);
      if(sum_all_betas>threshold)
      histo_sum_all_betas->Fill(sum_all_betas*electron_charge);
  
  }
  TCanvas *c=new TCanvas();
  c->Divide(2,1);
  c->cd(1);  
  histo->Draw();   
  //histo_protons->Draw("same");
  //histo_betas->Draw("same");
  Int_t MaxBin=histo_sum_all->GetMaximumBin();
  Double_t MaxBinCenter=histo_sum_all->GetBinCenter(MaxBin);
  TF1* fun=new TF1("fit","gaus",MaxBinCenter-30,MaxBinCenter+40);
  //histo->Fit(fun,"REM");
  Double_t mean=fit->GetParameter(1);
  Double_t sigma=fit->GetParameter(2);
  fit->SetRange(mean-2.5*sigma,mean+2.5*sigma);
  histo->Fit(fun,"NREM");
  fit->SetRange(mean-5*sigma,mean+5*sigma);
  fit->SetLineColor(3);
  //fit->Draw("same");
  cout<<fit->GetChisquare()<<" "<<fit->GetNDF()<<" "<<fit->GetChisquare()/fit->GetNDF()<<" "<<fit->GetProb()<<endl;;
  Double_t integral=GaussIntegral(fun->GetParameter(0),fun->GetParameter(2));
  cout<<"Events "<<n_events<<endl;
  cout<<"Entries "<<histo->GetEntries()<<endl;
  cout<<"Integral "<<integral<<endl;
  cout<<"Error "<<sqrt(pow(fun->GetParError(0)/fun->GetParameter(0),2)+pow(fun->GetParError(2)/fun->GetParameter(2),2))*integral<<endl;
  cout<<"Efficiencies "<<integral/histo->GetEntries()<<"\t"<<integral/n_events<<endl;
  //histo_protons->Draw("same");   
   // *out<<"Energy "<<atoi(filename)<<endl;
   // *out<<"Events "<<n_events<<endl;
   // *out<<"Entries "<<histo_sum_all->GetEntries()<<endl;
   // *out<<"Integral "<<integral<<endl;
   // *out<<"Error "<<sqrt(pow(fun2->GetParError(0)/fun2->GetParameter(0),2)+pow(fun2->GetParError(2)/fun2->GetParameter(2),2))*integral<<endl;
   // *out<<"Efficiencies "<<integral/histo_sum_all->GetEntries()<<"\t"<<integral/n_events<<endl;
  c->cd(2);
  cout<<"-----------------------------------------------------------------------------"<<endl;
  h_sum_all->Draw();
  histo_sum_all_protons->Draw("same");
  histo_sum_all_betas->Draw("same");
  MaxBin=histo_sum_all->GetMaximumBin();
  MaxBinCenter=histo_sum_all->GetBinCenter(MaxBin);
  TF1* fun2=new TF1("fit2","gaus",MaxBinCenter-30,MaxBinCenter+40);
  //histo_sum_all->Fit(fun2,"REM");
  integral=GaussIntegral(fun2->GetParameter(0),fun2->GetParameter(2));
  mean=fit2->GetParameter(1);
  sigma=fit2->GetParameter(2);
  fit2->SetRange(mean-2.5*sigma,mean+2.5*sigma);
  histo_sum_all->Fit(fun2,"NREM");
  fit2->SetRange(mean-5*sigma,mean+5*sigma);
  fit2->SetLineColor(3);
  //fit2->Draw("same");
  cout<<fit2->GetChisquare()<<" "<<fit2->GetNDF()<<" "<<fit2->GetChisquare()/fit2->GetNDF()<<" "<<fit2->GetProb()<<endl;;
  cout<<"Events "<<n_events<<endl;
  cout<<"Entries "<<histo_sum_all->GetEntries()<<endl;
  cout<<"Integral "<<integral<<endl;
  cout<<"Error "<<sqrt(pow(fun2->GetParError(0)/fun2->GetParameter(0),2)+pow(fun2->GetParError(2)/fun2->GetParameter(2),2))*integral<<endl;
  cout<<"Efficiencies "<<integral/histo_sum_all->GetEntries()<<"\t"<<integral/n_events<<endl;

   // *out<<"Energy "<<atoi(filename)<<endl;
   // *out<<"Events "<<n_events<<endl;
   // *out<<"Entries "<<histo_sum_all->GetEntries()<<endl;
   // *out<<"Integral "<<integral<<endl;
   // *out<<"Error "<<sqrt(pow(fun2->GetParError(0)/fun2->GetParameter(0),2)+pow(fun2->GetParError(2)/fun2->GetParameter(2),2))*integral<<endl;
   // *out<<"Efficiencies "<<integral/histo_sum_all->GetEntries()<<"\t"<<integral/n_events<<endl;
   //  *out<<"******************************************************************************"<<endl;
  //histo_betas->Draw("same");   
  //c->WaitPrimitive();
  //c->Close();
  return;
}

Double_t Norm_gaus(Double_t *x,Double_t *par){

  Double_t pi=TMath::Pi();
  Double_t exponent=pow((x[0]-par[1])/par[2],2)/2.;
  Double_t val=par[0]/sqrt(2*pi)/par[2]*exp(-exponent);
  return val;   
}

Double_t GaussIntegral(Double_t Constant,Double_t sigma){

  Double_t result=Constant*sigma*sqrt(2*TMath::Pi());

  return result;
}


void do(){
  
  Char_t filename[256];
  for(Int_t k=200;k<=2200;k=k+200){
    sprintf(filename,"cona_circ_%dkev.root",k);
    cout<<filename<<endl;
    plot_energies_circ(filename);
    cout<<"******************************************************************************"<<endl;
  }
}
