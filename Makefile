all: classifier.o integralimage.o caracteristique.o mpi_main.o mpi_manager.o BoostingDetection

CXX = mpic++ -O3

BoostingDetection: *.o
	$(CXX) -g -o BoostingDetection *.o

%.o: %.cpp
	$(CXX) -c -Wall $< -o $@

clean:
		rm *.o
		rm BoostingDetection
