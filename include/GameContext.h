#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>
#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "Tetromino.h"
#include "GameConstants.h"

class GameContext
{
    public:
        static GameContext* Instance()
        {
            static GameContext instance;
            return &instance;
        }

    public: //Functions
        bool ShouldDrop();
        bool CanSpawn();
        void SpawnTetromino();        
        void CheckForCompletedLines();
        void RemoveCompletedLine(int rowPosition);
        void Reset();
        void SaveTopScore();
        void LoadTopScore();

    public: //Setters
        void IncreaseCurrentLevel() { currentLevel++; SetCurrentLevelChanged(true); }
        void SetTopScore(int input) { topScore = input; SetTopScoreChanged(true); }                
        void IncreaseTetrominoTally(int type) { tetrominoTally[type]++; SetTetrominoTallyChanged(type, true); }
        void IncreaseCurrentLines(int input);
        void SetCurrentScore(int input);
        void SetCurrentLevelChanged(bool input) { levelChanged = input; }
        void SetCurrentScoreChanged(bool input) { scoreChanged = input; }
        void SetTopScoreChanged(bool input) { topScoreChanged = input; }
        void SetCurrentLinesChanged(bool input) { currentLinesChanged = input; }
        void SetNextPieceChanged(bool input) { nextPieceChanged = input; }
        void SetTetrominoTallyChanged(int type, bool input) { tetrominoTallyChanged[type] = input; }
        void SwapRandomizer() { useSevenBag = !useSevenBag; }

    public: //Getters
        int GetCurrentLevel() { return currentLevel; }
        int GetCurrentScore() { return currentScore; }
        int GetTopScore() { return topScore; }
        int GetCurrentLines() { return currentLines; }
        int GetTetrominoTally(int type) { return tetrominoTally[type]; }
        Tetromino& GetCurrentPiece() { return currentPiece; }
        Tetromino& GetNextPiece() { return nextPiece; }
        int** PlayGrid() { return playGrid; }
        bool** ChangedGrid() { return changedGrid; }
        bool CurrentLevelChanged() { return levelChanged; }
        bool CurrentScoreChanged() { return scoreChanged; }
        bool TopScoreChanged() { return topScoreChanged; }
        bool CurrentLinesChanged() { return currentLinesChanged; }
        bool NextPieceChanged() { return nextPieceChanged; }
        bool TetrominoTallyChanged(int type) { return tetrominoTallyChanged[type]; }
        bool UseSevenBag() { return useSevenBag; }

    public: //Audio
        mm_sound_effect GAMEOVER;
        mm_sound_effect POINTS;

    private:
        GameContext();
        ~GameContext() = default;
        GameContext(const GameContext&) = delete;
        GameContext& operator=(const GameContext&) = delete;

    private: //Functions
        int GetRandomTetromino(); 
        void EncodeDecodeScore(std::vector<char>& data, char key)
        {
            for(char& byte : data) { byte ^= key; }
        }

    private: //Context
        bool initialized = false;
        bool useSevenBag = true; //Later, add an option to switch randomization
        int topScore = 0;
        int currentScore = 0;        
        int currentLevel = 1;        
        int currentLines = 0;
        int historyIndex = 0;
        int dropCounter = 0;
        int tetrominoTally[7] = {0,0,0,0,0,0,0};
        int** playGrid = new int*[playGridHeight];
        bool** changedGrid = new bool*[playGridHeight];
        Tetromino nextPiece;
        Tetromino currentPiece;        
        std::vector<int> sevenBag = {0, 1, 2, 3, 4, 5, 6};
        int tgmHistory[4] = {-1, -1, -1, -1};

    private: //ChangedFlags
        bool levelChanged = true;
        bool scoreChanged = true;
        bool topScoreChanged = true;
        bool currentLinesChanged = true;
        bool nextPieceChanged = true;
        bool tetrominoTallyChanged[7] = {true,true,true,true,true,true,true};  
};

#endif // GAMECONTEXT_H