/*
 * ComunicaçãoUsuarioSistema.cpp
 *
 *  Created on: 12/08/2015
 *      Author: andre
 */

#include "include/serial/SerialComm.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// convencoes de configuracao da comunicao serial/usb
#define COM_PORTA "/dev/ttyACM0"     // porta de comunicacao USB com Arduino

using namespace std;

// cria instancia de classe comunicacao USB
SerialComm serial(COM_PORTA);

#define INTERVALO_LEITURA         500 		// intervalo entre cada leitura do joystick e envio de comando

#ifdef __linux__
// macro para acesso a comando de sleep no linux
#include <unistd.h>
#define Sleep(x) usleep(x * 1000);
#endif

long unsigned int bytesLer = 15;
long unsigned int bytesLidos;
string valorLido = "";

void execute() {
	while (true) {
		// restricao de tempo entre atualizacoes
		Sleep(INTERVALO_LEITURA);

		valorLido = serial.ler(bytesLer, bytesLidos);

		if(bytesLidos > 0){
			cout << valorLido << endl;
		}
		else {
			cout << "000000000000" << endl;
		}
	}
}

int main() {

	if (serial.iniciar() == 0) {
		cout << "Comunicação estabelecida" << endl;
		execute();
	}
}

