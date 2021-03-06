#include <serial/SerialComm.h>
#include <stdlib.h>
#include <iostream>

#ifdef __linux__
#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/select.h>
#include <fcntl.h>
#include <termios.h>
#endif

using namespace std;

SerialComm::SerialComm(string porta) {
	this->porta = porta;
	hPorta = 0;
#ifdef SEM_ARDUINO
	controleId = false;
#endif
}

int SerialComm::iniciar() {
	int result = EXIT_FAILURE;
#ifndef SEM_ARDUINO
#if _WIN32 || _WIN64
	hPorta = CreateFile(porta.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, 0, NULL);

	if (hPorta == INVALID_HANDLE_VALUE) {
		result = GetLastError();
		cerr << "Erro abrindo a porta de comunicacao!" << endl;
	} else {
		// configura timeouts
		COMMTIMEOUTS cto = {1, 100, 1000, 0, 0};
		if (!SetCommTimeouts(hPorta, &cto)) {
			cout << "Nao foi possivel configurar timeouts de comunicacao!"
			<< endl;
		}

		// configura acesso aa porta
		DCB dcb;
		memset(&dcb, 0, sizeof(dcb));
		dcb.DCBlength = sizeof(dcb);
		dcb.BaudRate = CBR_9600;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;

		if (!SetCommState(hPorta, &dcb)) {
			result = GetLastError();
			cerr << "Erro configurando a porta de comunicacao!" << endl;
		} else {
			result = EXIT_SUCCESS; // sem erros
		}
	}
#endif
#ifdef __linux__
	hPorta = open(porta.c_str(),
	O_RDWR | O_NOCTTY);
	if (hPorta < 0) {
		cerr << "Erro abrindo a porta de comunicacao!" << endl;
	} else {
		fcntl(hPorta, F_SETFL, O_NONBLOCK);

		speed_t baud = B9600;

		struct termios settings;
		tcgetattr(hPorta, &settings);

		cfsetospeed(&settings, baud);
		settings.c_cflag &= ~PARENB;
		settings.c_cflag &= ~CSTOPB;
		settings.c_cflag &= ~CSIZE;
		settings.c_cflag |= CS8 | CLOCAL;
		settings.c_lflag = ~ICANON;
		settings.c_cc[VTIME] = 10;
		settings.c_oflag &= ~OPOST;

		tcsetattr(hPorta, TCSANOW, &settings);
		tcflush(hPorta, TCOFLUSH);

		result = EXIT_SUCCESS;
	}
#endif
#endif
	// espera ateh que porta serial esteja pronta para receber comandos
	if (result == EXIT_SUCCESS) {
		Sleep(2000);
	}

	return result;
}

string SerialComm::ler(long unsigned int bytesLer,
		long unsigned int &bytesLidos) {
	string id = "";
#ifdef SEM_ARDUINO
	if (controleId) {
		id = "310047ACD50F";
	}
	else
	{
		id = "05002ED9986A";
	}
	controleId = !controleId;
	bytesLidos = bytesLer;
#else
	bytesLidos = 0;
	long unsigned int tentativas = 5, bytes = 0;
	char buffer[32] = { 0 };
	do {
#if defined _WIN32 || _WIN64
		ReadFile(hPorta, buffer, bytesLer, &bytes, NULL);
#endif
#ifdef __linux__
		bytes = read(hPorta, buffer, bytesLer);
#endif
		if (bytes > 0) {
			id = id + string(buffer);
			bytesLidos += bytes;
		}
		tentativas--;
	} while ((bytesLidos < bytesLer) && (tentativas > 0));
#endif
	return id;
}

void SerialComm::escrever(int comando, long unsigned int &bytesEscritos) {
	bytesEscritos = 0;
	char bcomando = (char) comando;
#ifdef SEM_ARDUINO
	bytesEscritos = sizeof(bcomando);
#else
#if defined _WIN32 || _WIN64
	WriteFile(hPorta, &bcomando, sizeof(bcomando), &bytesEscritos, NULL);
#else
	bytesEscritos = write(hPorta, &bcomando, sizeof(bcomando));
#endif
#endif
}

void SerialComm::escrever(int comando, int parametro,
		long unsigned int &bytesEscritos) {
	bytesEscritos = 0;
	char comandoPar[2];
	comandoPar[0] = (char) comando;
	comandoPar[1] = (char) parametro;
#ifdef SEM_ARDUINO
	bytesEscritos = sizeof(comandoPar);
#else
#if _WIN32 || _WIN64
	WriteFile(hPorta, &comandoPar, sizeof(comandoPar), &bytesEscritos, NULL);
#else
	bytesEscritos = write(hPorta, &comandoPar, sizeof(comandoPar));
#endif
#endif
}

void SerialComm::escrever(int comando, int parametro1, int parametro2,
		long unsigned int &bytesEscritos) {
	bytesEscritos = 0;
	char comandoPars[3];
	comandoPars[0] = (char) comando;
	comandoPars[1] = (char) parametro1;
	comandoPars[2] = (char) parametro2;
#ifdef SEM_ARDUINO
	bytesEscritos = sizeof(comandoPars);
#else
#if defined _WIN32 || _WIN64
	WriteFile(hPorta, &comandoPars, sizeof(comandoPars), &bytesEscritos, NULL);
#else
	bytesEscritos = write(hPorta, &comandoPars, sizeof(comandoPars));
#endif
#endif

}

void SerialComm::finalizar() {
#ifndef SEM_ARDUINO
#if _WIN32 || _WIN64
	CloseHandle(hPorta);
#endif
#endif
}
