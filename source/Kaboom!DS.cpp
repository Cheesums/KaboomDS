#include <nds.h>
#include <stdio.h>
#include <nds/arm9/paddle.h>
#include <nf_lib.h>

volatile int paddleState = 0;

int main() {

	consoleDemoInit();
	consoleClear();	
	setBrightness(3, 0);

	iprintf("\n Hello World!");

	while(1) {

		swiWaitForVBlank();

	}

	return 0;
}
