#include <TH2Poly.h>
#include <TVector3.h>

using namespace std;

class DetectorPadGeometry: public TH2Poly {
  
 public:
  
  DetectorPadGeometry();
  DetectorPadGeometry(const Int_t nrows);
  //DetectorPadGeometry(Double_t Xmin,Double_t Xmax, Double_t Ymin, Double_t Ymax);
  ~DetectorPadGeometry();
 private:
  
  Int_t numberOfPads;
  Int_t numberOfRows;
  Int_t *numberOfColumns;
  Int_t *padSizeX;
  Int_t *padSizeY;
  
  Double_t X0;
  Double_t Xf;
  Double_t Y0;
  Double_t Yf;
  
 public:
  
  Int_t GetNumberOfPads(){return numberOfPads;}
  Int_t GetNumberOfRows(){return numberOfRows;}
  Int_t GetNumberOfColumns(Int_t row){return numberOfColumns[row];}
  Int_t GetPadSizeX(Int_t row){return padSizeX[row];}
  Int_t GetPadSizeY(Int_t row){return padSizeX[row];}
  
  void SetNumberOfPads(Int_t npads){numberOfPads=npads;}
  void SetNumberOfRows(Int_t nrows){numberOfRows=nrows;}
  void SetNumberOfColumns(Int_t row,Int_t column){numberOfColumns[row]=column;}
  void SetPadSizeX(Int_t row,Int_t size){padSizeX[row]=size;}
  void SetPadSizeY(Int_t row,Int_t size){padSizeY[row]=size;}

  void SetPadSizesX(Int_t *size);
  void SetPadSizesY(Int_t *size);
  void SetNumberOfColumns(Int_t *column);

  Int_t GetRowNumber(TVector3 position);

  Int_t GetColumnNumber(TVector3 position);

  Int_t CalculatePad(Int_t row,Int_t column);

  Int_t CalculatePad(TVector3 position);

  void ConstructGeometry();

  void AddBinContent(Int_t bin,Double_t content);

  ClassDef(DetectorPadGeometry,1);

};




