/*
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
  Int_t nr_iter = 47;
  Float_t iter[nr_iter], real_theta[nr_iter], real_phi[nr_iter], out_theta[nr_iter], out_phi[nr_iter], error[nr_iter];
 
  //Fill iterations vector
  for (Int_t i = 0; i < nr_iter; i++)
    iter[i] = (float)i+1;
 
  //Open data file
  ifstream in;
  in.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_LEAKY_RELU_2L_40400_01.dat", ios::in);

  //Create temporary use variables
  Float_t real_t, real_p, out_t, out_p, err, title_r, title_o;
 
  //in >> title_r >> real_t >> real_p >> title_o >> out_t >> out_p >> err;
  //Start reading from the 2n line of the file
  Int_t nlines = 0;
  while (1){
    //Get the five columns
    in >> real_t >> real_p >> out_t >> out_p >> err;
    if(!in.good()) break;
    if(nlines < nr_iter + 1)
      //Print the read data
      //printf("real theta=%8f, real phi=%8f, out theta=%8f, out phi=%8f, error=%8f\n", real_t, real_p, out_t, out_p, err);
    //Fill the data vectors
    real_theta[nlines] = real_t;
    real_phi[nlines]   = real_p;
    out_theta[nlines]  = out_t*90;
    out_phi[nlines]    = out_p*360-180;
    error[nlines]      = err;
    nlines++;
  }

  //for(Int_t i=0; i < nr_iter; i++)
  //cout << iter[i] << " " << real_theta[i] << " " << real_phi[i] << " " << out_theta[i] << " " << out_phi[i] << " " << error[i] << endl;

  cout << nlines << endl;
 
  //Declare graph
  TGraph *ERvsIT = new TGraph(nr_iter, iter, error);
  ERvsIT->SetTitle("Error vs Iterations");

  TGraph *ERvsRT = new TGraph(nr_iter, real_theta, error);
  ERvsRT->SetTitle("Error vs Real Theta");

  TGraph *ERvsOT = new TGraph(nr_iter, out_theta, error);
  ERvsOT->SetTitle("Error vs Output Theta");

  TGraph *ERvsRP = new TGraph(nr_iter, real_phi, error);
  ERvsRP->SetTitle("Error vs Real Phi");

  TGraph *ERvsOP = new TGraph(nr_iter, out_phi, error);
  ERvsOP->SetTitle("Error vs Output Phi");

  //Declare canvas
  TCanvas *canvas = new TCanvas();
  canvas->Divide(2,3);
  //Draw graphs
  canvas->cd(1);
  ERvsOT->Draw("ap");

  canvas->cd(2);
  ERvsRT->Draw("ap");

  canvas->cd(3);
  ERvsOP->Draw("ap");

  canvas->cd(4);
  ERvsRP->Draw("ap");

  canvas->cd(5);
  ERvsIT->Draw();

  //Close data file
  in.close();
 
  return;

}

int main(){

  draw();

  return 0;

}
