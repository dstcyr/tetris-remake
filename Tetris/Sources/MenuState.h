#pragma once
#include "StateMachine.h"

class MenuState : public IState
{
public:
    MenuState();
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnRender() override;
    void OnExit() override;

private:
    size_t m_font8BitWonder;
    size_t m_background;
};