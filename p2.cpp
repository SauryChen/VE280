#include <iostream>
#include "p2.h"
#include "recursive.h"

using namespace std;

int size(list_t list)
// EFFECTS: Returns the number of elements in "list".
//          Returns zero if "list" is empty.
{
    if (list_isEmpty(list))
        return 0;
    else
        return (1+size(list_rest(list)));
}

bool memberOf(list_t list, int val)
// EFFECTS: Return true if the value "val" appears in "list".
//          Returns false otherwise.
{
    if (list_isEmpty(list))
        return false;
    else if (list_first(list) == val)
        return true;
    else
        return (memberOf(list_rest(list),val));
}

int dot(list_t v1, list_t v2)
// REQUIRES:Both "v1" and "v2" are non-empty
// EFFECTS: Treats both lists as vectors.
//          Return the dot product of the two vectors.
//          If one list is longer than the other, ignore the longer part of the vector.
{
    if (list_isEmpty(list_rest(v1)) || list_isEmpty(list_rest(v2)))
        return list_first(v1)*list_first(v2);
    else
        return (list_first(v1)*list_first(v2) + dot(list_rest(v1),list_rest(v2)));
}

bool isIncreasing(list_t v)
// EFFECTS: Check if the list of integers is increasing.
//          A list is increasing iff no element is smaller than its previous element.
//          By default, an empty list and a list of a single element are increasing.
{
    if (list_isEmpty(v) || list_isEmpty(list_rest(v)))
        return true;
    else
    {
        if (list_first(v) <= list_first(list_rest(v)))
            return isIncreasing(list_rest(v));
        else
            return false;
    }
}


list_t reverse(list_t list)
// EFFECTS: Return the reverse of "list".
// For example: the reverse of ( 3 2 1 ) is ( 1 2 3 ).
{
    if (list_isEmpty(list))
        return list_make();
    else
        return append(reverse(append(list_rest(list),list_make())),list_make(list_first(list),list_make()));
}



list_t append(list_t first, list_t second)
// EFFECTS: Returns the list (fist second).
{
    if (list_isEmpty(first))
        return second;
    else
        return (list_make(list_first(first),append(list_rest(first),second)));
}


bool isArithmeticSequence(list_t v)
// EFFECTS: Checks if the list of integers forms an arithmetic sequence.
//          By default, an empty list and a list of a single element are arithmetic sequences.
{
    if (list_isEmpty(v) || list_isEmpty(list_rest(v)) || list_isEmpty(list_rest(list_rest(v))))
        return true;
    else if (list_first(v) - list_first(list_rest(v)) == list_first(list_rest(v)) - list_first(list_rest(list_rest(v))))
        return isArithmeticSequence(list_rest(v));
    else
        return false;
}


list_t filter_odd(list_t list)
// EFFECTS: Return a new list containing only the elements of the original "list" which are odd in value,
//          in the order in which they appeared in list.
{
    if (list_isEmpty(list) == false)
    {
        if (list_first(list) % 2 != 0)
            return list_make(list_first(list), filter_odd(list_rest(list)));
        else
            return filter_odd(list_rest(list));
    }
    else
        return list;

}


list_t filter(list_t list, bool (*fn)(int))
// EFFECTS: Returns a list containing precisely the elements of "list"
//          for which the predicate fn() evaluates to true
//          in the order in which they appeared in list.
{
    if (list_isEmpty(list))
        return list_make();
    else
    {
        int first = list_first(list);
        list_t rest = list_rest(list);
        if (fn(first))
            return list_make(first,filter(rest,fn));
        else
            return filter(rest,fn);
    }
}



static list_t unique_helper (list_t list, list_t unique_list)
// EFFECTS: Returns a new list comprising of each unique element in "list".
//          The order is determined by the first occurrence of each unique element in "list".
{
    if (list_isEmpty(list))
        return unique_list;
    else
        if (memberOf(unique_list,list_first(list)))
            return unique_helper(list_rest(list),unique_list);
        else
        {
            unique_list = list_make(list_first(list), unique_list);
            return unique_helper(list_rest(list), unique_list);
        }
}

list_t unique(list_t list)
// EFFECTS: Returns a new list comprising of each unique element in "list".
//          The order is determined by the first occurrence of each unique element in "list".
{
    list_t unique_list = list_make();
    unique_list = unique_helper(list, unique_list);
    return reverse(unique_list);
}


list_t insert_list(list_t first, list_t second, unsigned int n)
// REQUIRES: n <= the number of elements in "first".
// EFFECTS: Returns a list comprising the first n elements of "first",
//          followed by any remaining elements of "first".
{
    if (list_isEmpty(second))
        return first;
    else
    {
        if (n > 0)
        {
            return list_make(list_first(first), insert_list(list_rest(first), second, n - 1));
        }
        else
        {
            return list_make(list_first(second), insert_list(first, list_rest(second), 0));
        }
    }
}

static list_t chop_helper(list_t list, unsigned int n)
// EFFECTS: pass in the reversed list and return the last (length()-n) term.
{
    if (n > 0)
    {
        list_t rest_list = list_rest(list);
        return (chop_helper(rest_list, n-1));
    }
    else
        return list;
}

list_t chop(list_t list, unsigned int n)
// REQUIRES: "list" has at least n elements.
// EFFECTS: Returns the list equal to "list" without its last n elements.
{
    list_t list_reversed = reverse(list);
    list = chop_helper(list_reversed, n);
    return reverse(list);
}


// Below are the trees

int tree_sum(tree_t tree)
// EFFECTS: Returns the sum of all elements in "tree".
//          Returns zero if "tree" is empty.
{
    if (tree_isEmpty(tree))
    {
        return 0;
    }
    else
    {
        return (tree_elt(tree)+tree_sum(tree_left(tree))+tree_sum(tree_right(tree)));
    }
}

bool tree_search (tree_t tree, int val)
// EFFECTS: Returns true if the value "val" appears in "tree".
//          Returns false otherwise.
{
    if (tree_isEmpty(tree))
        return false;
    else if (tree_elt(tree) == val)
        return true;
    else
        return (tree_search(tree_left(tree),val) || tree_search(tree_right(tree),val));
}


static int depth_helper(tree_t tree)
// EFFECTS: Returns the sum of depth of left tree and right tree.
{
    if (tree_isEmpty(tree))
        return 0;
    else
        return (1 + depth_helper(tree_left(tree)) + depth_helper(tree_right(tree)));
}

int depth(tree_t tree)
// EFFECTS: Returns the depth of "tree", which equals the number of layers of nodes in the trees.
//          Returns zero if "tree" is empty.
{
    int total_sum = depth_helper(tree);
    if (depth_helper(tree_left(tree)) >= depth_helper(tree_right(tree)))
    {
        total_sum -= depth_helper(tree_right(tree));
    }
    else
    {
        total_sum -= depth_helper(tree_left(tree));
    }
    return total_sum;
}

static int tree_max_helper(list_t list, int max)
// EFFECTS: Returns the max in a list.
{
    if (list_isEmpty(list))
        return max;
    else
        if (list_first(list) > max)
        {
            max = list_first(list);
            return tree_max_helper(list_rest(list),max);
        }
        else
            return tree_max_helper(list_rest(list),max);
}

int tree_max(tree_t tree)
// REQUIRES: "tree" is non-empty.
// EFFECTS: Returns the largest element in "tree".
{
    list_t list_tree = traversal(tree);
    int max = list_first(list_tree);
    return tree_max_helper(list_tree,max);
}



list_t traversal(tree_t tree)
// EFFECTS: Returns the elements of "tree" in a list using an in-order traversal.
//          An in-order traversal prints the "left most" element first,
//          then the second-left-most, and so on, until the right most element is printed.
{
    if (tree_isEmpty(tree))
        return list_make();
    else
        return append(traversal(tree_left(tree)), list_make(tree_elt(tree), traversal(tree_right(tree))));
}



static bool tree_hasMonotonicPath_increase_helper(tree_t tree)
// EFFECTS: Returns true iff the tree has at least one root-to-leaf path that is increasing.
{
    bool left = true;
    bool right = true;
    if (tree_isEmpty(tree))
        return false;
    else if ((tree_isEmpty(tree_left(tree))) && tree_isEmpty(tree_right(tree)))
        return true;
    else
    {
        if (!tree_isEmpty(tree_left(tree)) && tree_elt(tree) <= tree_elt(tree_left(tree)))
            left = (tree_hasMonotonicPath_increase_helper(tree_left(tree)));
        else if (!tree_isEmpty(tree_right(tree)) && tree_elt(tree) <= tree_elt(tree_right(tree)))
            right = (tree_hasMonotonicPath_increase_helper(tree_right(tree)));
        else if (tree_isEmpty(tree_left(tree)) && tree_elt(tree) > tree_elt(tree_right(tree)))
            return false;
        else if (tree_isEmpty(tree_right(tree)) && tree_elt(tree) > tree_elt(tree_left(tree)))
            return false;
        else if (!tree_isEmpty(tree_right(tree)) && !tree_isEmpty(tree_left(tree)) && tree_elt(tree) > tree_elt(tree_left(tree)) && tree_elt(tree_right(tree)))
            return false;
    }
        return (left || right);
}

static bool tree_hasMonotonicPath_decrease_helper(tree_t tree)
// EFFECTS: Returns true iff the tree has at least one root-to-leaf path that is decreasing.
{
    bool left = true;
    bool right = true;
    if (tree_isEmpty(tree))
        return false;
    else if ((tree_isEmpty(tree_left(tree))) && tree_isEmpty(tree_right(tree)))
        return true;
    else
    {
        if (!tree_isEmpty(tree_left(tree)) && tree_elt(tree) >= tree_elt(tree_left(tree)))
            left = (tree_hasMonotonicPath_decrease_helper(tree_left(tree)));
        else if (!tree_isEmpty(tree_right(tree)) && tree_elt(tree) >= tree_elt(tree_right(tree)))
            right = (tree_hasMonotonicPath_decrease_helper(tree_right(tree)));
        else if (tree_isEmpty(tree_left(tree)) && tree_elt(tree) < tree_elt(tree_right(tree)))
            return false;
        else if (tree_isEmpty(tree_right(tree)) && tree_elt(tree) < tree_elt(tree_left(tree)))
            return false;
        else if (!tree_isEmpty(tree_right(tree)) && !tree_isEmpty(tree_left(tree)) && tree_elt(tree) < tree_elt(tree_left(tree)) && tree_elt(tree) < tree_elt(tree_right(tree)))
            return false;
    }
        return (left || right);
}

bool tree_hasMonotonicPath(tree_t tree)
// EFFECTS: Returns true iff "tree" has at least one root-to-leaf path s.t.
//          all the elements along the path form a monotonically increasing or decreasing sequence.
{
    return (tree_hasMonotonicPath_increase_helper(tree) || tree_hasMonotonicPath_decrease_helper(tree));
}




bool tree_allPathSumGreater (tree_t tree, int sum)
// REQUIRES: tree is not empty
// EFFECTS: Returns true iff for each root-to-leaf path of "tree",
//          the sum of all elements along the path is greater than "sum".
{
    if ((!tree_isEmpty(tree_left(tree))) && (!tree_isEmpty(tree_right(tree))))
    {
        return (tree_allPathSumGreater(tree_left(tree), sum - tree_elt(tree)) && tree_allPathSumGreater(tree_right(tree), sum - tree_elt(tree)));
    }
    else if (tree_isEmpty(tree_left(tree)) || tree_isEmpty(tree_right(tree)))
    {
        int value = tree_elt(tree) - sum;
        if (value > 0)
            return true;
        else
            return false;
    }
    else
        return false;
}



bool covered_by(tree_t A, tree_t B)
// EFFECTS: Returns true if tree A is covered by tree B.
{
    if (tree_isEmpty(A))
        return true;
    else if (tree_isEmpty(A) && tree_isEmpty(B))
        return true;
    else if (!tree_isEmpty(A) && tree_isEmpty(B))
        return false;
    else if ((tree_elt(A) ==tree_elt(B)) && covered_by(tree_left(A),tree_left(B)) && covered_by(tree_right(A),tree_right(B)))
        return true;
    else
        return false;
}

bool contained_by(tree_t A, tree_t B)
// EFFECTS: Returns true if tree A is covered by tree B or a subtree of B.
{
    if (covered_by(A,B))
    {
        return true;
    }
    else if (!tree_isEmpty(B))
    {
        return (contained_by(A,tree_left(B)) || contained_by(A,tree_right(B)));
    }
    else if (tree_isEmpty(B))
    {
        return false;
    }
    else
    {
        return false;
    }
}


tree_t insert_tree(int elt, tree_t tree)
// REQUIRES: "tree" is a sorted binary tree.
//
// EFFECTS: Returns a new tree with elt inserted at a leaf such that
//          the resulting tree is also a sorted binary tree.
{
    if (tree_isEmpty(tree))
        return tree_make(elt, tree_make(), tree_make());
    else if (tree_isEmpty(tree_right(tree)) && tree_isEmpty(tree_left(tree)))
    {
        tree_t insert = tree_make(elt, tree_make(), tree_make());
        if (elt < tree_elt(tree))
            return tree_make(tree_elt(tree), insert, tree_make());
        else
            return tree_make(tree_elt(tree), tree_make(), insert);
    }
    else
    {
        if (elt < tree_elt(tree))
            return tree_make(tree_elt(tree), insert_tree(elt,tree_left(tree)), tree_right(tree));
        else
            return tree_make(tree_elt(tree), tree_left(tree),insert_tree(elt,tree_right(tree)));
    }
}



