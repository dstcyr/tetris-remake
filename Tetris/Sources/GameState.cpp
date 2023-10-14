#include "GameState.h"
#include "Log.h"
#include <sstream>
#include <iomanip>
#include "SaveGame.h"

int highScore = 0;

GameState::GameState()
{
    LOG(LL_VERBOSE, "GameState created");
    m_font8BitWonder32 = 0;
    m_font8BitWonder24 = 0;
    m_font8BitWonder32Blue = 0;
    m_font8BitWonder32Red = 0;
    m_background = 0;
    m_outline = 0;
    m_boardTint = NColor(110, 110, 110, 255);
}

void GameState::OnEnter()
{
    LOG(LL_VERBOSE, "OnEnter GameState");
    m_font8BitWonder32 = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "8bitwonderWhite32", 32, NColor::White);
    m_font8BitWonder32Red = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "8bitwonderRed32", 32, NColor(224, 80, 0, 255));
    m_font8BitWonder32Blue = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "", 32, NColor(80, 128, 255, 255));
    m_font8BitWonder24 = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "8bitwonderWhite24", 24, NColor::White);
    m_background = Engine::LoadTexture("Assets/Images/background.png");
    m_outline = Engine::LoadTexture("Assets/Images/outline.png");

    SaveGame::Load();
    highScore = SaveGame::highScore;

    m_board.Start();
    Engine::PlayMusic(SaveGame::selectedMusicID);
}

void GameState::OnUpdate(float dt)
{
    m_board.Update(dt);
}

void GameState::OnRender()
{
    if (m_board.IsPaused())
    {
        Engine::DrawString("PAUSE", m_font8BitWonder32Blue, 432.0f, 485.0f);
    }
    else
    {
        m_board.Render();

        if(m_board.CanDrawBackground())
        {
            Engine::DrawTexture(m_background, false, false, NColor::White);
        }
        else
        {
            Engine::DrawTexture(m_background, false, false, m_boardTint);
        }
        Engine::DrawTexture(m_outline, false, false, NColor::White);

        Engine::DrawString("NEXT", m_font8BitWonder32, 769.0f, 414.0f);
        Engine::DrawString("LEVEL", m_font8BitWonder32, 769.0f, 652.0f);
        Engine::DrawString("TOP", m_font8BitWonder32, 769.0f, 105.0f);
        Engine::DrawString("SCORE", m_font8BitWonder32, 769.0f, 209.0f);
        Engine::DrawString("STATISTICS", m_font8BitWonder24, 73.0f, 280.0f);
        Engine::DrawString("A-TYPE", m_font8BitWonder32, 98.0f, 105.0f);

        // POLISH : Find a better way than stringstream each frame
        std::stringstream topStream;
        topStream << std::setw(6) << std::setfill('0') << highScore;
        Engine::DrawString(topStream.str(), m_font8BitWonder32, 769.0f, 140.0f);

        std::stringstream lineStream;
        lineStream << std::setw(4) << std::setfill('0') << m_board.GetNumberOfCompletedLines();
        Engine::DrawString("LINES-" + lineStream.str(), m_font8BitWonder32, 385.0f, 71.0f);

        std::stringstream levelStream;
        levelStream << std::setw(3) << std::setfill('0') << m_board.GetCurrentLevel();
        Engine::DrawString(levelStream.str(), m_font8BitWonder32, 800.0f, 689.0f);

        std::stringstream scoreStream;
        scoreStream << std::setw(6) << std::setfill('0') << m_board.GetScore();
        Engine::DrawString(scoreStream.str(), m_font8BitWonder32, 769.0f, 245.0f);

        for (int i = 0; i < 7; i++)
        {
            int blockCount = m_board.GetBlockCount(i);
            std::stringstream scoreStream;
            scoreStream << std::setw(3) << std::setfill('0') << blockCount;
            Engine::DrawString(scoreStream.str(), m_font8BitWonder32Red, 192.0f, 383.0f + (i * 68));
        }
    }
}

void GameState::OnExit()
{
    LOG(LL_VERBOSE, "OnExit GameState");
    m_board.Destroy();
}
