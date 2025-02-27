/*
 * LCD8.c
 *
 * Created: 4/02/2025 18:18:14
 *  Author: joaos
 */ 

#include "LCD8.h"

//Funcion para inicializar LCD en modo de 4 Bits
void initLCD8bits(void){
	
	PORTC = 0;
	
	_delay_ms(20);
	LCD_CMD_8(0x30);
	_delay_ms(5);
	LCD_CMD_8(0x30);
	_delay_ms(5);
	LCD_CMD_8(0x30);
	_delay_ms(10);
	
	LCD_CMD_8(0x38); //Set de Funciones
	LCD_CMD_8(0x0C); //Display off
	LCD_CMD_8(0X01); //Display Clear
	LCD_CMD_8(0X06); //Entry mode
}

//Funcion para enviar un comando
void LCD_CMD_8(char a){
	PORTC &= ~(1 << PORTC0); //Apagar RS
	LCD_Port_8(a);
	
	PORTC |= (1 << PORTC1); //Activar Enable
	_delay_ms(4);
	
	PORTC &= ~(1 << PORTC1); // Apagar enable
}

//Funcion para colocar en el puerto un valor
void LCD_Port_8(char a){
	if (a & 1)
	PORTC |= (1 << PORTC2);
	else 
	PORTC &= ~(1 << PORTC2);
	
	if (a & 2)
	PORTC |= (1 << PORTC3);
	else 
	PORTC &= ~(1 << PORTC3);
	
	if (a & 4)
	PORTD |= (1 << PORTD6);
	else 
	PORTD &= ~(1 << PORTD6);
	
	if (a & 8)
	PORTD |= (1 << PORTD7);
	else 
	PORTD &= ~(1 << PORTD7);
	
	if (a & 16)
	PORTD |= (1 << PORTD2);
	else 
	PORTD &= ~(1 << PORTD2);
	
	if (a & 32)
	PORTD |= (1 << PORTD3);
	else 
	PORTD &= ~(1 << PORTD3);
	
	if (a & 64)
	PORTD |= (1 << PORTD4);
	else 
	PORTD &= ~(1 << PORTD4);
	
	if (a & 128)
	PORTD |= (1 << PORTD5);
	else 
	PORTD &= ~(1 << PORTD5);
}

//Funcion para enviar un caracter
void LCD_Write_Char_8(char a){
	
	//RS = 1 // =>  RS = 1 // Dato en el puerto lo va interpretar como caracter (dato)
	PORTC |= (1 << PORTC0); 
	LCD_Port_8(a);
	//EN = 1; // =>  EN = 1
	PORTC |= (1 << PORTC1);
	_delay_ms(4);
	//EN = 0; // =>  EN = 0
	PORTC &= ~(1 << PORTC1);
	
}

//Funcion para enviar una cadena
void LCD_Write_String_8(char *a){
	int i;
	for (i = 0; a[i] != '\0'; i++)
	LCD_Write_Char_8(a[i]);
}

//Establecer el cursor
void LCD_Set_Cursor_8(char c, char f){
	char temp;
	
	if (f == 1){
		temp = 0x80 + c - 1;
		LCD_CMD_8(temp);
		} else if (f == 2){
		temp = 0xC0 + c -1;
		LCD_CMD_8(temp);
	}
}