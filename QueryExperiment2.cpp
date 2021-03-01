/*
Author: Marco Marasco 834482.
Description: Query experiment 2 execution file.
Input: N/A.
Output: Query runtime results in csv format.
*/

#include "Generator.h"
#include "RangeTreeFC.h"
#include "RangeTreeOrg.h"

int main(int argc, char const *argv[])
{

    srand(time(0));
    auto start = chrono::high_resolution_clock::now();
    auto stop = chrono::high_resolution_clock::now();

    int tree_sizes[] = {2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000, 1024000};

    // Current workload.
    vector<QueryRange> workload;

    for (int i = 0; i < 100; i++)
    {
        workload.push_back(generate_a_query(50000));
    }

    long double total_time = 0.0;
    cout.precision(10);

    for (int s = 0; s < 10; s++)
    {

        total_time = 0.0;
        cout << tree_sizes[s] << ",";

        vector<Point *> points = generate_point_set(tree_sizes[s]);

        // Sorting completed here to remove duplicate sorting caused by
        // util functions for constructing trees - no need to call util func.
        sort(points.begin(), points.end(), [](const Point &lhs, const Point &rhs) {
            return *lhs.x < *rhs.x;
        });

        vector<Point *> ypoints;
        for (Point *i : points)
        {
            ypoints.push_back(i);
        }

        sort(ypoints.begin(), ypoints.end(), [](const Point &lhs, const Point &rhs) {
            return *lhs.y < *rhs.y;
        });

        // Build trees.
        Node *orgtree = contr_sorted(points, 0, points.size() - 1, ypoints);
        FracNode *fctree = frac_build(points, 0, points.size() - 1, ypoints);

        // Execute experiment.
        for (int i = 0; i < 100; i++)
        {
            start = chrono::high_resolution_clock::now();
            query_2d(workload[i], orgtree).size();
            stop = chrono::high_resolution_clock::now();
            total_time += get_time(start, stop);
        }

        // Print results.
        total_time = total_time / 100;
        cout << fixed << total_time << ",";
        total_time = 0;

        // Execute experiment.
        for (int i = 0; i < 100; i++)
        {

            start = chrono::high_resolution_clock::now();
            frac_query(workload[i], fctree).size();
            stop = chrono::high_resolution_clock::now();
            total_time += get_time(start, stop);
        }

        // Print results.
        total_time = total_time / 100;
        cout << fixed << total_time << endl;
    }

    return 0;
}
