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

static auto buildPath(int size, int x, int y, std::vector<std::pair<int, int>> isVisited) -> std::vector<std::pair<int, int>>
{
  if (x == size - 1 && y == size - 1)
    return isVisited;
  auto dirs = std::array<std::pair<int, int>, 4>{std::make_pair(0, 1), std::make_pair(0, -1), std::make_pair(1, 0), std::make_pair(-1, 0)};
  static std::random_device rd;
  static std::mt19937 g(rd());
  std::shuffle(std::begin(dirs), std::end(dirs), g);
  for (const auto d : dirs)
  {
    auto newX = x + d.first;
    auto newY = y + d.second;
    if (newX < 0 || newX >= size)
      continue;
    if (newY < 0 || newY >= size)
      continue;
    if (isVisited[newX + newY * size] != std::make_pair(0, 0))
      continue;
    auto newIsVisited = isVisited;
    newIsVisited[x + y * size] = d;
    auto ret = buildPath(size, newX, newY, newIsVisited);
    if (!ret.empty())
      return ret;
  }
  return {};
}

auto Maze::buildMap() -> void
{
  auto map = buildPath(size, 0, 0, std::vector<std::pair<int, int>>{static_cast<size_t>(size * size)});
  if (map.empty())
    return;
  {
    auto x = 0;
    auto y = 0;
    while (x != size - 1 || y != size - 1)
    {
      auto d = map[x + y * size];
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

  for (auto y = 0U; y < size; ++y)
  {
    auto row = std::string{};
    for (auto x = 0U; x < size; ++x)
    {
      if (map[x + y * size] == std::make_pair(0, 1))
        row += "V";
      else if (map[x + y * size] == std::make_pair(0, -1))
        row += "^";
      else if (map[x + y * size] == std::make_pair(1, 0))
        row += ">";
      else if (map[x + y * size] == std::make_pair(-1, 0))
        row += "<";
      else
        row += " ";
    }
    LOG(row);
  }
}
