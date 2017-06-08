#ifndef MPI_MANAGER_HPP
#define MPI_MANAGER_HPP

#include <fstream>

#include "integralimage.hpp"
#include "caracteristique.hpp"
#include "classifier.hpp"

static int root = 0;

class MPI_Manager{
  public:
    MPI_Manager(/*char* source_name, char* output_name,Classifier& classifier,*/double e);

    void MPI_Run();

    void newWeights(double oldWeights[2],double valeur);
    void MPI_Train(char* source_name, int classe);
    void printClassifWeights();
    Caracteristique *getCaracs();

  private:
    IntegralImage* img;
    Caracteristique *caracs;
    ofstream myfile;
    Classifier* classifieur;

    int numCaracs;
    double epsilon;
};

#endif // MPI_MANAGER
