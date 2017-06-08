#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <string>

enum {RED = 0,GREEN = 1,BLUE = 2};

class Pixel{
public:
  Pixel();
  Pixel(int r,int g,int b);
  Pixel(const Pixel& p);
  Pixel(const Pixel& p,const Pixel& q); // initialise comme la somme des deux Pixels

  int getR() const;
  int getG() const;
  int getB() const;

  void addPixel(const Pixel& p);
  void set(int r,int g,int b);
  void set(const Pixel& p);
  void set(const Pixel& p,const Pixel& q); // initialise comme la somme des deux Pixels
  std::string toString();

  static void printPixelMap(Pixel **map, int width, int height);


private:
  int content[3];

};

Pixel operator+(const Pixel& p,const Pixel& q);
Pixel operator*(const Pixel& p,int mult);

#endif //PIXEL_HPP
