#include "sculptor.h"
#include <iostream>
#include <cmath> //funções matemáticas
#include <fstream> //inclusão do fluxo para arquivos
#include <cstdlib> //inclusão da função exit
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

/*=================================================================================
IMPLEMENTAÇÃO DO CONSTRUTOR DA CLASSE:
==================================================================================*/
Sculptor::Sculptor(int _nx, int _ny, int _nz) { //parametro da função

    //Passagem dos valores dos parametros para as variáveis privadas:
    nx = _nx;
    ny = _ny;
    nz = _nz;

    // Atribuindo zero as  variáveis de cores e transparência:
    r = 0;
    g = 0;
    b = 0;
    a = 0;

    //Declarando incrementadores dos laços:
    int i, j, k;

    //ALOCANDO MEMÓRIA PARA MATRIZ 3D:
    //Alocando memória para o array auxiliar que receberá endereços das linhas onde inicia-se cada plano:
    v = new Voxel **[nx];
    //Alocando memória para o array que receberá o endereço do voxel de inicio de cada linha:
    v[0] = new Voxel *[nx * ny];
    //Alocando memória  para todos os voxels da matriz:
    v[0][0] = new Voxel [nx * ny * nz];
    //Passando os endereços de inicio de cada plano para o array auxiliar:
    for (i = 1; i < nx; i++){
        v[i] = v[i - 1] + ny;
    }
    //Passando os endereços dos voxels de início de cada linha de cada plano:
    for(i = 0; i < nx; i++){
        if(i != 0){
            v[i][0] = v[i - 1][ny - 1] + nz;
        }
        for (j = 1; j < ny; j++){
            v[i][j] = v[i][j - 1] + nz;
        }
    }

    //Iniciando os voxels com cores e transparência zero e estado false:
    for(i = 0; i < nx; i++){
        for (j = 0; j < ny; j++){
            for (k = 0; k < nz; k++){
                this -> v[i][j][k].r = this -> r;
                this -> v[i][j][k].g = this -> g;
                this -> v[i][j][k].b = this -> b;
                this -> v[i][j][k].a = this -> a;
                this -> v[i][j][k].show = false;
            }
        }
    }
}

/*=================================================================================
IMPLEMENTO DO DESTRUIDOR DE CLASSES:
=================================================================================*/
Sculptor::~Sculptor(){
    //Liberação da memória:
    delete[] v[0][0];
    delete[] v[0];
    delete[] v;
}

/*=================================================================================
IMPLEMENTAÇÃO DO SETCOLOR:
==================================================================================*/
void Sculptor::setColor(float r, float g, float b, float a){
    //r,g,b,a privados recebendo os valores dos parametros:
    this -> r = r;
    this -> g = g;
    this -> b = b;
    this -> a = a;
}

/*=================================================================================
IMPLEMENTAÇÃO DA ESFERA COM TEXTURA:
==================================================================================*/
void Sculptor::putTextureOnSphere(int xcenter, int ycenter, int zcenter, int radius, cv::Mat &image){
    if(image.empty()){
        cout << "Não foi possível abrir a imagem" << endl;
    }
    int i, j, k;
    for(i = 0; i < nx; i++){
        for(j = 0; j < ny; j++){
            for(k = 0; k < nz; k++){
                //Condição que verifica se o voxel satisfaz a equação da esfera:
                if((pow(i - xcenter, 2)) + (pow(j - ycenter, 2)) + (pow(k - zcenter, 2)) <= (pow(radius, 2))){
                    // Conversão das coordenadas da esfera de cartesiana para esférica:
                    double theta = atan2(j - ycenter, i - xcenter);
                    double phi = acos((k - zcenter) / (double)radius);
                    // Mapeamento das coordenadas esféricas para as coordenadas da imagem:
                    int u = image.cols * (theta + M_PI) / (2 * M_PI); //theta
                    int v = image.rows * phi / M_PI; //phi
                    // color recebe as cores dos pixels:
                    cv::Vec3b color = image.at<cv::Vec3b>(v, u);
                    // Atribuição dessas cores ao voxel:
                    this -> v[i][j][k].show = true;
                    this -> v[i][j][k].r = (color[2]/255.0f); // R
                    this -> v[i][j][k].g = (color[1]/255.0f); // G
                    this -> v[i][j][k].b = (color[0]/255.0f); // B
                    this -> v[i][j][k].a = this -> a;
                }
            }
        }
    }
}


/*=================================================================================
 IMPLEMENTAÇÃO DO CUTSPHERE:
=================================================================================*/
void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius){
    int i, j, k;

    for(i = 0; i < nx; i++){
        for(j = 0; j < ny; j++){
            for(k = 0; k < nz; k++){
                if((pow(i - xcenter, 2)) + (pow(j - ycenter, 2)) + (pow(k - zcenter, 2)) <= (pow(radius, 2))){
                    this -> v[i][j][k].show = false;
                }
            }
        }
    }
}


void Sculptor::planetfile (const char* planetvoxels){
    int i, j, k;
    ofstream fout; //Objeto para fluxo de saida
    fout.open(planetvoxels);
    //Verificação da abertura do fluxo
    if(!fout.is_open()){
        cout << "O arquivo não pôde ser aberto" << endl;
        exit(1);
    }

    fout << "Dim " << nx << " "<< ny << " " << nz << endl;
    // fout << n_voxels*8 << " " << n_voxels*6 << " " << n_voxels*0 << endl;

    for(i = 0; i < nx; i++){
        for(j = 0; j < ny; j++){
            for(k = 0; k <nz; k++){
                if(v[i][j][k].show == true){
                    fout << "PutVoxel "<< i << " " << j << " " << k << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << endl;

                }
            }
        }
    }
    fout.close();
}
