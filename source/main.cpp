#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <gba.h>
#include <maxmod.h>
#include <stdio.h>
#include <stdlib.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "GameProcessor.h"
#include "TitleState.h"

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------


	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	// since the default dispatcher handles the bios flags no vblank handler
	// is required
	irqInit();
	irqSet( IRQ_VBLANK, mmVBlank );
	irqEnable(IRQ_VBLANK);

	mmInitDefault( (mm_addr)soundbank_bin, 8 );

	consoleDemoInit();

	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH
	iprintf("\x1b[10;10HTetro-Advance\n");

	GameProcessor objGame;
    objGame.InitializeGame();
	objGame.ChangeState(TitleState::Instance());
    while ( true )
    {
        objGame.ProcessEvents();
		objGame.HandleEvents();
        objGame.Render();
    }
}