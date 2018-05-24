#ifdef _3DS
#include <string.h>
#include <stdio.h>

#include <3ds.h>

int main(int argc, char **argv) {

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	
	// We need these 2 buffers for APT_DoAppJump() later.
	u8 param[0x300];
	u8 hmac[0x20];

	// Vars
	static char gamePatchingState[255]  = "Game Patching is \x1B[31mDisabled\033[0m\n\n";

	// Main loop
	while (aptMainLoop()) {

		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();
		
		// CODE GOES HERE


		// Flush and swap framebuffers
		gfxFlushBuffers();
		//gfxSwapBuffers();
		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}
#elif WIIU
int (*OSDynLoad_Acquire)(const char* rpl, unsigned int* handle) = 0;
int (*OSDynLoad_FindExport)(unsigned int handle, int isdata, const char* symbol, void* address) = 0;

int (*OSFatal)(const char* msg) = 0;

int main(int argc, char **argv);

int _start(int argc, char **argv) {
    OSDynLoad_Acquire = *(void**) 0x00801500;
    OSDynLoad_FindExport = *(void**) 0x00801504;

    unsigned int coreinit_handle = 0;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSFatal", &OSFatal);

    return main(argc, argv);
}

int main(int argc, char **argv) {
    OSFatal("Hello, world!\n");
    return 0;
}
#else
#include <stdio.h>

int main(int argc, char **argv) {
    printf("Hello, world!\n");
    return 0;
}
#endif