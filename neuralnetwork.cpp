#include "neuralnetwork.hpp"


NeuralNetwork::NeuralNetwork ( unsigned int n_inputs, unsigned int n_hidden, unsigned int n_out ) {

    for( unsigned int i=0; i<n_hidden; i++ ) {
        Perceptron * p = new Perceptron ( n_inputs, i+1 );
        m_hid_layer.push_back(p);
    }

    for( unsigned int i=0; i<n_out; i++ ) {
        Perceptron * p = new Perceptron ( n_hidden, i+20 );
        m_out_layer.push_back(p);
    }


    m_train.push_back(0);
    m_train.push_back(0);
}


void NeuralNetwork::setInput ( double input ) { m_inputs.push_back(input); }



double gradientOut ( double base, double sig ) {
    double grad = - (( base - sig ) * (sig * ( 1 - sig )));

    return grad;
}

double gradientHidden ( double sig ) {
    double grad = (sig * ( 1 - sig ));

    return grad;
}


void NeuralNetwork::trainNetwork () {

    std::vector < double > temp;
    std::vector < double > out_hidden;
    std::vector < double > grad;

    int check=1;

    for( unsigned int i=0; i<m_inputs.size()-5; i+=6 ) {

        if( check == 1 ) { m_train.at(0) = 1.0; m_train.at(1) = 0.0; }
        if( check == 2 ) { m_train.at(0) = 0.0; m_train.at(1) = 1.0; }
        if( check == 3 ) { m_train.at(0) = 0.0; m_train.at(1) = 0.0; }


        for( unsigned int k=i; k<i+6; k++ ) {
            temp.push_back(m_inputs.at(k));
        }



        for( unsigned int j=0; j<m_hid_layer.size(); j++ ) {
            m_hid_layer.at(j)->calcNet(temp);
            m_hid_layer.at(j)->sigmoid();
            out_hidden.push_back(m_hid_layer.at(j)->getSigmoid());
        }

        for ( unsigned int n=0; n<m_out_layer.size(); n++ ) {
            m_out_layer.at(n)->calcNet(out_hidden);
            m_out_layer.at(n)->sigmoid();

            grad.push_back(gradientOut( m_train.at(n), m_out_layer.at(n)->getSigmoid()) );

        }

        // Calcula Delta Oculta
        for( unsigned int i=0; i<m_hid_layer.size(); i++ ) {
           m_delta_hidden.push_back( grad.at(0) * m_out_layer.at(0)->getWeights(i+1) * gradientHidden( m_hid_layer.at(i)->getSigmoid()) );
           m_delta_hidden.push_back( grad.at(1) * m_out_layer.at(1)->getWeights(i+1) * gradientHidden( m_hid_layer.at(i)->getSigmoid()) );
        }

        // Atualiza Pesos

        m_out_layer.at(0)->updateWeights(grad.at(0), Learning, out_hidden);
        m_out_layer.at(1)->updateWeights(grad.at(1), Learning, out_hidden);

        for( unsigned int i=0; i<m_hid_layer.size(); i++ ) {
            m_hid_layer.at(i)->updateWeights(m_delta_hidden.at((i*2)), Learning, temp);
            m_hid_layer.at(i)->updateWeights(m_delta_hidden.at((i*2)+1), Learning, temp);
        }

        temp.erase(temp.begin(), temp.end());
        out_hidden.erase(out_hidden.begin(), out_hidden.end());
        m_delta_hidden.erase(m_delta_hidden.begin(), m_delta_hidden.end());
        grad.erase(grad.begin(), grad.end());

        if( check == 1) { check++; }
        else if( check == 2) { check++; }
        else if( check == 3) {check = 1; }

    }


}

void NeuralNetwork::recordWeights () {
    for( unsigned int i=0; i<m_hid_layer.size(); i++ ) {
        m_hid_layer.at(i)->recordFile();
    }

    for( unsigned int i=0; i<m_out_layer.size(); i++ ) {
        m_out_layer.at(i)->recordFile();
    }

}



std::vector < double > NeuralNetwork::useNetwork () {
    m_train.erase(m_train.begin(), m_train.end());
    m_delta_hidden.erase(m_delta_hidden.begin(), m_delta_hidden.end());


    for( unsigned int i=0; i<m_hid_layer.size(); i++ ) {
        m_hid_layer.at(i)->calcNet(m_inputs);
        m_hid_layer.at(i)->sigmoid();

        m_delta_hidden.push_back(m_hid_layer.at(i)->getSigmoid());

    }

    for( unsigned int i=0; i<m_out_layer.size(); i++ ){
        m_out_layer.at(i)->calcNet(m_delta_hidden);
        m_out_layer.at(i)->sigmoid();

        m_train.push_back(m_out_layer.at(i)->getSigmoid());
    }

    m_inputs.erase(m_inputs.begin(), m_inputs.end());

    return m_train;

}

