/*
 * gyroscope.c
 *
 *  Created on: 12 kwi 2023
 *      Author: chedoska
 */
#include "gyroscope.h"


// Inicjalizacja modułu żyroskopu do odczytu
/*
 * SPI_HandleTypeDef *m_hspi_handle port - układ SPI który będzie wykorzystywany do komunikacji.
 * L3GD20 *L3GD20_data - struktura przechowuąca odczytane wartości  z żyroskopu
 */
int L3GD20_init(SPI_HandleTypeDef *m_hspi_handle, L3GD20 *L3GD20_data){
	L3GD20_data->hspi_handle = m_hspi_handle;

	HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_SET);

	if(L3GD20_recive(L3GD20_WHO_AM_I, L3GD20_data) != 212)
	{
		// Jeśli identyfikator urządzenia nie odpowiada żyroskopowi
		return 0;
	}

	L3GD20_send(L3GD20_CTRL_REG1, 0x0F, L3GD20_data); // aktywuje odczyty z żyroskopu
	L3GD20_send(L3GD20_CTRL_REG4, 0x20, L3GD20_data); // Okreslenie skali 500 dps
	L3GD20_send(L3GD20_CTRL_REG2, 0x00, L3GD20_data); // ustawia filtr górnoprzepustowy (Normal mode)
	L3GD20_send(L3GD20_CTRL_REG5, 0x10, L3GD20_data); // włącza filtr górnoprzepustowy (enable)
	return 1;
}

//Wysyła bajt na wskazany adres wewnątrz ukłądu L3GD20
/*
 * L3GD20 *L3GD20_data - struktura przechowuąca odczytane wartości z żyroskopu
 * uint8_t address - adres rejestru
 * uint8_t data - dane do przesłania (1 bajt)
 */
void L3GD20_send(uint8_t address, uint8_t data, L3GD20 *L3GD20_data)
{
	uint8_t transmit_data[2];
	transmit_data[0] = address;
	transmit_data[1] = data;
	HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(L3GD20_data->hspi_handle, transmit_data, 2, L3GD20_SPI_WAIT_TIME);
	HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_SET);
}

//Pobiera dane ze wskazanego adresu i zwraca je.
/*
 * L3GD20 *L3GD20_data - struktura przechowuąca odczytane wartości z żyroskopu
 * uint8_t address - adres rejestru
 */
uint8_t L3GD20_recive(uint8_t addres, L3GD20 *L3GD20_data)
{
	uint8_t transmit_data[2];
	uint8_t recive_data[2];
	transmit_data[0] = addres | 0x80;
	transmit_data[1] = 0;
	recive_data[1] = 0;
	HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(L3GD20_data->hspi_handle, transmit_data, recive_data, 2, L3GD20_SPI_WAIT_TIME);
	HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_SET);
	return recive_data[1];
}

// Odczytuje z żyroskopu nieprzetworzone dane o prędkościaach obrotowych wokół osi XYZ
// i zapisuje je do struktury L3GD20 *L3GD20_data
void L3GD20_readRawData(L3GD20 *L3GD20_data)
{
	L3GD20_data->X_raw = L3GD20_recive(L3GD20_OUT_X_L, L3GD20_data);		//młodszy bajt
	L3GD20_data->X_raw |= L3GD20_recive(L3GD20_OUT_X_H, L3GD20_data) << 8;	//starszy bajt

	L3GD20_data->Y_raw = L3GD20_recive(L3GD20_OUT_Y_L, L3GD20_data);
	L3GD20_data->Y_raw |= L3GD20_recive(L3GD20_OUT_Y_H, L3GD20_data) << 8;

	L3GD20_data->Z_raw = L3GD20_recive(L3GD20_OUT_Z_L, L3GD20_data);
	L3GD20_data->Z_raw |= L3GD20_recive(L3GD20_OUT_Z_H, L3GD20_data) << 8;
}

// Konwertuje nieprzetworzone dane o prędkości obrotowej na stopnie na seukndę
void L3GD20_convertRawData(struct L3GD20 *L3GD20_data)
{
	L3GD20_data->X_val = (L3GD20_data->X_raw + L3GD20_X_OFFSET) * SENSITIVITY;
	L3GD20_data->Y_val = (L3GD20_data->Y_raw + L3GD20_Y_OFFSET) * SENSITIVITY;
	L3GD20_data->Z_val = (L3GD20_data->Z_raw + L3GD20_Z_OFFSET) * SENSITIVITY;
}












