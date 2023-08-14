#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 20
#define INPUT_SIZE 2 // Station_id   Car_Number  512 cars

/////////////////// Types definition ////////////////

typedef enum { false, true} bool;
typedef enum { red, black } color_t;

typedef struct rb_node {

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
    struct path_s* link;                                        // Best connection for path
} path_node;

/////////////////////////////////////////////////////

/////////////////// Functions ///////////////////////

node find_node (node root, int key){
    if (root == NULL) return NULL;

    else if (root->key < key)
        return find_node(root->right_child, key);
    
    else if (root->key > key)
        return find_node(root->left_child, key);
    
    else return root;
}

node prev_node (node root, int key){                            // Grater of left subtree / Grater of upper nodes
    node target = find_node(root, key);

    if (target == NULL) return NULL;                            // No nodes found

    if(target->left_child != NULL){                             // Left subtree exists
        target = target->left_child;

        while(target->right_child != NULL)
            target = target->right_child;
    }
    else if (target->parent != NULL && target != root){         // No left subtree, checks if one of upper node could be the previous one
        if (target->parent->key < key){
            target = target->parent;
            return target;
        }
        else{
            while (target->parent != NULL && target != root)    // Stop when root is found
            {
                target = target->parent;
                if (target->key < key)
                    return target;
            }
            target = NULL;                                      // No previous node found
        }
    }
    else target = NULL;

    return target;
}

node next_node (node root, int key){                            // Smaller of right subtree / Smaller of upper nodes
    node target = find_node(root, key);

    if (target == NULL) return NULL;                            // Node not found

    if (target->right_child != NULL)
    {
        target = target->right_child;

        while (target->left_child != NULL)
            target = target->left_child;
    }
    else if (target->parent != NULL && target != root)          // Target parent is root
    {
        if (target->parent->key > key)
        {
            target = target->parent;
            return target;
        }
        else
        {
            while (target->parent != NULL && target != root)    // Stop searching when arrived to root
            {
                target = target->parent;
                if (target->key > key) return target;
            }
            target = NULL;                                      // No previous node found
        }   
    }
    else target = NULL;

    return target;
}

void rotate_left(node* root, node parent, node child){

    if(parent->right_child != child) return;
    
    child->parent = parent->parent;                             // Child parent is now grandparent
    
    if (child->parent != NULL)
    {
        if (parent->parent->left_child == parent)               // Update pointer on grandparent (if any)
            parent->parent->left_child = child;                 // Parent is grandparent's left child
        else
            parent->parent->right_child = child;                // Parent is grandparent's right child
    }
    else
        *root = child;
    
    parent->right_child = child->left_child;                    // Parent's right child is now child's left tree

    if (child->left_child != NULL)
        child->left_child->parent = parent;                     // Update child's left tree parent

    child->left_child = parent;
    parent->parent = child;
}

void rotate_right (node* root, node parent, node child){

    if(parent->left_child != child) return;

    child->parent = parent->parent;                             // Child parent is now grandparent
    
    if (child->parent != NULL)
    {
        if (parent->parent->left_child == parent)               // Update pointer on grandparent (if any)
            parent->parent->left_child = child;
        else
            parent->parent->right_child = child;
    }
    else
        *root = child;

    parent->left_child = child->right_child;                    // Parent's left child is now child's right subtree tree

    if (child->right_child != NULL)
        child->right_child->parent = parent;                    // Update child's right tree parent

    child->right_child = parent;
    parent->parent = child;
}

node delete_tree(node root)
{
    if (root == NULL) return NULL;

    else
    {   
        root->left_child = delete_tree(root->left_child);
        root->right_child = delete_tree(root->right_child);
        
        root->cars = delete_tree(root->cars);
           
        free(root); // Free root
    }

    return NULL;
}

void fix_insertion (node* root, node target)
{
    node parent, grandparent, uncle;

    if ((*root == NULL || target == NULL))                      // Return if tree is not defined or target is not defined
        return;        


    while (target->parent != NULL)                              // target is root
    {
        parent = target->parent;
        grandparent = parent->parent;                           // Can be NULL if parent is root

        if (grandparent == NULL || parent->color == black)                                 
            return;

        // Case 1: target's parent is grandparent's left child

        if (parent == grandparent->left_child)
        {
            uncle = grandparent->right_child;

            // Uncle is a red node
            
            if (uncle != NULL && uncle->color == red)
            {
                parent->color = black;                          // Push blackness down from grandparent
                uncle->color = black;                           
                grandparent->color = red;                       // Grandparent becomes red
                target = grandparent;                           // New violations can now occour on grandparent because of color changing
            }

            // Uncle is black or is missing - Left Heavy

            else if(parent->color == red && (uncle == NULL || uncle->color == black))
            {
                if (target == parent->right_child)              // If target is parent's right child, an extra left rotation is needed - "left list"
                {
                    rotate_left(root, parent, target);          // After this rotation, there will be a "left list" - [B R R]
                    parent = target;                            // Parent is now become target (node were inverted)
                }
                
                rotate_right(root, grandparent, parent);        // This rotation is to rebalance the left list
                
                parent->color = black;
                grandparent->color = red;

                target = grandparent;
            }
        }

        // Target's parent is grandparent's right child
        
        else
        {
            uncle = grandparent->left_child;

            // Uncle is red

            if (uncle != NULL && uncle->color == red)           // Only recoloring is needed
            {
                parent->color = black;                          // Push blackness down from grandparent
                uncle->color = black;
                grandparent->color = red;
                target = grandparent;                           // New violations can occour on grandparent after recoloring
            }
            
            // Uncle is black or missing - Right Heavy

            else if (parent->color == red && (uncle == NULL || uncle->color == black))
            {
                if (target == parent->left_child)               // Extra right rotation needed to create a "right list"
                {
                    rotate_right(root, parent, target);
                    parent = target;
                }
                
                rotate_left(root, grandparent, parent);
                
                parent->color = black;
                grandparent->color = red;

                target = grandparent;
            }
        }
    }

    (*root)->color = black; // Il nodo radice deve essere sempre nero
}

node insert_node (node* root, int key, bool is_car){
    //Every new node inserted starts as red except for root
    
    node parent = NULL;
    node* tmp;
    tmp = root;

    while(*tmp != NULL){
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
        new_node->color = black;
        
        *root = new_node;
    }

    else
    {
        new_node->parent = parent;
        new_node->color = red;
        
        *tmp = new_node;
        
        fix_insertion (root, *tmp); 
    }

    return new_node;
    
}

color_t get_color(node target){
    if (target == NULL || target->color == black)
        return black;
    else
        return red;
}

void fix_removal(node *root, node target)
{
    node parent = target->parent;
    node sibling;

    if (target == *root){
        target->color = black;
        return;
    }
    else                                                        // Identify sibling
    {
        if (parent->left_child == target)                       // Target is left child
            sibling = parent->right_child;
        else                                                    // Target is right child
            sibling = parent->left_child;
    }

    if (sibling != NULL)
    {
        // Sibling has both black children

        if ((get_color(sibling->left_child) == black) && (get_color(sibling->right_child) == black))
        {
            if(sibling->color == black)                         // Black sibling with black children
            {
                if (parent->color == red)                       // Double blackness could be solved pushing up blackness to parent
                {
                    parent->color = black;
                    sibling->color = red;
                }

                else                                            // Parent is also black
                {
                    sibling->color = red;                       // Push blackness up, parent is now double black
                    fix_removal(root, parent);                  // Fix parent;
                }
            }
            else                                                // Red sibling with black children
            {
                if (parent->color == black)                     // Double blackness could be fixed pushing down blackness from parent
                {
                    parent->color = red;
                    sibling->color = black;

                    if (parent->left_child == sibling)          // Sibling is left child -> Needed right rotation to balance deletion
                        rotate_right(root, parent, sibling);
                    
                    else
                        rotate_left(root, parent, sibling);     // Otherwise a left rotation is needed
                    
                    fix_removal(root, target);                  // After rotation, check again for double blackness fix up
                }
                

            }

        }

        else if (get_color(sibling->left_child) == red)
        {
            
            // Target is left child, sibling is black and nearest nephew is red

            if (target->parent->left_child == target && sibling->color == black)           
            {
                if (sibling->right_child == NULL)
                {
                    parent->color = black;
                    rotate_right(root, sibling, sibling->left_child);
                    rotate_left(root, parent, parent->right_child);
                }
                else
                {
                    sibling->right_child->color = black;         // Push down blackness from sibling to nephew
                    rotate_left(root, parent, sibling);
                }
            }

            // Target is right child, sibling is black and furthest nephew is red

            else if(target->parent->right_child == target && sibling->color == black)
            {
                
                sibling->left_child->color = black;             // Black pushed to nephew
                
                rotate_right(root, parent, sibling);
            }
            
            // In both subcases, color must be switched between parent and sibling
            
            color_t tmp = parent->color;
            parent->color = sibling->color;
            sibling->color = tmp;
        }

        else if (get_color(sibling->right_child) == red)
        {

            // Target is right child, sibling is black and nearest nephew is red

            if (target->parent->right_child == target && sibling->color == black)          
            {
                if(sibling->left_child == NULL)
                {
                    parent->color = black;
                    rotate_left(root, sibling, sibling->right_child);
                    rotate_right(root, parent, parent->left_child);
                }
                else
                {                
                    sibling->left_child->color = black;             // Push down blacknes from sibling to nephew
                    rotate_right(root, parent, sibling);
                }
            }

            // Target is left child, sibling is black and furthest nephew is red

            else if (target->parent->left_child == target && sibling->color == black)
            {
          
                sibling->right_child->color = black;                // Black pushed to nephew

                rotate_left(root, parent, sibling);
            }

            // In both subcases, color must be switched between parent and sibling

            color_t tmp = parent->color;
            parent->color = sibling->color;
            sibling->color = tmp;
        }
    }


       
}

bool remove_node (node* root, int key){                         
    node target = find_node(*root, key);
    
    int key_copy;
    int range_copy;
    int copies_copy;
    node cars_ptr_copy = NULL;

    if (target == NULL)                                         // Target node not found
        return false;

    if(target->copies > 0)                                      // If there are copies, decrement
    {
        target->copies -= 1;
        return true;
    }

    // Deleting a leaf-node [1]

    if (target->left_child == NULL && target->right_child == NULL)
    {
        if (target == *root)                                    // Target is root
        {
            target->cars = delete_tree(target->cars);

            free(target);
            *root = NULL;
            return true;
        }

        else                                                    // Target is not root
        {
            /////////// Fixing tree before removal ///////////////

            if (target->color != red)                           // Target is a red leaf, no possible violations
                fix_removal(root, target);

            //////////////////////////////////////////////////////

            if(target->parent->left_child == target)            // Target is a left child
                target->parent->left_child = NULL;              // Resetting parent's child pointer
            
            else                                               // Target is a right child
                target->parent->right_child = NULL;
            
        }

        target->cars = delete_tree(target->cars);

        free(target);
        
        return true;
    }

    // Deleting a central node

    node next = next_node(target, key);                         // Finde next element only inside subtree
    
    if(next == NULL)                                            // No next element found (left subtree only) [3]
        next = target->left_child;
  
    key_copy = next->key;
    range_copy = next->range;
    copies_copy = next->copies;
    cars_ptr_copy = next->cars;
    
    if(target->cars)
        next->cars = target->cars;                              // Exchange car park, so can be deleted when a leaf is found
    

    remove_node(root, next->key);

    target->key = key_copy;                                     // Substituting target node's infos with next node's infos (deleted, so copied)
    target->range = range_copy;
    target->cars = cars_ptr_copy;
    target->copies = copies_copy;

    return true;
}

void find_path(path_node* list, int size)
{
    for (int crnt = 0; crnt < size -1; crnt++)
    {
        int next = crnt + 1;
        
        while(next < size && (list[next].station->key) - (list[crnt].station->key) <= list[crnt].station->range)
        {

            if(list[next].link == NULL)                                 // If theree is no link
            {
                list[next].link = &list[crnt];                           // Add new link
                list[next].clicks = list[crnt].clicks + 1;
            }
            else if(list[next].clicks > (list[crnt].clicks + 1))        // If in-range station's clicks are more than current station's clicks + 1 (new step)
            {   
                list[next].link = &list[crnt];                          // Update link
                list[next].clicks = list[crnt].clicks + 1;
            }

            next++;
        }
    }

    int path_size = (list[size -1].clicks) + 1;                         // Path size must be the number of clicks of destination node
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
        for (int i = 0; i < path_size; i++)
            printf("%d ", path[i]);
        printf("\n");   
    }

    else
        printf("nessun percorso\n");

}

void print_level_order(node root)                               // Remove before flight - Utility
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
    char buffer[BUFF_SIZE];
    int input[INPUT_SIZE];

    node stations = NULL;

    node tmp_node;
    int tmp_int;
    bool tmp_bool;

    while (scanf(" %s", buffer) == 1 && !feof(stdin))
    {                                                           // Data collected, no EOF
        switch (buffer[0])
        {
        case 'a':

            int i = 0;
            while (buffer[i] != '-')
                i++;

            //////////////// Add a station ////////////////
            
            if (buffer[i + 1] == 's')
            {

                if (scanf(" %d %d", &input[0], &input[1]))
                {
                    tmp_node = insert_node(&stations, input[0], false);
                    
                    if(tmp_node && input[1] != 0)
                    {
                        for (i = 0; i < input[1]; i++)
                        {
                            if (!scanf("%d", &tmp_int)) return 1;
                            else
                            {
                                insert_node(&(tmp_node->cars), tmp_int, true);
                                
                                if(tmp_int > tmp_node->range)   // If new car has greater range
                                    tmp_node->range = tmp_int;
                            }
                        }
                        printf("aggiunta\n");
                    }
                    else if (tmp_node && input[1] == 0)
                        printf("aggiunta\n");
                    else if(!tmp_node && input[1] != 0)
                    {
                        for (int i = 0; i < input[1]; i++)
                            scanf("%d", &tmp_int);
                        
                        printf("non aggiunta\n");
                    }
                    
                }
                else return 1;

                //print_level_order(stations);
            }

            ////////////////// Add a car //////////////////

            else if (buffer[i + 1] == 'a')
            {
                if (!scanf(" %d %d", &input[0], &input[1])) return 1;

                tmp_node = find_node(stations, input[0]);
                
                if (tmp_node)
                {
                    insert_node(&(tmp_node->cars), input[1], true);
                    printf("aggiunta\n");

                    if(input[1] > tmp_node->range)              // If new car has greater range
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

            //print_level_order(stations);

            break;

        case 'r':
            if (scanf(" %d %d", &input[0], &input[1]))
            {
                tmp_node = find_node(stations, input[0]);

                if (tmp_node)
                {
                    node tmp_car = find_node(tmp_node->cars, input[1]);

                    if(tmp_car && tmp_car->copies == 0 && tmp_node->range == tmp_car->key)
                        tmp_node->range = prev_node(tmp_node->cars, tmp_car->key)->key;

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
                node start = find_node (stations, input[0]);
                node stop = find_node (stations, input[1]);

                node next = start;
                int size = 1;

                if (start && stop)
                {
                    while (next != stop)
                    {
                        next = next_node(stations, next->key);
                        size++;
                    }

                    path_node* list = (path_node*) malloc(size * sizeof(path_node));

                    next = start;

                    for (int i = 0; i < size; i++)
                    {
                        list[i].station = next;
                        list[i].clicks = 0;
                        list[i].link = NULL;
                        next = next_node(stations, next->key);
                    }

                    find_path(list, size);

                    free(list);
                }
            
                else
                    printf("nessun percorso\n");
            }
            break;

        default:
            return 1;
        }
    }

    remove_node(&stations, 978114036);

    // 499379011
    //print_level_order(stations);

    stations = delete_tree(stations);

    return 0;
}
