#include "MenuState.h"
#include "Log.h"
#include "Engine.h"

MenuState::MenuState()
{
    LOG(LL_VERBOSE, "MenuState created");
    m_font8BitWonder = 0;
    m_background = 0;
}

void MenuState::OnEnter()
{
    LOG(LL_VERBOSE, "OnEnter MenuState");
    m_font8BitWonder = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "8bitwonder32White", 32, NColor::White);
    m_background = Engine::LoadTexture("Assets/Images/mainmenu.png");
}

void MenuState::OnUpdate(float dt)
{
    if (Engine::GetKeyDown(KEY_START))
    {
        Engine::SetState("option");
    }
}

void MenuState::OnRender()
{
    Engine::DrawTexture(m_background, false, false, NColor::White);
    Engine::DrawString("PUSH START", m_font8BitWonder, 230.0f, 645.0f);
}

void MenuState::OnExit()
{
    LOG(LL_VERBOSE, "OnExit MenuState");
}
