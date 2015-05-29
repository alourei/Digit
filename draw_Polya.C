#include "read_simFile.C"

void draw_Polya(){

  TH1F *hist=new TH1F("h","Polya Distribution;Gain",500,0,5000);
  hist->SetLineColor(2);
  for(Int_t k=0;k<50000;k++)
    hist->Fill(1000*Polya());
  hist->Draw();

}
