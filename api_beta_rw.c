#include <stdio.h>
#include <stdlib.h>

/////////////////// Types definition ////////////////

typedef enum { false, true} bool;
typedef enum { red, black } color_t;

typedef struct rb_node 
{
    // Data section

    int key;
    int range;
    int copies;
    
    // Attributes

    color_t color;
    
    struct rb_node* left_child;
    struct rb_node* right_child;
    struct rb_node* parent;

    struct rb_node* cars;
} node_t;

typedef node_t* node;                                           //Definition of a node as a pointer

typedef struct path_s
{
    node station;
    
    int clicks;                                                 // Distance from origin
    struct path_s* link;                                        // Best connection for optimized path creation
} path_node;

/////////////////////////////////////////////////////

/////////////////// Functions ///////////////////////

node find_node (node root, int key)
{
    if (root == NULL) return NULL;

    else
    {
        while(root != NULL && root->key != key)
        {
            if(root->key > key)
                root = root->left_child;
            else
                root = root->right_child;
        }

        return root;
    }
}

node prev_node (node root, int key)
{    
    node target = find_node(root, key);                         // Find target node

    if (target == NULL) return NULL;                            // No nodes found

    if (target->left_child != NULL)                             // Previous node is gratest of left subtree
    {                             
        target = target->left_child;

        while(target->right_child != NULL)
            target = target->right_child;
        
        return target;
    }

    while (target != root)                                      // No left subtree, upper nodes until root
    {
        target = target->parent;
        if (target->key < key)
            return target;
    }
    
    return NULL;
}

node next_node (node root, int key)
{                             // Smaller of upper nodes
    node target = find_node(root, key);

    if (target == NULL) return NULL;                            // Node not found

    if (target->right_child != NULL)                            // Next node is smallest of right subtree
    {
        target = target->right_child;

        while (target->left_child != NULL)
            target = target->left_child;
    }
    
    while (target != root)                                      // No right subtree, stop searching when arrived to root
    {
        target = target->parent;
        if (target->key > key) 
            return target;
    }
    
    return NULL;                                      // No previous node found
    
}

node rotate_left(node root, node pivot)
{

    if (pivot->right_child == NULL)                             // Right rotation is not possible if there is no right child
        return root;                      
    
    node new_pivot = pivot->right_child;
    
    if (pivot != root)
    {
        if (pivot->parent->left_child == pivot)                 // Update pointer on grandparent (if any)
            pivot->parent->left_child = new_pivot;              // Parent is grandparent's left child
        else
            pivot->parent->right_child = new_pivot;             // Parent is grandparent's right child
    }
    else
        root = new_pivot;
    
    new_pivot->parent = pivot->parent;                          // Switch nodes' parents
    pivot->parent = new_pivot;
    
    pivot->right_child = new_pivot->left_child;                 // Pivot's right child is now new pivot's left tree

    if (new_pivot->left_child != NULL)
        new_pivot->left_child->parent = pivot;                  // Update new pivot's left tree parent

    new_pivot->left_child = pivot;

    return root;
}

node rotate_right (node root, node pivot)
{

    if(pivot->left_child == NULL) 
        return root;

    node new_pivot = pivot->left_child;
    
    if (pivot != root)
    {
        if (pivot->parent->left_child == pivot)                 // Update pointer on grandparent (if any)
            pivot->parent->left_child = new_pivot;
        else
            pivot->parent->right_child = new_pivot;
    }
    else
        root = new_pivot;

    new_pivot->parent = pivot->parent;                          // Switch nodes' parent
    pivot->parent = new_pivot;
    
    pivot->left_child = new_pivot->right_child;                 // Pivot's left child is now new pivot's right tree

    if (new_pivot->right_child != NULL)
        new_pivot->right_child->parent = pivot;                 // Update child's right tree parent

    new_pivot->right_child = pivot;

    return root;
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
           
        free(root);
    }

    return NULL;
}

color_t get_color(node target)
{
    if (target == NULL || target->color == black)
        return black;
    else
        return red;
}

node fix_insertion (node root, node target)
{
    node parent, uncle, grandparent;

    if (root == NULL || target == NULL)                         // Return if tree is not defined or target is not defined
        return NULL;        


    while (get_color(target->parent) == red)                    // While target's parent is red, there could be a violation (root has no parent, so root parent is black)
    {
        parent = target->parent;
        grandparent = parent->parent;                           

        // [1: target's parent is grandparent's left child]

        if (parent == grandparent->left_child)
        {
            uncle = grandparent->right_child;

            // [1A: Uncle is red - Recoloring]
            
            if (get_color(uncle) == red)
            {
                parent->color = black;                          // Push blackness down from grandparent
                uncle->color = black;                           
                grandparent->color = red;                       // Grandparent becomes red
                
                target = grandparent;                           // New violations can now occour on grandparent because of color changing
            }

            // [1B: Uncle is black or is missing - Rotation]

            else if(get_color(uncle) == black)
            {
                if (target == parent->right_child)              // If target is parent's right child, an extra left rotation is needed - "left list"
                {
                    root = rotate_left(root, parent);           // After this rotation, there will be a "left list" - [B R R]
                    parent = target;                            // Target is now became parent (node were inverted)
                }
                
                root = rotate_right(root, grandparent);         // This rotation is to rebalance the left list
                
                parent->color = black;
                grandparent->color = red;

                return root;                                    // Upper node will be black, no more violations possible
            }
        }

        // [2: Target's parent is grandparent's right child]
        
        else
        {
            uncle = grandparent->left_child;

            // [2A: Uncle is red - Recoloring]

            if (get_color(uncle) == red)          
            {
                parent->color = black;                          // Push blackness down from grandparent
                uncle->color = black;
                grandparent->color = red;

                target = grandparent;                           // New violations can occour on grandparent after recoloring
            }
            
            // [2B: Uncle is black or missing - Rotation]

            else if (get_color(uncle) == black)
            {
                if (target == parent->left_child)               // Extra right rotation needed to create a "right list"
                {
                    root = rotate_right(root, parent);
                    parent = target;
                }
                
                root = rotate_left(root, grandparent);
                
                parent->color = black;
                grandparent->color = red;

                return root;
            }
        }
    }

    root->color = black;                                        // Il nodo radice deve essere sempre nero
    
    return root;
}

node insert_node (node* root, int key, bool is_car)
{    
    //Every new node inserted starts as red except for root
    
    node parent = NULL;
    node temp = *root;

    while(temp != NULL)
    {
        parent = temp;

        if (key > temp->key)
            temp = temp->right_child;
        
        else if (key < temp->key)
            temp = temp->left_child;
            
        else if (is_car == true)                                    // If the node is a car and node is already present, increase copies value
        {
            printf("aggiunta\n");

            temp->copies += 1;
            return temp;
        }
            
        else
        {
            printf("non aggiunta\n");
            return NULL;
        }
    }

    printf("aggiunta\n");

    temp = (node_t *)malloc(sizeof(node_t));

    temp->key = key;
    temp->left_child = NULL;
    temp->right_child = NULL;
    temp->cars = NULL;
    temp->range = 0;
    temp->copies = 0;

    if (*root == NULL)
    {
        temp->parent = NULL;
        temp->color = black;
        
        *root = temp;

        return temp;
    }

    else
    {
        temp->parent = parent;
        temp->color = red;
        
        *root = fix_insertion (*root, temp); 
    }

    return temp;
    
}

node fix_removal(node root, node target)
{
    node parent = target->parent;
    node sibling;
    node left_nephew;
    node right_nephew;

    if(target == root)                                          // fix_removal is recursive, if double blackness is pushed up to root, fix is completed
    {
        target->color = black;
        return root;
    }

    if (parent->left_child == target)                           // Target is left child
        sibling = parent->right_child;
    else                                                        // Target is right child
        sibling = parent->left_child;

    if (sibling == NULL)                                        // If node to delete requires fixup and has no sibling, R&B properties must have been violated
        exit(1);

    left_nephew = sibling->left_child;
    right_nephew = sibling->right_child;
    
    // [1: Sibling has both black children]

    if (get_color(left_nephew) == black && get_color(right_nephew) == black)
    {
        if(sibling->color == black)                             // [1.1: Black sibling with black children]
        {
            if (parent->color == red)                           // Double blackness could be solved pushing up blackness to parent
            {
                parent->color = black;
                sibling->color = red;
            }

            else                                                // Parent is also black
            {
                sibling->color = red;                           // Push blackness up, parent is now double black
                root = fix_removal(root, parent);               // Recoloring can cause more violations, check parent
            }
        }
        else                                                    // [1.2: Red sibling with black children] 
        {   
            parent->color = red;                                // Parent must have been black or double red violation occurred in insertion
            sibling->color = black;

            if (parent->left_child == sibling)                  // Sibling is left child -> Needed right rotation to balance deletion
                root = rotate_right(root, parent);
                    
            else
                root = rotate_left(root, parent);               // Otherwise a left rotation is needed

            root = fix_removal(target, root);

        }
    }

    // [2: Sibling has red left child] - Note: sibling must be black to not violate R&B properties

    /*
        If sibling s is black and at least one of sibling’s children is red, perform rotation(s).
        RED child of SIB is R
        (2.1.1) Left Left Case - sibling is left child, left nephew is red or both nephews are red)
        (2.1.2) Left Right Case - sibling is left child and right nephew is red)
        (2.2.1) Right Left Case (s is right child of its parent and R is LEFT CHILD of s)
        (2.2.2) Right Right Case (s is right child of its parent and r is right child of s or both children of s are red)
    */

    // [2.1: Sibling is left child]
    
    else if (sibling == parent->left_child)
    {
        // [2.1.1] Left Left -> Single right rotation

        if (get_color(left_nephew) == red)
        {
            root = rotate_right (root, parent);
            
            left_nephew->color = black;
            
            color_t tmp = parent->color;
            parent->color = sibling->color;
            sibling->color = tmp;
        }

        // [2.1.2] Left Right -> Double rotation: right/left
        else if (get_color(right_nephew) == red)
        {
            root = rotate_left(root, sibling);
            root = rotate_right(root, parent);
            
            right_nephew->color = parent->color;
            parent->color = black;
            sibling->color = black;
            
        }
    }

    // [2.2: Sibling is right child]

    else if (sibling == parent->right_child)
    {
        // [2.1.1] Right Right -> Single left rotation

        if (get_color(right_nephew) == red)
        {
            root = rotate_left(root, parent);

            right_nephew->color = black;

            color_t tmp = parent->color;
            parent->color = sibling->color;
            sibling->color = tmp;
        }

        // [2.1.2] Right Left -> Double rotation: left/right
        
        else if (get_color(left_nephew) == red)
        {
            root = rotate_right(root, sibling);
            root = rotate_left(root, parent);

            left_nephew->color = parent->color;
            parent->color = black;
            sibling->color = black;
        }
    }

    return root;
}

node remove_node (node root, node target, bool is_car)
{                         
    int key_copy;
    int range_copy;
    int copies_copy;
    node cars_copy = NULL;

    if (target == NULL)                                         // Target node not found
        return root;


    if(target->copies > 0 && is_car == true)                   // If there are copies and node is a car, decrement
    {
        target->copies -= 1;

        return root;
    }

    // [1: Deleting a leaf-node]

    if (target->left_child == NULL && target->right_child == NULL)
    {
        if (target == root)                                     // Target is root
        {
            target->cars = delete_tree(target->cars);

            free(target);
            
            return NULL;
        }

        else                                                    // Target is not root
        {
            if (target->color != red)                           // Target is a red leaf, no possible violations
                root = fix_removal(root, target);               // Fixing tree before removing node

            if(target->parent->left_child == target)            // Target is a left child
                target->parent->left_child = NULL;              // Resetting parent's child pointer
            
            else                                                // Target is a right child
                target->parent->right_child = NULL;
            
        }

        target->cars = delete_tree(target->cars);

        free(target);
        
        return root;
    }

    // [2: Deleting a central node]

    node trasplant = next_node(target, target->key);            // Finde node to transplant inside target's subtree
    
    if(trasplant == NULL)                                       // No next element found (left subtree only)
        trasplant = prev_node(target, target->key);
  
    key_copy = trasplant->key;
    range_copy = trasplant->range;
    copies_copy = trasplant->copies;
    cars_copy = trasplant->cars;
    
    if(target->cars)
        trasplant->cars = target->cars;                         // Exchange car park, so can be deleted when a leaf is found
    

    root = remove_node(root, trasplant, is_car);

    target->key = key_copy;                                     // Substituting target node's infos with transplant node's infos
    target->range = range_copy;
    target->cars = cars_copy;
    target->copies = copies_copy;

    return root;
}

void find_path(path_node* list, int size)
{
    for (int crnt = 0; crnt < size -1; crnt++)
    {
        int next = crnt + 1;
        int dist = (list[next].station->key) - (list[crnt].station->key);

        while (next < size && dist * (dist > 0 ? 1 : -1) <= list[crnt].station->range)
        {

            if(list[next].link == NULL)                                 // If theree is no link
            {
                list[next].link = &list[crnt];                          // Add new link
                list[next].clicks = list[crnt].clicks + 1;
            }
            else if(list[next].clicks > (list[crnt].clicks + 1))        // If in-range station's clicks are more than current station's clicks + 1 (new step)
            {   
                list[next].link = &list[crnt];                          // Update link
                list[next].clicks = list[crnt].clicks + 1;              // Update clicks
            }
            else if (list[next].clicks >= (list[crnt].clicks + 1))      // If clicks are grater or same
            {
                if(list[0].station->key > list[1].station->key)         // And path is from a further station to a nearer one
                list[next].link = &list[crnt];                          // Update link
            }

            next++;
            
            if(next < size)
                dist = (list[next].station->key) - (list[crnt].station->key);
        }
    }

    int path_size = (list[size -1].clicks) + 1;                         // Path size must be destinantopn node's click + 1 (to contain starting point)
    int path [path_size];

    path_node tmp = list[size -1];
    
    for (int i = path_size - 1; i >= 0; i--)
    {
        path[i] = tmp.station->key;
        if(tmp.link)
            tmp = *(tmp.link);
    }

    if (path[0] == list[0].station->key)
    {
        for (int i = 0; i < path_size - 1; i++)
            printf("%d ", path[i]);

        printf ("%d\n", path[path_size - 1]);                           // For matching whitespaces   
    }

    else
        printf("nessun percorso\n");

}

void print_level_order(node root)                                       // Remove before flight - Utility
{
    if (root == NULL)
    {
        printf("+ Empty tree\n\n");
        return;
    }

    node queue[1000000];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear)
    {
        node current = queue[front++];
        printf("{'Key' : %d, 'Color' : '%s', 'Parent' : %d},\n", current->key, current->color == red ? "red" : "black", current->parent != NULL ? current->parent->key : -1);

        if (current->left_child)
            queue[rear++] = current->left_child;
        if (current->right_child)
            queue[rear++] = current->right_child;
    }

    printf("\n");
}

/////////////////////////////////////////////////////

int main()
{
    char buffer[20];
    int input[2];

    node stations = NULL;

    node temp_node;
    bool temp_bool;
    int temp_int;

    int i = 0;

    while (scanf(" %s", buffer) == 1 && !feof(stdin))                               // Data collected, no EOF
    {                                                           
        switch (buffer[0])
        {
        case 'a':

            //////////////// Add a station ////////////////
            
            if (buffer[9] == 's')
            {
                if (scanf(" %d %d", &input[0], &input[1]))
                {
                    temp_node = insert_node(&stations, input[0], false);
                    
                    if(temp_node && input[1] != 0)
                    {
                        temp_node->copies = (input[1] <= 512 ? input[1] : 512);     // Copies is used to store car number if node is a station
                        
                        for (int i = 0; i < input[1]; i++)
                        {
                            if (!scanf("%d", &temp_int)) exit(1);
                            
                            else if (i < 512)
                            {
                                insert_node(&(temp_node->cars), temp_int, true);
                                
                                if(temp_int > temp_node->range)                     // If new car has greater range
                                    temp_node->range = temp_int;
                            }
                        }
                    }
                    
                    else if(!temp_node && input[1] != 0)                            // Flush input if station is not added
                    {
                        for (int i = 0; i < input[1]; i++)
                            if(!scanf("%d", &temp_int)) exit(1);
                    }
                    
                }
                
                else exit(1);
            }

            ////////////////// Add a car //////////////////

            else if (buffer[9] == 'a')
            {
                if (!scanf(" %d %d", &input[0], &input[1])) exit(1);

                temp_node = find_node(stations, input[0]);
                
                if (temp_node && temp_node->copies < 512)
                {
                    insert_node(&(temp_node->cars), input[1], true);

                    if(input[1] > temp_node->range)                                 // If new car has greater range
                        temp_node->range = input[1];
                }
                
                else
                    printf("non aggiunta\n");
            }

            else exit(1);

            break;

        ///////// Remove a station /////////

        case 'd':

            if (scanf(" %d", &input[0]))
            {
                temp_node = find_node(stations, input[0]);

                if(temp_node)
                {
                    stations = remove_node(stations, temp_node, false);
                    printf("demolita\n");
                }

                else
                {
                    printf("non demolita\n");
                }
            }

            else exit(1);
            
            break;

        /////////// Remove a car ///////////

        case 'r':
            if (scanf(" %d %d", &input[0], &input[1]))
            {
                temp_node = find_node(stations, input[0]);

                if (temp_node)
                {
                    node temp_car = find_node(temp_node->cars, input[1]);

                    if (temp_car)
                    {
                        if (temp_car->copies == 0 && temp_node->range == temp_car->key)
                            temp_node->range = prev_node(temp_node->cars, temp_car->key)->key;
                        
                        temp_node->cars = remove_node(temp_node->cars, temp_car, true);

                        printf("rottamata\n");

                    }

                    else
                        printf("non rottamata\n");    
                }
                else
                    printf("non rottamata\n");
            }

            else exit(1);

            break;

        case 'p':
            if (scanf(" %d %d", &input[0], &input[1]))
            {
                node start = find_node (stations, input[0]);
                node stop = find_node (stations, input[1]);

                node next = start;
                int size = 1;

                if (start && stop)
                {
                    while (next != stop)
                    {
                        if(start->key < stop->key)
                            next = next_node(stations, next->key);
                        else
                            next = prev_node(stations, next->key);

                        size++;
                    }

                    path_node* list = (path_node*) malloc(size * sizeof(path_node));

                    next = start;

                    for (int i = 0; i < size; i++)
                    {
                        list[i].station = next;
                        list[i].clicks = 0;
                        list[i].link = NULL;

                        if (start->key < stop->key)
                            next = next_node(stations, next->key);
                        else
                            next = prev_node(stations, next->key);
                    }

                    find_path(list, size);

                    free(list);
                }
            
                else
                    printf("nessun percorso\n");
            }
            break;

        default:
            exit(1);
        }
    }

    stations = delete_tree(stations);

    return 0;
}