#pragma once
#include "File.h"

class SaveGame
{
public:
    static size_t selectedMusicID;
    static size_t selectedFastMusicID;
    static size_t selectedGameModeID;
    static int highScore;

    static void CheckHighScore(int score);
    static void Save();
    static void Load();

private:
    static CFile m_saveGameFile;
};