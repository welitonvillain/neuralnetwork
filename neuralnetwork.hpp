#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include "perceptron.hpp"

const double Learning = 0.3;


class NeuralNetwork {
private:
    std::vector < double > m_inputs;
    std::vector < Perceptron * > m_hid_layer;
    std::vector < Perceptron * > m_out_layer;
    std::vector < double > m_train;

    std::vector < double > m_delta_hidden;
    std::vector < double > m_delta_out;


public:
    NeuralNetwork( unsigned int n_inputs, unsigned int n_hidden, unsigned int n_out );

    void setInput ( double input );
    void trainNetwork ();

    void recordWeights ();

    std::vector < double > useNetwork ();


};

#endif // NEURALNETWORK_HPP
