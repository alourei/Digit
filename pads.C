{
   TCanvas *ch2p2 = new TCanvas("ch2p2","ch2p2",600,400);
   TH2Poly *h2p = new TH2Poly();
   h2p->SetName("Boxes");
   h2p->SetTitle("Boxes");
   h2p->SetLineWidth(2);
   h2p->SetLineColor(2);
   gStyle->SetPalette(1);
   Int_t i,j;
   Int_t nx = 3;
   Int_t x1,y1,x2,y2;
   Int_t dx=40,dy=20;
   for (i = 0; i<=nx; i++) {
      y1 = -40.;
      y2 = y1+dy;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }

  Int_t dx=20,dy=40;
  nx=5;
  x1=0;x2=dx;
   for (i = 0; i<=nx; i++) {
      y1 = -20.;
      y2 = y1+dy;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y1+dy;
      x1 = x2;
      x2 = x2+dx;
   }
  nx=2;
  Int_t dx=40,dy=20;
  x1=0;x2=dx;
   for (i = 0; i<=nx; i++) {
      y1 = 20.;
      y2 = y1+dy;
         h2p->AddBin(x1, y1, x2, y2);
         y1 = y2;
         y2 = y2+dy;
       x1 = x2;
      x2 = x2+dx;
   }

   h2p->Draw();
}
