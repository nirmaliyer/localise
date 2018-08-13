/*
** name: draw.cpp
** author : Joan Bachs Esteban
** email : bachsjoan@gmail.com
*/

#include "TCanvas.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"
#include <fstream>

using namespace std;

void draw(){

  //Declare value vectors
  Int_t nr_iter = 31397;
  Float_t iter[nr_iter], real_xmom[nr_iter], real_ymom[nr_iter], out_xmom[nr_iter], out_ymom[nr_iter], error[nr_iter];
 
  //Fill iterations vector
  for (Int_t i = 0; i < nr_iter; i++)
    iter[i] = (float)i+1;
 
  //Open data file
  ifstream in_2;
  in_2.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_TANHL_2.dat", ios::in);

  //Create temporary use variables
  Float_t real_x2, real_y2, out_x2, out_y2, err;
 
  Int_t nlines = 0;
  while (1){
    //Get the five columns
    in_2 >> real_x2 >> real_y2 >> out_x2 >> out_y2 >> err;
    if(!in_2.good()) break;
    if(nlines < nr_iter)
    //Fill the data vectors
    real_xmom[nlines] = 2*real_x2-1;
    real_ymom[nlines] = 2*real_y2-1;
    out_xmom[nlines]  = 2*out_x2-1;
    out_ymom[nlines]  = 2*out_y2-1;
    error[nlines]     = err;
    nlines++;
  }
  
  cout << nlines << endl;
 
  //Declare graph
  TGraph *ERvsIT_2 = new TGraph(nr_iter, iter, error);
  ERvsIT_2->SetTitle("Error vs Iterations (2 layers)");

  TGraph *ERvsRX_2 = new TGraph(nr_iter, real_xmom, error);
  ERvsRX_2->SetTitle("Error vs Real x-momentum (2 layers)");

  TGraph *ERvsOX_2 = new TGraph(nr_iter, out_xmom, error);
  ERvsOX_2->SetTitle("Error vs Output x-momentum (2 layers)");

  TGraph *ERvsRY_2 = new TGraph(nr_iter, real_ymom, error);
  ERvsRY_2->SetTitle("Error vs Real y-momentum (2 layers)");

  TGraph *ERvsOY_2 = new TGraph(nr_iter, out_ymom, error);
  ERvsOY_2->SetTitle("Error vs Output y-momentum (2 layer)s");

  //Open data file
  ifstream in_3;
  in_3.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_TANHL_3.dat", ios::in);
  
  nlines = 0;
  while (1){
    //Get the five columns
    in_3 >> real_x2 >> real_y2 >> out_x2 >> out_y2 >> err;
    if(!in_3.good()) break;
    if(nlines < nr_iter)
    //Fill the data vectors
    real_xmom[nlines] = 2*real_x2-1;
    real_ymom[nlines] = 2*real_y2-1;
    out_xmom[nlines]  = 2*out_x2-1;
    out_ymom[nlines]  = 2*out_y2-1;
    error[nlines]     = err;
    nlines++;
  }
  
  cout << nlines << endl;
 
  //Declare graph
  TGraph *ERvsIT_3 = new TGraph(nr_iter, iter, error);
  ERvsIT_3->SetTitle("Error vs Iterations (3 layers)");

  TGraph *ERvsRX_3 = new TGraph(nr_iter, real_xmom, error);
  ERvsRX_3->SetTitle("Error vs Real x-momentum (3 layers)");

  TGraph *ERvsOX_3 = new TGraph(nr_iter, out_xmom, error);
  ERvsOX_3->SetTitle("Error vs Output x-momentum (3 layers)");

  TGraph *ERvsRY_3 = new TGraph(nr_iter, real_ymom, error);
  ERvsRY_3->SetTitle("Error vs Real y-momentum (3 layers)");

  TGraph *ERvsOY_3 = new TGraph(nr_iter, out_ymom, error);
  ERvsOY_3->SetTitle("Error vs Output y-momentum (3 layers)");

  //Declare canvas
  TCanvas *canvas = new TCanvas();
  canvas->Divide(2,1);

  //Draw graohs
  canvas->cd(1);
  ERvsIT_2->Draw();
  canvas->cd(2);
  ERvsIT_3->Draw();

  //Close data files
  in_2.close();
  in_3.close();
 
  return;

}

int main(){

  draw();

  return 0;

}
