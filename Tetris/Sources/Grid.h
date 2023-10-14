#pragma once

#include <vector>

class Grid
{
public:
    void Initialize(int width, int height, int cellSize);
    void Destroy();
    int GetWidth() const;
    int GetHeight() const;
    int GetValueAt(int idx) const;
    int GetValueAt(int x, int y) const;
    void SetValueAt(int x, int y, int value);
    void SetValueAt(int idx, int value);
    int GetCellSize() const;
    int GetCellCount() const;
    void GetWorldPosition(int x, int y, int* outX, int* outY);
    int GetIndexFromLocalPosition(int x, int y, int width) const;
    void GetWorldPositionFromIndex(int index, int width, int* outX, int* outY) const;
    void GetLocalPosition(int index, int width, int* outX, int* outY) const;
    bool CheckCollision(int x, int y, const std::vector<int>& matrix, int size, int minValue = 0);
    int FindLowestRowWithValueGreater(int value);

private:
    int* m_gridData;
    int m_cellSize;
    int m_gridWidth;
    int m_gridHeight;
    int m_cellCount;
};