#include "Board.h"
#include "Engine.h"
#include "Log.h"
#include "SaveGame.h"
#include <vector>

#define _FULLDEBUG 0

const int BLOCK_NONE = 0;
const int BLOCK_TILE = 1;
const int PLACED_TILE = 2;
const int GAME_OVER_TILE = 3;
const int BLOCK_COLOR_COUNT = 9;
const int MUSIC_CHANGE_LEVEL = 7;

std::vector<float> secondsToBottomAtLevel =
{
    15.974f,    // 0
    14.310f,    // 1
    12.646f,    // 2
    10.982f,    // 3
    9.318f,     // 4
    7.654f,     // 5
    5.990f,     // 6
    4.326f,     // 7
    2.662f,     // 8
    1.997f,     // 9
    1.664f,     // 10
    1.664f,     // 11
    1.664f,     // 12
    1.331f,     // 13
    1.331f,     // 14
    1.331f,     // 15
    0.998f,     // 16
    0.998f,     // 17
    0.998f,     // 18
    0.666f,     // 19
    0.666f,     // 20
    0.666f,     // 21
    0.666f,     // 22
    0.666f,     // 23
    0.666f,     // 24
    0.666f,     // 25
    0.666f,     // 26
    0.666f,     // 27
    0.666f,     // 28
    0.333f      // 29
};

Board::Board()
{
    m_x = 0;
    m_y = 0;
    m_rotation = 0;
    m_fallingDelay = 0.0f;
    m_fallingElapsed = 0.0f;
    m_nextBlockID = 0;
    m_blockTexture = 0;
    m_offsetX = 382;
    m_offsetY = 156;
    m_blockTexture = 0;
    m_pauseSfx = 0;
    m_moveSfx = 0;
    m_turnSfx = 0;
    m_dropSfx = 0;
    m_lineSfx = 0;
    m_tetrisSfx = 0;
    m_levelUpSfx = 0;
    m_gameOverSfx = 0;
    m_gameOverBar = 0;
    m_FastMusicSpeed = false;
    m_level = 0;
    m_totalLines = 0;
    m_lineForNextLevel = m_level * 10 + 10;
    m_pauseGame = false;
    m_state = PLAYING;
    m_showNextBlock = true;
    m_translateDelay = 0.15f;
    m_translateElapsed = 0.0f;
    m_gameOverElapsed = 0.0f;
    m_gameOverHeight = 0;
    m_flashEffect = false;
    m_score = 0;
    m_grid = Grid();
}

void Board::Render()
{
    RenderGrid();
    RenderNextBlock();
    RenderPreview();
    RenderGameOver();
}

void Board::Update(float dt)
{
    ProcessInput(dt);

    if (!m_pauseGame)
    {
        switch (m_state)
        {
        case PLAYING:
            UpdatePlaying(dt);
            break;

        case REMOVE_LINE:
            UpdateRemoveLines(dt);
            break;

        case HIDE_GAME:
            UpdateGameOver(dt);
            break;
        }
    }
}

void Board::Destroy()
{
    m_grid.Destroy();
}

void Board::Start()
{
    m_x = 0;
    m_y = 0;
    m_rotation = 0;
    m_fallingDelay = 0.0f;
    m_fallingElapsed = 0.0f;
    m_nextBlockID = 0;
    m_blockTexture = 0;
    m_grid.Initialize(10, 22, 32);
    m_offsetX = 382;
    m_offsetY = 156;
    m_blockTexture = Engine::LoadTexture("Assets/Images/blockAtlas.png");
    m_pauseSfx = Engine::LoadSound("Assets/Audio/SFX3.wav");
    m_moveSfx = Engine::LoadSound("Assets/Audio/SFX4.wav");
    m_turnSfx = Engine::LoadSound("Assets/Audio/SFX6.wav");
    m_dropSfx = Engine::LoadSound("Assets/Audio/SFX8.wav");
    m_lineSfx = Engine::LoadSound("Assets/Audio/SFX11.wav");
    m_tetrisSfx = Engine::LoadSound("Assets/Audio/SFX5.wav");
    m_levelUpSfx = Engine::LoadSound("Assets/Audio/SFX7.wav");
    m_gameOverSfx = Engine::LoadSound("Assets/Audio/SFX14.wav");
    m_gameOverBar = Engine::LoadTexture("Assets/Images/bar.png");
    m_FastMusicSpeed = false;
    m_level = 0;
    m_totalLines = 0;
    m_lineForNextLevel = m_level * 10 + 10;
    m_pauseGame = false;
    m_state = PLAYING;
    m_showNextBlock = true;
    m_translateDelay = 0.15f;
    m_translateElapsed = 0.0f;
    m_gameOverElapsed = 0.0f;
    m_gameOverHeight = 0;

    m_nextBlockID = m_currentBlock.RandomBlock();
    m_nextBlock.SetBlock(m_nextBlockID);
    SpawnNextBlock();
    UpdateCurrentBlock(m_currentBlock.GetBlockActiveType());
    m_fallingDelay = GetNormalFallingSpeed();

    m_blockCount.clear();
    for (int i = 0; i < 7; i++)
    {
        Block previewBlock;
        previewBlock.SetBlock(i);
        m_statisticPreview.push_back(previewBlock);
        m_blockCount.push_back(0);
    }
}

bool Board::IsPaused()
{
    return m_pauseGame;
}

bool Board::CanDrawBackground()
{
    return m_flashEffect;
}

int Board::GetNumberOfCompletedLines()
{
    return m_totalLines;
}

int Board::GetCurrentLevel()
{
    return m_level;
}

int Board::GetScore()
{
    return m_score;
}

void Board::RenderGrid()
{
    const int cellSize = m_grid.GetCellSize();
    const int gWidth = m_grid.GetWidth();
    const int gHeight = m_grid.GetHeight();
    const int colorLevel = m_level % BLOCK_COLOR_COUNT;
    int worldX = 0, worldY = 0;
    int value = 0;
    int topCellHeight = m_grid.FindLowestRowWithValueGreater(4);
    if (topCellHeight < MUSIC_CHANGE_LEVEL)
    {
        if (!m_FastMusicSpeed)
        {
            m_FastMusicSpeed = true;
            Engine::PlayMusic(SaveGame::selectedFastMusicID);
        }
    }
    else
    {
        if (m_FastMusicSpeed)
        {
            m_FastMusicSpeed = false;
            Engine::PlayMusic(SaveGame::selectedMusicID);
        }
    }

    for (int y = 0; y < gHeight; y++)
    {
        for (int x = 0; x < gWidth; x++)
        {
            value = m_grid.GetValueAt(x, y);
            m_grid.GetWorldPosition(x, y, &worldX, &worldY);

            if (value == 0)
            {
#if _FULLDEBUG
                Engine::DrawRect(worldX + m_offsetX, worldY + m_offsetY, cellSize, cellSize, NColor::Red);
#endif
            }
            else
            {
                Rect<float> dst = {
                    static_cast<float>(worldX + m_offsetX),
                    static_cast<float>(worldY + m_offsetY),
                    static_cast<float>(cellSize),
                    static_cast<float>(cellSize)
                };

                Rect<int> src = {
                    cellSize * colorLevel,
                    32 * ((4 - (value % 4)) - 1),
                    cellSize,
                    cellSize
                };

                Engine::DrawTexture(m_blockTexture, src, dst, 0.0f, false, false, NColor::White);
            }
        }
    }

#if _FULLDEBUG
    if (topCellHeight > 0)
    {
        int idx = m_grid.GetIndexFromLocalPosition(0, topCellHeight, m_grid.GetWidth());

        int outX, outY;
        m_grid.GetWorldPositionFromIndex(idx, m_grid.GetWidth(), &outX, &outY);

        for (int i = -1; i < 2; i++)
        {
            Engine::DrawLine(
                static_cast<float>(outX + m_offsetX),
                static_cast<float>(outY + m_offsetY + i),
                static_cast<float>(outX + m_offsetX + (m_grid.GetWidth() * m_grid.GetCellSize())),
                static_cast<float>(outY + m_offsetY + i),
                NColor::White
            );
        }
    }
#endif
}

void Board::RenderNextBlock()
{
    if (!m_showNextBlock) return;

    int offsetX1 = 766;
    int offsetY1 = 450;
    int offsetX2 = 782;
    int offsetY2 = 450;

    const int cellSize = m_grid.GetCellSize();
    const int gWidth = m_grid.GetWidth();
    const int gHeight = m_grid.GetHeight();
    const int colorLevel = m_level % BLOCK_COLOR_COUNT;
    std::vector<int> tData = m_nextBlock.GetData(0);
    const int blockType = m_nextBlock.GetBlockActiveType();
    const int blockSize = m_nextBlock.GetSize();

    for (size_t i = 0; i < tData.size(); i++)
    {
        const int tValue = tData[i];

        if (tValue == 1)
        {
            int colorLevel = m_level % 9;
            const int tY = static_cast<int>(i) / blockSize;
            const int tX = static_cast<int>(i) - (blockSize * tY);

            Rect<float> dst;

            if (m_nextBlockID == 3 || m_nextBlockID == 6)
            {
                dst.x = static_cast<float>(tX * cellSize + offsetX1);
                dst.y = static_cast<float>(tY * cellSize + offsetY1);
            }
            else
            {
                dst.x = static_cast<float>(tX * cellSize + offsetX2);
                dst.y = static_cast<float>(tY * cellSize + offsetY2);
            }

            dst.w = static_cast<float>(cellSize);
            dst.h = static_cast<float>(cellSize);

            Rect<int> src = {
                cellSize * colorLevel,
                32 * ((4 - (blockType % 4)) - 1),
                cellSize,
                cellSize
            };

            Engine::DrawTexture(m_blockTexture, src, dst, 0.0f, false, false, NColor::White);
        }
    }
}

void Board::RenderPreview()
{
    int offsetX1 = 95;
    int offsetY1 = 450;

    for (int i = 0; i < 7; i++)
    {
        const int blockSize = m_statisticPreview[i].GetSize();
        std::vector<int> tData = m_statisticPreview[i].GetData(0);
        const int blockType = m_statisticPreview[i].GetBlockActiveType();

        offsetY1 = 360 + (68 * i);

        float tileSize = m_grid.GetCellSize() / 1.5f;

        for (size_t j = 0; j < tData.size(); j++)
        {
            const int tValue = tData[j];

            if (tValue == 1)
            {
                int colorLevel = m_level % 9;
                const int tY = static_cast<int>(j) / blockSize;
                const int tX = static_cast<int>(j) - (blockSize * tY);

                Rect<float> dst;
                dst.x = static_cast<float>(tX * tileSize + offsetX1);
                dst.y = static_cast<float>(tY * tileSize + offsetY1);
                dst.w = static_cast<float>(tileSize);
                dst.h = static_cast<float>(tileSize);

                if (blockType == 3)
                {
                    Rect<int> src = {
                        32 * colorLevel,
                        0,
                        32,
                        32
                    };

                    Engine::DrawTexture(m_blockTexture, src, dst, 0.0f, false, false, NColor::White);
                }
                else if (blockType == 2)
                {
                    Rect<int> src = {
                        32 * colorLevel,
                        32,
                        32,
                        32
                    };

                    Engine::DrawTexture(m_blockTexture, src, dst, 0.0f, false, false, NColor::White);
                }
                else if (blockType == 1)
                {
                    Rect<int> src = {
                        32 * colorLevel,
                        64,
                        32,
                        32
                    };

                    Engine::DrawTexture(m_blockTexture, src, dst, 0.0f, false, false, NColor::White);
                }
            }
        }
    }
}

void Board::RenderGameOver()
{
    const int colorLevel = m_level % BLOCK_COLOR_COUNT;
    for (int i = 0; i < m_gameOverHeight; i++)
    {
        int idx = m_grid.GetIndexFromLocalPosition(0, i, m_grid.GetWidth());

        int outX, outY;
        m_grid.GetWorldPositionFromIndex(idx, m_grid.GetWidth(), &outX, &outY);

        Rect<int> src(
            0, 
            m_grid.GetCellSize() * colorLevel,
            320,
            32
        );

        Rect<float> dst(
            static_cast<float>(outX + m_offsetX),
            static_cast<float>(outY + m_offsetY),
            320.0f, 32.0f
        );


        Engine::DrawTexture(m_gameOverBar, src, dst, 0, false, false, NColor::White);
    }
}

float Board::GetNormalFallingSpeed()
{
    float timePerCells = secondsToBottomAtLevel[m_level] / m_grid.GetHeight();
    float delay = timePerCells;
    return delay;
}

int Board::GetBlockCount(int blockType)
{
    return m_blockCount[blockType];
}

float Board::GetFastFallingSpeed()
{
    return 0.02f;
}

void Board::SpawnNextBlock()
{
    m_fallingDelay = GetNormalFallingSpeed();

    ChooseNextBlock();
    m_x = std::rand() % (m_grid.GetWidth() - 5);
    m_y = 0;
    m_rotation = 0;
}

void Board::ChooseNextBlock()
{
    m_currentBlock.SetBlock(m_nextBlockID);
    m_nextBlockID = m_currentBlock.RandomBlock();
    m_nextBlock.SetBlock(m_nextBlockID);
}

void Board::UpdateCurrentBlock(int value)
{
    std::vector<int> blockData = m_currentBlock.GetData(m_rotation);
    const int tBlockSize = m_currentBlock.GetSize();

    for (size_t i = 0; i < blockData.size(); i++)
    {
        if (blockData[i] > BLOCK_NONE)
        {
            int tX, tY;
            m_grid.GetLocalPosition(static_cast<int>(i), tBlockSize, &tX, &tY);
            m_grid.SetValueAt(m_x + tX, m_y + tY, value);
        }
    }
}

void Board::Fall()
{
    UpdateCurrentBlock(BLOCK_NONE);

    std::vector<int>& blockMatrix = m_currentBlock.GetData(m_rotation);
    const int blockSize = m_currentBlock.GetSize();

    if (m_grid.CheckCollision(m_x, m_y + 1, blockMatrix, blockSize))
    {
        m_blockCount[m_currentBlock.GetID()]++;
        Place();
    }
    else
    {
        m_y += 1;
        UpdateCurrentBlock(m_currentBlock.GetBlockActiveType());
    }
}

void Board::UpdatePlaying(float dt)
{
    m_fallingElapsed += dt;
    if (m_fallingElapsed >= m_fallingDelay)
    {
        m_fallingElapsed = 0.0f;
        Fall();
    }
}

void Board::Place()
{
    Engine::PlaySFX(m_dropSfx);
    UpdateCurrentBlock(m_currentBlock.GetBlockPlacedType());

    if (!CheckGameOver())
    {
        CheckLines();
        if (m_state == PLAYING)
        {
            SpawnNextBlock();
            UpdateCurrentBlock(m_currentBlock.GetBlockActiveType());
        }
    }
}

void Board::ProcessInput(float dt)
{
    if (!m_pauseGame && m_state == PLAYING)
    {
        ProcessRotation(dt);
        ProcessTranslation(dt);
        ProcessSoftDrop(dt);

        if (Engine::GetKeyDown(KEY_SELECT))
        {
            m_showNextBlock = !m_showNextBlock;
        }
    }

    if (Engine::GetKeyDown(KEY_START))
    {
        if (m_state == PLAYING || m_state == REMOVE_LINE)
        {
            m_pauseGame = !m_pauseGame;

            if (m_pauseGame)
            {
                Engine::PlaySFX(m_pauseSfx);
                Engine::PauseMusic();
            }
            else
            {
                Engine::ResumeMusic();
            }
        }
        else if (m_state == GAME_OVER)
        {
            Engine::SetState("menu");
        }
    }

}

void Board::ProcessRotation(float dt)
{
    if (Engine::GetKeyDown(KEY_B))
    {
        Rotate(-1);
    }
    else if (Engine::GetKeyDown(KEY_A))
    {
        Rotate(1);
    }
}

void Board::ProcessTranslation(float dt)
{
    static bool movePressed = false;

    if (Engine::GetKey(KEY_LEFT))
    {
        if (!movePressed)
        {
            movePressed = true;
            Translate(-1);
            m_translateElapsed = 0.0f;
        }
        else
        {
            m_translateElapsed += dt;
            if (m_translateElapsed >= m_translateDelay)
            {
                m_translateElapsed = 0.0f;
                Translate(-1);
            }
        }
    }
    else if (Engine::GetKey(KEY_RIGHT))
    {
        if (!movePressed)
        {
            movePressed = true;
            Translate(1);
            m_translateElapsed = 0.0f;
        }
        else
        {
            m_translateElapsed += dt;
            if (m_translateElapsed >= m_translateDelay)
            {
                m_translateElapsed = 0.0f;
                Translate(1);
            }
        }
    }
    else if (movePressed)
    {
        movePressed = false;
    }
}

void Board::ProcessSoftDrop(float dt)
{
    if (Engine::GetKeyDown(KEY_DOWN))
    {
        m_fallingDelay = GetFastFallingSpeed();
    }
    else if (Engine::GetKeyUp(KEY_DOWN))
    {
        m_fallingDelay = GetNormalFallingSpeed();
    }
}

void Board::Rotate(int direction)
{
    int oldRotation = m_rotation;
    int rotation = m_rotation;

    if (direction > 0)
    {
        rotation = m_currentBlock.NextRotation(m_rotation);
    }
    else if (direction < 0)
    {
        rotation = m_currentBlock.PreviousRotation(m_rotation);
    }

    std::vector<int>& blockMatrix = m_currentBlock.GetData(rotation);
    const int blockSize = m_currentBlock.GetSize();

    if (!m_grid.CheckCollision(m_x, m_y, blockMatrix, blockSize, 4))
    {
        Engine::PlaySFX(m_turnSfx);
        UpdateCurrentBlock(BLOCK_NONE);
        m_rotation = rotation;
        UpdateCurrentBlock(m_currentBlock.GetBlockActiveType());
    }
}

bool Board::CheckGameOver()
{
    int value = m_grid.FindLowestRowWithValueGreater(4);
    if (value <= 1)
    {
        Engine::StopMusic();
        m_state = HIDE_GAME;
        m_gameOverElapsed = 0.0f;
        m_gameOverHeight = 0;
        SaveGame::CheckHighScore(m_score);
        Engine::PlaySFX(m_gameOverSfx);
        return true;
    }

    return false;
}

void Board::CheckLines()
{
    bool tLineComplete = false;
    const int cellSize = m_grid.GetCellSize();
    const int gWidth = m_grid.GetWidth();
    const int gHeight = m_grid.GetHeight();

    size_t tCurrIdx = 0;
    int tY = -1;
    m_lineToRemove.clear();

    for (int y = 0; y < gHeight; y++)
    {
        tLineComplete = true;
        for (int x = 0; x < gWidth; x++)
        {
            int value = m_grid.GetValueAt(x, y);
            // tCurrIdx = GetIndexFromLocalPosition(x, y, gWidth);
            if (value < 5)
            {
                tLineComplete = false;
                break;
            }
        }

        if (tLineComplete)
        {
            if (tY < 0)
            {
                tY = y;
            }

            m_lineToRemove.push_back(y);
        }
    }

    if (m_lineToRemove.size() > 0)
    {
        m_state = REMOVE_LINE;
    }
}

void Board::UpdateRemoveLines(float dt)
{
    static float elapsedLineRemoval = 0.0f;

    elapsedLineRemoval += dt;

    if (elapsedLineRemoval > 0.03f)
    {
        m_flashEffect = false;
    }

    if (elapsedLineRemoval > 0.05f)
    {
        elapsedLineRemoval = 0.0f;

        int hW = m_grid.GetWidth() / 2;
        int cX = hW - 1;
        int cY = cX + 1;
        bool blockRemoved = false;

        for (int i = 0; i < hW; i++)
        {
            for (int row : m_lineToRemove)
            {
                const int idxA = m_grid.GetIndexFromLocalPosition(cX, row, m_grid.GetWidth());
                const int idxB = m_grid.GetIndexFromLocalPosition(cY, row, m_grid.GetWidth());

                int value = m_grid.GetValueAt(idxA);
                if (value != BLOCK_NONE)
                {
                    blockRemoved = true;
                    m_grid.SetValueAt(idxA, BLOCK_NONE);
                    m_grid.SetValueAt(idxB, BLOCK_NONE);
                    m_flashEffect = m_lineToRemove.size() >= 4;
                }
            }

            if (blockRemoved)
            {
                return;
            }
            else
            {
                cX--;
                cY++;
            }
        }

        int lineToRemoveSize = static_cast<int>(m_lineToRemove.size());
        if (lineToRemoveSize > 0)
        {
            ScrollBoard(0, m_lineToRemove);

            AddScore(lineToRemoveSize);
            if (lineToRemoveSize >= 4)
            {
                Engine::PlaySFX(m_tetrisSfx);
            }
            else
            {
                Engine::PlaySFX(m_lineSfx);
            }
        }

        m_lineToRemove.clear();
        m_state = PLAYING;
        SpawnNextBlock();
        UpdateCurrentBlock(m_currentBlock.GetBlockActiveType());
    }
}

void Board::Translate(int direction)
{
    const int tX = m_x + direction;
    std::vector<int>& blockMatrix = m_currentBlock.GetData(m_rotation);
    const int blockSize = m_currentBlock.GetSize();

    if (!m_grid.CheckCollision(tX, m_y, blockMatrix, blockSize, 4))
    {
        Engine::PlaySFX(m_moveSfx);
        UpdateCurrentBlock(BLOCK_NONE);
        m_x = tX;
        UpdateCurrentBlock(m_currentBlock.GetBlockActiveType());
    }
}

void Board::ScrollBoard(int row, const std::vector<int>& removedLines)
{
    const int tLineCount = static_cast<int>(removedLines.size());
    AddLines(tLineCount);

    for (int line : removedLines)
    {
        for (int y = line; y >= 0; y--)
        {
            for (int x = 0; x < m_grid.GetWidth(); x++)
            {
                const int tIdxA = m_grid.GetIndexFromLocalPosition(x, y, m_grid.GetWidth());
                const int tIdxB = m_grid.GetIndexFromLocalPosition(x, y + 1, m_grid.GetWidth());
                int valueA = m_grid.GetValueAt(tIdxA);
                int valueB = m_grid.GetValueAt(tIdxB);
                if (valueA > 4 && valueB == BLOCK_NONE)
                {
                    int temp = valueA;
                    m_grid.SetValueAt(tIdxA, BLOCK_NONE);
                    m_grid.SetValueAt(tIdxB, temp);
                }
            }
        }
    }
}

void Board::AddLines(int count)
{
    m_totalLines += count;
    if (m_totalLines >= m_lineForNextLevel)
    {
        m_level++;
        Engine::PlaySFX(m_levelUpSfx);
        if (m_level > 29)
        {
            m_level = 29;
        }

        m_lineForNextLevel = m_level * 10 + 10;
    }
}

void Board::UpdateGameOver(float dt)
{
    if (m_gameOverHeight < m_grid.GetHeight())
    {
        m_gameOverElapsed += dt;
        if (m_gameOverElapsed > 0.05f)
        {
            m_gameOverElapsed = 0.0f;
            m_gameOverHeight += 1;
        }
    }
    else
    {
        m_state = GAME_OVER;
    }
}

void Board::AddScore(int numberOfLines)
{
    int score = 0;
    switch (numberOfLines)
    {
    case 1: score = 40 * (m_level + 1); break;
    case 2: score = 100 * (m_level + 1); break;
    case 3: score = 300 * (m_level + 1); break;
    case 4: score = 1200 * (m_level + 1); break;
    }

    m_score += score;
}
