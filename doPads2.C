{
  TH2Poly *pads= new TH2Poly("pads","Circular Pads",-85,85,-85,85);
  Double_t x1[200],y1[200];
  Double_t x2[200],y2[200];
  Double_t x3[200],y3[200];
  Double_t x[402],y[402];
  Double_t radius=25;
  Float_t radius2=68;
  Float_t radius3=75;
  Double_t pi=TMath::Pi();
  for(Int_t k=0;k<=200;k++){
    x1[k]=radius*cos(2*pi/200*k);
    y1[k]=radius*sin(2*pi/200*k);
    x2[k]=radius2*cos(2*pi/200*k);
    y2[k]=radius2*sin(2*pi/200*k);
    x3[k]=radius3*cos(2*pi/200*k);
    y3[k]=radius3*sin(2*pi/200*k);
  }
  pads->AddBin(200,x1,y1);

  //pads->AddBin(200,x3,y3);
  
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
    //cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
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
    //cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
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
    //cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
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
    //cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
  }
 pads->AddBin(102,x,y);

 for(Int_t k=0;k<402;k++){
   if(k<=200){
      x[k]=x3[k];
      y[k]=y3[k];
   }
    else if(k==201){
      x[k]=x2[0];
      y[k]=y2[0];
    
    }
    else if(k>201&&k<=400){
      x[k]=x2[k-200];
      y[k]=y2[k-200];
    }
   cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
  }
   pads->AddBin(401,x,y);

  //pads->AddBin(201,x3,y3);
  pads->FillRandom("gaus",100);
   //pads->SetBinContent(6,50);
   pads->Draw("Lcolz");
  
}
