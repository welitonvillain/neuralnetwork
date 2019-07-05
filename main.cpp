#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#define MIN -1.0
#define MAX 1.0

#include "neuralnetwork.hpp"

//simxSetJointTargetVelocity(clientID, left_motor_handle, (simxFloat) 2.0, simx_opmode_streaming);
//simxSetJointTargetVelocity(clientID, right_motor_handle, (simxFloat) 2.0, simx_opmode_streaming);


extern "C" {
  #include "remoteApi/extApi.h"
}

using namespace std;

int connectServer( string serverIP, int serverPort );
int connectMotorLeft( int clientID, int left_motor_handle );
int connectMotorRight( int clientID, int right_left_handle );
vector < int > connectProximitySensors( unsigned int n_sensors, int clientID );
double seed ( double min, double max );



int main(int argc, char **argv) {

    // Trecho usado na Etapa de treinamento

    //srand(time(NULL));

    /*ifstream file;
    string str;

    file.open("D:/Development/C++/v-repQt/untitled/data1.txt", ios::app);

    NeuralNetwork * network = new NeuralNetwork( 6, 15, 2);


    if( file.is_open() ){
        while ( !file.eof() ){
            getline(file, str);

            istringstream is(str);
            double line;
            line = strtod(str.c_str(), nullptr);
            is >> line;

            network->setInput(line);
        }
    }

    for ( unsigned int i=0; i<25000; i++ ) {
        network->trainNetwork();
    }

    network->recordWeights();*/




    int left_motor_handle = -1;
    int right_motor_handle = -1;
    vector < int > proximity_sensor;
    vector < int > ultrasonic;
    unsigned int n_sensors = 6;
    vector < string > name_sensors;
    simxUChar state_sensor;
    simxFloat detect_point[3];
    vector < double > motor;

    float mLeft;
    float mRight;

    int clientID = connectServer( "127.0.0.1", 19999 );

    if ( clientID != -1 ) {
        cout << "Servidor Conectado!!" << endl;

        NeuralNetwork * network = new NeuralNetwork( n_sensors, 10, 2);

        for( unsigned int i=0; i<n_sensors; i++ ) { proximity_sensor.push_back(0); }

        // conectando aos motores
        left_motor_handle = connectMotorLeft( clientID, left_motor_handle );
        right_motor_handle = connectMotorRight( clientID, right_motor_handle );


        // conectando aos sensores de proximidade
        proximity_sensor = connectProximitySensors( n_sensors, clientID );

        while (simxGetConnectionId(clientID) != -1) {
            mLeft = 1.0;
            mRight = 1.0;

            int detected = 0;

            simxSetJointTargetVelocity(clientID, left_motor_handle, (simxFloat) mLeft, simx_opmode_streaming);
            simxSetJointTargetVelocity(clientID, right_motor_handle, (simxFloat) mRight, simx_opmode_streaming);

            for( unsigned int i=0; i<n_sensors; i++ ) {
                simxReadProximitySensor(clientID, proximity_sensor.at(i), &state_sensor, detect_point, nullptr, nullptr, simx_opmode_streaming);

                if( state_sensor <= 0 ) { detect_point[2] = 1.0; detected++; }
                network->setInput(detect_point[2]);
            }

            motor = network->useNetwork();

            if( detected >= 6 ){
                simxSetJointTargetVelocity(clientID, left_motor_handle, (simxFloat) mLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, right_motor_handle, (simxFloat) mRight, simx_opmode_streaming);
            } else {
                simxSetJointTargetVelocity(clientID, left_motor_handle, (simxFloat) (mLeft - motor.at(1)), simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, right_motor_handle, (simxFloat) (mRight - motor.at(0)), simx_opmode_streaming);
            }

            extApi_sleepMs(5);
        }

    }

    //file.close();

    return 0;
}


int connectServer( string serverIP, int serverPort ) {
   int clientID = simxStart((simxChar *) serverIP.c_str(), serverPort, true, true, 2000, 5);

   return clientID;
}

int connectMotorLeft( int clientID, int left_motor_handle ) {

    if( simxGetObjectHandle(clientID, (const simxChar* ) "Pioneer_p3dx_leftMotor", (simxInt* ) &left_motor_handle, (simxInt) simx_opmode_oneshot_wait) == simx_return_ok) {
        cout << "Conectado ao motor esquerdo!" << endl;
        return left_motor_handle;
    } else {
        cout << "Não foi possível se conectar ao motor esquerdo! " << endl;
        return -1;
    }
}

int connectMotorRight( int clientID, int right_motor_handle ) {

    if( simxGetObjectHandle(clientID, (const simxChar* ) "Pioneer_p3dx_rightMotor", (simxInt* ) &right_motor_handle, (simxInt) simx_opmode_oneshot_wait) == simx_return_ok) {
        cout << "Conectado ao motor direito!" << endl;
        return right_motor_handle;
    } else {
        cout << "Não foi possível se conectar ao motor direito! " << endl;
        return -1;
    }
}

vector < int > connectProximitySensors( unsigned int n_sensors, int clientID ) {
    vector < int > p_sensor;
    vector < string > name_sensors;

    for( unsigned int i=0; i<n_sensors; i++ ) {
        name_sensors.push_back("Proximity_sensor" + to_string(i + 1));
        p_sensor.push_back(0);


        if ( simxGetObjectHandle(clientID, (simxChar *) name_sensors.at(i).c_str(), (simxInt *) &p_sensor.at(i), (simxInt) simx_opmode_oneshot_wait) == simx_return_ok) {
            cout << "Conectado ao sensor " << i+1 << endl;
            cout << p_sensor.at(i) << endl;
        }

    }

    return p_sensor;

}

double seed (double min, double max) {
    double a = min + (rand() / (RAND_MAX / ( max - min )));
    cout << a << endl;
    return a ;
}

