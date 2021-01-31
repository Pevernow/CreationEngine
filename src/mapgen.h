#ifndef NOISE_H
#define NOISE_H

class Perlin
{
public:
    float persistence;
    int Number_Of_Octaves;
    Perlin();

    double PerlinNoise(float x, float y);

private:
    double Noise(int x, int y); // 根据(x,y)获取一个初步噪声值

    double SmoothedNoise(int x, int y); //光滑噪声

    double Cosine_Interpolate(double a, double b, double x); // 余弦插值

    double Linear_Interpolate(double a, double b, double x); //线性插值

    double InterpolatedNoise(float x, float y); // 获取插值噪声
};
#endif // NOISE_H