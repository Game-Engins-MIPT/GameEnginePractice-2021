// initial skeleton is a clone from https://github.com/jpcy/bgfx-minimal-example
//
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <debugdraw/debugdraw.h>
#include <functional>
#include "app.h"

#include <vector>

/*
struct NavNode
{
  std::vector<NavNode*> edges;
  bx::Vec3 pos;

  NavNode() = default;
  NavNode(const bx::Vec3 &p) : pos(p) {}
};

struct NavGraph
{
  std::vector<NavNode*> nodes;
};

// we assume that input is NxN grid of a regular size where 'x' is not passable and ' ' is passable
static NavGraph build_nav_graph(const char *input)
{
  int sz = int(sqrtf(float(strlen(input))));
  // find first passable position
  NavGraph res;
  for (int y = 0; y < sz; ++y)
    for (int x = 0; x < sz; ++x)
    {
      if (input[y * sz + x] == ' ')
        res.nodes.emplace_back(NavNode(bx::Vec3(float(x), float(y), 0.f)));
    }
}
*/


struct GridPos
{
  int x;
  int y;

  GridPos() = default;
  GridPos(int _x, int _y): x(_x), y(_y) {}
};

bool operator==(const GridPos &lhs, const GridPos &rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
bool operator!=(const GridPos &lhs, const GridPos &rhs) { return !(lhs == rhs); }

static void draw_nav_grid(DebugDrawEncoder &dde, const char *input, int dim)
{
  dde.push();
    for (int y = 0; y < dim; ++y)
      for (int x = 0; x < dim; ++x)
      {
        dde.setColor(input[y * dim + x] == ' ' ? 0xffeeeeee : 0xff222222);
        dde.drawQuad(bx::Vec3(0.f,0.f,1.f), bx::Vec3(float(x - dim/2), float(y - dim/2), 0.f), 1.f);
      }
  dde.pop();
}

static void draw_path(DebugDrawEncoder &dde, std::vector<GridPos> path, const char *input, int dim)
{
  dde.push();

    dde.setColor(0xff0000ff);
    for (const GridPos &p : path)
      dde.drawQuad(bx::Vec3(0.f,0.f,1.f), bx::Vec3(float(p.x - dim/2), float(p.y - dim/2), -0.01f), 1.f);

  dde.pop();
}

static std::vector<GridPos> reconstruct_path(std::vector<GridPos> prev, GridPos to, size_t dim)
{
  GridPos curPos = to;
  std::vector<GridPos> res = {curPos};
  while (prev[curPos.y * dim + curPos.x] != GridPos(-1, -1))
  {
    curPos = prev[curPos.y * dim + curPos.x];
    res.insert(res.begin(), curPos);
  }
  return res;
}

static std::vector<GridPos> find_path_a_star(const char *input, int dim, GridPos from, GridPos to)
{
  if (from.x < 0 || from.y < 0 || from.x >= dim || from.y >= dim)
    return std::vector<GridPos>();
  size_t inpSize = dim * dim;

  std::vector<float> g(inpSize, FLT_MAX);
  std::vector<float> f(inpSize, FLT_MAX);
  std::vector<GridPos> prev(inpSize, {-1,-1});

  auto getG = [&](GridPos p) -> float { return g[p.y * dim + p.x]; };
  auto getF = [&](GridPos p) -> float { return f[p.y * dim + p.x]; };

  auto heuristic = [](GridPos lhs, GridPos rhs) -> float
  {
    return sqrtf(bx::square(float(lhs.x - rhs.x)) + bx::square(float(lhs.y - rhs.y)));
  };

  g[from.y * dim + from.x] = 0;
  f[from.y * dim + from.x] = heuristic(from, to);

  std::vector<GridPos> openList = {from};
  std::vector<GridPos> closedList;

  while (!openList.empty())
  {
    int bestIdx = 0;
    float bestScore = getF(openList[0]);
    for (int i = 1; i < openList.size(); ++i)
    {
      float score = getF(openList[i]);
      if (score < bestScore)
      {
        bestIdx = i;
        bestScore = score;
      }
    }
    if (openList[bestIdx] == to)
      return reconstruct_path(prev, to, dim);
    GridPos curPos = openList[bestIdx];
    openList.erase(openList.begin() + bestIdx);
    if (std::find(closedList.begin(), closedList.end(), curPos) != closedList.end())
      continue;
    closedList.emplace_back(curPos);
    auto checkNeighbour = [&](GridPos p)
    {
      // out of bounds
      if (p.x < 0 || p.y < 0 || p.x >= dim || p.y >= dim)
        return;
      int idx = p.y * dim + p.x;
      // not empty
      if (input[idx] != ' ')
        return;
      float gScore = getG(curPos) + 1; // we're exactly 1 unit away
      if (gScore < getG(p))
      {
        prev[idx] = curPos;
        g[idx] = gScore;
        g[idx] = gScore + heuristic(p, to);
      }
      bool found = std::find(openList.begin(), openList.end(), p) != openList.end();
      if (!found)
        openList.emplace_back(p);
    };
    checkNeighbour({curPos.x + 1, curPos.y + 0});
    checkNeighbour({curPos.x - 1, curPos.y + 0});
    checkNeighbour({curPos.x + 0, curPos.y + 1});
    checkNeighbour({curPos.x + 0, curPos.y - 1});
  }
  // empty path
  return std::vector<GridPos>();
}

void draw_nav_data(DebugDrawEncoder &dde, const char *input, int dim, GridPos from, GridPos to)
{
  draw_nav_grid(dde, input, dim);
  std::vector<GridPos> path = find_path_a_star(input, dim, from, to);
  draw_path(dde, path, input, dim);
}

int main(int argc, const char **argv)
{
  int width = 1920;
  int height = 1080;
  if (!app_init(width, height))
    return 1;
  ddInit();

  bx::Vec3 eye(0.f, 0.f, -16.f);
  bx::Vec3 at(0.f, 0.f, 0.f);
  bx::Vec3 up(0.f, 1.f, 0.f);

  constexpr int dim = 16;
  char navGrid[dim * dim];
  for (int i = 0; i < dim * dim; ++i)
    navGrid[i] = ' ';

  float view[16];
  float proj[16];
  bx::mtxLookAt(view, bx::load<bx::Vec3>(&eye.x), bx::load<bx::Vec3>(&at.x), bx::load<bx::Vec3>(&up.x) );

  GridPos from(0, 0);
  GridPos to(0, 0);

  app_set_on_mouse_click([&](int button, float x, float y)
  {
    // pick pos
    float mtxVp[16];
    bx::mtxMul(mtxVp, view, proj);

    float mtxInvVp[16];
    bx::mtxInverse(mtxInvVp, mtxVp);

    bx::Ray ray = bx::makeRay(
         (float(x)/float(width)  * 2.0f - 1.0f)
      , -(float(y)/float(height) * 2.0f - 1.0f)
      , mtxInvVp
      );
		bx::Hit hit;
    bx::Plane plane(bx::Vec3(0.f, 0.f, -1.f), 0.f);
    if (bx::intersect(ray, plane, &hit))
    {
      GridPos p(hit.pos.x + dim/2 + 0.5f, hit.pos.y + dim/2 + 0.5f);
      if (button == 2)
      {
        int idx = p.y * dim + p.x;
        if (idx >= 0 && idx < dim * dim)
        {
          navGrid[idx] = navGrid[idx] == ' ' ? 'x' : ' ';
        }
      }
      else
      {
        GridPos &target = button == 0 ? from : to;
        target = p;
      }
    }
  });

  while (!app_should_close())
  {
    app_poll_events();
    // Handle window resize.
    app_handle_resize(width, height);
    bx::mtxProj(proj, 60.0f, float(width)/float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);


    // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
    const bgfx::ViewId kClearView = 0;
    bgfx::touch(kClearView);

    DebugDrawEncoder dde;

    dde.begin(0);

    draw_nav_data(dde, navGrid, dim, from, to);

    dde.end();

    // Advance to next frame. Process submitted rendering primitives.
    bgfx::frame();
  }
  ddShutdown();
  bgfx::shutdown();
  app_terminate();
  return 0;
}
