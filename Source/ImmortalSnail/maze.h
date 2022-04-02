#pragma once
#include <vector>

class Maze
{
public:
  Maze(int size);
  auto getSize() const { return size; }
  auto getWall(int x, int y) { return data[x + y * size]; }

private:
  int size;
  std::vector<int> data;

  auto buildMap() -> void;
};
