#include <iostream>
#include <sstream>
#include "pixel.hpp"

using namespace std;

Pixel::Pixel(){
  content[RED]=0;
  content[GREEN]=0;
  content[BLUE]=0;
}

Pixel::Pixel(int r,int g,int b){
  content[RED]=r;
  content[GREEN]=g;
  content[BLUE]=b;
}

Pixel::Pixel(const Pixel& p){
  content[RED]=p.getR();
  content[GREEN]=p.getG();
  content[BLUE]=p.getB();
}

Pixel::Pixel(const Pixel& p,const Pixel& q){
  content[RED]=p.getR()+q.getR();
  content[GREEN]=p.getG()+q.getG();
  content[BLUE]=p.getB()+q.getB();
}

int Pixel::getR() const{
  return content[RED];
}

int Pixel::getG() const{
  return content[GREEN];
}

int Pixel::getB() const{
  return content[BLUE];
}

void Pixel::addPixel(const Pixel& p){
  content[RED]+=p.getR();
  content[GREEN]+=p.getG();
  content[BLUE]+=p.getB();
}

void Pixel::set(int r, int g, int b){
  content[RED]=r;
  content[GREEN]=g;
  content[BLUE]=b;
}

void Pixel::set(const Pixel& p){
  content[RED]=p.getR();
  content[GREEN]=p.getG();
  content[BLUE]=p.getB();
}

void Pixel::set(const Pixel& p,const Pixel& q){
  content[RED]=p.getR()+q.getR();
  content[GREEN]=p.getG()+q.getG();
  content[BLUE]=p.getB()+q.getB();
}

Pixel operator+(const Pixel& p,const Pixel& q){
  Pixel* temp = new Pixel(p,q);
  return *temp;
}

Pixel operator*(const Pixel& p,int mult){
  Pixel *temp = new Pixel(mult * p.getR(),
                          mult * p.getG(),
                          mult * p.getB());
  return *temp;
}

string Pixel::toString(){
  stringstream sstm;
  sstm << "("<<content[RED]<<","<<content[GREEN]<<","<<content[BLUE]<<")";

  return sstm.str(); //content[RED]+","+content[GREEN]+","+content[BLUE]+")";
}

void Pixel::printPixelMap(Pixel **map, int width, int height){
  cout << "Printing PixelMap : " << endl
       << "\theight = " << height << endl
       << "\twidth = " << width << endl;

  for (int i = 0;i<height;i++){
    for (int j = 0;j<width;j++){
      cout << map[i][j].toString();
    }
    cout << endl;
  }
}
