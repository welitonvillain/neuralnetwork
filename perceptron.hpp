#ifndef PERCEPTRON_HPP
#define PERCEPTRON_HPP

#define MIN 0.0
#define MAX 1.0

#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <string>

#include <sstream>
#include <fstream>

class Perceptron
{
private:
    std::vector < double > m_weights;
    double Bias;
    double m_sum;
    double m_sigmoid;
    unsigned int m_num_file;

    std::ifstream m_file;
    std::ofstream m_record;

public:
    Perceptron( unsigned int weights, unsigned int number );

    void calcNet ( std::vector < double > inputs );
    void sigmoid ();
    void recordFile ();


    void updateWeights ( double error, const double Learning, std::vector < double > inputs );

    double getSigmoid ();
    double getWeights (unsigned int i);


};

#endif // PERCEPTRON_HPP
