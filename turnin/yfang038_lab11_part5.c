/*	Author: Yunjie Fang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #5
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

unsigned char input = ~PINB & 0x07;
unsigned char pos = 0x01;
enum ButtonStates {Start, Wait, Release, Up, Down};
int ButtonTick(int state){
	switch(state){
		case Start:
			state = Wait;
			break;
		case Wait:
			if(input == 0x01){
				state = Down;
			}else if(input == 0x02){
				state = Up;
			}else{
				state = Init;
			}
			break;
		case Down:
			state = Release;
			break;
		case Up:
			state = Release;
			break;
		case Release:
			if(input == 0x00){
				state = Wait;
			}else{
				state = Release;
			}
			break;
		default:
			state = Start;
			break;
	}
	switch(state){
		case Start:
			break;
		case Wait:
			break;
		case Down:
			pos = 17;
			break;
		case Up:
			pos = 1;
			break;
		case Release:
			break;
		default:
			break;
	}
	return state;
}


enum LCDStates {Start, LCD, Display};
unsigned char text[] = "           #             #                ";
unsigned char lose[] = "game over";
unsigned char j = 0x00;
int LCDTick(int state){
	switch(state){
		case Start:
			state = LCD;
			break;
		case LCD:
			if(text[pos] == '#'){
				state = Display;
			}else{
				state = LCD;
			}
			break;
		case Display:
			if(input == 0x04){
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
			for(int i = 1; i <= 32; i++){
				LCD_Cursor(i);
				LCD_WriteData(text[(i + j)%32]);
			}
			LCD_Cursor(pos);
			if( j >=32){
				j = 0;
			}else{
				j++;
			}
			break;
		case Display:
			LCD_DisplayString(3,lose);
			break;
		default:
			break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0x00; PORTB = 0xFF;
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
    task1.TickFct = &ButtonTick;

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
		tasks[i]->elapsedTime += 1;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}
