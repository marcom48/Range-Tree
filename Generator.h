/*
Author: Marco Marasco 834482.
Description: Header file for Data Generator.
*/

#ifndef G_H
#define G_H

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define M 1000000

#define POINT_MIN 1
#define POINT_MAX 1000000

/*
Structure for a point in 2-dimensional space
*/
struct Point
{

    int *x, *y, *id;
    Point(const Point &p)
    {
        x = p.x;
        y = p.y;
        id = p.id;
    }
    Point(Point *p2)
    {
        x = p2->x;
        y = p2->y;
        id = p2->id;
    }
    Point(int _x, int _y, int _id)
    {
        x = (int *)malloc(sizeof(int));
        *x = _x;
        y = (int *)malloc(sizeof(int));
        *y = _y;
        id = (int *)malloc(sizeof(int));
        *id = _id;
    }
};

/*
Structure for a query range.
*/
struct QueryRange
{
    int x1, x2, y1, y2;
    QueryRange(Point p, int s)
    {
        x1 = *p.x;
        x2 = *p.x + s;
        y1 = *p.y;
        y2 = *p.y + s;
    }
    QueryRange(QueryRange *p2)
    {
        x1 = p2->x1;
        x2 = p2->x2;
        y1 = p2->y1;
        y2 = p2->y2;
    }
};

/*
Structure for a node in RangeTree-Org.
*/
class Node
{
public:
    Point *p;
    Node *left;
    Node *right;
    Node *tree;
};

/*
Structure for an element in the secondary array in RangeTree-FC nodes.
*/
class FracEl
{
public:
    int *succ_left, *succ_right;
    Point *p;
};

/*
Structure for a node in RangeTree-FC.
*/
class FracNode
{
public:
    Point *p;
    vector<FracEl *> arr;
    FracNode *left;
    FracNode *right;
};

/*
Function generates a point as per assignment specification.
*/
Point *generate_a_point(int _min, int _max, int _id)
{

    int x = 1 + rand() % (_max);
    int y = 1 + rand() % (_max);

    return new Point(x, y, _id);
}
/*
Function generates a set of points as per assignment specification.
*/
vector<Point *> generate_point_set(int n)
{
    map<int, map<int, bool>> vals;
    vector<Point *> points;

    for (int i = 0; i < n; i++)
    {
        Point *p = generate_a_point(POINT_MIN, POINT_MAX, i);

        while (vals[*p->x][*p->y] == true)
        {
            p = generate_a_point(POINT_MIN, POINT_MAX, i);
        }

        vals[*p->x][*p->y] = true;

        points.push_back(p);
    }

    return points;
}

/*
Function generates a query as per assignment specification.
*/
QueryRange generate_a_query(int s)
{
    // Arbitrary ID.
    Point *p = generate_a_point(1, M - s, -1);

    return QueryRange(p, s);
}

#endif