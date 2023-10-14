#pragma once
#include "Block.h"
#include "Grid.h"

using EBoardState = enum
{
    PLAYING,
    REMOVE_LINE,
    GAME_OVER,
    HIDE_GAME,
};

class Board
{
public:
    Board();
    void Render();
    void Update(float dt);
    void Destroy();
    void Start();
    bool IsPaused();
    bool CanDrawBackground();
    int GetNumberOfCompletedLines();
    int GetCurrentLevel();
    int GetScore();
    int GetBlockCount(int blockType);

private:
    void RenderGrid();
    void RenderNextBlock();
    void RenderPreview();
    void RenderGameOver();
    void SpawnNextBlock();
    float GetNormalFallingSpeed();
    float GetFastFallingSpeed();
    void ChooseNextBlock();
    void UpdateCurrentBlock(int value);
    void Fall();
    void UpdatePlaying(float dt);
    void Place();
    void ProcessInput(float dt);
    void ProcessRotation(float dt);
    void ProcessTranslation(float dt);
    void ProcessSoftDrop(float dt);
    void Rotate(int direction);
    void Translate(int direction);
    bool CheckGameOver();
    void CheckLines();
    void UpdateRemoveLines(float dt);
    void ScrollBoard(int row, const std::vector<int>& removedLines);
    void AddLines(int count);
    void UpdateGameOver(float dt);
    void AddScore(int numberOfLines);

private:
    Grid m_grid;
    size_t m_blockTexture;
    int m_level;
    int m_offsetX;
    int m_offsetY;
    int m_x;
    int m_y;
    int m_rotation;
    float m_fallingDelay;
    float m_fallingElapsed;
    int m_nextBlockID;
    Block m_currentBlock;
    Block m_nextBlock;
    bool m_pauseGame;
    EBoardState m_state;
    bool m_showNextBlock;
    float m_translateDelay;
    float m_translateElapsed;
    std::vector<int> m_lineToRemove;
    bool m_flashEffect;
    int m_totalLines;
    int m_lineForNextLevel;
    size_t m_pauseSfx;
    size_t m_moveSfx;
    size_t m_turnSfx;
    size_t m_dropSfx;
    size_t m_lineSfx;
    size_t m_tetrisSfx;
    size_t m_levelUpSfx;
    size_t m_gameOverSfx;
    size_t m_gameOverBar;
    float m_gameOverElapsed;
    int m_gameOverHeight;
    bool m_FastMusicSpeed;
    std::vector<int> m_blockCount;
    std::vector<Block> m_statisticPreview;
    int m_score;
};
