{
//=========Macro generated from canvas: c1_n2/c1_n2
//=========  (Thu Apr 24 16:12:22 2014) by ROOT version5.34/09
  TCanvas *cancas=new TCanvas();
  gStyle->SetLabelSize(0.05,"XY");
  gStyle->SetTitleSize(0.05,"XY");
  gStyle->SetOptStat(0);
  h_all->SetTitle("Protons+Betas");
  h_betas->SetTitle("Betas");
  h_all->SetLineWidth(2);
  h_betas->SetLineWidth(2);
  h_all->SetLineColor(1);
  h_betas->SetLineColor(1);
  h_betas->SetFillColor(1);
  h_betas->SetFillStyle(3004);

  h_all->GetYaxis()->SetRangeUser(0,150);
  h_all->GetXaxis()->SetRangeUser(0,6000);
  h_all->Draw();
  h_betas->Draw("same");

  TLegend *leg=new TLegend(0.5,0.65,0.88,0.88);
  leg->SetFillColor(0);
  leg->AddEntry(h_all,"Protons+Betas","LF");
  leg->AddEntry(h_betas,"Betas","LF");

  leg->Draw();

      tex = new TLatex(1292.683,76.51822,"197 keV");
   tex->SetTextSize(0.04115226);
   tex->SetTextAngle(90);
   tex->SetLineWidth(2);
   tex->Draw();
     tex = new TLatex(1586.21,49.18,"222 keV");
   tex->SetTextSize(0.04115226);
   tex->SetTextAngle(90);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(1739.837,94.73684,"255 keV");
   tex->SetTextSize(0.04115226);
   tex->SetTextAngle(90);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(3630.081,83.72976,"554 keV");
   tex->SetTextSize(0.04115226);
   tex->SetTextAngle(90);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(5256.098,85.62753,"828 keV");
   tex->SetTextSize(0.04115226);
   tex->SetTextAngle(90);
   tex->SetLineWidth(2);
   tex->Draw();
}
