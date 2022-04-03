#include "maze.h"
#include "log.h"
#include <array>
#include <random>

static const auto WallTypesNum = 8;

Maze::Maze(int size) : size(size)
{
  for (auto i = 0U; i < size * (2 * size - 1); ++i)
  {
    auto t = rand() % (WallTypesNum + 4);
    if (t > WallTypesNum)
      data.push_back(0);
    else
      data.push_back(t);
  }
  buildMap();
}

static auto buildPath(std::vector<std::pair<int, int>> solution, int size, int x, int y) -> std::vector<std::pair<int, int>>
{
  if (solution.size() > 32 * size)
    return {};
  if (x == size - 1 && y == size - 1)
    return solution;

  auto dirs = std::array<std::pair<int, int>, 4>{std::make_pair(0, 1), std::make_pair(1, 0), std::make_pair(0, -1), std::make_pair(-1, 0)};
  if (rand() % 4 != 0)
  {
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(std::begin(dirs), std::end(dirs), g);
  }
  else if (rand() % 2 == 0)
    std::swap(dirs[0], dirs[1]);
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

auto Maze::isOpen(int x1, int y1, int x2, int y2) const -> bool
{
  if (x1 < 0)
    return false;
  if (x2 < 0)
    return false;
  if (y1 < 0)
    return false;
  if (y2 < 0)
    return false;

  if (x1 >= size)
    return false;
  if (x2 >= size)
    return false;
  if (y1 >= size)
    return false;
  if (y2 >= size)
    return false;
  if ((x2 - x1 == 1) && (y2 - y1 == 0))
    return data[x1 + y1 * 2 * size] == 0;
  else if ((x2 - x1 == -1) && (y2 - y1 == 0))
    return data[x1 - 1 + y1 * 2 * size] == 0;
  else if ((x2 - x1 == 0) && (y2 - y1 == 1))
    return data[x1 + (y1 * 2 + 1) * size] == 0;
  else if ((x2 - x1 == 0) && (y2 - y1 == -1))
    return data[x1 + (y1 * 2 - 1) * size] == 0;
  return false;
}
