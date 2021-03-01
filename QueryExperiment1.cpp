/*
Author: Marco Marasco 834482.
Description: Query experiment 1 execution file.
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
    cout.precision(10);

    // Experiment workloads.
    int workloads[] = {10000, 20000, 50000, 100000, 200000};

    // Generate point set.
    vector<Point *> points = generate_point_set(M);

    // Current workload.
    vector<QueryRange> workload;

    // Construct Trees
    Node *orgtree = contr_sorted_util(points);
    FracNode *fctree = frac_build_util(points);
    long double total_time = 0.0;

    cout << "Built trees" << endl;
    for (int w = 0; w < 5; w++)
    {

        cout << workloads[w] << ",";

        workload.clear();
        total_time = 0.0;

        // Create 100 queries.
        for (int i = 0; i < 100; i++)
        {
            workload.push_back(generate_a_query(workloads[w]));
        }

        for (int i = 0; i < 100; i++)
        {
            start = chrono::high_resolution_clock::now();
            query_2d(workload[i], orgtree).size();
            stop = chrono::high_resolution_clock::now();
            total_time += get_time(start, stop);
        }

        total_time = total_time / 100;
        cout << total_time << ",";
        total_time = 0.0;

        for (int i = 0; i < 100; i++)
        {

            start = chrono::high_resolution_clock::now();
            frac_query(workload[i], fctree).size();
            stop = chrono::high_resolution_clock::now();
            total_time += get_time(start, stop);
        }
        total_time = total_time / 100;
        cout << total_time << endl;

        total_time = 0.0;
    }

    return 0;
}
