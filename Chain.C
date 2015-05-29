{
  //TFile *fil-new TFile("digiChain.root","new");

  TChain *ch=new TChain("theTree");
  ch->Add(" digiFile_1.root");
  ch->Add(" digiFile_2.root");
  ch->Add(" digiFile_3.root");
  ch->Add(" digiFile_4.root");
  ch->Add(" digiFile_5.root");
  ch->Add(" digiFile_6.root");
  ch->Add(" digiFile_7.root");

  ch->Merge("digiFile_Chain.root");
   
}
