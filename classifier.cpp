#include "classifier.hpp"
#include <iostream>

using namespace std;

Classifier::Classifier(int numCaracs,
		Caracteristique* caracteristiques_employees, double pas,
		int largeur_images, int hauteur_images) {

	cout << "Bilan de l'init : ";

	numCaracteristiques = numCaracs;
	caracs = caracteristiques_employees;
	epsilon = pas;
	width = largeur_images;
	height = hauteur_images;

	int multiplicateur_x;
	int multiplicateur_y;

	int cpt = 0;

	for (int i = 0; i < width / pasPixel; i++) {
		for (int j = 0; j < height / pasPixel; j++) {
			//traiter le pixel 4*i 4*j
			for (int c = 0; c < numCaracteristiques; c++) {
				multiplicateur_y = 0;
				while (caracs[c].getHauteur(multiplicateur_y) <= height - pasPixel * j) {
					multiplicateur_x = 0;
					while (caracs[c].getLargeur(multiplicateur_x) <= width - pasPixel * i) {
						// cout << "getLarg = "<<caracs[c].getLargeur(multiplicateur_y);
						// cout << " ; mx = "<<multiplicateur_x << " ; my = "<<multiplicateur_y << endl;
						weights1[c][i][j][multiplicateur_x][multiplicateur_y] = 1;
						weights2[c][i][j][multiplicateur_x][multiplicateur_y] = 0;
						multiplicateur_x++;
						cpt++;
					}
					multiplicateur_y++;
				}
			}
		}
		// cout << i << endl;
	}
	cout << cpt<<" lignes." << std::endl;
	// return;
}

int Classifier::getNumCaracteristiques() {
	return numCaracteristiques;
}

double Classifier::scoreClassifier(int c, int x, int y, int multiplicateur_x,
		int multiplicateur_y, double composante) {
	double sum = 0;
	sum += weights1[c][x][y][multiplicateur_x][multiplicateur_y] * composante
			+ weights2[c][x][y][multiplicateur_x][multiplicateur_y];
	return sum;
}

// void Classifier::updateWeights(int c, int x, int y, int multiplicateur_x,
// 		int multiplicateur_y, double composante, int classe) {
// 	int h;
// 	if (scoreClassifier(c, x, y, multiplicateur_x, multiplicateur_y, composante)
// 			< 0) {
// 		h = -1;
// 	} else {
// 		h = 1;
// 	}
// 	weights1[c][x][y][multiplicateur_x][multiplicateur_y] -= epsilon
// 			* (h - classe) * composante;
// 	weights2[c][x][y][multiplicateur_x][multiplicateur_y] -= epsilon
// 			* (h - classe);
// }

double *Classifier::getWeights(int c, int x, int y, int mx, int my, int i) {
	if(i==1)
		return &(weights1[c][x][y][mx][my]);
	else // i == 2
		return &(weights2[c][x][y][mx][my]);
}

bool Classifier::testClassif(){
	int multiplicateur_x,multiplicateur_y;
	int cpt=0;
	for (int i = 0; i < 112/4; i++) {
		for (int j = 0; j < 92/4; j++) {
			//traiter le pixel 4*i 4*j
			for (int c = 0; c < 4; c++) {
				multiplicateur_y = 0;
				while (caracs[c].getHauteur(multiplicateur_y) <= height - 4 * j) {
					multiplicateur_x = 0;
					while (caracs[c].getLargeur(multiplicateur_x) <= width - 4 * i) {
						if(weights1[c][i][j][multiplicateur_x][multiplicateur_y] != 1 ||
							weights2[c][i][j][multiplicateur_x][multiplicateur_y] != 0)
							return false;
						multiplicateur_x++;
						cpt++;
					}
					multiplicateur_y++;
				}
			}
		}
		// cout << i << endl;
	}

	// std::cout << "Compteur : "<<cpt << std::endl;
	return true;
}

void Classifier::printWeights(){
	int multiplicateur_x,multiplicateur_y;
	for (int i = 0; i < 112/4; i++) {
		for (int j = 0; j < 92/4; j++) {
			//traiter le pixel 4*i 4*j
			for (int c = 0; c < 4; c++) {
				multiplicateur_y = 0;
				while (caracs[c].getHauteur(multiplicateur_y) <= height - 4 * j) {
					multiplicateur_x = 0;
					while (caracs[c].getLargeur(multiplicateur_x) <= width - 4 * i) {
						cout << c << " " << i << " " << j
											<< " " << multiplicateur_x
											<< " " << multiplicateur_y << " : "
											<< weights1[c][i][j][multiplicateur_x][multiplicateur_y] << ";"
											<< weights2[c][i][j][multiplicateur_x][multiplicateur_y] << endl;
						multiplicateur_x++;
					}
					multiplicateur_y++;
				}
			}
		}
		// cout << i << endl;
	}
}

// void Classifier::train(ifstream& file) {
// 	int classe;
// 	int x, y, multiplicateur_x, multiplicateur_y, c;
// 	double valeur;
//
// 	file >> classe;
// 	while (!file.eof()) {
// 		file >> c >> x >> y >> multiplicateur_x >> multiplicateur_y >> valeur;
// 		updateWeights(c, x, y, multiplicateur_x, multiplicateur_y, valeur,
// 				classe);
// 	}
//
// }

// void Classifier::setWeights(int c, int x, int y, int mx, int my,
// 		double weights[2]) {
// 	weights1[c][x][y][mx][my] = weights[0];
// 	weights2[c][x][y][mx][my] = weights[1];
// }

// void Classifier::updateWeights(int c, int x, int y, int mx, int my,
// 		double composante, int classe, double weights[2], double epsilon) {
// 	int h;
// 	if (scoreClassifier(c, x, y, mx, my, composante) < 0) {
// 		h = -1;
// 	} else {
// 		h = 1;
// 	}
//
// 	weights[0] -= epsilon * (h - classe) * composante;
// 	weights[1] -= epsilon * (h - classe);
// }
