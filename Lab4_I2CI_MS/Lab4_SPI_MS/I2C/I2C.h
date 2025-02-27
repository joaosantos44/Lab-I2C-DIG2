/*
 * I2C.h
 *
 * Created: 18/02/2025 09:38:46
 *  Author: joaos
 */ 


#ifndef I2C_H_
#define I2C_H_

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <stdint.h>

//Funcion inicializar I2C Maestro
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler);

//inicio de la comunicacion
void I2C_Master_Start(void);

//fin de la comunicacion
void I2C_Master_Stop(void);

//transmicion de datos del maestro sobre el esclavo
uint8_t I2C_Master_Write(uint8_t dato);

//recepcion de datos del maestro sobre el esclavo
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack);

//inicializar de Esclavo
void I2C_Slave_Init(uint8_t address);

#endif /* I2C_H_ */