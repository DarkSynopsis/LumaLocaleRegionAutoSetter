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
	u8 consoleRegion = 0;
	
	printf("Luma Locale Region Auto Setter (226 Titles)\n\n");
	
	// Get Console Region
    cfguInit();
    CFGU_SecureInfoGetRegion(&consoleRegion);
	cfguExit();
	
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
		char gpHex[23] = "\x50\x41\x54\x43\x48\x03\x30\x14\x00\x0A\x32\x6D\x45\x6E\x61\x62\x6C\x65\x64\x20\x45\x4F\x46";
		fwrite(gpHex, 1, sizeof(gpHex), gamepatchCode);
		fclose(gamepatchCode);
		
		printf("\n\x1B[31mPlease Relaunch!\033[0m\n");
		printf("Press 'START' to exit!\n");
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
			const char *arrayTitleIDJPN[] = {"000400000012DE00", "0004000000102F00", "0004000000190E00", "0004000000115400", "000400000014E000", "0004000000169D00", "0004000000197200", "000400000014AD00", "00040000000B8B00", "00040000000C3A00", "0004000000163C00", "0004000000065C00", "0004000000196D00", "00040000000CF600", "0004000000120500", "00040000000A5300", "00040000000F5800", "00040000000C3600", "00040000000BAA00", "0004000000095000", "00040000000C3900", "00040000000FB100", "0004000000166B00", "0004000000078200", "000400000010E200", "0004000000112800", "00040000000C0100", "0004000000169F00", "000400000016A000", "0004000000134500", "00040000000FA600", "0004000000078A00", "0004000000127900", "0004000000134300", "0004000000190A00", "000400000018EA00", "0004000000182F00", "0004000000182E00", "0004000000197500", "000400000018D000", "000400000012F500", "0004000000140000", "0004000000101200", "0004000000102D00", "000400000016AD00", "00040000001ACB00", "000400000018E900", "000400000018CA00", "0004000000117200", "00040000000A4D00", "0004000000118300", "0004000000073900", "0004008C0015D500", "00040000001A2B00", "000400000012EF00", "000400000018B300", "00040000001B5700", "0004000000199200", "000400000F70C100", "00040000001B2400", "00040000001A0C00", "000400000016C700", "000400000016C600", "000400000018B000", "00040000001AF400", "0004000000191000", "0004000000191100", "00040000001C9C00", "00040000001C9400", "0004000000031500", "0004000000031000", "0004000000030B00", "0004000000154000", "0004000000180E00", "00040000001AA300", "000400000017FD00", "0004000000196700"};
			const char *arrayTitleIDUSA[] = {"000400000018A400", "000400000018F100", "000400000018F800", "0004000000159500", "000400000016CD00", "00040000001AA900", "00040000000CD300", "0004000000095800", "0004000000128600", "000400000017BA00", "0004000000112C00", "0004000000034F00", "00040000000EA900", "000400000015F200", "000400000015B200", "0004000000113200", "00040000000A0500", "0004000000179800", "0004000000179400", "0004000000179600", "000400000010F600", "000400000017EA00", "000400000015D800", "00040000000DCD00", "000400000014CD00", "000400000012E700", "000400000007AE00", "0004000000123D00", "0004000000123400", "00040000000F1400", "000400000018F400", "00040000000BA800", "0004000000160C00", "0004000000137700", "0004000000106400", "000400000016F200", "0004000000165000", "00040000000E5C00", "000400000019A200", "00040000000B3500", "000400000008CD00", "000400000012D900", "00040000000D7D00", "0004000000163100", "00040000000EDF00", "0004000000095100", "00040000000FD500", "0004000000196E00", "000400000019F500", "000400000016DE00", "00040000000F1700", "00040000001A3500", "00040000001AB800", "0004000000125500", "00040000001B4000", "00040000001A4800", "00040000001BD500", "00040000001BD000", "00040000001C7700", "000400000F70CC00", "00040000001C5100", "00040000001BEC00", "000400000019AA00", "00040000001C8F00", "00040000001CD200", "0004000000031700", "0004000000030D00", "0004000000031200", "0004000000185C00", "00040000001B8700", "00040000001C1C00", "00040000001CEB00", "00040000001CEF00", "00040000001D1900"};
			const char *arrayTitleIDEUR[] = {"000400000018F200", "00040000001A6C00", "000400000016E600", "0004000000169600", "00040000001AAA00", "0004000000153B00", "0004000000084D00", "000400000017BB00", "0004000000123C00", "0004000000038A00", "00040000000D4B00", "000400000016E900", "000400000016EA00", "000400000017CD00", "0004000000113100", "000400000009F100", "000400000017A800", "0004000000179500", "0004000000179700", "0004000000163200", "000400000012BE00", "000400000017EB00", "000400000004DC00", "000400000015B100", "00040000000DCE00", "0004000000165700", "0004000000084F00", "000400000016A100", "000400000007AF00", "0004000000106200", "0004000000117E00", "0004000000149F00", "00040000000F1E00", "000400000018FA00", "00040000000BA900", "0004000000162000", "0004000000137800", "0004000000173B00", "0004000000165800", "0004000000141C00", "00040000001A6B00", "000400000008FC00", "000400000008CC00", "0004000000157C00", "00040000000D7E00", "00040000000EE000", "0004000000091600", "00040000000FCA00", "00040000000EDA00", "000400000009AD00", "00040000001AB900", "0004000000125600", "00040000001B4100", "00040000001A4900", "00040000001B9D00", "00040000001BC300", "00040000001C7600", "000400000F70CD00", "00040000001C5300", "00040000001BCA00", "000400000019AB00", "000400000019B000", "000400000019AC00", "000400000019B100", "00040000001B2900", "00040000001CD900", "00040000001CE100", "0004000000030C00", "0004000000031600", "0004000000031100", "000400000019A700", "00040000001C1D00", "00040000001CF000", "00040000001CEC00", "00040000001D6800", "000400000018A500"};
			size_t iJPN = 0;
			size_t iUSA = 0;
			size_t iEUR = 0;
			
			// Handle JPN TitleIDs
			if (consoleRegion != 0)
			{
				for(iJPN = 0; iJPN < sizeof(arrayTitleIDJPN) / sizeof(arrayTitleIDJPN[0]); iJPN++)
				{
					snprintf(pathLocale, 45, "/luma/titles/%s/locale.txt", arrayTitleIDJPN[iJPN]);
					
					// Check to see if locale already set.
					FILE *checkGameJPN = fopen(pathLocale, "r");
					fclose(checkGameJPN);
					
					// If no locale set create file.
					if (checkGameJPN == NULL)
					{
						snprintf(pathDirectory, 35, "/luma/titles/%s", arrayTitleIDJPN[iJPN]);
						
						mkdir(pathDirectory, 0777);
						FILE *gameJPN = fopen(pathLocale, "w");
						fprintf(gameJPN, "JPN JP");
						fclose(gameJPN);		
						printf ("JPN Locale set for %s\n", arrayTitleIDJPN[iJPN]);
					}
					else
					{
						printf ("JPN Locale already set for %s\n", arrayTitleIDJPN[iJPN]);
					}
				}
			}
			
			// Handle USA TitleIDs
			if (consoleRegion != 1)
			{
				for(iUSA = 0; iUSA < sizeof(arrayTitleIDUSA) / sizeof(arrayTitleIDUSA[0]); iUSA++)
				{
					snprintf(pathLocale, 45, "/luma/titles/%s/locale.txt", arrayTitleIDUSA[iUSA]);
					
					// Check to see if locale already set.
					FILE *checkGameUSA = fopen(pathLocale, "r");
					fclose(checkGameUSA);
					
					// If no locale set create file.
					if (checkGameUSA == NULL)
					{
						snprintf(pathDirectory, 35, "/luma/titles/%s", arrayTitleIDUSA[iUSA]);
						
						mkdir(pathDirectory, 0777);
						FILE *gameUSA = fopen(pathLocale, "w");
						fprintf(gameUSA, "USA EN");
						fclose(gameUSA);		
						printf ("USA Locale set for %s\n", arrayTitleIDUSA[iUSA]);
					}
					else
					{
						printf ("USA Locale already set for %s\n", arrayTitleIDUSA[iUSA]);
					}
				}
			}
			
			// Handle EUR TitleIDs
			if	(consoleRegion != 2)
			{
				for(iEUR = 0; iEUR < sizeof(arrayTitleIDEUR) / sizeof(arrayTitleIDEUR[0]); iEUR++)
				{
					snprintf(pathLocale, 45, "/luma/titles/%s/locale.txt", arrayTitleIDEUR[iEUR]);
					
					// Check to see if locale already set.
					FILE *checkGameEUR = fopen(pathLocale, "r");
					fclose(checkGameEUR);
					
					// If no locale set create file.
					if (checkGameEUR == NULL)
					{
						snprintf(pathDirectory, 35, "/luma/titles/%s", arrayTitleIDEUR[iEUR]);
						
						mkdir(pathDirectory, 0777);
						FILE *gameEUR = fopen(pathLocale, "w");
						fprintf(gameEUR, "EUR EN");
						fclose(gameEUR);		
						printf ("EUR Locale set for %s\n", arrayTitleIDEUR[iEUR]);
					}
					else
					{
						printf ("EUR Locale already set for %s\n", arrayTitleIDEUR[iEUR]);
					}
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
