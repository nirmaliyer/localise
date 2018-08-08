/*
** author : Joan Bachs Esteban
** email : bachsjoan@gmail.com
*/

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"

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
	if(z_mom>0.000001){
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
	for(i=0;i<Nr_Hits.size();i++){
		Nr_Hits[i] = Nr_Hits[i]/Hits_total;
	}

	//Close input file
	delete t3;
	f1.Close();
}

//Train function
void trainLocalisation() {
	ofstream file;
	string filename_iter = "NN_data__LEAKY_RELU.dat";
	string filename_output = "testlocalisation_LEAKY_RELU.nn";
	file.open(filename_iter.c_str());
	int t;
	int nr_runs = 40400;
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
	
	//Initialize Neural Network
	NeuralNetwork NN;
	NN.init(topo, eta, alpha, actFuns);

	int iter = 0;
	//Write heading to the phile
	file << "real: theta	phi	output: theta	phi	error" << endl;
	
	vector <double> output;
	while (true) {
		int t = rand() % 40401;
		double limit_error;

		filename_database = path_database + "Hits" + to_string(t) + suffix_database;
		read(filename_database, theta, phi,  xmom, ymom, zmom ,det,Nr_Hits);
		if(zmom>0.00001){
			iter++;
			cout << Nr_Hits.size() << endl;

			output = {theta, phi};
			limit_error = 3;
			
			NN.feedForward(Nr_Hits);
	
			NN.backProp(output);

			std::vector<double> res;
			NN.getResults(res);
			std::cout << "\nGiven output : " << output[0] << ", " << output[1];
			std::cout << "\nResult : " << res[0] << ", " << res[1];
			std::cout << "\nError : " << NN.getError() << "\n";
			//getchar();
			if (iter%400 == 0) {
			  file << theta << "	" << phi << "	" << res[0] << "	" << res[1] << "	" << NN.getError() << endl;
			}			
			if (NN.getError() < limit_error) {
				std::cout << "\nNN converged in " << iter << " iterations!\n";
				NN.writeNNToFile(filename_output.c_str());
				break;
			}
		}
		det.clear();
		Nr_Hits.clear();
	
	}
	file.close();	
}

int main(){

  trainLocalisation();

  cout << "something" << endl;

  return 0;

}