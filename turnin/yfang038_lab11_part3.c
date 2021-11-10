/*	Author: Yunjie Fang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: 
 */
#include <avr/io.h>
#include <io.h>
#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char keypad = 0x00;
unsigned char input = 0x00;

enum States {Start, Keypad};
int Tick(int state){
	input = GetKeypadKey();
	switch(state){
		case Start:
			state = Keypad;
			break;
		case Keypad:
			state = Keypad;
			break;
		default:
			state = Start;
			break;
	}
	switch(state){
		case Start:
			break;
		case Keypad:
			switch(input){
				case '\0':
					keypad = 0x1F;
					break;
				case '1':
					keypad = 0x01;
					break;
				case '2':
					keypad = 0x02;
					break;
				case '3':
					keypad = 0x03;
					break;
				case '4':
					keypad = 0x04;
					break;
				case '5':
					keypad = 0x05;
					break;
				case '6':
					keypad = 0x06;
					break;
				case '7':
					keypad = 0x07;
					break;
				case '8':
					keypad = 0x08;
					break;
				case '9':
					keypad = 0x09;
					break;
				case 'A':
					keypad = 0x0A;
					break;
				case 'B':
					keypad = 0x0B;
					break;
				case 'C':
					keypad = 0x0C;
					break;
				case 'D':
					keypad = 0x0D;
					break;
				case '*':
					keypad = 0x0E;
					break;
				case '0':
					keypad = 0x00;
					break;
				case '#':
					keypad = 0x0F;
					break;
				default:
					keypad = 0x1B;
					break;
			}
			PORTB = keypad;
			break;
		default:
			break;
	}
	return state;
}


enum LCDStates {Start, LCD, Display};
int LCDTick(int state){
	switch(state){
		case Start:
			state = LCD;
			break;
		case LCD:
			if(keypad == 0x1F){
				state = LCD;
			}else{
				state = Display;
			}
			break;
		default:
			state = Start;
			break;
	}
	switch(state){
		case Start:
			break;
		case LCD:
			break;
		case Display:
			LCD_Cursor(1);
			LCD_WriteData(input);
			break;
		default:
			break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xF0; PORTA = 0x0F;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;


    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = -1;
    /* Task 1 */
    task1.state = start;
    task1.period = 10;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Tick;

    task2.state = start;
    task2.period = 20;
    task2.elapsedTime = task2.period;
    task2.TickFct = &LCDTick;

    TimerSet(10);
    TimerOn();
    LCD_init();


    unsigned short i;
    /* Insert your solution below */
    while (1) {
	for (i = 0; i < numTasks; i++ ) {
		if (tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 50;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}