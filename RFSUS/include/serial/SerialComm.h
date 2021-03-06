/*
 * SerialComm.h
 *
 * Realiza a leitura e escrita de informacoes
 * atraves da porta serial/usb do computador.
 *
 * Created on: 27/06/2013
 * Author: Luis Paulo
 */
#ifndef LEITORRFID_H_
#define LEITORRFID_H_

#include <string>
#if _WIN32 || _WIN64
#include <windows.h>
#endif

//#define SEM_ARDUINO
#define COM_ARDUINO

using namespace std;

#ifdef __linux__
// macro para acesso a comando de sleep no linux
#include <unistd.h>
#define Sleep(x) usleep(x * 1000);
#endif

class SerialComm {
private:
	// nome/identificacao da porta
	string porta;
#ifdef __linux__
	int hPorta;
#endif
#if _WIN32 || _WIN64
	// handle para arquivo de acesso aa porta
	HANDLE hPorta;
#endif
#ifdef SEM_ARDUINO
	bool controleId;
#endif

public:
	SerialComm(string porta);

	/**
	 * realiza a configuracao da porta
	 * COM/tty especificada, contem
	 * estrategias para cada sistema
	 * operacional.
	 *
	 * ocorrendo um erro, retorna valor
	 * correspondente a erro ou zero
	 * caso contrario.
	 */
	int iniciar();

	/**
	 * realiza a leitura de uma informacao
	 * a partir de uma porta serial
	 */
	string ler(long unsigned int bytesLer, long unsigned int &bytesLidos);

	/**
	 * realiza a escrita de um comando
	 * atraves de uma porta serial
	 */
	void escrever(int comando, long unsigned int &bytesEscritos);

	/**
	 * realiza a escrita de um comando parametrizado
	 * atraves de uma porta serial
	 */
	void escrever(int comando, int parametro, long unsigned int &bytesEscritos);
	void escrever(int comando, int parametro1, int parametro2, long unsigned int &bytesEscritos);

	/**
	 * finaliza utilizacao da porta serial
	 */
	void finalizar();
};

#endif
