#include <nds.h>
#include <stdio.h>
#include <nds/arm9/paddle.h>
#include <nf_lib.h>

volatile int paddleState = 0;

int main() {

	NF_Set2D(0, 0);

	NF_SetRootFolder("NITROFS");

	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);

	NF_LoadTiledBg("bg/bg", "bg", 256, 256);
	NF_CreateTiledBg(0, 3, "bg");



	while(1) {

		swiWaitForVBlank();

	}

	return 0;
}
