#include "caracteristique.hpp"
#include <iostream>

Caracteristique::Caracteristique(){
  largeur = 0;
  hauteur = 0;
}

Caracteristique::Caracteristique(int l, int h){
  largeur = l;
  hauteur = h;
}

Caracteristique::Caracteristique(int pix[3][3],int l,int h){
  largeur = l;
  hauteur = h;

  for(int i = 0;i<l;i++){
    for(int j=0;j<h;j++){
      pixels[i][j] = pix[i][j];
    }
  }
}

int Caracteristique::getHauteur(int multiplicateur_y){
  return hauteur*(pasFace+pasPixel*multiplicateur_y);
}

int Caracteristique::getLargeur(int multiplicateur_x) {
	return largeur*(pasFace+pasPixel*multiplicateur_x);
}

double Caracteristique::application(int ii[113][93], int multiplicateur_x, int multiplicateur_y,
			int position_x, int position_y) {
		double sum=0;

		for (int i=0;i<largeur;i++) {
			for (int j=0;j<hauteur;j++) {
				sum += pixels[i][j] *
                (ii[position_x + i * (pasFace + pasPixel * multiplicateur_x)][position_y + j * (pasFace + pasPixel * multiplicateur_y)] +
                  ii[position_x + (i+1) * (pasFace + pasPixel * multiplicateur_x)][position_y + (j+1) * (pasFace + pasPixel * multiplicateur_y)] -
                  ii[position_x + i * (pasFace + pasPixel * multiplicateur_x)][position_y + (j+1) * (pasFace + pasPixel * multiplicateur_y)] -
                  ii[position_x + (i+1) * (pasFace + pasPixel * multiplicateur_x)][position_y + j * (pasFace + pasPixel * multiplicateur_y)])
                  / getHauteur(multiplicateur_y) / getLargeur(multiplicateur_x);
      }
		}

    return sum;
	}
