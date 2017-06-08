#include <stdlib.h>
#include "iostream"
#include "fstream"
#include "sstream"

#include "caracteristique.hpp"
// #include "pixel.hpp"
#include "integralimage.hpp"

using namespace std;

IntegralImage::IntegralImage(char* source, int classe_entree) {

	classe = classe_entree;
	ifstream sourceFile(source);

	if (!sourceFile) {
		cerr << "Failed to open " << sourceFile << endl;
		exit(EXIT_FAILURE);  //abort program
	}

	stringstream buffer;

	buffer << sourceFile.rdbuf();
	sourceFile.close();

	buffer >> height;
	buffer >> width;

	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			buffer >> map[x][y];
		}
	}

	buffer.str(std::string());

	// Calcul de l'image intégrale

	int **s = new int*[height];
	for (int i = 0; i < height; i++) {
		// ii[i] = new int[width];
		s[i] = new int[width];
	}

	for (int x = 0; x < height; x++) { //On remplit s initialement
		int sum = 0;
		for (int y = 0; y < width; y++) {
			sum += map[x][y];
			s[x][y] = sum;
		}
	}

	for (int y = 0; y < width; y++) {
		ii[0][y] = s[0][y];
	}

	for (int x = 1; x < height; x++) {
		for (int y = 0; y < width; y++) {
			ii[x][y] = ii[x - 1][y] + s[x][y];
		}
		ii[x][width] = ii[x][width-1];
	}
	for(int y = 0; y < width; y++)
		ii[height][y] = ii[height-1][y];
	ii[height][width] = ii[height -1][width -1];

	delete[] s;
}

int IntegralImage::getWidth() {
	return width;
}
int IntegralImage::getHeight() {
	return height;
}
int IntegralImage::getClass() {
	return classe;
}

void IntegralImage::traiterPixel(int x, int y, Caracteristique* caracs,
		ofstream& file) {
	int dimx = width - x;
	int dimy = height - y;

	int multiplicateur_x, multiplicateur_y;

	for (int c = 0; c < 4; c++) { // pour chaque caracs
		multiplicateur_y = 0;

		while (caracs[c].getHauteur(multiplicateur_y) <= dimy) {
			multiplicateur_x = 0;
			while (caracs[c].getLargeur(multiplicateur_x) <= dimx) {
				file << c << "\t" << x << "\t" << y << "\t" << multiplicateur_x
						<< "\t" << multiplicateur_y << "\t";
				file
						<< (caracs[c].application(ii, multiplicateur_x,
								multiplicateur_y, x, y));
				file << "\n";
				multiplicateur_x++;
			}
			multiplicateur_y++;
		}
	}
}

double IntegralImage::traiterUnite(int x, int y, int mx, int my,
		Caracteristique* carac) {
	return carac->application(ii, mx, my, x, y);
}
