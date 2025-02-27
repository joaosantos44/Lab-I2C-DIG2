/*
 * I2C.c
 *
 * Created: 18/02/2025 09:38:02
 *  Author: joaos
 */ 

#include "I2C.h"

//Funcion inicializar I2C Maestro
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler){
	DDRC &= ~((1 << DDC4) | (1 << DDC5));
	
	switch(Prescaler){
		case 1:
			TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
		break;
		case 4:
			TWSR &= ~(1 << TWPS1);
			TWSR |= (1 << TWPS0);
		break;
		case 16:
			TWSR &= ~(1 << TWPS0);
			TWSR |= (1 << TWPS1);
		break;
		case 64:
			TWSR |= (1 << TWPS1) | (1 << TWPS0);
		break;
		default:
			TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
			Prescaler = 1;
		break;
	}
	TWBR = ((F_CPU/SCL_Clock) - 16)/(2*Prescaler);
	TWCR = (1 << TWEN);
}

//inicio de la comunicacion
void I2C_Master_Start(void){
	uint8_t estado;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //iniciar la condicion de start
	while(!(TWCR & (1 << TWINT))); //esperandoa terminar la flag de TWINT
	
}

//fin de la comunicacion
void I2C_Master_Stop(void){
	
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);	
	
}

//transmicion de datos del maestro sobre el esclavo
uint8_t I2C_Master_Write(uint8_t dato){
	uint8_t estado;
	TWDR = dato;//cargar dato
	TWCR = (1 << TWEN) | (1 << TWINT);//Iniciar el envio
	
	while(!(TWCR & (1 << TWINT)));//espera flag TWINT
	estado = TWSR & 0xF8;//verificar el estado
	//comprovar si se envio: SLA + W, SLA + R o un dato con ACK
	if(estado == 0x18 || estado == 0x28 || estado == 0x40){
		return 1;
	}else{
		return estado;
	}
}

//recepcion de datos del maestro sobre el esclavo
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack){
	uint8_t estado;
	if(ack){
		TWCR |= (1 << TWEA);//lectura con ACK
	}else{
		TWCR &= ~(1 << TWEA);//Lectura sin ACK
	}
	
	TWCR |= (1 << TWINT);//Iniciar lectura
	while(!(TWCR & (1 << TWINT)));//espera flag TWINT
	estado = TWSR & 0xF8;//verificar el estado
	
	//verificar lectura con ACK o sin ACK
	if(estado == 0x58 || estado == 0x50){
		*buffer = TWDR;
		return 1;
	}else{
		return estado;
	}
}

//inicializar de Esclavo
void I2C_Slave_Init(uint8_t address){
	DDRC &= ~((1 << DDC4) | (1 << DDC5));
	
	TWAR = address << 1;//se asigna el valor que va a tener
	
	//se habilita la interfaz, ACK automatico, habilita la ISR
	TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
}