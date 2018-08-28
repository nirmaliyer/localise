/*
** name: train_poisson.cpp
** author : Joan Bachs Esteban
** email : bachsjoan@gmail.com
*/

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TRandom.h"

#include <iostream>
#include <vector>
#include <cmath>
#include "NeuralNetwork.h"
#include <fstream>

using namespace NN;
using namespace std;

//Read function
void read(string filename, Float_t &theta, Float_t &phi, Float_t &x_mom, Float_t &y_mom, Float_t &z_mom, vector<double> &detector, vector<double> &Nr_Hits){

    int i,j;
    double Hits_total=0;
    //vectors and helpvariables to read in Hits per det ID
    vector<double> *help_det=0;
    vector<double> *help_NrHits=0;

    //Read in initial GRB position and save to vector
    TFile f1(filename.c_str());

    TTree *t3 = (TTree*) f1.Get("Run_info");
   
    t3->SetBranchAddress("theta_GRB",&theta);
    t3->SetBranchAddress("phi_GRB",&phi);
    t3->SetBranchAddress("x_GRB",&x_mom);
    t3->SetBranchAddress("y_GRB",&y_mom);
    t3->SetBranchAddress("z_GRB",&z_mom);


    for(i=0;i<t3->GetEntries();i++){
        t3->GetEntry(i);
    }
    //Only use starting positions on the sphere
    if(z_mom>0.36){
        TTree *t6 = (TTree*) f1.Get("one_hit");
        t6->SetBranchAddress("detector",&help_det);
        t6->SetBranchAddress("Nr_Hits",&help_NrHits);
   
        for(i=0;i<t6->GetEntries();i++){
            t6->GetEntry(i);
            for(j=0;j<help_det->size();j++){
                //Save all the data in the final vectors
                detector.push_back(help_det->at(j));
                Nr_Hits.push_back(help_NrHits->at(j));
                Hits_total += help_NrHits->at(j);
            }
        }
        delete t6;
        help_det->clear();
        help_NrHits->clear();
    }

    //Close input file
    delete t3;
    f1.Close();
}

//Train function
void trainLocalisation() {
  ofstream file;

  //Configuration
  string function = "LEAKY_RELU";
  int nr_layers = 2;
  int nr_epochs = 2;

  int epoch = 1;
  string filename_iter = "NN_data_"+function+"_"+to_string(nr_layers)+"_epoch_"+to_string(epoch)+"_Poisson.dat";
  string filename_output = "testlocalisation_"+function+"_"+to_string(nr_layers)+"_epoch_"+to_string(epoch)+"_Poisson.nn";
  file.open(filename_iter.c_str());

  //Running parameters
  int nr_runs = 40400;
  
  //Random shuffle vector
  int a[nr_runs];
  for(int i = 0; i < nr_runs; i++)
    a[i] = i;
    
  random_shuffle(&a[0],&a[nr_runs-1]);
    
  string path_database;
  path_database = "/media/nirmal/B856D99F56D95EA6/SPHiNX/localisation_runs/05_01_smallergrid_withContourShield/Hits/";
  string suffix_database;
  suffix_database = "_FeisCode.root";
  string filename_database;
  
  //vector for storing chi2 results per mac
  Float_t xmom=0;
  Float_t ymom=0;
  Float_t zmom=0;
  Float_t theta=0;
  Float_t phi=0;
   
  std::vector<size_t> topo;
  std::vector<double> eta;
  std::vector<double> alpha;
  std::vector<ActivationFunction> actFuns;
  vector<double> det, Nr_Hits;
  
  //Backpropagation algorithm
  topo = { 162, 2 };
  eta = { 0.2, 0.2 };
  alpha = { 0.5, 0.5 };
  actFuns = { LEAKY_RELU, LEAKY_RELU };
  
  //Reading vectors and variables
  vector <double> output;
  double xmom2, ymom2;
  
  //Iteration counter
  int iter = 0;
  //Writen events counter
  int lines = 0;
  //Data selector
  int t;
  
  //Initialize Neural Network
  NeuralNetwork NN;
  NN.init(topo, eta, alpha, actFuns);

  //Declare random pointer
  TRandom *random = new TRandom();
  
  while (true) {
    
    t = a[0];
    filename_database = path_database + "Hits" + to_string(t) + suffix_database;
    read(filename_database, theta, phi, xmom, ymom, zmom ,det,Nr_Hits);

    //Declare sum
    double Total_Hits = 0;

    //Randomization
    for (int i = 0; i < Nr_Hits.size(); i++){
      Nr_Hits[i] = random->Poisson(Nr_Hits[i]);
      Total_Hits += Nr_Hits[i];
    }

    //Normalization
    for(int i = 0; i < Nr_Hits.size(); i++)
      Nr_Hits[i] = Nr_Hits[i]/Total_Hits;
    
    //Normalize cartesian variables
    xmom2 = (xmom+1)/2;
    ymom2 = (ymom+1)/2;
    
    if(zmom>0.36){
      lines++;
      //Print out seleted data and size of Nr_Hits
      cout << "iter = " << iter << endl;
      cout << "t = " << t << endl;
      cout << Nr_Hits.size() << endl;
      cout << "Total Hits = " << Total_Hits << endl;
      
      output = {xmom2, ymom2};
      
      NN.feedForward(Nr_Hits);
      
      NN.backProp(output);
      
      std::vector<double> res;
      NN.getResults(res);
      std::cout << "Given output : " << output[0] << ", " << output[1];
      std::cout << "\nResult : " << res[0] << ", " << res[1];
      std::cout << "\nError : " << NN.getError() << "\n\n";
      file << xmom2 << " " << ymom2 << " " << zmom << " " << t << " " << res[0] << " " << res[1] << " " << NN.getError() << endl;           
    }
    
    det.clear();
    Nr_Hits.clear();
    
    iter++;
    
    if(iter > nr_runs){
      NN.writeNNToFile(filename_output.c_str());
      break;
    }
  }

  file << lines << endl;
  file.close();

  for(epoch = 2; epoch <= nr_epochs ; epoch ++){

    //Take previous Neural Network
    NN.readNNFromFile("testlocalisation_"+function+"_"+to_string(nr_layers)+"_epoch_"+to_string(epoch-1)+"_Poisson.nn");

    //Reinitialize strings
    filename_iter = "NN_data_"+function+"_"+to_string(nr_layers)+"_epoch_"+to_string(epoch)+"_Poisson.dat";
    filename_output =  "testlocalisation_"+function+"_"+to_string(nr_layers)+"_epoch_"+to_string(epoch)+"_Poisson.nn";

    //Reinitialize counters
    iter = 0;
    lines = 0;

    //Open new NN data file
    file.open(filename_iter.c_str());

    while (true) {
    
      t = a[0];
      filename_database = path_database + "Hits" + to_string(t) + suffix_database;
      read(filename_database, theta, phi, xmom, ymom, zmom ,det,Nr_Hits);

      //Declare sum
      double Total_Hits = 0;
      
      //Randomization
      for (int i = 0; i < Nr_Hits.size(); i++){
	Nr_Hits[i] = random->Poisson(Nr_Hits[i]);
	Total_Hits += Nr_Hits[i];
      }

      //Normalization
      for(int i = 0; i < Nr_Hits.size(); i++)
	Nr_Hits[i] = Nr_Hits[i]/Total_Hits;
    
      //Normalize cartesian variables
      xmom2 = (xmom+1)/2;
      ymom2 = (ymom+1)/2;
    
      if(zmom>0.36){
	lines++;
	//Print out seleted data and size of Nr_Hits
	cout << "iter = " << iter << endl;
	cout << "t = " << t << endl;
	cout << Nr_Hits.size() << endl;
	cout << "Total Hits = " << Total_Hits << endl;
      
	output = {xmom2, ymom2};
      
	NN.feedForward(Nr_Hits);
      
	NN.backProp(output);
      
	std::vector<double> res;
	NN.getResults(res);
	std::cout << "Given output : " << output[0] << ", " << output[1];
	std::cout << "\nResult : " << res[0] << ", " << res[1];
	std::cout << "\nError : " << NN.getError() << "\n\n";
	file << xmom2 << " " << ymom2 << " " << zmom << " " << t << " " << res[0] << " " << res[1] << " " << NN.getError() << endl;           
      }
      
      det.clear();
      Nr_Hits.clear();
      
      iter++;
      
      if(iter > nr_runs){
	NN.writeNNToFile(filename_output.c_str());
	break;
      }
    }
    
    file << lines << endl;
    file.close(); 
    
  }

  random->Delete();
    
}

int main(){

  trainLocalisation();

  cout << endl << "Training program has been executed" << endl;

  return 0;

}
