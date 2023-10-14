#include "OptionState.h"
#include "Log.h"
#include "Engine.h"
#include "SaveGame.h"

Rect<float> m_arrowGameTypeLeft;
Rect<float> m_arrowGameTypeRight;
Rect<float> m_arrowMusicLeft;
Rect<float> m_arrowMusicRight;
size_t m_arrowSfx;
size_t m_musicId[4];
size_t m_fastMusicId[4];

OptionState::OptionState()
{
    LOG(LL_VERBOSE, "OptionState created");
    m_font8BitWonder = 0;
    m_selectedGameType = 0;
    m_selectedMusic = 0;
    m_font8BitWonderGray = 0;
    m_background = 0;
    m_arrow = 0;
    flashElapsed = 0.0f;
    showArrow = false;
}

void OptionState::OnEnter()
{
    showArrow = true;

    LOG(LL_VERBOSE, "OnEnter OptionState");

    m_font8BitWonder = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "8bitwonder32White", 32, NColor::White);
    m_font8BitWonderGray = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "8bitwonder32Grey", 32, NColor(61, 61, 61, 255));
    m_background = Engine::LoadTexture("Assets/Images/options.png");
    m_arrow = Engine::LoadTexture("Assets/Images/arrow.png");
    m_arrowSfx = Engine::LoadSound("Assets/Audio/SFX2.wav");
    m_musicId[0] = Engine::LoadMusic("Assets/Audio/Music1.wav");
    m_musicId[1] = Engine::LoadMusic("Assets/Audio/Music2.wav");
    m_musicId[2] = Engine::LoadMusic("Assets/Audio/Music3.wav");
    m_musicId[3] = 0;
    m_fastMusicId[0] = Engine::LoadMusic("Assets/Audio/Music1Fast.wav");
    m_fastMusicId[1] = Engine::LoadMusic("Assets/Audio/Music2Fast.wav");
    m_fastMusicId[2] = Engine::LoadMusic("Assets/Audio/Music3Fast.wav");
    m_fastMusicId[3] = 0;

    Engine::PlayMusic(m_musicId[0]);

    m_arrowGameTypeLeft = {
        640.0f,
        242.0f,
        32, 32
    };

    m_arrowGameTypeRight = {
        873.0f,
        242.0f,
        32, 32
    };

    m_arrowMusicLeft = {
        416.0f,
        578.0f,
        32, 32
    };

    m_arrowMusicRight = {
        710.0f,
        578.0f,
        32, 32
    };
}

namespace InternalOptionState
{
    bool CheckUp()
    {
        if (Engine::GetKeyDown(KEY_UP))
        {
            return true;
        }

        return false;
    }

    bool CheckDown()
    {
        if (Engine::GetKeyDown(KEY_DOWN))
        {
            return true;
        }

        return false;
    }

    bool CheckStart()
    {
        if (Engine::GetKeyDown(KEY_START))
        {
            return true;
        }

        return false;
    }
}

void OptionState::OnUpdate(float dt)
{
    // if (InternalOptionState::CheckLeft())
    // {
    //     m_selectedGameType -= 1;
    //     if (m_selectedGameType < 0)
    //     {
    //         m_selectedGameType = 0;
    //     }
    // 
    //     LOG(LL_VERBOSE, "Selecting Game Type #%d", m_selectedGameType);
    // }
    // else if (InternalOptionState::CheckRight())
    // {
    //     m_selectedGameType += 1;
    //     if (m_selectedGameType > 1)
    //     {
    //         m_selectedGameType = 1;
    //     }
    // 
    //     LOG(LL_VERBOSE, "Selecting Game Type #%d", m_selectedGameType);
    // }
    if (InternalOptionState::CheckDown())
    {
        if (m_selectedMusic < 3)
        {
            m_selectedMusic += 1;
            if (m_selectedMusic >= 3)
            {
                m_selectedMusic = 3;
                Engine::StopMusic();
            }
            else
            {
                Engine::PlayMusic(m_musicId[m_selectedMusic]);
            }
        }

        LOG(LL_VERBOSE, "Selecting Music Type #%d", m_selectedMusic);
    }
    else if (InternalOptionState::CheckUp())
    {
        m_selectedMusic -= 1;
        if (m_selectedMusic < 0)
        {
            m_selectedMusic = 0;
        }
        else
        {
            Engine::PlayMusic(m_musicId[m_selectedMusic]);
        }

        LOG(LL_VERBOSE, "Selecting Music Type #%d", m_selectedMusic);
    }
    else if (InternalOptionState::CheckStart())
    {
        SaveGame::selectedMusicID = m_musicId[m_selectedMusic];
        SaveGame::selectedFastMusicID = m_fastMusicId[m_selectedMusic];
        SaveGame::selectedGameModeID = m_selectedGameType;
        Engine::StopMusic();
        Engine::SetState("game");
    }

    if (m_selectedGameType == 0)
    {
        m_arrowGameTypeLeft.x = 250.0f;
        m_arrowGameTypeLeft.y = 242.0f;
        m_arrowGameTypeRight.x = 487.0f;
        m_arrowGameTypeRight.y = 242.0f;
    }
    else
    {
        m_arrowGameTypeLeft.x = 640.0f;
        m_arrowGameTypeLeft.y = 242.0f;
        m_arrowGameTypeRight.x = 873.0f;
        m_arrowGameTypeRight.y = 242.0f;
    }

    m_arrowMusicLeft.x = 416.0f;
    m_arrowMusicLeft.y = 578.0f + (70.0f * m_selectedMusic);
    m_arrowMusicRight.x = 710.0f;
    m_arrowMusicRight.y = 578.0f + (70.0f * m_selectedMusic);

    flashElapsed += dt;
    if (flashElapsed > 0.04f)
    {
        showArrow = !showArrow;
        flashElapsed = 0.0f;
    }
}

void OptionState::OnRender()
{
    Engine::DrawTexture(m_background, false, false, NColor::White);
    Engine::DrawString("GAME TYPE", m_font8BitWonder, 180.0f, 105.0f);
    Engine::DrawString("A-TYPE", m_font8BitWonder, 290.0f, 244.0f);
    Engine::DrawString("B-TYPE", m_font8BitWonderGray, 680.0f, 244.0f);
    Engine::DrawString("MUSIC TYPE", m_font8BitWonder, 162.0f, 415.0f);
    Engine::DrawString("MUSIC-1", m_font8BitWonder, 470.0f, 580.0f);
    Engine::DrawString("MUSIC-2", m_font8BitWonder, 470.0f, 650.0f);
    Engine::DrawString("MUSIC-3", m_font8BitWonder, 470.0f, 720.0f);
    Engine::DrawString("OFF", m_font8BitWonder, 530.0f, 790.0f);

    if (showArrow)
    {
        Engine::DrawTexture(m_arrow, m_arrowGameTypeLeft, 0.0, false, false, NColor::White);
        Engine::DrawTexture(m_arrow, m_arrowGameTypeRight, 0.0, true, false, NColor::White);

        Engine::DrawTexture(m_arrow, m_arrowMusicLeft, 0.0, false, false, NColor::White);
        Engine::DrawTexture(m_arrow, m_arrowMusicRight, 0.0, true, false, NColor::White);
    }
}

void OptionState::OnExit()
{
    LOG(LL_VERBOSE, "OnExit OptionState");
}
