//#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <naiveConsole.h>
#include <timeDriver.h>
#include <scheduler.h>


void int20(uint64_t rsp);
void int21(void);



void irqDispatcher(uint64_t irq, uint64_t rsp) {

	switch (irq) {
		case 0:
			int20(rsp);	//timer tick interruption
			break;
        case 1:
        	int21();	//keyboard interruption
            break;
	}
	return;
}

void int20(uint64_t rsp) {
	timeHandler();
	schedule(rsp);
}

void int21() {
	keyboardHandler();
}