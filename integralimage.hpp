#ifndef INTEGRAL_IMAGE_HPP
#define INTEGRAL_IMAGE_HPP

#include <fstream>

#include "caracteristique.hpp"
#include "string"

using namespace std;

class IntegralImage{
public:
  IntegralImage(char* source,int classe_entree);

  int getWidth();
  int getHeight();
  int getClass();

  void traiterPixel(int x, int y, Caracteristique* caracs, ofstream& file);
  double traiterUnite(int x, int y, int mx, int my, Caracteristique* carac);

private:
  int width;
  int height;
  int map[112][92];
  int ii[113][93];
  int classe;
};

#endif //INTEGRAL_IMAGE_HPP
