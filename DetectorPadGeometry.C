#include <iostream>
#include "DetectorPadGeometry.h"

ClassImp(DetectorPadGeometry);


DetectorPadGeometry::DetectorPadGeometry(){
  //Default Constructor
}

DetectorPadGeometry::DetectorPadGeometry(const Int_t nRows){

  X0=-90;
  Y0=-30;
  numberOfRows=nRows;
  numberOfColumns=new Int_t[numberOfRows];
  padSizeX=new Int_t[numberOfRows];
  padSizeY=new Int_t[numberOfRows];

}

DetectorPadGeometry::~DetectorPadGeometry(){

  delete[] numberOfColumns;
  delete[] padSizeX;
  delete[] padSizeY;

}


void DetectorPadGeometry::SetPadSizesX(Int_t *size){
  
  for(Int_t i=0;i<numberOfRows;i++){
    SetPadSizeX(i,size[i]);
  }
}

void DetectorPadGeometry::SetPadSizesY(Int_t *size){
  
  for(Int_t i=0;i<numberOfRows;i++){
    SetPadSizeY(i,size[i]);
  }
}


void DetectorPadGeometry::SetNumberOfColumns(Int_t *column){

  for(Int_t i=0;i<numberOfRows;i++){
    SetNumberOfColumns(i,column[i]);
  }

}


Int_t DetectorPadGeometry::GetRowNumber(TVector3 position){

  Double_t x=position.X();
  Double_t z=position.Z();

  Double_t sumLength=Y0;
  Int_t row=-1;

  for(Int_t i=0;i<numberOfRows;i++){
    //cout<<"X "<<x<<" pad "<<padSizeY[i]<<" Sum "<<sumLength<<endl;
    if(x>sumLength && x<sumLength+padSizeY[i]){
      row=i+1;
      break;
    }
    else
      sumLength+=padSizeY[i];
  }
  return row;
}


Int_t DetectorPadGeometry::GetColumnNumber(TVector3 position){

  Int_t Row=GetRowNumber(position);

  //cout<<Row<<endl;

  Double_t z=position.Z();

  Double_t sumLength=X0;
  Int_t column=-1;

  for(Int_t i=0;i<numberOfColumns[Row-1];i++){
    //cout<<"X "<<z<<" pad "<<padSizeX[Row-1]<<" Sum "<<sumLength<<endl;
    if(z>sumLength && z<sumLength+padSizeX[Row-1]){
      column=i+1;
      break;
    }
    else
      sumLength+=padSizeX[Row-1];
  }
  return column;
}


Int_t DetectorPadGeometry::CalculatePad(Int_t row,Int_t column){

  //cout<<"row "<<row<<" nCols["<<row<<"]"<<numberOfColumns[row-1]<<endl;

  if(row<0 || row>numberOfRows||column>numberOfColumns[row-1]||column<=0)
    return -1;
  else{
    Int_t pad=0;
    for(Int_t i=0;i<row;i++){
      for(Int_t j=0;j<numberOfColumns[i-1];j++){
	pad++;
      }
    }
  return pad+column;
  }
}

Int_t DetectorPadGeometry::CalculatePad(TVector3 position){

Int_t row=GetRowNumber(position);
Int_t column=GetColumnNumber(position);

 return CalculatePad(row,column);

}


void DetectorPadGeometry::AddBinContent(Int_t bin,Double_t content){
  Double_t value=this->GetBinContent(bin);
  value+=content;
  this->SetBinContent(bin,value);
  return;
}


void DetectorPadGeometry::ConstructGeometry(){

  Int_t sumX=X0;
  Int_t sumY=Y0;
  
  for(Int_t row=0;row<numberOfRows;row++){
    
    for(Int_t column=0;column<numberOfColumns[row];column++){
      numberOfPads++; 
      this->AddBin(sumX+column*padSizeX[row],sumY,sumX+(column+1)*padSizeX[row],sumY+(1)*padSizeY[row]);
       cout<<row<<" "<<column<<" Bin "<<sumX+column*padSizeX[row]<<" "<<sumY<<" "<<sumX+(column+1)*padSizeX[row]<<" "<<sumY+(1)*padSizeY[row]<<endl;
    }
    //sumX+=padSizeX[row];
    sumY+=padSizeY[row];
    cout<<"------------------------------------------------------------------"<<endl;
    
  }
  
}
