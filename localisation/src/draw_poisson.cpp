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
  Int_t nr_iter = 40401, nr_inpoints = 40398;
  Float_t iter[nr_iter], inpoints[nr_inpoints], real_xmom[nr_inpoints], real_ymom[nr_inpoints], real_zmom[nr_inpoints], out_xmom[nr_inpoints], out_ymom[nr_inpoints], out_zmom[nr_inpoints], error[nr_inpoints], ang_dist[nr_inpoints];
 
  //Fill iterations vector
  for (Int_t i = 0; i < nr_iter; i++)
    iter[i] = (float)i+1;
 
  //Open data files
  ifstream in;
  in.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_LEAKY_RELU_2_epoch_1_Poisson.dat", ios::in);

  //Create temporary use variables
  Float_t real_x2, real_y2, real_z, out_x2, out_y2, err;
  Int_t t;
 
  //Counter for output values outside the unit sphere
  Int_t outside = 0;
  //Counter for the computed vectors
  Int_t inside = 0;
  //Read data file
  Int_t nlines = 0;
  while (1){
    //Get the five columns
    in >> real_x2 >> real_y2 >> real_z >> t >> out_x2 >> out_y2 >> err;
    if(!in.good()) break;
    if(nlines < nr_iter){
      //Fill the data vectors
      if((out_x2*2-1)*(out_x2*2-1) + (out_y2*2-1)*(out_y2*2-1) > 1){
	outside++;
      }else{
	inpoints[inside] = (float)inside+1;
	real_xmom[inside] = real_x2*2-1;
	real_ymom[inside] = real_y2*2-1;
	real_zmom[inside] = real_z;

	out_xmom[inside] = out_x2*2-1;
	out_ymom[inside] = out_y2*2-1;
	out_zmom[inside] = TMath::Sqrt(1-out_xmom[inside]*out_xmom[inside]-out_ymom[inside]*out_ymom[inside]);
	
	error[inside] = err;
	
	ang_dist[inside] = (180/TMath::Pi())*TMath::ACos(real_xmom[inside]*out_xmom[inside]+real_ymom[inside]*out_ymom[inside]+real_zmom[inside]*out_zmom[inside]);	
	inside++;
      }
      nlines++;
    }
  }
  
  cout << "Number of lines: " << nlines << endl;
  cout << "Number of output points outside the unit sphere: " << outside << endl;
  cout << "Number of output points inside the unit sphere: " << inside << endl;
 
  //Declare graph
  TGraph *ERvsIT = new TGraph(nr_inpoints, inpoints, error);
  ERvsIT->SetTitle("Error vs Iterations");
  
  //draw graph 
  TCanvas *Canvas1 = new TCanvas("ERvsIT","ERvsIT");
  ERvsIT->Draw("al");
  ERvsIT->SetTitle("Error vs Iterations");
  ERvsIT->GetXaxis()->SetTitle("iterations");
  ERvsIT->GetYaxis()->SetTitle("error");
  gPad->Update();
  gPad->Modified();

  //Declare angle graphs
  TGraph *ADvsIT = new TGraph(nr_inpoints, inpoints, ang_dist);
  ADvsIT->SetTitle("Angular distance vs Iterations");

  //draw graph 
  TCanvas *Canvas2 = new TCanvas("ADvsIT","ADvsIT");
  ADvsIT->Draw("al");
  ADvsIT->SetTitle("Angular Distance vs Iterations");
  ADvsIT->GetXaxis()->SetTitle("iterations");
  ADvsIT->GetYaxis()->SetTitle("angular distance");
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
