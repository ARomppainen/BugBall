#include "AStar.h"

#include <queue>
#include "HashSet.h"

#include <functional>

std::vector<vec2i> AStar::findPath(vec2i start, vec2i goal, Board& board)
{
    std::vector<vec2i> path;

    HashSet<Node> foundNodes(50);
    std::priority_queue<Node, std::vector<Node>, std::greater<Node> > pq;

    Node gn(goal);
    Node sn(start);
    sn.g = 0;
    sn.h = board.distance(start, goal);

    foundNodes.add(sn);
    pq.push(sn);

    bool pathFound = false;

    if (sn == gn) {
        return path;
    }

    while (!pq.empty()) {

        Node node = pq.top();
        pq.pop();

        if (node == gn) {
            pathFound = true;
            break;
        }

        std::vector<vec2i> neighbors = board.getFreeNeighbors(node.point.x, node.point.y);

        for (unsigned i = 0; i < neighbors.size(); ++i) {

            int distance = node.g + 1;
            Node neighbor(neighbors[i]);

            if ( !foundNodes.contains(neighbor) ) {
                neighbor.setPredecessor(node);
                neighbor.g = distance;
                neighbor.h = board.distance(neighbors[i], goal);
                pq.push(neighbor);
                foundNodes.add(neighbor);
            } else if (foundNodes.find(neighbor)->g > distance) {
                foundNodes.find(neighbor)->g = distance;
                foundNodes.find(neighbor)->setPredecessor(
                    *foundNodes.find(node));
            }
        }
    }

    if (pathFound) {
        Node* n = foundNodes.find(gn);
        path = n->predecessors;
        path.push_back(n->point);
    }

    return path;
}