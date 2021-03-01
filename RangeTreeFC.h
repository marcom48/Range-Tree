/*
Author: Marco Marasco 834482.
Description: Header file for RangeTree-FC.
*/

#include "Generator.h"

/*
Function finds the predecessor of some value in a RangeTree-FC.
*/
FracNode *frac_find_pred(FracNode *root, int b, bool y_dim)
{

    FracNode *curr = root, *ans = NULL;
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
Function finds the successor of some value in a RangeTree-FC.
*/
FracNode *frac_find_succ(FracNode *root, int a, bool y_dim)
{
    FracNode *curr = root, *ans = NULL;
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
Function finds the LCA of some nodes in a RangeTree-FC.
*/
FracNode *frac_find_split(FracNode *root, FracNode *a, FracNode *b, bool y_dim)
{
    if (root == NULL)
        return root;
    if (*root->p->x == *a->p->x || *root->p->x == *b->p->x)
    {
        return root;
    }
    FracNode *l = frac_find_split(root->left, a, b, false);
    FracNode *r = frac_find_split(root->right, a, b, false);
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
Function asserts if a point should be reported for a query range in a RangeTree-FC.
*/
bool report(QueryRange &query, FracNode *FracNode)
{

    return (*FracNode->p->x >= query.x1) && (*FracNode->p->x <= query.x2) && (*FracNode->p->y >= query.y1) && (*FracNode->p->y <= query.y2);
}

/*
Function builds a RangeTree-FC.
*/
FracNode *frac_build(vector<Point *> &points, int start, int end, vector<Point *> &ypoints)
{
    if (start > end)
        return NULL;

    int median = (start + end) / 2;

    if (start == end)
    {
        FracNode *root = new FracNode();
        root->p = points[start];
        root->left = NULL;
        root->right = NULL;

        FracEl *el = new FracEl();
        el->p = points[start];

        vector<FracEl *> arr = {el};
        return root;
    }
    else
    {
        FracNode *root = new FracNode();
        root->arr = {};
        root->p = points[median];

        vector<Point *> left_y, right_y;

        // Build secondary array.
        for (Point *i : ypoints)
        {
            FracEl *el = new FracEl();

            el->p = i;

            el->succ_left = el->succ_right = NULL;
            root->arr.push_back(el);
        }

        int median_x = *points[median]->x;

        int left_succ = 0;
        int right_succ = 0;
        int left = 0;
        int right = 0;

        // Split y points maintaining sorted y order.
        for (int i = 0; i < ypoints.size(); i++)
        {

            if (i == median)
            {
                continue;
            }
            else if (*ypoints[i]->x < median_x)
            {

                left_y.push_back(ypoints[i]);

                while (left_succ < root->arr.size() && *ypoints[left_succ]->y <= *ypoints[i]->y)
                {
                    // Record index of successor in left tree y array.
                    root->arr[left_succ]->succ_left = (int *)malloc(sizeof(int));
                    *root->arr[left_succ]->succ_left = left;
                    left_succ++;
                }
                // Size of left tree y array.
                left++;
            }
            else
            {
                right_y.push_back(ypoints[i]);

                while (right_succ < root->arr.size() && *ypoints[right_succ]->y <= *ypoints[i]->y)
                {
                    // Record index of successor in right tree y array.
                    root->arr[right_succ]->succ_right = (int *)malloc(sizeof(int));
                    *root->arr[right_succ]->succ_right = right;
                    right_succ++;
                }

                right++;
            }
        }

        root->left = frac_build(points, start, median - 1, left_y);
        root->right = frac_build(points, median + 1, end, right_y);

        return root;
    }
}

/*
Function performs preliminary sorting and call of frac_build.
*/
FracNode *frac_build_util(vector<Point *> &points)
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

    // Sort by y coordinate.
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

    return frac_build(points, 0, points.size() - 1, ypoints);
}

/*
Function binary searches a value in a node's secondary y-coordinate array.
*/
FracEl *bin_search(vector<FracEl *> arr, int succ_y)
{
    int low = 0;
    int high = arr.size() - 1;
    int max_limit = high;

    while (low < high)
    {
        int mid = low + (high - low) / 2.0;
        if (*arr[mid]->p->y < succ_y)
        {
            low = mid + 1;
        }
        else
            high = mid;
    }

    return high == max_limit ? NULL : arr[high];
}

/*
Function returns a set of reported poitns from a RangeTree-FC for a given query range.
*/
vector<Point *> frac_query(QueryRange &query, FracNode *root)
{

    vector<Point *> results;

    FracNode *succ_x = frac_find_succ(root, query.x1, false);
    FracNode *pred_x = frac_find_pred(root, query.x2, false);

    if (pred_x == NULL || succ_x == NULL)
        return {};

    FracNode *split = frac_find_split(root, succ_x, pred_x, false);

    FracEl *bin_y = bin_search(split->arr, query.y1);

    if (bin_y == NULL)
    {
        return {};
    }

    if (report(query, split))
    {
        results.push_back(split->p);
    }

    if (split->left == NULL && split->right == NULL)
    {
        return results;
    }

    // Traverse down path
    FracNode *curr = split->left;

    // Pointer to successor of y in traversal node
    FracEl *curr_y = curr->arr[*bin_y->succ_left];

    int smallest_y;

    while (curr != NULL)
    {

        if (report(query, curr))
        {
            results.push_back(curr->p);
        }

        if (*succ_x->p->x <= *curr->p->x)
        {
            // Has right child.
            if (curr->right != NULL && curr_y->succ_right != NULL)
            {

                smallest_y = *curr_y->succ_right;

                // Report elements in array.
                for (int i = smallest_y; i < curr->right->arr.size(); i++)
                {
                    if (*curr->right->arr[i]->p->y <= query.y2)
                    {
                        results.push_back(curr->right->arr[i]->p);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (curr->left != NULL && curr_y->succ_left != NULL)
            {
                if (curr->left->arr.size() <= *curr_y->succ_left)
                    break;
                curr_y = curr->left->arr[*curr_y->succ_left];
                curr = curr->left;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (curr->right != NULL && curr_y->succ_right != NULL)
            {

                if (curr->right->arr.size() <= *curr_y->succ_right)
                    break;
                curr_y = curr->right->arr[*curr_y->succ_right];
                curr = curr->right;
            }
            else
            {
                break;
            }
        }
    }

    curr = split->right;

    curr_y = split->arr[*bin_y->succ_right];

    while (curr != NULL)
    {

        if (report(query, curr))
        {
            results.push_back(curr->p);
        }

        if (*curr->p->x <= *pred_x->p->x)
        {
            if (curr->left != NULL && curr_y->succ_left != NULL)
            {
                smallest_y = *curr_y->succ_left;

                // Report elements in array.
                for (int i = smallest_y; i < curr->left->arr.size(); i++)
                {
                    if (*curr->left->arr[i]->p->y <= query.y2)
                    {
                        results.push_back(curr->left->arr[i]->p);
                    }
                    else
                    {
                        break;
                    }
                }
            }

            if (curr->right != NULL && curr_y->succ_right != NULL)
            {

                if (curr->right->arr.size() <= *curr_y->succ_right)
                    break;

                curr_y = curr->right->arr[*curr_y->succ_right];
                curr = curr->right;
            }

            else
            {
                break;
            }
        }
        else
        {
            if (curr->left != NULL && curr_y->succ_left != NULL)
            {
                if (curr->left->arr.size() <= *curr_y->succ_left)
                    break;
                curr_y = curr->left->arr[*curr_y->succ_left];
                curr = curr->left;
            }
            else
            {
                break;
            }
        }
    }

    return results;
}
