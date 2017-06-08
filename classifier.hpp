/*
 * classifier.hpp
 *
 *  Created on: 19 mai 2016
 *      Author: marc
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <fstream>

#include "caracteristique.hpp"
#include "string"
#include "integralimage.hpp"

using namespace std;

class Classifier {
public:
	Classifier(int numCaracs, Caracteristique* caracs, double pas,
			int largeur_images, int hauteur_images);

	int getNumCaracteristiques();
	double scoreClassifier(int c, int x, int y, int multiplicateur_x,
			int multiplicateur_y, double composante);
	// void updateWeights(int c, int x, int y, int multiplicateur_x,
	// 		int multiplicateur_y, double composante, int classe);

	// envoie l'addresse de la case que l'on veut
	double *getWeights(int c, int x, int y, int mx, int my, int i);
	bool testClassif();
	void printWeights();

	// void train(ifstream& file);
	// void setWeights(int c, int x, int y, int mx, int my, double weights[2]);


private:
	int numCaracteristiques;
	double weights1[4][112 / 4][92 / 4][26][21];
	double weights2[4][112 / 4][92 / 4][26][21];
	Caracteristique* caracs;
	double epsilon;
	int width;
	int height;
}
;

#endif /* CLASSIFIER_H_ */
