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
  Float_t iter[nr_iter], real_theta[nr_iter], real_phi[nr_iter], out_theta[nr_iter], out_phi[nr_iter], error[nr_iter];
 
  //Fill iterations vector
  for (Int_t i = 0; i < nr_iter; i++)
    iter[i] = (float)i+1;
 
  //Open data file
  ifstream in;
  in.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_TANHL.dat", ios::in);

  //Create temporary use variables
  Float_t real_t, real_p, out_t, out_p, err;
 
  Int_t nlines = 0;
  while (1){
    //Get the five columns
    in >> real_t >> real_p >> out_t >> out_p >> err;
    if(!in.good()) break;
    if(nlines < nr_iter + 1)
    //Fill the data vectors
    real_theta[nlines] = real_t;
    real_phi[nlines]   = real_p;
    out_theta[nlines]  = out_t*90;
    out_phi[nlines]    = out_p*360-180;
    error[nlines]      = err;
    nlines++;
  }
  
  cout << nlines << endl;
 
  //Declare graph
  TGraph *ERvsIT = new TGraph(nr_iter, iter, error);
  ERvsIT->SetTitle("Error vs Iterations 2 layers");

  TGraph *ERvsRT = new TGraph(nr_iter, real_theta, error);
  ERvsRT->SetTitle("Error vs Real Theta 2 layers");

  TGraph *ERvsOT = new TGraph(nr_iter, out_theta, error);
  ERvsOT->SetTitle("Error vs Output Theta 2 layers");

  TGraph *ERvsRP = new TGraph(nr_iter, real_phi, error);
  ERvsRP->SetTitle("Error vs Real Phi 2 layers");

  TGraph *ERvsOP = new TGraph(nr_iter, out_phi, error);
  ERvsOP->SetTitle("Error vs Output Phi 2 layers");

  //Open data file
  ifstream in_3;
  in_3.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_TANHL_3.dat", ios::in);

 Int_t nlines_3 = 0;
  while (1){
    //Get the five columns
    in_3 >> real_t >> real_p >> out_t >> out_p >> err;
    if(!in_3.good()) break;
    if(nlines_3 < nr_iter + 1)
    //Fill the data vectors
    real_theta[nlines_3] = real_t;
    real_phi[nlines_3]   = real_p;
    out_theta[nlines_3]  = out_t*90;
    out_phi[nlines_3]    = out_p*360-180;
    error[nlines_3]      = err;
    nlines_3++;
  }
  
  cout << nlines_3 << endl;
 
  //Declare graph
  TGraph *ERvsIT_3 = new TGraph(nr_iter, iter, error);
  ERvsIT_3->SetTitle("Error vs Iterations 3 layers");

  TGraph *ERvsRT_3 = new TGraph(nr_iter, real_theta, error);
  ERvsRT_3->SetTitle("Error vs Real Theta 3 layers");

  TGraph *ERvsOT_3 = new TGraph(nr_iter, out_theta, error);
  ERvsOT_3->SetTitle("Error vs Output Theta 3 layers");

  TGraph *ERvsRP_3 = new TGraph(nr_iter, real_phi, error);
  ERvsRP_3->SetTitle("Error vs Real Phi 3 layers");

  TGraph *ERvsOP_3 = new TGraph(nr_iter, out_phi, error);
  ERvsOP_3->SetTitle("Error vs Output Phi 3 layers");

  //Declare canvas
  TCanvas *canvas = new TCanvas();
  canvas->Divide(2,1);
  //Draw graphs
  canvas->cd(1);
  //ERvsOT->Draw("ap");

  //canvas->cd(2);
  //ERvsRT->Draw("ap");

  //canvas->cd(3);
  //ERvsOP->Draw("ap");

  //canvas->cd(4);
  //ERvsRP->Draw("ap");

  //canvas->cd(5);
  ERvsIT->Draw();

  canvas->cd(2);
  ERvsIT_3->Draw();

  //Close data file
  in.close();
 
  return;

}

int main(){

  draw();

  return 0;

}
