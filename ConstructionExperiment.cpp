/*
Author: Marco Marasco 834482.
Description: Construction experiment execution file.
Input: Point set size.
Output: Construction runtime results in csv format.
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

    // Read point set size.
    istringstream iss(argv[1]);
    int n;
    iss >> n;

    // Generate point set.
    vector<Point *> points = generate_point_set(n);

    start = chrono::high_resolution_clock::now();
    Node *naive = contr_naive_util(points);
    stop = chrono::high_resolution_clock::now();
    cout << (stop - start) << ",";

    start = chrono::high_resolution_clock::now();
    Node *sorted = contr_sorted_util(points);
    stop = chrono::high_resolution_clock::now();
    cout << (stop - start) << endl;

    return 0;
}
