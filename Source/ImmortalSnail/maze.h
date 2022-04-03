#pragma once
#include <vector>

class Maze
{
public:
  Maze(int size);
  auto getSize() const { return size; }
  auto getWall(int x, int y) const { return data[x + y * size]; }
  auto isOpen(int x1, int y1, int x2, int y2) const -> bool;

private:
  int size;
  std::vector<int> data;

  auto buildMap() -> void;
};
