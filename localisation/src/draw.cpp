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
#include "TMath.h"

using namespace std;

void draw(){

  //Declare value vectors
  Int_t nr_iter = 30197;
  Float_t iter[nr_iter], real_xmom_2[nr_iter], real_ymom_2[nr_iter], out_xmom_2[nr_iter], out_ymom_2[nr_iter], error_2[nr_iter], real_xmom_3[nr_iter], real_ymom_3[nr_iter], out_xmom_3[nr_iter], out_ymom_3[nr_iter], error_3[nr_iter],r_i[nr_iter],r_o[nr_iter] ;
 
  //Fill iterations vector
  for (Int_t i = 0; i < nr_iter; i++)
    iter[i] = (float)i+1;
 
  //Open data files
  ifstream in_2, in_3;
  in_2.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_TANHL_2.dat", ios::in);
  in_3.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_TANHL_3.dat", ios::in);

  //Create temporary use variables
  Float_t real_x2_2, real_y2_2, real_z_2, out_x2_2, out_y2_2, err_2, real_x2_3, real_y2_3, real_z_3, out_x2_3, out_y2_3, err_3;
  Int_t t_2, t_3;
 
  Int_t nlines_2 = 0;
  while (1){
    //Get the five columns
    in_2 >> real_x2_2 >> real_y2_2 >> real_z_2 >> t_2 >> out_x2_2 >> out_y2_2 >> err_2;
    if(!in_2.good()) break;
    if(nlines_2 < nr_iter)
    //Fill the data vectors
    real_xmom_2[nlines_2] = 2*real_x2_2-1;
    real_ymom_2[nlines_2] = 2*real_y2_2-1;
    out_xmom_2[nlines_2]  = 2*out_x2_2-1;
    out_ymom_2[nlines_2]  = 2*out_y2_2-1;
    error_2[nlines_2]     = err_2;
    nlines_2++;
  }
  
  cout << "Number of lines in NN_data_TANHL_2.dat :" << nlines_2 << endl;

  Int_t nlines_3 = 0;
  while (1){
    //Get the five columns
    in_3 >> real_x2_3 >> real_y2_3 >> real_z_3 >> t_3 >> out_x2_3 >> out_y2_3 >> err_3;
    if(!in_3.good()) break;
    if(nlines_3 < nr_iter)
    //Fill the data vectors
    real_xmom_3[nlines_3] = 2*real_x2_3-1;
    real_ymom_3[nlines_3] = 2*real_y2_3-1;
    out_xmom_3[nlines_3]  = 2*out_x2_3-1;
    out_ymom_3[nlines_3]  = 2*out_y2_3-1;
    error_3[nlines_3]     = err_3;
    r_i[nlines_3] = TMath::Sqrt(real_xmom_3[nlines_3]*real_xmom_3[nlines_3]+real_ymom_3[nlines_3]*real_ymom_3[nlines_3]);
    r_o[nlines_3] = TMath::ATan2(real_ymom_3[nlines_3],real_xmom_3[nlines_3]);
    nlines_3++;
  }
  
  cout << "Number of lines in NN_data_TANHL_3.dat :" << nlines_3 << endl;
 
  //Declare graphs for the 2 layers data
  TGraph *ERvsIT_2 = new TGraph(nr_iter, iter, error_2);
  ERvsIT_2->SetTitle("Error vs Iterations (2 layers)");
  ERvsIT_2->SetLineColor(2);

  TGraph *ERvsRX_2 = new TGraph(nr_iter, real_xmom_2, error_2);
  ERvsRX_2->SetTitle("Error vs Real x-momentum (2 layers)");

  TGraph *ERvsOX_2 = new TGraph(nr_iter, out_xmom_2, error_2);
  ERvsOX_2->SetTitle("Error vs Output x-momentum (2 layers)");

  TGraph *ERvsRY_2 = new TGraph(nr_iter, real_ymom_2, error_2);
  ERvsRY_2->SetTitle("Error vs Real y-momentum (2 layers)");

  TGraph *ERvsOY_2 = new TGraph(nr_iter, out_ymom_2, error_2);
  ERvsOY_2->SetTitle("Error vs Output y-momentum (2 layers)");

  //Declare graphs for the 3 layers data
  TGraph *ERvsIT_3 = new TGraph(nr_iter, iter, error_3);
  ERvsIT_3->SetTitle("Error vs Iterations (3 layers)");
  ERvsIT_3->SetLineColor(3);
  
  TGraph *ERvsRX_3 = new TGraph(nr_iter, real_xmom_3, error_3);
  ERvsRX_3->SetTitle("Error vs Real x-momentum (3 layers)");

  TGraph *ERvsOX_3 = new TGraph(nr_iter, out_xmom_3, error_3);
  ERvsOX_3->SetTitle("Error vs Output x-momentum (3 layers)");

  TGraph *ERvsRY_3 = new TGraph(nr_iter, real_ymom_3, error_3);
  ERvsRY_3->SetTitle("Error vs Real y-momentum (3 layers)");

  TGraph *ERvsOY_3 = new TGraph(nr_iter, out_ymom_3, error_3);
  ERvsOY_3->SetTitle("Error vs Output y-momentum (3 layers)");

  TGraph *ERvsRradius_3 = new TGraph(nr_iter, r_i, error_3);
  ERvsRradius_3->SetTitle("Error vs Radius (3 layers)");

  TGraph *ERvsRazimuth_3 = new TGraph(nr_iter, r_o, error_3);
  ERvsRazimuth_3->SetTitle("Error vs Azimuth (3 layers)");

  //build multigraph
  TMultiGraph *ERvsIT = new TMultiGraph();
  ERvsIT->Add(ERvsIT_2);
  ERvsIT->Add(ERvsIT_3);
  
  //draw multigraph 
  TCanvas *Canvas = new TCanvas("ERvsIT","ERvsIT");
  ERvsIT->Draw("al");
  ERvsIT->SetTitle("Error vs Iterations");
  ERvsIT->GetXaxis()->SetTitle("iterations");
  ERvsIT->GetYaxis()->SetTitle("error");
  gPad->Update();
  gPad->Modified();

  //build and draw a legend
  TLegend *leg = new TLegend(0.7,0.1,0.9,0.3);
  leg->SetFillColor(0);
  leg->SetHeader("TANHL");
  leg->AddEntry(ERvsIT_2,"2 layers");
  leg->AddEntry(ERvsIT_3,"3 layers");
  leg->Draw("Same");

  //Declare canvas
  TCanvas *c = new TCanvas("Momenta","Momenta");
  c->Divide(2,2);
  c->cd(1);
  ERvsRX_3->Draw("ap");
  c->cd(2);
  ERvsOX_3->Draw("ap");
  c->cd(3);
  ERvsRY_3->Draw("ap");
  c->cd(4);
  ERvsOY_3->Draw("ap");

  //Declare canvas
  TCanvas *c2 = new TCanvas("Radius","Radius");
  c2->Divide(2,1);
  c2->cd(1);
  ERvsRradius_3->Draw("ap");
  c2->cd(2);
  ERvsRazimuth_3->Draw("ap");

  //Declare canvas
  TCanvas *c3 = new TCanvas("Error","Error");
  ERvsIT_3->Draw("ap");
  
  //Close data files
  in_2.close();
  in_3.close();
 
  return;

}

int main(){

  draw();

  return 0;

}
