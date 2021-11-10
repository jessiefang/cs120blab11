/*	Author: Yunjie Fang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #2
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


enum States {Start, LCD};
unsigned char pos = 0x00;
unsigned char text[52]={' ',' ','C','S','1','2','0','B',' ','i','s',' ','L','e','g','e','n','d','.','.','.','w','a','i','t',' ','f','o','r',' ','i','t',' ','D','A','R','Y','!',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ', ' '};
unsigned char display[16];
unsigned char beg = 0;
unsigned char end = 16;
int Tick(int state){
	switch(state){
		case Start:
			state = LCD;
			break;
		case LCD:
			state = LCD;
			break;
		default:
			state = Start;
			break;
	}
	switch(state){
		case Start:
			break;
		case LCD:
			for (int i = 0; i < 16; i++){
				display[i] = text[(i + pos)%52];
			}
			LCD_DisplayString(1, display);
			pos = ((pos + 1)%52);
			break;
		default:
			break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    static task task1;
    task *tasks[] = { &task1 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = -1;
    /* Task 1 */
    task1.state = start;
    task1.period = 20;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Tick;

    TimerSet(20);
    TimerOn();
    LCD_init();
    LCD_DisplayString(1, "Hi");

    unsigned short i;
    /* Insert your solution below */
    while (1) {
	for (i = 0; i < numTasks; i++ ) {
		if (tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 1;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}
