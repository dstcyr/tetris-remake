#pragma once
#include "StateMachine.h"
#include <string>
#include <map>

class OptionState : public IState
{
public:
    OptionState();
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnRender() override;
    void OnExit() override;

private:
    int m_selectedGameType = 0;
    int m_selectedMusic = 0;
    size_t m_font8BitWonder;
    size_t m_font8BitWonderGray;
    size_t m_background;
    size_t m_arrow;
    float flashElapsed;
    bool showArrow;
};