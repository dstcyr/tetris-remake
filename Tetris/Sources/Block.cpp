#include "Block.h"

std::vector<int> m_blockActiveType{ 1, 2, 3, 1, 2, 3, 1 };
std::vector<int> m_blockPlacedType{ 5, 6, 7, 5, 6, 7, 5 };

std::vector<std::vector<std::vector<int>>> m_data =
{
    // 0 - T-tetromimo
    {
        {
            0, 0, 0, 0,
            1, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 1, 0, 0,
            1, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 1, 0, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }
    },
    // 1 - J-tetromimo
    {
        {
            0, 0, 0, 0,
            1, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0
        },

        {
            0, 1, 0, 0,
            0, 1, 0, 0,
            1, 1, 0, 0,
            0, 0, 0, 0
        },
        {
            1, 0, 0, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }
    },
    // 2 - Z-tetromimo
    {
        {
            0, 0, 0, 0,
            1, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        },
        {
            0, 1, 0, 0,
            1, 1, 0, 0,
            1, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            1, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }
    },
    // 3 - O-tetromimo
    {
        {
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        },
        {
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        },
        {
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        },
        {
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        }
    },
    // 4 - S-tetromimo
    {
        {
            0, 0, 0, 0,
            0, 1, 1, 0,
            1, 1, 0, 0,
            0, 0, 0, 0
        },
        {
            1, 0, 0, 0,
            1, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 1, 1, 0,
            1, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0
        }
    },
    // 5 - L-tetromimo
    {
        {
            0, 0, 0, 0,
            1, 1, 1, 0,
            1, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            1, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 0, 1, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        }
    },
    // 6 - I-tetromimo
    {
        {
            0, 0, 0, 0,
            0, 0, 0, 0,
            1, 1, 1, 1,
            0, 0, 0, 0
        },
        {
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0
        },
        {
            0, 0, 0, 0,
            0, 0, 0, 0,
            1, 1, 1, 1,
            0, 0, 0, 0
        },
        {
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0
        }
    }
};

Block::Block()
{
    m_blockID = 0;
    m_blockSize = 4;
}

void Block::SetBlock(int blockid)
{
    m_blockID = blockid;
}

int Block::GetBlock() const
{
    return m_blockID;
}

int Block::GetSize() const
{
    return m_blockSize;
}

int Block::NextRotation(int rotation) const
{
    int nextRotation = rotation + 1;
    size_t size = m_data[m_blockID].size() - 1;
    if (nextRotation > size)
    {
        nextRotation = 0;
    }

    return nextRotation;
}

int Block::PreviousRotation(int rotation) const
{
    int nextRotation = rotation - 1;
    if (nextRotation < 0)
    {
        nextRotation = static_cast<int>(m_data[m_blockID].size() - 1);
    }

    return nextRotation;
}

int Block::RandomBlock() const
{
    // https://simon.lc/the-history-of-tetris-randomizers
    // To cut down on piece floods(repeating pieces) a history check was added to the
    // randomizer.This simple check would,
    //      - choose a piece,
    //      - check if the piece was the same as the last,
    //      - If it was it would chose a new piece, but only once,
    //      - and whatever was the result, was the piece dealt.

    int nextBlock = std::rand() % static_cast<int>(m_data.size());
    if (m_blockID == nextBlock)
    {
        nextBlock = std::rand() % static_cast<int>(m_data.size());
    }

    return nextBlock;
}

std::vector<int>& Block::GetData(int rotation)
{
    return m_data[m_blockID][rotation];
}

int Block::GetBlockActiveType()
{
    return m_blockActiveType[m_blockID];
}

int Block::GetBlockPlacedType()
{
    return m_blockPlacedType[m_blockID];
}

std::string Block::GetDebugString(int id)
{
    switch (id)
    {
    case 0: return std::string("{0 - T-tetromimo}");
    case 1: return std::string("{1 - L-tetromimo}");
    case 2: return std::string("{2 - Z-tetromimo}");
    case 3: return std::string("{3 - O-tetromimo}");
    case 4: return std::string("{4 - S-tetromimo}");
    case 5: return std::string("{5 - J-tetromimo}");
    case 6: return std::string("{6 - I-tetromimo}");
    }

    return "Unknown block";
}
