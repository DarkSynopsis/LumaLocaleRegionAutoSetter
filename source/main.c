#ifdef _3DS
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#include <3ds.h>

int main(int argc, char **argv) {

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	
	// Vars
	static char gamePatchingState[255]  = "Game Patching is \x1B[31mDisabled\033[0m\n\n";
	
	printf("Luma Locale Region Auto Setter\n\n");
	
	// Checking to see if code.ips exists, if not assume this is first run and create needed files.
	FILE *gamepatchEnabled = fopen("/luma/titles/0004000005445000/code.ips", "r");
	fclose(gamepatchEnabled);
	if (gamepatchEnabled == NULL)
	{
		printf("Creating Additional Files!\n");

		// Make sure directories exist.
		mkdir("/luma", 0777);
		mkdir("/luma/titles", 0777);
		mkdir("/luma/titles/0004000005445000", 0777);
		
		// Since code.ips doesn't exist create it, this is used to turn Game Patching is Disabled to Enabled via Game Patching.
		FILE *gamepatchCode = fopen("/luma/titles/0004000005445000/code.ips", "wb");
		char gpHex[23] = "\x50\x41\x54\x43\x48\x03\x10\x14\x00\x0A\x32\x6D\x45\x6E\x61\x62\x6C\x65\x64\x20\x45\x4F\x46";
		fwrite(gpHex, 1, sizeof(gpHex), gamepatchCode);
		fclose(gamepatchCode);
		
		printf("\x1B[31mPlease Relaunch!\033[0m\n");
	}
	else
	{
		printf(gamePatchingState);
		printf("Press 'A' to start creating locale files!\n");
	}

	// Main loop
	while (aptMainLoop()) {

		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();
		
		u32 kDown = hidKeysDown();
		
		// Start Button Returns to Home Menu.
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		
		// A Button will start the process of creating locale.txt files.
		if (kDown & KEY_A)
		{
			// Fire Emblem IF (JPN)
			// Check to see if locale already set.
			FILE *checkGame001 = fopen("/luma/titles/000400000012DE00/locale.txt", "r");
			fclose(checkGame001);
			
			// If no locale set create it.
			if (checkGame001 == NULL)
			{
				mkdir("/luma/titles/000400000012DE00", 0777);
				FILE *game001 = fopen("/luma/titles/000400000012DE00/locale.txt", "w");
				fprintf(game001, "JPN JP");
				fclose(game001);
				printf("Locale set for Fire Emblem IF (JPN)\n");				
			}
		}

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