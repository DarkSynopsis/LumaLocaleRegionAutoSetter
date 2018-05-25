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
			char pathDirectory[35];
			char pathLocale[45];
			const char *arrayTitleIDJPN[] = {"000400000012DE00", "0004000000102F00", "0004000000190E00", "0004000000115400", "000400000014E000", "0004000000169D00", "0004000000197200", "000400000014AD00", "00040000000B8B00", "00040000000C3A00", "0004000000163C00", "0004000000065C00", "0004000000196D00", "00040000000CF600", "0004000000120500", "00040000000A5300", "00040000000F5800", "00040000000C3600", "00040000000BAA00", "0004000000095000", "00040000000C3900", "00040000000FB100", "0004000000166B00", "0004000000078200", "000400000010E200", "0004000000112800", "00040000000C0100", "0004000000169F00", "000400000016A000", "0004000000134500", "00040000000FA600", "0004000000078A00", "0004000000127900", "0004000000134300", "0004000000190A00", "000400000018EA00", "0004000000182F00", "0004000000182E00", "0004000000197500", "000400000018D000", "000400000012F500", "0004000000140000", "0004000000101200", "0004000000102D00", "000400000016AD00", "00040000001ACB00", "000400000018E900", "000400000018CA00", "0004000000117200", "00040000000A4D00", "0004000000118300", "0004000000073900", "0004008C0015D500", "00040000001A2B00", "000400000012EF00", "000400000018B300", "00040000001B5700", "0004000000199200", "000400000F70C100", "00040000001B2400", "00040000001A0C00", "000400000016C700", "000400000016C600", "000400000018B000", "00040000001AF400", "0004000000191000", "0004000000191100", "00040000001C9C00", "00040000001C9400"};

			size_t iJPN = 0;
			for(iJPN = 0; iJPN < sizeof(arrayTitleIDJPN) / sizeof(arrayTitleIDJPN[0]); iJPN++)
			{
				snprintf(pathLocale, 45, "/luma/titles/%s/locale.txt", arrayTitleIDJPN[iJPN]);
				
				// Check to see if locale already set.
				FILE *checkGame = fopen(pathLocale, "r");
				fclose(checkGame);
				
				// If no locale set create file.
				if (checkGame == NULL)
				{
					snprintf(pathDirectory, 35, "/luma/titles/%s", arrayTitleIDJPN[iJPN]);
					
					mkdir(pathDirectory, 0777);
					FILE *game = fopen(pathLocale, "w");
					fprintf(game, "JPN JP");
					fclose(game);		
					printf ("Locale set for %s\n", arrayTitleIDJPN[iJPN]);
				}
			}
			
			// Text to Print when all files created.
			printf("\n\x1B[32mFinished!\033[0m\n");
			printf("Press 'START' to exit and enjoy!\n");
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