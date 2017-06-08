#include "mpi.h"
#include "mpi_manager.hpp"

#include <iostream>

MPI_Manager::MPI_Manager(/*char* source_name, char* output_name,
		Classifier& classifier, */double e) {

	epsilon = e;
	numCaracs = 4;
	caracs = new Caracteristique[4];


	int taskid;
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

	int pix1[3][3];
	int pix2[3][3];
	int pix3[3][3];
	int pix4[3][3];

/*  ____ ____
	 |    |####|
	 |____|####|
	 |####|    |
	 |####|____|
	 */

	pix1[0][0] = -1;
	pix1[0][1] = 1;
	pix1[1][0] = 1;
	pix1[1][1] = -1;

/*  ____ ____ ____
	 |    |####|    |
	 |____|####|____|
	 */

	pix2[0][0] = -1;
	pix2[1][0] = 1;
	pix2[2][0] = -1;

/*  ____ ____
	 |    |####|
	 |____|####|
	 */

	pix3[0][0] = -1;
	pix3[1][0] = 1;

/*  ____
	 |    |
	 |____|
	 |####|
	 |####|
	 */

	pix4[0][0] = -1;
	pix4[0][1] = 1;

	Caracteristique *c = new Caracteristique(pix1, 2, 2);
	Caracteristique *c1 = new Caracteristique(pix2, 3, 1);
	Caracteristique *c2 = new Caracteristique(pix3, 2, 1);
	Caracteristique *c3 = new Caracteristique(pix4, 1, 2);

	caracs[0] = *c;
	caracs[1] = *c1;
	caracs[2] = *c2;
	caracs[3] = *c3;


	if(taskid == root){
		classifieur = new Classifier(4,caracs,epsilon,112,92);
	}
}

void MPI_Manager::MPI_Run() {

	int numtasks, taskid;
	int width, height;

	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	width = img->getWidth();
	height = img->getHeight();

	myfile << img->getClass();

	for (int j = 0; j < height / pasPixel; j++) {
		for (int i = 0; i < width / pasPixel; i++) {
			if ((((i + j * width / pasPixel) % (numtasks-1)) +1) == taskid) {
				img->traiterPixel(pasPixel * i, pasPixel * j, caracs, myfile); // écrit dans le fichier
			}
		}
	}

	myfile.close();
}

void MPI_Manager::newWeights(double oldWeights[2], double valeur) {
	double w1 = oldWeights[0];
	double w2 = oldWeights[1];

	double score = w1 * valeur + w2;
	int h = (score < 0) ? -1 : 1;

	oldWeights[0] -= epsilon * (h - img->getClass()) * valeur;
	oldWeights[1] -= epsilon * (h - img->getClass());
}

void MPI_Manager::MPI_Train(char* source_name,int classe) {
	int numtasks, taskid, height, width;
	int x, y, dimx, dimy, multiplicateur_y, multiplicateur_x, compteur;
	double weights[2];
	double valeur;

	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	// if(taskid == root && !classifieur->testClassif())
	// 	{std::cout << "ERREUR" << std::endl;}

	img = new IntegralImage((char*) source_name, classe);

	MPI_Status status;
	MPI_Request request;


	width = img->getWidth();
	height = img->getHeight();

	compteur = 0;

/*	if (taskid == root) {
		classifieur = new Classifier(4, caracs, epsilon, 112, 92);
	}*/

	for (int i = 0; i < width / pasPixel; i++) {
		for (int j = 0; j < height / pasPixel; j++) {
			x = pasPixel * i;
			y = pasPixel * j;
			dimx = width - x;
			dimy = height - y;

			for (int c = 0; c < numCaracs; c++) {
				// std::cout << "proc "<<taskid<<" ; i = "<<i << " ; j = "<<j << " ; c = "<<c << std::endl;

				multiplicateur_y = 0;
				while (caracs[c].getHauteur(multiplicateur_y) <= dimy) {
					multiplicateur_x = 0;
					while (caracs[c].getLargeur(multiplicateur_x) <= dimx) {
						// attention au boulot de root
						if (taskid == root) {
							if(!classifieur)
							{std::cout << "Classifieur vide : "<<taskid << std::endl;}

							weights[0] = *(classifieur->getWeights(c, i, j,multiplicateur_x, multiplicateur_y,1));
							weights[1] = *(classifieur->getWeights(c, i, j,multiplicateur_x, multiplicateur_y,2));
							// if(weights[0] != 1 || weights[1] != 0)
							// 	std::cout << "WTF : "<< weights[0] << " , "<< weights[1] << std::endl;
							MPI_Isend(weights,2, MPI_DOUBLE, (compteur % (numtasks-1)) +1, 0,MPI_COMM_WORLD,&request);
						}
							/**********************/
							/****ENVOI DU SLAVE****/
							/**********************/

							else if ((compteur % (numtasks-1)) +1 == taskid) {
								MPI_Recv(weights, 2, MPI_DOUBLE, root, 0,MPI_COMM_WORLD, &status);
								valeur = img->traiterUnite(x, y, multiplicateur_x,multiplicateur_y, &(caracs[c]));
								/*cout << c << " " << i << " " << j
											<< " " << multiplicateur_x
											<< " " << multiplicateur_y << " : "
											<< valeur << endl;*/
								newWeights(weights, valeur);
								MPI_Send(weights, 1, MPI_DOUBLE, root, 0,MPI_COMM_WORLD);
								MPI_Send(weights+1, 1, MPI_DOUBLE, root, 1,MPI_COMM_WORLD);
						}
						if(taskid == root){
							// std::cout << "Modif ? "<< *(classifieur->getWeights(c, x, y,
																				// multiplicateur_x, multiplicateur_y,1));
							MPI_Irecv(classifieur->getWeights(c, i, j,multiplicateur_x, multiplicateur_y,1),1, MPI_DOUBLE,(compteur % (numtasks-1)) +1, 0, MPI_COMM_WORLD,&request);
							// std::cout << " --> "<< *(classifieur->getWeights(c, x, y,
																				// multiplicateur_x, multiplicateur_y,1))<< std::endl;
							MPI_Irecv(classifieur->getWeights(c, i, j,multiplicateur_x, multiplicateur_y,2),1, MPI_DOUBLE,(compteur % (numtasks-1)) +1, 1, MPI_COMM_WORLD,&request);
						}
						compteur++;
						multiplicateur_x++;
					}
					multiplicateur_y++;
				}
			}
		}
	}

	// if(taskid == root)
	// 	classifieur->printWeights();
		// std::cout << compteur * 100 / 128227<< "%\r";
}

Caracteristique * MPI_Manager::getCaracs(){
	return caracs;
}

void MPI_Manager::printClassifWeights(){
	classifieur->printWeights();
}
