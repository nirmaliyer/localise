/*
** name: ang_dist.cpp
** author : Joan Bachs Esteban
** email : bachsjoan@gmail.com
*/

#include "TCanvas.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"
#include <fstream>
#include "TMath.h"

using namespace std;

void ang_dist(){

  //Declare value vectors
  Int_t nr_iter = 27345, nr_inpoints = 23506, nr_layers = 3, epoch = 1;
  Float_t inpoints[nr_inpoints], real_xmom[nr_inpoints], real_ymom[nr_inpoints], real_zmom[nr_inpoints], real_xy[nr_inpoints], real_azimuth[nr_inpoints], out_xmom[nr_inpoints], out_ymom[nr_inpoints], out_zmom[nr_inpoints], out_xy[nr_inpoints], out_azimuth[nr_inpoints], ang_dist[nr_inpoints], error[nr_inpoints];
 
  //Data file stream
  ifstream in;

  //Open data file
  in.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_TANHL_"+to_string(nr_layers)+"_epoch_"+to_string(epoch)+".dat", ios::in);
  
  //Create temporary use variables
  Float_t real_x, real_y, out_x, out_y, err;

  //Counter for output values outside the unit sphere
  Int_t outside = 0;
  //Counter for the computed vectors
  Int_t inside = 0;
  //Read data file
  Int_t nlines = 0;
  while (1){
    //Get the five columns
    in >> real_x >> real_y >> out_x >> out_y >> err;
    if(!in.good()) break;
    if(nlines < nr_iter){
      //Fill the data vectors
      if(out_x*out_x + out_y*out_y > 1){
	outside++;
      }
      else{
	inpoints[inside] = (float)inside+1;
	real_xmom[inside] = real_x;
	real_ymom[inside] = real_y;
	real_zmom[inside] = TMath::Sqrt(1-real_xmom[inside]*real_xmom[inside]-real_ymom[inside]*real_ymom[inside]);
	real_xy[inside] = TMath::Sqrt(real_xmom[inside]*real_xmom[inside]+real_ymom[inside]*real_ymom[inside]);
	real_azimuth[inside] = TMath::ATan2(real_ymom[inside], real_xmom[inside]);

	out_xmom[inside] = out_x;
	out_ymom[inside] = out_y;
	out_zmom[inside] = TMath::Sqrt(1-out_xmom[inside]*out_xmom[inside]-out_ymom[inside]*out_ymom[inside]);
	out_xy[inside] = TMath::Sqrt(out_xmom[inside]*out_xmom[inside]+out_ymom[inside]*out_ymom[inside]);
	out_azimuth[inside] = TMath::ATan2(out_ymom[inside], out_xmom[inside]);

	error[inside] = err;

	ang_dist[inside] = (180/TMath::Pi())*TMath::ACos(real_xmom[inside]*out_xmom[inside]+real_ymom[inside]*out_ymom[inside]+real_zmom[inside]*out_zmom[inside]);

	inside++;
	  }
      nlines++;
    }
  }
  
  //Close data file
  in.close();
  
  cout << "Number of lines in NN_data_TANHL_"+to_string(nr_layers)+"_epoch_"+to_string(epoch)+".dat :" << nlines << endl;
  cout << "Number of output points outside the unit sphere: " << outside << endl;
  cout << "Number of output points inside the unit sphere: " << inside << endl;
  
  //Declare angle graphs
  TGraph *ADvsIT = new TGraph(nr_inpoints, inpoints, ang_dist);
  ADvsIT->SetTitle("Angular distance vs Iterations");

  TGraph *ADvsRR = new TGraph(nr_inpoints, real_xy, ang_dist);
  ADvsRR->SetTitle("Angular distance vs Real XY vector");

  TGraph *ADvsOR = new TGraph(nr_inpoints, out_xy, ang_dist);
  ADvsOR->SetTitle("Angular distance vs Ouput XY vector");

  TGraph *ADvsRP = new TGraph(nr_inpoints, real_azimuth, ang_dist);
  ADvsRP->SetTitle("Angular distance vs Real azimuth");

  TGraph *ADvsOP = new TGraph(nr_inpoints, out_azimuth, ang_dist);
  ADvsOP->SetTitle("Angular distance vs Output azimuth");

  //Declare error graphs
  TGraph *ERvsIT = new TGraph(nr_inpoints, inpoints, error);
  ERvsIT->SetTitle("Error vs Iterations");
  
  TGraph *ERvsRR = new TGraph(nr_inpoints, real_xy, error);
  ERvsRR->SetTitle("Error vs Real XY vector");

  TGraph *ERvsOR = new TGraph(nr_inpoints, out_xy, error);
  ERvsOR->SetTitle("Error vs Ouput XY vector");

  TGraph *ERvsRP = new TGraph(nr_inpoints, real_azimuth, error);
  ERvsRP->SetTitle("Error vs Real azimuth");

  TGraph *ERvsOP = new TGraph(nr_inpoints, out_azimuth, error);
  ERvsOP->SetTitle("Error vs Output azimuth");
  
  //draw graphs 
  TCanvas *angdist = new TCanvas("Angle vs XY-distance & Azimuth (3 layers epoch 1)","Angle vs XY-distance & Azimuth (3 layers epoch 1)");
  angdist->Divide(2,2);
  angdist->cd(1);
  ADvsRR->Draw("ap");
  angdist->cd(2);
  ADvsOR->Draw("ap");
  angdist->cd(3);
  ADvsRP->Draw("ap");
  angdist->cd(4);
  ADvsOP->Draw("ap");

  TCanvas *errdist = new TCanvas("Error vs XY-distance & Azimuth (3 layers epoch 1)","Error vs XY-distance & Azimuth (3 layers epoch 1)");
  errdist->Divide(2,2);
  errdist->cd(1);
  ERvsRR->Draw("ap");
  errdist->cd(2);
  ERvsOR->Draw("ap");
  errdist->cd(3);
  ERvsRP->Draw("ap");
  errdist->cd(4);
  ERvsOP->Draw("ap");

  TCanvas *angit = new TCanvas("Angle vs Iterations (3 layers epoch 1)","Angle vs Iterations (3 layers epoch 1)");
  ADvsIT->Draw("ap");

  TCanvas *errit = new TCanvas("Error vs Iterations (3 layers epoch 1)","Error vs Iterations (3 layers epoch 1)");
  ERvsIT->Draw("ap");
  
  return;

}

int main(){

  ang_dist();

  return 0;

}
