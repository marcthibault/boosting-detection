#ifndef CARACTERISTIQUE_HPP
#define CARACTERISTIQUE_HPP

static const int pasPixel = 4;
static const int pasFace = 8;

class Caracteristique{
public:
  Caracteristique();
  Caracteristique(int l,int h);
  Caracteristique(int pix[3][3],int l,int h);

  int getHauteur(int multiplicateur_y);
  int getLargeur(int multiplicateur_x);
  double application(int ii[113][93],int multiplicateur_x, int multiplicateur_y,
			int position_x, int position_y);

private:
  int largeur;  // en nombre de blocs
  int hauteur;  // en nombre de blocs
  int pixels[3][3]; // les blocs
};

#endif //CARACTERISTIQUE_HPP
