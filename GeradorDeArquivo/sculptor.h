#ifndef SCULPTOR_H
#define SCULPTOR_H
#include <opencv2/opencv.hpp>


struct Voxel {
    float r;
    float g;
    float b; // Cores
    float a; // Transparencia
    bool show; // Incluido ou nao o voxel
};

class Sculptor {
private:
    Voxel ***v; // 3D matrix
    int nx; //Dimensão x
    int ny; // Dimensão y
    int nz; // Dimensão z
    float r;
    float g;
    float b;
    float a; // cores e transparência
public:
    Sculptor(int _nx, int _ny, int _nz); //Construtor da classe
    ~Sculptor();
    void setColor(float r, float g, float b, float a);
    void putTextureOnSphere(int xcenter, int ycenter, int zcenter, int radius, cv::Mat &image); //CASCA E TEXTURA
    void cutSphere(int xcenter, int ycenter, int zcenter, int radius);
    void planetfile (const char* planetvoxels);

};


#endif // ESCULTORDIGITAL_H
