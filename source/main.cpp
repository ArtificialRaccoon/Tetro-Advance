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
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2;
    REG_BG0CNT = BG_CBB(0) | BG_SBB(BACKGROUND_LAYER_ID) | BG_8BPP | BG_REG_32x32 | BG_PRIO(2);
    REG_BG1CNT = BG_CBB(2) | BG_SBB(TEXT_LAYER_ID) | BG_4BPP | BG_REG_32x32 | BG_PRIO(1);
    REG_BG2CNT = BG_CBB(0) | BG_SBB(ACTION_LAYER_ID) | BG_8BPP | BG_REG_32x32 | BG_PRIO(0); 
    
	GameProcessor objGame;
    objGame.InitializeGame();
	objGame.ChangeState(TitleState::Instance());
    while ( true )
    {        
        objGame.ProcessEvents();
		objGame.HandleEvents();
        VBlankIntrWait();
        objGame.Render();
    }
}