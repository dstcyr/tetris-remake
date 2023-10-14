#include "SaveGame.h"
// #include <iostream>
// #include <fstream>
// https://cplusplus.com/doc/tutorial/files/

size_t SaveGame::selectedMusicID = 0;
size_t SaveGame::selectedFastMusicID = 0;
size_t SaveGame::selectedGameModeID = 0;
int SaveGame::highScore = 1000;
CFile SaveGame::m_saveGameFile;

void SaveGame::CheckHighScore(int score)
{
    if (score > highScore)
    {
        highScore = score;
    }

    Save();
}

void SaveGame::Save()
{
    m_saveGameFile.Seek(0);
    m_saveGameFile.WriteInt(highScore);
    m_saveGameFile.Save("tetris.sav");
    m_saveGameFile.Release();
}

void SaveGame::Load()
{
    if (CFile::Exists("tetris.sav"))
    {
        m_saveGameFile.Load("tetris.sav");
        highScore = m_saveGameFile.ReadInt();
        m_saveGameFile.Release();
    }
    else
    {
        highScore = 1000;
    }
}
