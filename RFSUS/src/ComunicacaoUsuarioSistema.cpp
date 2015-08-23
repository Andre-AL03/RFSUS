/*
 * ComunicaçãoUsuarioSistema.cpp
 *
 *  Created on: 12/08/2015
 *      Author: andre
 */

#include "serial/SerialComm.h"
#include "observadores/ObservadorSerial.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

// convencoes de configuracao da comunicao serial/usb
#define COM_PORTA "/dev/ttyACM0"     // porta de comunicacao USB com Arduino

#ifdef __linux__
// macro para acesso a comando de sleep no linux
#include <unistd.h>
#define Sleep(x) usleep(x * 1000);
#endif

/*string valorLido = "";
 long unsigned int bytesLer = 15;
 long unsigned int bytesLidos;

 void execute() {
 while (true) {
 // restricao de tempo entre atualizacoes
 Sleep(INTERVALO_LEITURA);

 valorLido = serial.ler(bytesLer, bytesLidos);

 if (bytesLidos > 0) {
 cout << valorLido << endl;
 } else {
 cout << "000000000000" << endl;
 }
 }
 }*/

int main() {

	// cria instancia de classe comunicacao USB
	SerialComm serial(COM_PORTA);

	// cria instancia de classe Observador
	ObservadorSerial observador(serial);

	//if (serial.iniciar() == 0) {
		cout << "Comunicação estabelecida" << endl;
		observador.iniciar();
	//}

	while(true){
		Sleep(1000);
		cout << "To dormindo" << endl;
	}
}

