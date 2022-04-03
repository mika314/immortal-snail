#include "maze.h"
#include "log.h"
#include <array>
#include <random>

static const auto WallTypesNum = 8;

Maze::Maze(int size) : size(size)
{
  for (auto i = 0U; i < size * (2 * size - 1); ++i)
    data.push_back(rand() % (WallTypesNum + 1));
  buildMap();
}

static auto buildPath(std::vector<std::pair<int, int>> solution, int size, int x, int y) -> std::vector<std::pair<int, int>>
{
  if (solution.size() > 32 * size)
    return {};
  if (x == size - 1 && y == size - 1)
    return solution;
  auto dirs = std::array<std::pair<int, int>, 4>{std::make_pair(0, 1), std::make_pair(1, 0), std::make_pair(0, -1), std::make_pair(-1, 0)};
  if (rand() % 16 != 0)
  {
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(std::begin(dirs), std::end(dirs), g);
  }
  for (const auto d : dirs)
  {
    auto newX = x + d.first;
    auto newY = y + d.second;
    if (newX < 0 || newX >= size)
      continue;
    if (newY < 0 || newY >= size)
      continue;
    auto newSolution = solution;
    newSolution.push_back(d);
    auto ret = buildPath(newSolution, size, newX, newY);
    if (!ret.empty())
      return ret;
  }
  return {};
}

auto Maze::buildMap() -> void
{
  auto map = buildPath({}, size, 0, 0);
  if (map.empty())
    return;
  {
    auto x = 0;
    auto y = 0;
    for (auto d : map)
    {
      if (d == std::make_pair(1, 0))
        data[x + y * 2 * size] = 0;
      else if (d == std::make_pair(-1, 0))
        data[x - 1 + y * 2 * size] = 0;
      else if (d == std::make_pair(0, 1))
        data[x + (y * 2 + 1) * size] = 0;
      else if (d == std::make_pair(0, -1))
        data[x + (y * 2 - 1) * size] = 0;
      x += d.first;
      y += d.second;
    }
  }
}
