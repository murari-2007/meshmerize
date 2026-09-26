#include "planning/route_builder.h"

#include "robot/orientation.h"

namespace meshmerize {

Direction RouteBuilder::edgeDirection(
    const Graph& graph,
    int from,
    int to
) const
{
    const Edge* edge =
        graph.getEdge(from, to);

    if (edge == nullptr) {
        return Direction::North;
    }

    /*
     * Edge direction is stored from edge.from
     * to edge.to.
     *
     * Graph traversal is undirected, so if we
     * traverse the edge backwards, reverse the
     * stored direction.
     */
    if (edge->from == from) {
        return edge->direction;
    }

    return turnAround(edge->direction);
}

Route RouteBuilder::build(
    const Graph& graph,
    const std::vector<int>& node_path,
    Direction initial_direction
) const
{
    Route route;

    /*
     * No movement is required when the path
     * contains fewer than two nodes.
     */
    if (node_path.size() < 2) {
        return route;
    }

    Direction current_direction =
        initial_direction;

    for (std::size_t i = 0;
         i + 1 < node_path.size();
         ++i) {

        const int from =
            node_path[i];

        const int to =
            node_path[i + 1];

        /*
         * The physical direction required to
         * traverse this graph edge.
         */
        const Direction target_direction =
            edgeDirection(
                graph,
                from,
                to
            );

        /*
         * Convert absolute target direction
         * into a relative turn.
         */
        const RelativeDirection relative =
            [&]() {

                if (target_direction ==
                    current_direction) {

                    return RelativeDirection::Straight;
                }

                if (target_direction ==
                    turnLeft(current_direction)) {

                    return RelativeDirection::Left;
                }

                if (target_direction ==
                    turnRight(current_direction)) {

                    return RelativeDirection::Right;
                }

                return RelativeDirection::Back;
            }();

        /*
         * Add the required turn.
         *
         * Straight does not need a turn action.
         */
        switch (relative) {

        case RelativeDirection::Straight:
            break;

        case RelativeDirection::Left:
            route.actions.push_back(
                Action::Left
            );
            break;

        case RelativeDirection::Right:
            route.actions.push_back(
                Action::Right
            );
            break;

        case RelativeDirection::Back:
            route.actions.push_back(
                Action::UTurn
            );
            break;
        }

        /*
         * After turning, the robot moves through
         * the edge.
         */
        route.actions.push_back(
            Action::Forward
        );

        /*
         * The robot is now facing the direction
         * of the traversed edge.
         */
        current_direction =
            target_direction;
    }

    return route;
}

} // namespace meshmerize
