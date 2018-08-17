/*
** name: statistics.cpp
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

void statistics(){

  //Declare value vectors
  Int_t nr_iter = 27345, nr_epochs = 50;
  Float_t iter[nr_iter], error[nr_iter], epochs[nr_epochs], max[nr_epochs], min[nr_epochs], mean[nr_epochs], std_dev[nr_epochs];
 
  //Fill iterations vector
  for(Int_t i = 0; i < nr_iter; i++)
    iter[i] = (float)i+1;

  //Fill epochs vector
  for(Int_t epoch = 0; epoch < nr_epochs; epoch ++)
    epochs[epoch] = (float)epoch+1;
 
  //Data file stream
  ifstream in;
  
  for(Int_t epoch = 1; epoch <= nr_epochs; epoch ++){

    //Open data file
    in.open("/home/joan/git_joan_localise/localise/localisation/build/NN_data_TANHL_3_epoch_"+to_string(epoch)+".dat", ios::in);

    //Create temporary use variables
    Float_t real_x2, real_y2, real_z, out_x, out_y, err;
    Int_t t;

    //Read data file
    Int_t nlines = 0;
    while (1){
      //Get the five columns
      in >> real_x2 >> real_y2 >> real_z >> t >> out_x >> out_y >> err;
      if(!in.good()) break;
      if(nlines < nr_iter)
	//Fill the data vectors
	error[nlines] = err;
      nlines++;
    }

    //Close data file
    in.close();
  
    cout << "Number of lines in NN_data_TANHL_2_epoch_"+to_string(epoch)+".dat :" << nlines << endl;

    max[epoch-1]     = TMath::MaxElement(nr_iter, error);
    cout << "max" << epoch-1 << " = " << max[epoch-1] << endl;
    min[epoch-1]     = TMath::MinElement(nr_iter, error);
    cout << "min" << epoch-1 << " = " << min[epoch-1] << endl;
    mean[epoch-1]    = TMath::Mean(nr_iter, error, 0);
    cout << "mean" << epoch-1 << " = " << mean[epoch-1] << endl;
    std_dev[epoch-1] = TMath::StdDev(nr_iter, error, 0);
    cout << "std_dev" << epoch-1 << " = " << std_dev[epoch-1] << endl;

  }
 
  //Declare graphs
  TGraph *MAX = new TGraph(nr_epochs, epochs, max);
  MAX->SetTitle("Maximum Error");
  MAX->SetLineColor(2);

  TGraph *MIN = new TGraph(nr_epochs, epochs, min);
  MIN->SetTitle("Minimum Error");
  MIN->SetLineColor(3);

  TGraphErrors *MEAN = new TGraphErrors(nr_epochs, epochs, mean, 0, std_dev);
  MEAN->SetTitle("Mean Error");
  MEAN->SetLineColor(4);

  //build multigraph
  TMultiGraph *ERRORS = new TMultiGraph();
  ERRORS->Add(MAX);
  ERRORS->Add(MIN);
  ERRORS->Add(MEAN);
  
  //draw multigraph 
  TCanvas *Statis = new TCanvas("Max Mean Min","Max Mean Min");
  ERRORS->Draw("al");
  ERRORS->SetTitle("Errors vs Epochs");
  ERRORS->GetXaxis()->SetTitle("epochs");
  ERRORS->GetYaxis()->SetTitle("error");
  gPad->Update();
  gPad->Modified();

  //build and draw a legend
  TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(MAX,"Maximum");
  leg->AddEntry(MIN,"Minimum");
  leg->AddEntry(MEAN,"Mean with StdDev");
  leg->Draw("Same");
  
  return;

}

int main(){

  statistics();

  cout << endl << "Statistics program has been executed" << endl;

  return 0;

}
