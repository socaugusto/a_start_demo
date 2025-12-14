#pragma once
#include <vector>

namespace astar
{
struct Node
{
    Node *parent;
    std::vector<Node *> neighbours;

    bool isObstacle = false;
    bool wasVisited = false;

    float globalGoal;
    float localGoal;

    int x;
    int y;
};

struct Input
{
    Node *begin;
    Node *finish;

    Node *nodeVector;

    int numberOfNodes;
};

void solve(Input &);
} // namespace astar
