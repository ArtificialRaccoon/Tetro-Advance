#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

const int framesBeforeDrop[20] = { 48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 2, 1 };
const int blockSize = 8;
const int playGridX = 10;
const int playGridY = 0;
const int playGridWidth = 10;
const int playGridHeight = 20;
const int previewGridX = 20;
const int previewGridY = 14;
const int tilesetWidth = 16;
const int tilesetHeight = 8;
const int tileWidth = 8;
const int tileHeight = 8;
const int tileStartXpos = 72;
const int tileStartYpos = 56;
const int backgroundWidth = 40;
const int backgroundHeight = 25;
const int dialogWidth = 14;
const int dialogHeight = 4;
const int menuXStart = 120;
const int menuYStart = 78;
const int menuYRandom = 94;
const int TETROMINO_TILE_START = 120;
const int BACKGROUND_LAYER_ID = 31;
const int TEXT_LAYER_ID = 30;
const int ACTION_LAYER_ID = 29;
const int BLANK_TILE = 96;
const int CURSOR_TILE = 189;
const int CURSOR_TILE_POS_PLAY = 334;
const int CURSOR_TILE_POS_SWAP = 398;

const char ScoreKey = 0x45; //Nice.

#endif // GAMECONSTANTS_H