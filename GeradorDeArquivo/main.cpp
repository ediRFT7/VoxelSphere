#include <sculptor.h>
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;

int main(){
    Sculptor escultura(200, 200, 200);
    string filename = "/home/rafaett/VoxelSphere/arquivos/terra.jpg";
    cv::Mat image = cv::imread(filename);


    //Criação da casca esférica:
    escultura.putTextureOnSphere(100, 100, 100, 45, image);
    escultura.cutSphere(100, 100, 100, 43); //remoção da parte interna da esfera
    escultura.planetfile((char *) "/home/rafaett/VoxelSphere/arquivos/planetparameters.txt");

    return 0;
}
