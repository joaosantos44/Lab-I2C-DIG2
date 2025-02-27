/*
 * LCD8.h
 *
 * Created: 4/02/2025 18:18:31
 *  Author: joaos
 */ 


#ifndef LCD8_H_
#define LCD8_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/delay.h>

//Funcion para inicializar LCD en modo de 4 Bits
void initLCD8bits(void);

//Funcion para colocar en el puerto un valor
void LCD_Port_8(char a);

//Funcion para enviar un comando
void LCD_CMD_8(char a);

//Funcion para enviar un caracter
void LCD_Write_Char_8(char c);

//Funcion para enviar una cadena
void LCD_Write_String_8(char *a);

//Establecer el cursor
void LCD_Set_Cursor_8(char c, char f);

#endif /* LCD8_H_ */