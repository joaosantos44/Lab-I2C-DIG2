/*
 * Lab4_SPI_MS.c
 *
 * Created: 18/02/2025 09:32:35
 * Author : joaos
 */ 

#define F_CPU 16000000

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "I2C/I2C.h"
#include "LCD8/LCD8.h"

#define slave1 0x30
#define slave2 0x40

uint8_t direccion;
uint8_t temp;
uint8_t temp2;
uint8_t bufferI2C;
uint8_t bufferI2C2;
uint8_t valorI2C = 0;
uint8_t valorI2C2 = 0;


uint8_t contador = 0;
volatile char indic;
float volt1 = 0;
uint8_t volt2 = 0;
char buf[20];

void Setup(void);
void initUART9600(void);
void refreshPORT(uint8_t valor);
void floatToString(float num, char *str, int precision);



int main(void)
{
	I2C_Master_Init(100000, 1);//inicializar como master FsCL 100KHz prescaler 1
	Setup(); 
	
	LCD_Set_Cursor_8(1,1);
	LCD_Write_String_8("ADC:");
	
	LCD_Set_Cursor_8(7,1);
	LCD_Write_String_8("CONT:");
	//PORTB |= (1 << PORTB5);

    while (1) 
    {
		PORTB |= (1 << PORTB5);
		
		I2C_Master_Start();
		//Escribir
		bufferI2C = slave1 << 1 & 0b11111110;
		
		temp = I2C_Master_Write(bufferI2C);
		
		if(temp != 1){
			I2C_Master_Stop();
			CadenChar("if ");
		}else{
			I2C_Master_Write('R');
			I2C_Master_Stop();
			CadenChar("else ");
		}
		_delay_ms(500);
		
		PORTB &=  ~(1 << PORTB5);
		I2C_Master_Start();
		// Lectura Slave 1
		bufferI2C = slave1 << 1 | 0b00000001;
		
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
		}
		
		TWCR = (1<<TWINT)|(1<<TWEN);
		while (!(TWCR & (1<<TWINT)));
		
		valorI2C= TWDR;
		
		I2C_Master_Stop();
		
		refreshPORT(valorI2C);
		
		

		//refreshPORT(valorI2C);
		_delay_ms(500);
		
/**********************************************************************
Esclavo 2
***********************************************************************/
		
		I2C_Master_Start();
		//Escribir
		bufferI2C = slave2 << 1 & 0b11111110;
		
		temp2 = I2C_Master_Write(bufferI2C2);
		
		if(temp2 != 1){
			I2C_Master_Stop();
			CadenChar("if ");
			}else{
			I2C_Master_Write('R');
			I2C_Master_Stop();
			CadenChar("else ");
		}
		_delay_ms(500);
		
		PORTB &=  ~(1 << PORTB5);
		I2C_Master_Start();
		// Lectura Slave 1
		bufferI2C2 = slave2 << 1 | 0b00000001;
		
		temp2 = I2C_Master_Write(bufferI2C2);
		if(temp2 != 1){
			I2C_Master_Stop();
		}
		
		TWCR = (1<<TWINT)|(1<<TWEN);
		while (!(TWCR & (1<<TWINT)));
		
		valorI2C2= TWDR;
		
		I2C_Master_Stop();
		
		refreshPORT(valorI2C2);
		
		

		//refreshPORT(valorI2C);
		_delay_ms(500);
		
		
		volt1 = valorI2C * 5.0 / 255;
		floatToString(volt1, buf, 2);
		LCD_Set_Cursor_8(1,2);
		LCD_Write_String_8(buf);
		if (valorI2C != 0){
			CadenChar("hay ");
			}else if(valorI2C == 0){
			CadenChar("nada ");
		}
		LCD_Write_Char_8('V');
		
		volt2 = valorI2C2;
		floatToString(volt2, buf, 1);
		LCD_Set_Cursor_8(7,2);
		LCD_Write_String_8(buf);
		if (valorI2C2 != 0){
			CadenChar("hay2 ");
			}else if(valorI2C2 == 0){
			CadenChar("nada ");
		}
		LCD_Write_Char_8('N');
		
		
    }
}


void Setup(void){
	DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5);
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	PORTC = 0;
	
	initLCD8bits();
	initUART9600();
	refreshPORT(0);
	
	DDRB |= (1 << DDB5);
	sei();
}

void initUART9600(void){
	uint16_t ubrr = F_CPU / 16 / 9600 - 1;
	UBRR0H = (ubrr >> 8);   // Parte alta del baud rate
	UBRR0L = ubrr;          // Parte baja del baud rate
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Habilita TX y RX
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits de datos
}



void floatToString(float num, char *str, int preci) {
	
	// Parte entera
	int ENPart = (int)num;
	// Parte decimal
	float deciPart = num - ENPart;

	// Convertir parte entera con itoa()
	itoa(ENPart, str, 10);

	// Agregar punto decimal
	int i = 0;
	while (str[i] != '\0') i++;  // Buscar Nulo
	str[i++] = '.';

	// Convertir parte decimal manualmente
	for (int j = 0; j < preci; j++) {
		deciPart *= 10;
		int decDigit = (int)deciPart;
		str[i++] = decDigit + '0';
		deciPart -= decDigit;
	}
	
	
	// Terminar la cadena
	str[i] = '\0';
}


void refreshPORT(uint8_t valor){
	if(valor & 0b10000000){
		PORTB |= (1 << PORTB0);
		}else{
		PORTB &= ~(1 << PORTB0);
	}
	if(valor & 0b01000000){
		PORTB |= (1 << PORTB1);
		}else{
		PORTB &= ~(1 << PORTB1);
	}
	if(valor & 0b00100000){
		PORTD |= (1 << PORTD2);
		}else{
		PORTD &= ~(1 << PORTD2);
	}
	if(valor & 0b00010000){
		PORTD |= (1 << PORTD3);
		}else{
		PORTD &= ~(1 << PORTD3);
	}
	if(valor & 0b00001000){
		PORTD |= (1 << PORTD4);
		}else{
		PORTD &= ~(1 << PORTD4);
	}
	if(valor & 0b00000100){
		PORTD |= (1 << PORTD5);
		}else{
		PORTD &= ~(1 << PORTD5);
	}
	if(valor & 0b00000010){
		PORTD |= (1 << PORTD6);
		}else{
		PORTD &= ~(1 << PORTD6);
	}
	if(valor & 0b00000001){
		PORTD |= (1 << PORTD7);
		}else{
		PORTD &= ~(1 << PORTD7);
	}
}


// Envía un caracter
void MandarP(char letra){
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = letra;
}

//recive un caracter
char ReceivP(void) {
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

// Envía una cadena de texto
void CadenChar(const char *str) {
	while (*str) {
		MandarP(*str++);
	}
}

// Convierte un número a binario
void Binario(uint8_t valor) {
	for (int i = 7; i >= 0; i--) {
		MandarP((valor & (1 << i)) ? '1' : '0');
	}
	CadenChar("\r\n");
}


int InterDeTeclas() {
	char buffer[4] = {0};   // Almacena hasta 3 dígitos
	uint8_t index = 0;
	char received;

	while (1) {
		while (!(UCSR0A & (1 << RXC0)));  // Espera un dato (¡Aquí puede trabarse!)
		
		received = UDR0;  // Lee el dato recibido
		MandarP(received); // Imprime lo que recibe (eco)

		// Si encuentra '+', termina
		if (received == '+') {
			break;
		}

		// Acepta solo dígitos (0-9)
		if (received >= '0' && received <= '9' && index < 3) {
			buffer[index++] = received;  // Guarda el número
		}
	}

	return (uint8_t)atoi(buffer);  // Convierte a entero
}
