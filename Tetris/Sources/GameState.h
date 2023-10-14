#pragma once
#include "StateMachine.h"
#include "Board.h"
#include "Engine.h"

class GameState : public IState
{
public:
    GameState();
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnRender() override;
    void OnExit() override;

private:
    size_t m_font8BitWonder32;
    size_t m_font8BitWonder32Blue;
    size_t m_font8BitWonder32Red;
    size_t m_font8BitWonder24;
    size_t m_background;
    size_t m_outline;
    Board m_board;
    NColor m_boardTint;
};