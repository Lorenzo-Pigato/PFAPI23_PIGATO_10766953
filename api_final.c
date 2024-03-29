#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 20
#define INPUT_SIZE 2

/////////////////// Types definition ////////////////

typedef enum { false, true } bool;

typedef struct rb_node
{
    // Data section

    int key;
    int range;
    int copies;

    // Attributes

    struct rb_node *left_child;
    struct rb_node *right_child;
    struct rb_node *parent;

    struct rb_node *cars;
} node_t;

typedef node_t *node;                                               // Definition of a node as a pointer

typedef struct path_s
{
    node station;

    int clicks;                                                     // Distance from origin
    struct path_s *link;                                            // Best connection for path

    struct path_s *next;
} path_node;

/////////////////////////////////////////////////////

/////////////////// Functions ///////////////////////

node find_node(node root, int key)
{
    if (root == NULL)
        return NULL;

    else
    {
        while (root != NULL && root->key != key)
        {
            if (root->key > key)
                root = root->left_child;
            else
                root = root->right_child;
        }

        return root;
    }
}

node prev_node(node root, int key)
{
    node target = find_node(root, key);                             // Find target node

    if (target == NULL)
        return NULL;                                                // No nodes found

    if (target->left_child != NULL)                                 // Previous node is gratest of left subtree
    {
        target = target->left_child;

        while (target->right_child != NULL)
            target = target->right_child;

        return target;
    }

    while (target != root)                                          // No left subtree, upper nodes until root
    {
        target = target->parent;
        if (target->key < key)
            return target;
    }

    return NULL;
}

node next_node(node root, int key)
{
    node target = find_node(root, key);

    if (target == NULL)
        return NULL;                                                // Node not found

    if (target->right_child != NULL)                                // Next node is smallest of right subtree
    {
        target = target->right_child;

        while (target->left_child != NULL)
            target = target->left_child;

        return target;
    }

    while (target != root)                                          // No right subtree, stop searching when arrived to root
    {
        target = target->parent;
        if (target->key > key)
            return target;
    }

    return NULL;                                                    // No next node found
}

node delete_tree(node root)
{
    if (root == NULL)
        return NULL;

    else
    {
        root->left_child = delete_tree(root->left_child);
        root->right_child = delete_tree(root->right_child);

        root->cars = delete_tree(root->cars);

        free(root);                                                 // Free root
    }

    return NULL;
}

node insert_node(node *root, int key, bool is_car)
{
    // Every new node inserted starts as red except for root

    node parent = NULL;
    node *tmp;
    tmp = root;

    while (*tmp != NULL)
    {
        parent = *tmp;

        if (key > (*tmp)->key)
            tmp = &((*tmp)->right_child);

        else if (key < (*tmp)->key)
            tmp = &((*tmp)->left_child);

        else if (is_car == true)                                    // If the node is a car and node is already present, increase copies value
        {
            (*tmp)->copies += 1;
            return (*tmp);
        }

        else
            return NULL;
    }

    node new_node = (node_t *)malloc(sizeof(node_t));

    new_node->key = key;
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    new_node->cars = NULL;
    new_node->range = 0;
    new_node->copies = 0;

    if ((*tmp) == (*root))
    {
        new_node->parent = NULL;
        *root = new_node;
    }

    else
    {
        new_node->parent = parent;
        *tmp = new_node;
    }

    return new_node;
}

bool remove_node(node *root, int key)
{
    node target = find_node(*root, key);

    int key_copy;
    int range_copy;
    int copies_copy;
    node cars_ptr_copy = NULL;

    if (target == NULL)                                             // Target node not found
        return false;

    if (target->copies > 0)                                         // If there are copies, decrement
    {
        target->copies -= 1;
        return true;
    }

    // Deleting a leaf-node [1]

    if (target->left_child == NULL && target->right_child == NULL)
    {
        if (target == *root)                                        // Target is root
        {
            target->cars = delete_tree(target->cars);

            free(target);
            *root = NULL;
            return true;
        }

        else                                                        // Target is not root
        {
            if (target->parent->left_child == target)               // Target is a left child
                target->parent->left_child = NULL;                  // Resetting parent's child pointer

            else                                                    // Target is a right child
                target->parent->right_child = NULL;
        }

        target->cars = delete_tree(target->cars);

        free(target);

        return true;
    }

    // Deleting a central node

    node next = next_node(target, key);                             // Finde next element only inside subtree

    if (next == NULL)                                               // No next element found (left subtree only) [3]
        next = prev_node(target, key);

    key_copy = next->key;
    range_copy = next->range;
    copies_copy = next->copies;
    cars_ptr_copy = next->cars;

    if (target->cars)
        next->cars = target->cars;                                  // Exchange car park, so can be deleted when a leaf is found

    remove_node(root, next->key);

    target->key = key_copy;                                         // Substituting target node's infos with next node's infos (deleted, so copied)
    target->range = range_copy;
    target->cars = cars_ptr_copy;
    target->copies = copies_copy;

    return true;
}

void find_path(path_node *list)
{
    path_node *cur = list;

    while (cur->next != NULL)
    {
        path_node *next = cur->next;
        int dist = (cur->station->key) - (next->station->key);

        while (next != NULL && dist * (dist > 0 ? 1 : -1) <= cur->station->range)
        {

            if (next->link == NULL)                                 // If there is no link
            {
                next->link = cur;                                   // Add new link
                next->clicks = cur->clicks + 1;
            }
            else if (next->clicks > (cur->clicks + 1))              // If in-range station's clicks are more than current station's clicks + 1 (new step)
            {
                next->link = cur;                                   // Update link
                next->clicks = cur->clicks + 1;                     // Update clicks
            }
            else if (next->clicks >= (cur->clicks + 1) && (list->station->key > list->next->station->key))
            {
                next->link = cur;                                   // Update link
            }

            next = next->next;

            if (next != NULL)
                dist = (next->station->key) - (cur->station->key);
        }

        cur = cur->next;
    }

    int path_size = cur->clicks + 1;                                // Path size must be the number of clicks of destination node
    int path[path_size];

    path_node tmp = *cur;

    for (int i = path_size - 1; i >= 0; i--)
    {
        path[i] = tmp.station->key;
        if (tmp.link)
            tmp = *(tmp.link);
    }

    if (path[0] == list[0].station->key)
    {
        for (int i = 0; i < path_size - 1; i++)
            printf("%d ", path[i]);

        printf("%d\n", path[path_size - 1]);                        // For matching whitespaces
    }

    else
        printf("nessun percorso\n");
}

/////////////////////////////////////////////////////

int main()
{
    char buffer[BUFF_SIZE];
    int input[INPUT_SIZE];

    node stations = NULL;

    node tmp_node;
    int tmp_int;
    bool tmp_bool;

    node recent = NULL;

    int i = 0;

    while (scanf(" %s", buffer) == 1 && !feof(stdin))
    {   // Data collected, no EOF
        switch (buffer[0])
        {
        case 'a':

            i = 0;
            while (buffer[i] != '-')
                i++;

            //////////////// Add a station ////////////////

            if (buffer[i + 1] == 's')
            {

                if (scanf(" %d %d", &input[0], &input[1]))
                {
                    tmp_node = insert_node(&stations, input[0], false);

                    if (tmp_node && input[1] != 0)
                    {
                        for (i = 0; i < input[1]; i++)
                        {
                            if (!scanf("%d", &tmp_int))
                                return 1;
                            else
                            {
                                insert_node(&(tmp_node->cars), tmp_int, true);

                                if (tmp_int > tmp_node->range)      // If new car has greater range
                                    tmp_node->range = tmp_int;
                            }
                        }
                        printf("aggiunta\n");
                    }
                    else if (tmp_node && input[1] == 0)
                    {
                        printf("aggiunta\n");
                        recent = tmp_node;
                    }
                    else if (!tmp_node && input[1] != 0)
                    {
                        for (i = 0; i < input[1]; i++)
                            if (!scanf("%d", &tmp_int))
                                return 1;

                        printf("non aggiunta\n");
                    }
                }
                else
                    return 1;
            }

            ////////////////// Add a car //////////////////

            else if (buffer[i + 1] == 'a')
            {
                if (!scanf(" %d %d", &input[0], &input[1]))
                    return 1;
                
                if(recent && recent->key == input[0])
                    tmp_node = recent;
                else
                    tmp_node = find_node(stations, input[0]);

                if (tmp_node)
                {
                    insert_node(&(tmp_node->cars), input[1], true);
                    printf("aggiunta\n");

                    if (input[1] > tmp_node->range)                 // If new car has greater range
                        tmp_node->range = input[1];
                }
                else
                    printf("non aggiunta\n");
            }
            break;

        ///////// Remove a station /////////

        case 'd':

            if (scanf(" %d", &input[0]))
            {
                tmp_bool = remove_node(&stations, input[0]);

                if (tmp_bool == true)
                    printf("demolita\n");

                else
                    printf("non demolita\n");
            }
            break;

        /////////// Remove a car ///////////
        
        case 'r':
            if (scanf(" %d %d", &input[0], &input[1]))
            {
                tmp_node = find_node(stations, input[0]);

                if (tmp_node)
                {
                    node tmp_car = find_node(tmp_node->cars, input[1]);

                    if (tmp_car && tmp_car->copies == 0 && tmp_node->range == tmp_car->key)
                    {
                        tmp_node->range = prev_node(tmp_node->cars, tmp_car->key) != NULL ? prev_node(tmp_node->cars, tmp_car->key)->key : 0;
                    }

                    tmp_bool = remove_node(&(tmp_node->cars), input[1]);

                    if (tmp_bool == true)
                        printf("rottamata\n");
                    else
                        printf("non rottamata\n");
                }
                else
                    printf("non rottamata\n");
            }
            break;

        case 'p':
            if (scanf(" %d %d", &input[0], &input[1]))
            {
                node start = find_node(stations, input[0]);
                node stop = find_node(stations, input[1]);
                node next = start;

                if (!start || !stop)
                    exit(1);

                if(start->key == stop->key)
                {
                    printf("%d\n", start->key);
                    break;
                }

                if(start->key < stop->key)
                {
                    if(next_node(stations, start->key))
                    {
                        if (next_node(stations, start->key)->key - start->key > start->range)
                        {
                            printf("nessun percorso\n");
                            break;
                        }   
                    }
                }

                if (start->key > stop->key)
                {
                    if (prev_node(stations, start->key))
                    {
                        if (start->key - prev_node(stations, start->key)->key > start->range)
                        {
                            printf("nessun percorso\n");
                            break;
                        }
                    }
                }

                path_node *list = (path_node *)malloc(sizeof(path_node));
                list->station = start;
                list->clicks = 0;
                list->link = NULL;

                path_node *temp = list;

                while (next != stop)
                {
                    next = start->key < stop->key ? next_node(stations, next->key) : prev_node(stations, next->key);
                    temp->next = (path_node *)malloc(sizeof(path_node));

                    temp = temp->next;

                    temp->station = next;
                    temp->clicks = 0;
                    temp->link = NULL;
                }

                temp->next = NULL;

                find_path(list);

                while (list != NULL)
                {
                    temp = list->next;
                    free(list);
                    list = temp;
                }
            }
            break;

        default:
            return 1;
        }
    }

    stations = delete_tree(stations);

    return 0;
}
