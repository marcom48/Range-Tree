/*
Author: Marco Marasco 834482.
Description: Header file for RangeTree-Org.
*/

#include "Generator.h"

/*
Function finds the predecessor of some value in a tree.
*/
Node *find_pred(Node *root, int b, bool y_dim)
{

    Node *curr = root, *ans = NULL;
    while (curr)
    {
        if (!y_dim)
        {
            // Less than pred
            if (*curr->p->x <= b)
            {
                ans = curr;
                curr = curr->right;
            }
            else
            {
                // Greater than pred
                curr = curr->left;
            }
        }
        else
        {
            // Less than pred
            if (*curr->p->y <= b)
            {
                ans = curr;
                curr = curr->right;
            }
            else
            {
                // Greater than pred
                curr = curr->left;
            }
        }
    }

    if (ans)
    {
        return ans;
    }
    return NULL;
}

/*
Function finds the successor of some value in a tree.
*/
Node *find_succ(Node *root, int a, bool y_dim)
{
    Node *curr = root, *ans = NULL;
    while (curr)
    {
        if (!y_dim)
        {
            // Greater than successor
            if (*curr->p->x >= a)
            {
                ans = curr;
                curr = curr->left;
            }
            else
            {
                // Less than successor
                curr = curr->right;
            }
        }
        else
        {
            // Greater than successor
            if (*curr->p->y >= a)
            {
                ans = curr;
                curr = curr->left;
            }
            else
            {
                // Less than successor
                curr = curr->right;
            }
        }
    }

    if (ans)
    {
        return ans;
    }

    return NULL;
}

/*
Function finds the LCA of some nodes in a tree.
*/
Node *find_split(Node *root, Node *a, Node *b, bool y_dim)
{
    if (root == NULL)
        return root;
    if (*root->p->x == *a->p->x || *root->p->x == *b->p->x)
    {
        return root;
    }
    Node *l = find_split(root->left, a, b, false);
    Node *r = find_split(root->right, a, b, false);
    if (l != NULL && r != NULL)
        return root;
    if (l)
    {
        return l;
    }
    else
    {
        return r;
    }

    return root;
}

/*
Function asserts if a point should be reported for a query range.
*/
bool report(QueryRange &query, Node *node)
{

    return (*node->p->x >= query.x1) && (*node->p->x <= query.x2) && (*node->p->y >= query.y1) && (*node->p->y <= query.y2);
}

/*
Function builds the y-coordinate tree for the RangeTree-Org.
*/
Node *build_1d_tree(vector<Point *> &points, int start, int end)
{

    if (start > end)
        return NULL;

    // Base case.
    if (start == end)
    {
        Node *root = new Node();
        root->p = points[start];
        root->left = NULL;
        root->right = NULL;
        root->tree = NULL;
        return root;
    }

    int median = (start + end) >> 1;
    Node *root = new Node();

    root->p = points[median];
    root->tree = NULL;

    // Construct children.
    root->left = build_1d_tree(points, start, median - 1);
    root->right = build_1d_tree(points, median + 1, end);

    return root;
}

/*
Function builds a Range Tree using the Contr-Sorted method.
*/
Node *contr_sorted(vector<Point *> &points, int start, int end, vector<Point *> &ypoints)
{

    if (start > end)
        return NULL;

    int median = (start + end) / 2;

    if (start == end)
    {
        Node *root = new Node();
        root->p = points[start];
        root->left = NULL;
        root->right = NULL;

        // Add in singleton y tree
        root->tree = new Node();
        root->tree->p = points[start];
        root->tree->left = NULL;
        root->tree->right = NULL;

        return root;
    }
    else
    {
        Node *root = new Node();
        root->p = points[median];

        vector<Point *> left_y, right_y;

        int median_x = *points[median]->x;

        // Split y points maintaining sorted y order.
        for (int i = 0; i < ypoints.size(); i++)
        {

            if (i == median)
                continue;

            if (*ypoints[i]->x < median_x)
            {
                left_y.push_back(ypoints[i]);
            }
            else
            {
                right_y.push_back(ypoints[i]);
            }
        }

        root->left = contr_sorted(points, start, median - 1, left_y);
        root->right = contr_sorted(points, median + 1, end, right_y);

        // Build y coordinate tree.
        root->tree = build_1d_tree(ypoints, 0, ypoints.size() - 1);

        return root;
    }
}

/*
Function performs preliminary sorting and call of contr_sorted.
*/
Node *contr_sorted_util(vector<Point *> &points)
{
    // Sort points by x coordinate.
    sort(points.begin(), points.end(), [](const Point &lhs, const Point &rhs) {
        if (*lhs.x < *rhs.x)
        {
            return true;
        }
        else if (*lhs.x == *rhs.x && *lhs.y < *rhs.y)
        {
            return true;
        }

        return false;
    });

    vector<Point *> ypoints;

    for (Point *i : points)
    {
        ypoints.push_back(new Point(i));
    }
    sort(ypoints.begin(), ypoints.end(), [](const Point &lhs, const Point &rhs) {
        if (*lhs.y < *rhs.y)
        {
            return true;
        }
        else if (*lhs.y == *rhs.y && *lhs.x < *rhs.x)
        {
            return true;
        }

        return false;
    });

    return contr_sorted(points, 0, points.size() - 1, ypoints);
}

/*
Function builds a Range Tree using the Contr-Naive method.
*/
Node *contr_naive(vector<Point *> &points, int start, int end)
{

    if (start > end)
        return NULL;

    if (start == end)
    {
        Node *root = new Node();

        root->p = points[start];
        root->left = NULL;
        root->right = NULL;

        root->tree = new Node();
        root->tree->p = points[start];
        root->tree->left = NULL;
        root->tree->right = NULL;
        return root;
    }
    else
    {
        Node *root = new Node();
        int median = (start + end) >> 1;
        root->p = points[median];

        root->left = contr_naive(points, start, median - 1);
        root->right = contr_naive(points, median + 1, end);

        // Sort by y coordinate.
        sort(points.begin() + start, points.begin() + end + 1, [](const Point &lhs, const Point &rhs) {
            if (*lhs.y < *rhs.y)
            {
                return true;
            }
            else if (*lhs.y == *rhs.y && *lhs.x < *rhs.x)
            {
                return true;
            }

            return false;
        });

        root->tree = build_1d_tree(points, start, end);

        return root;
    }
}

/*
Function performs preliminary sorting and call of contr_naive.
*/
Node *contr_naive_util(vector<Point *> &points)
{
    // Sort points by x coordinate then y coordinate for total order.
    sort(points.begin(), points.end(), [](const Point &lhs, const Point &rhs) {
        if (*lhs.x < *rhs.x)
        {
            return true;
        }
        else if (*lhs.x == *rhs.x && *lhs.y < *rhs.y)
        {
            return true;
        }

        return false;
    });

    return contr_naive(points, 0, points.size() - 1);
}

/*
Function reports a whole y-coordinate tree.
*/
void report_whole_tree(Node *root, vector<Point *> &results)
{
    if (root == NULL)
        return;

    results.push_back(root->p);
    report_whole_tree(root->left, results);
    report_whole_tree(root->right, results);
}

/*
Function queries over y-coordinate tree.
*/
void query_1d(Node *root, QueryRange &query, vector<Point *> &results)
{

    if (root == NULL)
        return;

    Node *succ_y = find_succ(root, query.y1, true);
    Node *pred_y = find_pred(root, query.y2, true);

    if (pred_y == NULL || succ_y == NULL)
        return;
    Node *split = find_split(root, succ_y, pred_y, true);

    if (report(query, split))
    {
        results.push_back(split->p);
    }

    Node *curr = split->left;

    // Check path to successor.
    while (curr != NULL)
    {
        if (report(query, curr))
        {
            results.push_back(curr->p);
        }

        if (*succ_y->p->y <= *curr->p->y)
        {
            // Report right subtree.
            report_whole_tree(curr->right, results);
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
    }

    curr = split->right;

    while (curr != NULL)
    {
        if (report(query, curr))
        {
            results.push_back(curr->p);
        }

        if (*curr->p->y <= *pred_y->p->y)
        {
            // Report left sub tree.
            report_whole_tree(curr->left, results);
            curr = curr->right;
        }
        else
        {
            curr = curr->left;
        }
    }
}

/*
Function returns report points in a RangeTree-Org for a given range query.
*/
vector<Point *> query_2d(QueryRange &query, Node *root)
{
    // Reported points.
    vector<Point *> results;

    Node *succ_x = find_succ(root, query.x1, false);
    Node *pred_x = find_pred(root, query.x2, false);

    if (pred_x == NULL || succ_x == NULL)
        return {};

    Node *split = find_split(root, succ_x, pred_x, false);

    if (report(query, split))
    {
        results.push_back(split->p);
    }

    if (split->left == NULL && split->right == NULL)
    {
        return results;
    }

    Node *curr = split->left;

    while (curr != NULL)
    {

        if (report(query, curr))
        {
            results.push_back(curr->p);
        }

        if (*succ_x->p->x <= *curr->p->x)
        {
            // 1D query on right child/
            if (curr->right != NULL)
            {
                query_1d(curr->right->tree, query, results);
                curr = curr->left;
            }
            else
            {
                break;
            }
        }
        else
        {
            curr = curr->right;
        }
    }

    curr = split->right;

    while (curr != NULL)
    {

        if (report(query, curr))
        {
            results.push_back(curr->p);
        }

        if (*curr->p->x <= *pred_x->p->x)
        {
            // 1D query on left child.
            if (curr->left != NULL)
            {
                query_1d(curr->left->tree, query, results);
                curr = curr->right;
            }
            else
            {
                break;
            }
        }
        else
        {
            curr = curr->left;
        }
    }

    return results;
}
