/*
 * gyroscope.h
 *
 *  Created on: 12 kwi 2023
 *      Author: chedoska
 */

#include "main.h"

#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#define L3GD20_CTRL_REG1 0x20		// Rejestr kontroli ukłądu 1
#define L3GD20_CTRL_REG2 0x21		// Rejestr kontroli ukłądu 2
#define L3GD20_CTRL_REG3 0x22		// Rejestr kontroli ukłądu 3
#define L3GD20_CTRL_REG4 0x23		// Rejestr kontroli ukłądu 4
#define L3GD20_CTRL_REG5 0x24		// Rejestr kontroli ukłądu 5

#define L3GD20_OUT_X_L 0x28			// Młodzy bajt wartości prędkości obrotowej wookół osi X
#define L3GD20_OUT_X_H 0x29			// Starszy bajt wartości prędkości obrotowej wookół osi X
#define L3GD20_OUT_Y_L 0x2A			// Młodzy bajt wartości prędkości obrotowej wookół osi Y
#define L3GD20_OUT_Y_H 0x2B			// Starszy bajt wartości prędkości obrotowej wookół osi Y
#define L3GD20_OUT_Z_L 0x2C			// Młodzy bajt wartości prędkości obrotowej wookół osi Z
#define L3GD20_OUT_Z_H 0x2D			// Starszy bajt wartości prędkości obrotowej wookół osi Z

#define L3GD20_WHO_AM_I 0x0F		// Rejestr określający identyfikator urządzenia

#define L3GD20_SPI_WAIT_TIME 500	// Czas oczekiwania przez SPI na wartości z żyroskopu

#define L3GD20_X_OFFSET -30			// Określone doświadczalnie przesunięcie dla odczytów prędkości obrotwej X
#define L3GD20_Y_OFFSET -5			// Określone doświadczalnie przesunięcie dla odczytów prędkości obrotwej Y
#define L3GD20_Z_OFFSET 13			// Określone doświadczalnie przesunięcie dla odczytów prędkości obrotwej Z
#define SENSITIVITY 0.09

struct L3GD20
{
	SPI_HandleTypeDef *hspi_handle;
	int16_t X_raw;
	int16_t Y_raw;
	int16_t Z_raw;
	float X_val;
	float Y_val;
	float Z_val;
} typedef L3GD20;

extern struct L3GD20 L3GD20_data;

int L3GD20_init(SPI_HandleTypeDef *hspi_handle, L3GD20 *L3GD20_data);
void L3GD20_send(uint8_t address, uint8_t data, L3GD20 *L3GD20_data);
uint8_t L3GD20_recive(uint8_t addres, L3GD20 *L3GD20_data);
void L3GD20_readRawData(L3GD20 *L3GD20_data);
void L3GD20_convertRawData(L3GD20 *L3GD20_data);

#endif /* GYROSCOPE_H_ */
