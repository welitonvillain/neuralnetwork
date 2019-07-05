/**
* Seed gera um numeo aleatório entre 0 e 1
*
* Perceptron gera semente e preenche matriz de pesos, além de inicializar o Bias
*
* calcNet faz o somatório dos pesos com as respectivas entradas

**/

#include "perceptron.hpp"

using namespace std;


Perceptron::Perceptron ( unsigned int weights, unsigned int number ) {

    stringstream ss;
    ss << "D:/Development/C++/v-repQt/untitled/w_n" << number << ".txt";
    string str = ss.str();
    string s;

    m_file.open(str);

    if(m_file.is_open()) {
        while ( !m_file.eof() ){
            getline(m_file, s);
            istringstream is(s);
            double line;
            line = strtod(s.c_str(), nullptr);
            is >> line;

            m_weights.push_back(line);
        }
    } else { cout << "Erro de leitura em W" << number << endl; }

    Bias = 1.0;
    m_num_file = number;

    m_file.close();

}


void Perceptron::calcNet ( std::vector < double > inputs ) {

    m_sum = Bias * m_weights.at(0);

    for( unsigned int i=0; i<inputs.size(); i++ ) {
        m_sum += inputs.at(i) * m_weights.at(i+1);
    }

}

void Perceptron::sigmoid () {

    m_sigmoid = 1/(1+exp(-m_sum));

}

void Perceptron::recordFile () {

    stringstream ss;
    ss << "D:/Development/C++/v-repQt/untitled/w_n" << m_num_file << ".txt";
    string str = ss.str();

    m_record.open(str, ios::trunc);
    if( m_record.is_open() ) {

        for ( unsigned int i=0; i<m_weights.size(); i++ ) {
            m_record << m_weights.at(i);
            m_record << "\n";
        }
    } else {
        cout << "Não foi possível gravar pesos W" << m_num_file << endl;
    }

    m_record.close();

}


void Perceptron::updateWeights ( double delta, const double Learning, std::vector < double > inputs ) {

    m_weights.at(0) = m_weights.at(0) - (Learning * delta * Bias);

    for( unsigned int i=0; i<inputs.size(); i++ ) {
        m_weights.at(i+1) = m_weights.at(i+1) - (Learning * delta * inputs.at(i));
    }
}


double Perceptron::getSigmoid () { return m_sigmoid; }

double Perceptron::getWeights ( unsigned int i ) { return m_weights.at(i); }
