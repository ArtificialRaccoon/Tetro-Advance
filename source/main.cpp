#include <tonc.h>
#include <maxmod.h>
#include <stdio.h>
#include <stdlib.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "GameProcessor.h"
#include "TitleState.h"

int main(void) {	

    irq_init(NULL);
    irq_add(II_VBLANK, mmVBlank);    
	mmInitDefault( (mm_addr)soundbank_bin, 8 );
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
    REG_BG0CNT = BG_CBB(0) | BG_SBB(31) | BG_8BPP | BG_REG_32x32;

	GameProcessor objGame;
    objGame.InitializeGame();
	objGame.ChangeState(TitleState::Instance());
    while ( true )
    {
        VBlankIntrWait();
        objGame.ProcessEvents();
		objGame.HandleEvents();
        objGame.Render();
    }
}