/*
** name: draw_poisson.cpp
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
#include "TMath.h"

using namespace std;

void draw_poisson(){

  //Declare value vectors
  Int_t nr_iter = 40401;
  Float_t iter[nr_iter], real_xmom[nr_iter], real_ymom[nr_iter], out_xmom[nr_iter], out_ymom[nr_iter], error[nr_iter];
 
  //Fill iterations vector
  for (Int_t i = 0; i < nr_iter; i++)
    iter[i] = (float)i+1;
 
  //Open data files
  ifstream in;
  in.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_TANHL_3_epoch_1_Poisson.dat", ios::in);

  //Create temporary use variables
  Float_t real_x2, real_y2, real_z, out_x2, out_y2, err;
  Int_t t;
 
  Int_t nlines = 0;
  while (1){
    //Get the five columns
    in >> real_x2 >> real_y2 >> real_z >> t >> out_x2 >> out_y2 >> err;
    if(!in.good()) break;
    if(nlines < nr_iter)
    //Fill the data vectors
    real_xmom[nlines] = 2*real_x2-1;
    real_ymom[nlines] = 2*real_y2-1;
    out_xmom[nlines]  = 2*out_x2-1;
    out_ymom[nlines]  = 2*out_y2-1;
    error[nlines]     = err;
    nlines++;
  }
  
  cout << "Number of lines in NN_data_TANHL_3_epoch_1_Poisson.dat :" << nlines << endl;
 
  //Declare graph
  TGraph *ERvsIT = new TGraph(nr_iter, iter, error);
  ERvsIT->SetTitle("Error vs Iterations (3 layers 1 epoch Poisson)");
  
  //draw graph 
  TCanvas *Canvas = new TCanvas("ERvsIT","ERvsIT");
  ERvsIT->Draw("al");
  ERvsIT->SetTitle("Error vs Iterations");
  ERvsIT->GetXaxis()->SetTitle("iterations");
  ERvsIT->GetYaxis()->SetTitle("error");
  gPad->Update();
  gPad->Modified();
  
  //Close data files
  in.close();
 
  return;

}

int main(){

  draw_poisson();

  return 0;

}
