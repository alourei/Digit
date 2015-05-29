{
  TFile *file=new TFile("digi_23Al_box.root");

  TTree *T=(TTree*)file->Get("theTree");

  Double_t E_protons[100];
  Double_t E_pad[100];
  Double_t E_betas[100];

  Double_t sum=0;
  Double_t betas_sum=0;
  Double_t protons_sum=0;
  Double_t sum_outer=0;

  Double_t electron_charge=1.60217657e-4; //fC

  TH1F *histo=new TH1F("h_all","Charge deposit Central pads;Charge [fC];Counts",500,0,8000);
  TH1F *histo_protons=new TH1F("h_protons","Charge Deposit Central pads;Charge [fC];Counts",500,0,8000);
  TH1F *histo_betas=new TH1F("h_betas","Charge Deposit Central pads;Charge [fC];Counts",500,0,8000);

  TH1F *histo2=new TH1F("h_all2","Charge deposit Outer pads;Charge [fC];Counts",500,0,8000);
  TH1F *histo_protons2=new TH1F("h_protons2","Charge Deposit Outer pads;Charge [fC];Counts",500,0,8000);
  TH1F *histo_betas2=new TH1F("h_betas2","Charge Deposit Outer pads;Charge [fC];Counts",500,0,8000);

  TH1F *histo_sum=new TH1F("h_sum","Charge Sum central  pads;Charge [fC];Counts",500,0,8000);
  TH1F *histo_sum_betas=new TH1F("h_sum_b"," Betas Charge Sum central  pads;Charge [fC];Counts",500,0,8000);
  TH1F *histo_sum_protons=new TH1F("h_sum_p"," Proton Charge Sum central  pads;Charge [fC];Counts",500,0,8000);
  TH1F *histo_antic=new TH1F("h_sum_ac","Charge Sum central  pads antic;Charge [fC];Counts",500,0,8000);
  TH1F *histo_antic_betas=new TH1F("h_sum_ac_b","Charge Sum central  pads antic;Charge [fC];Counts",500,0,8000);
  TH1F *histo_antic_protons=new TH1F("h_sum_ac_p","Charge Sum central  pads antic;Charge [fC];Counts",500,0,8000);

  TH1F *histo_sum_outer=new TH1F("h_sum_out","Charge Sum outer pads;Charge [fC];Counts",500,0,8000);

  Bool_t out=kFALSE;
  Bool_t out_beta=kFALSE;
  Bool_t out_proton=kFALSE;

  Int_t threshold=10000;
  
  histo_sum_protons->SetLineColor(2);
  histo_protons->SetLineColor(2);
  histo_betas->SetLineColor(3);

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
      out=kFALSE;
      out_beta=kFALSE;
      out_proton=kFALSE;
      for(Int_t j=0;j<15;j++){
	if(!electrons){
	  E_pad[j]*=1000;
	  E_betas[j]*=1000;
	  E_protons[j]*=1000;
	}

      if(j>2&&j<12){
	if(E_pad[j]>threshold)
	  histo->Fill(E_pad[j]*electron_charge);   
	if(E_protons[j]>threshold)
	  histo_protons->Fill(E_protons[j]*electron_charge);   
	if(E_betas[j]>threshold)
	  histo_betas->Fill(E_betas[j]*electron_charge);   
	if(j>4&&j<10){
	  sum+=E_pad[j]*electron_charge;
	  betas_sum+=E_betas[j]*electron_charge;
	  protons_sum+=E_protons[j]*electron_charge;
	}
      }
      else if((j>0&&j<5)||(j>9&&j<15)){
	if(E_pad[j]>threshold){
	  histo2->Fill(E_pad[j]*electron_charge);   
	  sum_outer+=E_pad[j]*electron_charge;
	  if(!out)
	    out=kTRUE;
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
      if(sum>0){
	histo_sum->Fill(sum);  
	if(!out)
	  histo_antic->Fill(sum);
      }
      if(sum_outer>0){
	histo_sum_outer->Fill(sum_outer);  
      }
      if(betas_sum>0){
	  histo_sum_betas->Fill(betas_sum);  
	if(!out_beta)
	  histo_antic_betas->Fill(betas_sum);
      }
      if(protons_sum>0){
	histo_sum_protons->Fill(protons_sum);  
	if(!out_proton)
	  histo_antic_protons->Fill(protons_sum);
      }
      
  }
  
  histo->Draw();   
  histo_protons->Draw("same");   
  histo_betas->Draw("same");   

}
