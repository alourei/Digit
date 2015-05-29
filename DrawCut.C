 {TCutG *cut3=new TCutG("cut3",51);

 for(Int_t k=0;k<51;k++){


    Double_t X=30*cos(2*k*TMath::Pi()/50);
    Double_t Y=30*sin(2*k*TMath::Pi()/50);
    cut3->SetPoint(k,X,Y);
    
 }
 cut3->Draw("same");
 }
