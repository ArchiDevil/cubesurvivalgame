#pragma once

#define DEFAULT_OCTAVES 6
#define DEFAULT_FREQUENCY 1.0
#define DEFAULT_LACUNARITY 2.0
#define DEFAULT_PERSISTENCE 0.5

double dot(int * g, double x, double y);
double dot(int * g, double x, double y, double z);

class cNoise
{
public:
    cNoise();

    double SimplexNoise(double x, double y, double z);
    double SimplexNoise(double x, double y);

    void SetFrequency(double freq);
    void SetPersistence(double pers);
    void SetLacunarity(double lac);
    void SetSeed(int seed);
    void SetOctaves(int oct);

private:
    double Noise(double xin, double yin);
    double Noise(double xin, double yin, double zin);

    int grad3[12][3];
    int perm[512];

    int m_octaves;
    double m_frequency;
    double m_persistence;
    double m_lacunarity;

};
