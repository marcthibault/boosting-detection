#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>

#include "mpi.h"

#include <fstream>
#include <sstream>
#include <string>

#include "integralimage.hpp"
#include "caracteristique.hpp"
#include "mpi_manager.hpp"
#include "classifier.hpp"

using namespace std;

void usage(char* s) {
	std::cout << s << " K epsilon" << std::endl;
}

int main(int argc, char** argv) {

	int taskid;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

	/**** Question 1.2 ****/
//  MPI_Manager *manager = new MPI_Manager((char*) "im0.txt", (char*)"sortie.txt",0,0);
//  manager->MPI_Run();
	/**** Question 2.1 ****/

	if (argc != 3) {
		usage(argv[0]);
		exit(1);
	}

	double epsilon = atof(argv[2]);
	int _k = atoi(argv[1]);
	string nomImage;

	// std::cout << "Epsilon = 0.03 ? " <<(epsilon==0.03)<< std::endl;
	MPI_Manager *manager = new MPI_Manager(epsilon);

	for (int k = 0; k < _k; k++) {
		int classe = rand() % 2;
    stringstream ss;
    int v1;
		if (classe == 0) {
			classe = -1;
			v1 = rand() % 4415;
			// std::cout << "ss.str() = "<<ss.str() << std::endl;
      ss << v1;
			nomImage = "/home/vincent/Documents/Cours_INF/INF442/Projet/Images/app/neg/im" + ss.str() + ".txt";
		} else {
			v1 = rand() % 818;
      ss << v1;
			// std::cout << "ss.str() = "<<ss.str() << std::endl;
			nomImage = "/home/vincent/Documents/Cours_INF/INF442/Projet/Images/app/pos/im" +  ss.str() + ".txt";
		}

		if(taskid == root)
			std::cout << "Fichier traité n°"<<k<<": "<< nomImage << std::endl;

		manager->MPI_Run2((char*) nomImage.c_str(),classe);
		MPI_Barrier(MPI_COMM_WORLD);
		// std::cout << "Erreur ?" << std::endl;

	}

	if(taskid == root)
		manager->printClassifWeights();

	MPI_Finalize();

	return 0;
}
