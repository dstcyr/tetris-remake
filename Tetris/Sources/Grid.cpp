#include "Grid.h"
#include "Log.h"
#include <cstdlib>
#include <cstring>

void Grid::Initialize(int width, int height, int cellSize)
{
    m_gridWidth = width;
    m_gridHeight = height;
    m_cellSize = cellSize;
    m_cellCount = width * height;

    const size_t byteSize = m_cellCount * sizeof(int);
    m_gridData = static_cast<int*>(std::malloc(byteSize));

    CHECK(m_gridData);
    std::memset(m_gridData, 0, byteSize);
}

void Grid::Destroy()
{
    if (m_gridData)
    {
        free(m_gridData);
        m_gridData = nullptr;
    }
}

int Grid::GetWidth() const
{
    return m_gridWidth;
}

int Grid::GetHeight() const
{
    return m_gridHeight;
}

int Grid::GetValueAt(int idx) const
{
    return m_gridData[idx];
}

int Grid::GetValueAt(int x, int y) const
{
    const int idx = GetIndexFromLocalPosition(x, y, m_gridWidth);
    return GetValueAt(idx);
}

void Grid::SetValueAt(int x, int y, int value)
{
    const int idx = GetIndexFromLocalPosition(x, y, m_gridWidth);
    SetValueAt(idx, value);
}

void Grid::SetValueAt(int idx, int value)
{
    m_gridData[idx] = value;
}

int Grid::GetCellSize() const
{
    return m_cellSize;
}

int Grid::GetCellCount() const
{
    return m_cellCount;
}

void Grid::GetWorldPosition(int x, int y, int* outX, int* outY)
{
    *outX = x * m_cellSize;
    *outY = y * m_cellSize;
}

int Grid::GetIndexFromLocalPosition(int x, int y, int width) const
{
    return x + width * y;
}

void Grid::GetWorldPositionFromIndex(int index, int width, int* outX, int* outY) const
{
    int localX, localY;
    GetLocalPosition(index, width, &localX, &localY);
    *outX = localX * m_cellSize;
    *outY = localY * m_cellSize;
}

void Grid::GetLocalPosition(int index, int width, int* outX, int* outY) const
{
    *outY = static_cast<int>(index) / width;
    *outX = static_cast<int>(index) - width * *outY;
}

bool Grid::CheckCollision(int x, int y, const std::vector<int>& matrix, int size, int minValue)
{
    for (size_t i = 0; i < matrix.size(); i++)
    {
        if (matrix[i] > 0)
        {
            int tX, tY;
            GetLocalPosition(static_cast<int>(i), size, &tX, &tY);
            const size_t idx = GetIndexFromLocalPosition(x + tX, y + tY, m_gridWidth);

            const int newX = x + tX;
            if (newX < 0)
            {
                return true;
            }

            if (newX > m_gridWidth - 1)
            {
                return true;
            }

            if (idx > m_cellCount - 1)
            {
                return true;
            }

            if (idx >= 0 && idx < m_cellCount)
            {
                if (m_gridData[idx] > minValue)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

int Grid::FindLowestRowWithValueGreater(int value)
{
    for (int y = 0; y < m_gridHeight; y++)
    {
        for (int x = 0; x < m_gridWidth; x++)
        {
            int idx = GetIndexFromLocalPosition(x, y, m_gridWidth);
            if (m_gridData[idx] >= value)
            {
                return y;
            }
        }
    }
    
    return m_gridHeight;
}
