#include "utils.h"

// Function to compute the assignments
std::vector<location> compute_assignments(const std::vector<location> &points)
{
    std::vector<location> assignment = points;
    std::vector<int> capacities(points.size(), 0);

    for (location& u : assignment)
    {
        double min_value = std::numeric_limits<double>::max();

        location* best_location;
        for (location& v : assignment)
        {
            double distance = euclidean_distance(u.x, u.y, v.x, v.y);
            double value = v.f + distance;
            if (value < min_value)
            {
                best_location = &v;
                min_value = value;
            }
        }
        if (!best_location){
            continue;
        }

        best_location->capacity += u.b;
        u.connected_to = best_location->id;
    }

    return assignment;
}