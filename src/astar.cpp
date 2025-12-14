#include "astar.h"

#include <algorithm>
#include <list>

namespace
{
using namespace astar;

float distance(Node *a, Node *b)
{
    return sqrtf((a->x - b->x) * (a->x - b->x) + (float)((a->y - b->y) * (a->y - b->y)));
}
} // namespace

namespace astar
{
void solve(Input &input)
{
    Node *begin = input.begin;
    Node *finish = input.finish;
    Node *nodes = input.nodeVector;
    int numberOfNodes = input.numberOfNodes;

    for (int index = 0; index < numberOfNodes; index++)
    {
        nodes[index].parent = nullptr;

        nodes[index].wasVisited = false;

        nodes[index].globalGoal = INFINITY;
        nodes[index].localGoal = INFINITY;
    }

    Node *current = begin;

    begin->localGoal = 0.0f;
    begin->globalGoal = distance(begin, finish);

    std::list<Node *> nodesMissingTest;
    nodesMissingTest.push_back(begin);

    while (!nodesMissingTest.empty() && current != finish)
    {
        nodesMissingTest.sort([](const Node *first, const Node *second) {
            return first->globalGoal < second->globalGoal;
        });

        while (!nodesMissingTest.empty() && nodesMissingTest.front()->wasVisited)
        {
            nodesMissingTest.pop_front();
        }

        if (nodesMissingTest.empty())
        {
            break;
        }

        current = nodesMissingTest.front();

        current->wasVisited = true;

        for (auto nodeNeighbour : current->neighbours)
        {
            if (!nodeNeighbour->wasVisited && nodeNeighbour->isObstacle == 0)
            {
                nodesMissingTest.push_back(nodeNeighbour);
            }

            float tryLowerGoal = current->localGoal + distance(current, nodeNeighbour);

            if (tryLowerGoal < nodeNeighbour->localGoal)
            {
                nodeNeighbour->parent = current;
                nodeNeighbour->localGoal = tryLowerGoal;

                nodeNeighbour->globalGoal
                    = nodeNeighbour->localGoal + distance(nodeNeighbour, finish);
            }
        }
    };
}
} // namespace astar
