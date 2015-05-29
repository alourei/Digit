{
  TH2Poly *pads= new TH2Poly("pads","Circular Pads",-60,60,-60,60);
  Double_t x1[200],y1[200];
  Double_t x2[200],y2[200];
  Double_t x[200],y[200];
  Double_t radius=30;
  Float_t radius2=60;
  Double_t pi=TMath::Pi();
  for(Int_t k=0;k<200;k++){
    x1[k]=radius*cos(2*pi/200*k);
    y1[k]=radius*sin(2*pi/200*k);
    x2[k]=radius2*cos(2*pi/200*k);
    y2[k]=radius2*sin(2*pi/200*k);
  }
  pads->AddBin(200,x1,y1);
  
  pads->SetBinContent(1,50);
  for(Int_t k=0;k<102;k++){
    if(k<=50){
      x[k]=x2[k];
      y[k]=y2[k];
    }
    else if(k==51){
      x[k]=x1[k-1];
      y[k]=y1[k-1];
    
    }
    else if (k>51&&k<=100){
      x[k]=-x1[k];
      y[k]=y1[k];
    }
    else if (k==101){
      x[k]=x2[0];
      y[k]=y2[0];    
    }
    cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
  }
  pads->AddBin(102,x,y);
  for(Int_t k=0;k<102;k++){
    if(k<=50){
      x[k]=-x2[k];
      y[k]=y2[k];
    }
    else if(k==51){
      x[k]=x1[k-1];
      y[k]=y1[k-1];
    
    }
    else if (k>51&&k<=100){
      x[k]=x1[k];
      y[k]=y1[k];
    }
    else if (k==101){
      x[k]=-x2[0];
      y[k]=y2[0];    
    }
    cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
  }
  pads->AddBin(102,x,y);
  for(Int_t k=0;k<102;k++){
    if(k<=50){
      x[k]=-x2[k];
      y[k]=-y2[k];
    }
    else if(k==51){
      x[k]=-x1[k-1];
      y[k]=-y1[k-1];
    
    }
    else if (k>51&&k<=100){
      x[k]=x1[k];
      y[k]=-y1[k];
    }
    else if (k==101){
      x[k]=-x2[0];
      y[k]=y2[0];    
    }
    cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
  }
  pads->AddBin(102,x,y);
 for(Int_t k=0;k<102;k++){
    if(k<=50){
      x[k]=x2[k];
      y[k]=-y2[k];
    }
    else if(k==51){
      x[k]=x1[k-1];
      y[k]=-y1[k-1];
    
    }
    else if (k>51&&k<=100){
      x[k]=-x1[k];
      y[k]=-y1[k];
    }
    else if (k==101){
      x[k]=x2[0];
      y[k]=y2[0];    
    }
    cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
  }
  pads->AddBin(102,x,y);
  //pads->FillRandom("gaus",5000);
  pads->Draw("L");
  
}
