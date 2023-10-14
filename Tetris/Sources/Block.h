#pragma once
#include <vector>
#include <string>

class Block
{
public:
    Block();
    int GetID() const { return m_blockID; }
    void SetBlock(int blockid);
    int GetBlock() const;
    int GetSize() const;
    int NextRotation(int rotation) const;
    int PreviousRotation(int rotation) const;
    int RandomBlock() const;
    std::vector<int>& GetData(int rotation);
    int GetBlockActiveType();
    int GetBlockPlacedType();
    static std::string GetDebugString(int id);

private:
    int m_blockID;
    int m_blockSize;
};