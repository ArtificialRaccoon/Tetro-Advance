#include "GameContext.h"

GameContext::GameContext()
{
    GAMEOVER = {
		{ SFX_GAMEOVER },
		(int)(1.0f * (1<<10)),	// rate
		0,                      // handle
		255,                    // volume
		128,                    // panning
	};

    POINTS = {
		{ SFX_CLEAR },
		(int)(1.0f * (1<<10)),	// rate
		0,                      // handle
		255,                    // volume
		128,                    // panning
	};

    srand(static_cast<unsigned int>(time(0)));
    Reset();
}

void GameContext::Reset()
{
    topScore = 0;
    currentScore = 0;        
    currentLevel = 1;        
    currentLines = 0;
    historyIndex = 0;
    dropCounter = 0;

    levelChanged = true;
    scoreChanged = true;
    topScoreChanged = true;
    currentLinesChanged = true;
    nextPieceChanged = true;

    for(int i = 0; i < 7; i++)
    {
        tetrominoTally[i] = 0;
        tetrominoTallyChanged[i] = true;
    }

    if(!initialized)
    {
        for (int i = 0; i < playGridHeight; i++) 
        {
            playGrid[i] = new int[playGridWidth];
            changedGrid[i] = new bool[playGridWidth];
        }
        initialized = true;
    }

    for (int i = 0; i < playGridHeight; i++) 
    {
        for (int j = 0; j < playGridWidth; j++) 
        {
            playGrid[i][j] = 0;
            changedGrid[i][j] = true;
        }
    }

    this->currentPiece = Tetromino(GetRandomTetromino());
    this->nextPiece = Tetromino(GetRandomTetromino());

    this->LoadTopScore();
}

bool GameContext::CanSpawn()
{
    return nextPiece.CanSpawn(playGrid);
}

void GameContext::SpawnTetromino()
{
    this->dropCounter = 0;
    currentPiece = Tetromino(nextPiece.GetType());
    nextPiece = Tetromino(GetRandomTetromino());
    SetNextPieceChanged(true);
}

int GameContext::GetRandomTetromino()
{
    //Standard 7-Bag Randomization
    if(useSevenBag)
    {
        if(sevenBag.size() == 0)
            sevenBag.assign({0, 1, 2, 3, 4, 5, 6});
        int randomIndex = rand() % sevenBag.size();
        int value = sevenBag[randomIndex];
        sevenBag.erase(sevenBag.begin() + randomIndex);
        return value;
    }

    //TGM Randomization     
    int randomType = rand() % 7; 
    int matchCount = 0;

    while (matchCount < 4) 
    {
        bool found = false;
        for (int i = 0; i < 4; i++) 
        {
            if (tgmHistory[i] == randomType) 
            {
                found = true;
                break;
            }
        }

        if (found) 
        {
            randomType = rand() % 7;             
            matchCount++;
        } 
        else 
            break;
    }

    tgmHistory[historyIndex] = randomType;
    historyIndex = (historyIndex + 1) % 4;

    return randomType;
}

void GameContext::IncreaseCurrentLines(int input) 
{ 
    currentLines += input; 
    SetCurrentLinesChanged(true); 

    if(currentLines > 0 && (currentLines % 10) == 0)
        IncreaseCurrentLevel();
}

void GameContext::SetCurrentScore(int input) 
{ 
    currentScore += input; 
    SetCurrentScoreChanged(true); 

    if(currentScore > topScore)
        SetTopScore(currentScore);
}

void GameContext::CheckForCompletedLines()
{
    int completedLineCount = 0;

    for(int y = 0; y < playGridHeight; y++)
    {
        bool lineCompleted = true;
        for(int x = 0; x < playGridWidth; x++)
        {
            if(playGrid[y][x] == 0)
            {
                lineCompleted = false;
                break;
            }
        }

        if(lineCompleted)
        {
            IncreaseCurrentLines(1);
            RemoveCompletedLine(y);
            completedLineCount++;
        }
    }

    switch(completedLineCount)
    {
        case 1:
            SetCurrentScore(100);
            mmEffectEx(&POINTS);
            break;
        case 2:
            SetCurrentScore(300);
            mmEffectEx(&POINTS);
            break;
        case 3:
            SetCurrentScore(500);
            mmEffectEx(&POINTS);
            break;
        case 4:
            SetCurrentScore(1000);
            mmEffectEx(&POINTS);
            break;
        default:
            break;
    }
}

void GameContext::RemoveCompletedLine(int rowPosition)
{
    for (int y = rowPosition; y > 0; y--) 
    {
        for (int x = 0; x < playGridWidth; x++) 
        {
            if(playGrid[y][x] != playGrid[y - 1][x])
            {
                changedGrid[y][x] = true;
                changedGrid[y - 1][x] = true;
            }

            playGrid[y][x] = playGrid[y - 1][x];
        }
    }

    for (int x = 0; x < playGridWidth; x++) 
    {
        playGrid[0][x] = 0;
        changedGrid[0][x] = true;
    }
}

bool GameContext::ShouldDrop()
{
    this->dropCounter++;    
    int frameCountBeforeDrop = framesBeforeDrop[this->currentLevel > 19 ? 19 : this->currentLevel];
    if(this->dropCounter >= frameCountBeforeDrop)
    {
        this->dropCounter = 0;
        return true;
    }
    return false;
}

void GameContext::SaveTopScore()
{

}

void GameContext::LoadTopScore()
{

}