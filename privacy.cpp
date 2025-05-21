#include "utils.h"

std::vector<location> private_assignment(const std::vector<location> &points, float eps, float alpha)
{
    std::vector<location> assignment = points;
    std::unordered_map<int, std::unordered_set<int>> reverse_assignment;

    for (location &u : assignment)
    {
        double min_value = std::numeric_limits<double>::max();

        location *best_location;
        for (location &v : assignment)
        {
            double distance = euclidean_distance(u.x, u.y, v.x, v.y);
            double value = v.f + distance;
            if (value < min_value)
            {
                best_location = &v;
                min_value = value;
            }
        }

        if (!best_location)
        {
            continue;
        }

        u.connected_to = best_location->id;
        reverse_assignment[best_location->id].insert(u.id);
    }

    for (const auto &[id, connected_locations] : reverse_assignment)
    {
        location *v = &assignment[id];

        for (const int u : connected_locations)
        {
            v->capacity += points[u].b_noisy;
        }

        v->capacity += 2 / eps * std::sqrt(connected_locations.size()) * std::log(2 * points.size() / alpha);
    }

    return assignment;
}

// Function to compute Maximal Independent Set (MIS) using a greedy approach
std::vector<location> computeMISwithAssignmentRecalculation(const std::map<int, std::unordered_set<int>> &graph,
                                                            const std::vector<int> marked_nodes,
                                                            const std::vector<location> assignment,
                                                            std::unordered_map<int, std::unordered_set<int>> reverse_assignment,
                                                            double delta)
{
    std::set<int> mis;
    std::unordered_set<int> visited;
    std::vector<location> updated_assignment = assignment;

    // std::cout << "Start computing maximal indpendent set. Size of marked nodes: " << marked_nodes.size() << std::endl;
    for (const int node : marked_nodes)
    {
        // std::cout << node << std::endl;
        if (visited.find(node) == visited.end())
        {
            // std::cout << "Add " << node << " to MIS." << std::endl;
            mis.insert(node);
            updated_assignment[node].delta = delta;
            visited.insert(node);

            for (int neighbor : graph.at(node))
            {
                visited.insert(neighbor);
            }
        }
    }

    // std::cout << "Start reconnection in delta ball..." << std::endl; 
    std::vector<bool> was_reconnected = std::vector<bool>(assignment.size(), false);

    // Reconnect all location in the delta ball around a location in mis
    for (int v_id : mis)
    {
        location v = updated_assignment[v_id];
        // std::cout << v_id << " is in MIS." << std::endl;
        was_reconnected[v_id] = true;
        for (location u : updated_assignment)
        {
            // std::cout << u.id << std::endl;
            double distance = euclidean_distance(v.x, v.y, u.x, u.y);
            if (distance <= delta)
            {
                was_reconnected[u.id] = true;
                updated_assignment[u.id].connected_to = v_id;
            }
        }
    }

    // std::cout << "Start reconnection of locations outside any delta ball..." << std::endl;
    // Recompute the optimal assignment for all nodes outside any ball restricted to the facilities in maximal independent set
    for (location v : updated_assignment)
    {
        if (was_reconnected[v.id])
        {
            // std::cout << v.id << " was already reconnected to " << v.connected_to << std::endl;
            continue;
        }

        double min_value = std::numeric_limits<double>::max();

        location best_location;
        for (int u_id : mis)
        {
            location u = updated_assignment[u_id];
            double distance = euclidean_distance(u.x, u.y, v.x, v.y);
            double value = u.f + distance;
            if (value < min_value)
            {
                best_location = u;
                min_value = value;
            }
        }

        // std::cout << v.id << " is reconnected to " << best_location.id << std::endl;
        updated_assignment[v.id].connected_to = best_location.id;
    }

    return updated_assignment;
}

// Function to compute Maximal Independent Set (MIS) using a greedy approach
std::vector<location> computeMIS(const std::map<int, std::unordered_set<int>> &graph,
                                 const std::vector<int> marked_nodes,
                                 const std::vector<location> assignment,
                                 std::unordered_map<int, std::unordered_set<int>> reverse_assignment,
                                 double delta)
{
    std::set<int> mis;
    std::unordered_set<int> visited;
    std::vector<location> updated_assignment = assignment;

    // std::cout << "Start computing maximal indpendent set." << std::endl;
    for (const int node : marked_nodes)
    {
        // std::cout << node << std::endl;
        if (visited.find(node) == visited.end())
        {
            // std::cout << "Add " << node << " to MIS." << std::endl;
            // Add the node to the MIS
            mis.insert(node);
            updated_assignment[node].delta = delta;
            visited.insert(node);

            for (int neighbor : graph.at(node))
            {
                if (visited.find(neighbor) == visited.end()) // Check if the neighbor is not already visited
                {
                    // If neighbor was previously visited it is already reconnected
                    // only reconnect not yet visited neighbors
                    visited.insert(neighbor);
                    // std::cout << "Mark " << neighbor << " as visited." << std::endl;

                    // Reconnect nodes of neighbors
                    for (int w : reverse_assignment[neighbor])
                    {
                        // std::cout << w << " is reconnected to " << node << std::endl;
                        updated_assignment[w].connected_to = node;
                        // std::cout << updated_assignment[w] << std::endl;
                    }
                }
            }
        }
    }

    // Reconnect all location in the delta ball around a location in mis
    for (auto it1 = mis.begin(); it1 != mis.end(); ++it1)
    {
        location v = updated_assignment[*it1];
        for (location u : assignment)
        {
            double distance = euclidean_distance(v.x, v.y, u.x, u.y);
            if (distance <= delta)
            {
                updated_assignment[u.id].connected_to = v.id;
            }
        }
    }

    return updated_assignment;
}

std::vector<location> reconnect(std::vector<location> points, double delta, std::unordered_map<int, std::unordered_set<int>> reverse_assignment)
{
    std::set<int> marked_nodes;
    for (const auto &pair : reverse_assignment)
    {
        // std::cout << "Insert " << pair.first << " into marked nodes list." << std::endl;
        marked_nodes.insert(pair.first);
    }

    std::map<int, std::unordered_set<int>> graph;
    // Build the graph with ordered nodes
    for (int v_id : marked_nodes)
    {
        location v = points[v_id];
        graph[v.id].insert(v.id);
        for (int u_id : marked_nodes)
        {
            if (u_id == v_id)
            {
                continue;
            }
            location u = points[u_id];

            double dis_v_u = euclidean_distance(v.x, v.y, u.x, u.y);
            // std::cout << "Distance between " << v_id << " " << u_id << ": " << dis_v_u << std::endl;
            if (dis_v_u <= 2 * delta)
            {
                graph[v.id].insert(u.id);
                graph[u.id].insert(v.id);
            }
        }
    }

    std::vector<int> ordered_nodes;
    for (const auto &[node, neighbors] : graph)
    {
        ordered_nodes.push_back(node);
    }

    // Sort the nodes based on the comparator
    std::sort(ordered_nodes.begin(), ordered_nodes.end(), [&points](int lhs, int rhs)
              {
        if (points[lhs].f != points[rhs].f) {
            return points[lhs].f < points[rhs].f;
        }
        return lhs < rhs; });

    // return computeMIS(graph, ordered_nodes, points, reverse_assignment, delta);
    return computeMISwithAssignmentRecalculation(graph, ordered_nodes, points, reverse_assignment, delta);
}

void compute_opt_assignment(std::vector<location> &points, std::unordered_map<int, std::unordered_set<int>> reverse_assignment)
{
    for (location &u : points)
    {
        double min_value = std::numeric_limits<double>::max();

        location *best_location;
        for (location &v : points)
        {
            double distance = euclidean_distance(u.x, u.y, v.x, v.y);
            double value = v.f + distance;
            if (value < min_value)
            {
                best_location = &v;
                min_value = value;
            }
        }

        if (!best_location)
        {
            continue;
        }

        u.connected_to = best_location->id;
        reverse_assignment[best_location->id].insert(u.id);
    }
}

std::vector<location> private_reconnection_assignment(const std::vector<location> &points, double eps, double alpha, double delta)
{
    std::vector<location> assignment = points;
    std::unordered_map<int, std::unordered_set<int>> reverse_assignment;

    for (location &u : assignment)
    {
        double min_value = std::numeric_limits<double>::max();

        location *best_location;
        for (location &v : assignment)
        {
            double distance = euclidean_distance(u.x, u.y, v.x, v.y);
            double value = v.f + distance;
            if (value < min_value)
            {
                best_location = &v;
                min_value = value;
            }
        }

        if (!best_location)
        {
            continue;
        }

        u.connected_to = best_location->id;
        reverse_assignment[best_location->id].insert(u.id);
    }

    // perform reconnection step
    auto reconnected_assignment = reconnect(assignment, delta, reverse_assignment);

    std::unordered_map<int, std::unordered_set<int>> reconnected_reverse_assignment;
    for (location point : reconnected_assignment)
    {
        reconnected_reverse_assignment[point.connected_to].insert(point.id);
        // std::cout << point.connected_to << " reverse connected to " << point.id << std::endl;
    }

    for (const auto &[id, connected_locations] : reconnected_reverse_assignment)
    {
        location *v = &reconnected_assignment[id];

        // std::cout << v->id << " has " << v->capacity << " connected clients." << std::endl;

        for (const int u : connected_locations)
        {
            v->capacity += points[u].b_noisy;
        }

        // std::cout << v->id << " has " << v->capacity << " connected clients." << std::endl;
        // double additional_capacity = 2 / eps * std::sqrt(connected_locations.size()) * std::log(2 * points.size() / alpha);
        // std::cout << "additional capacity: " << additional_capacity << ", delta: " << v->delta << std::endl;
        v->capacity += 2 / eps * std::sqrt(connected_locations.size()) * std::log(2 * points.size() / alpha);
    }

    return reconnected_assignment;
}