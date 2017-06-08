#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>

#include "pixel.hpp"
#include "integralimage.hpp"
#include "caracteristique.hpp"

static const int width = 112;
static const int height = 92;


using namespace std;

struct thread_data{
   int  thread_id;
   int  modulo;

   int nb_threads;

   Pixel *****matToFill;
   Pixel **ii;
   Caracteristique *caracs;
};



void *actionThread(void *threadarg){
  struct thread_data *my_data;
  my_data = (struct thread_data *) threadarg;

  int nb_threads = my_data->nb_threads;
  int modulo = my_data->modulo;

  Pixel *****matToFill = my_data->matToFill;
  Pixel **ii;
  Caracteristique *caracs = my_data->caracs;

  int indice, x, y, x_vrai, y_vrai, multiplicateur_x, multiplicateur_y, larg, haut;

  cout << "Arrivé ici" << endl;

  for(indice = modulo ; indice < width * height ; indice += nb_threads*pasPixel){
    x_vrai = indice % height;      // abscisse du point de départ
    y_vrai = (int) indice / height; // ordonnée du point de départ

    x = (int) x_vrai / pasPixel;
    y = (int) y_vrai / pasPixel;

    cout << "Bien ici" << endl;

    for(int i = 0;i<4;i++){ //pour chaque caracteristique

      multiplicateur_x = 0; //gère la largeur de la caract.
      multiplicateur_y = 0; //gère la hauteur de la caract.

      int nbHaut = (int) max((height - x_vrai - caracs[i].getHauteur(0))/pasPixel,1);
      int nbLarg = (int) max((width - y_vrai - caracs[i].getLargeur(0))/pasPixel,1);
      // cout << "nbHaut = "<<nbHaut<<"; nbLarg = "<<nbLarg<<endl;
      // cout << "x_vrai = "<<x_vrai<<"; y_vrai = "<<y_vrai<<endl;
      // cout << "problème là ? x = " << x << "; y = " << y;
      matToFill[i][x][y] = new Pixel*[nbLarg];
      // cout << " --> non" << endl;

      for(int k=0;i<nbLarg;i++){
        // cout << "problème ici ? ";
        matToFill[i][x][y][k] = new Pixel[nbHaut];
        // cout << "non" << endl;
      }

      while((haut = caracs[i].getHauteur(multiplicateur_x))<height){
        multiplicateur_y = 0;
        while((larg = caracs[i].getLargeur(multiplicateur_y))<width){
          matToFill[i][x][y][multiplicateur_x][multiplicateur_y].set(caracs[i].application(ii,multiplicateur_x,multiplicateur_y,x_vrai,y_vrai));
          multiplicateur_y++;
        }
        multiplicateur_x++;
      }
    }
  }

  pthread_exit(NULL);
}

void matriceCaracteristiques(int nb_threads, Pixel *****matToFill, Caracteristique *caracs, Pixel **ii){
  pthread_t threads[nb_threads];
  struct thread_data td[nb_threads];
  int rc;
  int i;
  for( i=0; i < nb_threads; i++ ){
    cout << "matriceCaracteristiques(...) : creating thread, " << i << endl;
    td[i].thread_id = i;
    td[i].modulo = i*pasPixel;
    td[i].nb_threads = nb_threads;
    td[i].matToFill = matToFill;
    td[i].ii = ii;
    td[i].caracs = caracs;
    rc = pthread_create(&threads[i], NULL, actionThread, (void *)&td[i]);
    if (rc){
       cout << "Error: unable to create thread," << rc << endl;
       exit(-1);
    }
  }
}

int main(int argc, char** argv) {

  /**** Gestion des arguments ****/
  if(argc != 2){
    printf("BoostingDetection <nombre_de_threads>\n");
    return 0;
  }

  int nb_threads = atoi(argv[1]);

  /**** Tests ****/
  Pixel* p = new Pixel(2,5,8);
  Pixel* q = new Pixel(8,4,6);
  cout << (*p+*q).toString() << endl;

  /**** Question 1 : calcul d'image intégrale****/
  IntegralImage *img = new IntegralImage((char*) "d.txt");
  Pixel::printPixelMap(img->getIntegralImage(),img->getWidth(),img->getHeight());

  /**** Question 1.2 : calcul parallèle de l'ensemble des caractéristiques de l'image****/
  Pixel *****matriceCarac = (Pixel*****) new Pixel**[4][(int) width / pasPixel][(int) height / pasPixel];
  cout << "Dimensions de matriceCarac : 4x"<<(int) width / pasPixel<<"x"<<(int) height / pasPixel<<endl;

    //1è dimension : caractéristique utilisée
    //2è dimension : abscisse du pixel de base
    //3è dimension : ordonnée du pixel de base
    //4è dimension : largeur de la caractéristique
    //5è dimension : hauteur de la caractéristique

  Caracteristique *caracs = new Caracteristique[4];

  /*  ____ ____
     |    |####|
     |____|####|
     |####|    |
     |####|____|
  */

  int pix1[2][2];
  pix1[0][0] = 1;
  pix1[0][1] = -1;
  pix1[1][0] = -1;
  pix1[1][1] = 1;

/*    ____ ____ ____
     |    |####|    |
     |____|####|____|
  */

  int pix2[1][3];
  pix2[0][0] = -1;
  pix2[0][1] = 1;
  pix2[0][2] = -1;

/*    ____ ____
     |    |####|
     |____|####|
  */

  int pix3[1][2];
  pix3[0][0] = -1;
  pix3[0][1] = 1;

  /*  ____
     |    |
     |____|
     |####|
     |####|
  */

  int pix4[2][1];
  pix4[0][0] = 1;
  pix4[1][0] = -1;

  caracs[0].set((int**) pix1,2,2);
  caracs[1].set((int**) pix2,3,1);
  caracs[2].set((int**) pix3,2,1);
  caracs[3].set((int**) pix4,1,2);

  matriceCaracteristiques(nb_threads,matriceCarac,caracs,img->getIntegralImage());

  return 0;
}
