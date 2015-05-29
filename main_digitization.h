#include <cmath>
#include <TROOT.h>
#include <iostream>

#include "DetectorPadGeometry.h"

Float_t Polya(Float_t param);
DetectorPadGeometry *CreateGeometry2(Char_t *name);
DetectorPadGeometry *CreateGeometry3(Char_t *name);
DetectorPadGeometry *CreateGeometry(Char_t *name);
static  inline void loadbar(unsigned int k, unsigned int n, unsigned int w);


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

TH2Poly *CreateCircularPads(Char_t *name="pads"){
  TH2Poly *pads= new TH2Poly(name,"Circular Pads;X [mm];Y[mm];Charge [e^{-}]",-85,85,-85,85);
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
  //pads->FillRandom("gaus",5000);
  //pads->Draw("L");
  return pads;
}

TH2Poly *CreateCircularPads_2(Char_t *name="pads_modified"){
  TH2Poly *pads= new TH2Poly("pads","Circular Pads",-85,85,-85,85);
  Double_t x1[201],y1[201];
  Double_t x2[201],y2[201];
  Double_t x3[201],y3[201];
  Double_t x[402],y[402];
  Double_t radius=30;
  Double_t radius2=72;
  Double_t radius3=80;
  Double_t pi=TMath::Pi();
  for(Int_t k=0;k<=200;k++){
    x1[k]=radius*cos(2*k*pi/200);
    y1[k]=radius*sin(2*k*pi/200);
    x2[k]=radius2*cos(2*k*pi/200);
    y2[k]=radius2*sin(2*k*pi/200);
    x3[k]=0;
    x3[k]=radius3*TMath::Cos(2*k*pi/200);
    //cout<<k<<" "<<(2*k*pi/200)<<" "<<cos(2*k*pi/200)<<" "<<sin(2*k*pi/200)<<endl;
    y3[k]=0;
    y3[k]=radius3*TMath::Sin(2*k*pi/200);
    //cout<<"x3,y3"<<" "<<x3[k]<<" "<<y3[k]<<endl;
  }
  //cout<<"x3,y3"<<" "<<x3[0]<<" "<<y3[0]<<endl;
  pads->AddBin(201,x1,y1);

  
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
  //pads->AddBin(102,x,y);
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

 //cout<<"x3,y3"<<" "<<x3[0]<<" "<<y3[0]<<endl;
 //cout<<"x2,y2"<<" "<<x2[0]<<" "<<y2[0]<<endl;
 for(Int_t k=0;k<402;k++){
   if(k==0){
      x[k]=x3[k];
      y[k]=y3[k];
   }
   else if(k>0&&k<=200){
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
   //cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
  }
   pads->AddBin(401,x,y);

  //pads->AddBin(201,x3,y3);
  //pads->FillRandom("gaus",100);
   //pads->SetBinContent(6,50);
   //pads->Draw("L");
   //gPad->WaitPrimitive();
  return pads;
  
}

TH2Poly *CreateCircularPads_3(Char_t *name="pads_modified"){
  TH2Poly *pads= new TH2Poly("pads","Circular Pads",-85,85,-85,85);
  Double_t x1[201],y1[201];
  Double_t x2[201],y2[201];
  Double_t x3[201],y3[201];
  Double_t x[402],y[402];
  Double_t radius=30;
  Double_t radius2=70;
  Double_t radius3=75;
  Double_t pi=TMath::Pi();
  for(Int_t k=0;k<=200;k++){
    x1[k]=radius*cos(2*k*pi/200);
    y1[k]=radius*sin(2*k*pi/200);
    x2[k]=radius2*cos(2*k*pi/200);
    y2[k]=radius2*sin(2*k*pi/200);
    x3[k]=0;
    x3[k]=radius3*TMath::Cos(2*k*pi/200);
    //cout<<k<<" "<<(2*k*pi/200)<<" "<<cos(2*k*pi/200)<<" "<<sin(2*k*pi/200)<<endl;
    y3[k]=0;
    y3[k]=radius3*TMath::Sin(2*k*pi/200);
    //cout<<"x3,y3"<<" "<<x3[k]<<" "<<y3[k]<<endl;
  }
  //cout<<"x3,y3"<<" "<<x3[0]<<" "<<y3[0]<<endl;
  pads->AddBin(201,x1,y1);

  
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
  //pads->AddBin(102,x,y);
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

 //cout<<"x3,y3"<<" "<<x3[0]<<" "<<y3[0]<<endl;
 //cout<<"x2,y2"<<" "<<x2[0]<<" "<<y2[0]<<endl;
 for(Int_t k=0;k<402;k++){
   if(k==0){
      x[k]=x3[k];
      y[k]=y3[k];
   }
   else if(k>0&&k<=200){
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
   //cout<<k<<" "<<x[k]<<" "<<y[k]<<endl;
  }
   pads->AddBin(401,x,y);

  //pads->AddBin(201,x3,y3);
  //pads->FillRandom("gaus",100);
   //pads->SetBinContent(6,50);
   //pads->Draw("L");
   //gPad->WaitPrimitive();
  return pads;
  
}




Double_t Pulse_gaus (Double_t *x,Double_t *par){
  
  Int_t j=1;
  Double_t result=0;

  while(par[j]!=0){
    Double_t aux=(x[0]-par[2*j])/par[0];
    aux=pow(aux,2);
    result+=par[2*j-1]*exp(-aux/2);
    j++;
  }
  if(par[0]==0) 
    result=0;
  return result;
}


Double_t Noise(Double_t sigma){

  return gRandom->Gaus(0,sigma);

}


Double_t Pulse_gaus_noise (Double_t *x,Double_t *par){
  
  return Pulse_gaus(x,par)+Noise(500);
}


Double_t Pulse_AGET (Double_t *x,Double_t *par){
  
  Int_t j=1;
  Double_t result=0;

  while(par[j]!=0){
    Double_t aux=(x[0]-par[2*j])/par[0];
    aux=exp(-3.*aux)*sin(aux)*pow(aux,3);
    result+=par[2*j-1]*aux;
    j++;
  }
  
  return result;
}

Double_t Pulse_AGET_noise (Double_t *x,Double_t *par){
  
  return Pulse_AGET(x,par)+Noise(2500);
}


