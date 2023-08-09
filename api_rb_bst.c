/*
    Implementation of a Red and Black Binary Search Tree
    + Root is black
    + leaves are black
    + red nodes' children are black
    + for every node, every path from its children to the leaves must contain same number of black nodes
    + keys and data are contained only inside the trree, not in the leaves
*/

#include <stdio.h>
#include <stdlib.h>

/////////////////// Types definition ////////////////

typedef enum { red, black } color_t;

typedef struct rb_node {
    int key;
    color_t color;
    struct rb_node* left_child;
    struct rb_node* right_child;
    struct rb_node* parent;
} node_t;

typedef node_t* node;                                           //Definition of a node as a pointer

typedef struct linked_list {                                    //List of nodes in a range
    node node;
    struct linked_list* next;
} list;

/////////////////////////////////////////////////////

/////////////////// Functions ///////////////////////

node find_node (node root, int key){
    if (root == NULL)
    {
        printf("+ Node not found\n\n");
        return NULL;
    }
    else if (root->key < key)
    {
        printf("+ Right branch taken\n");
        return find_node(root->right_child, key);
    }
    else if (root->key > key)
    {
        printf("+ Left branch taken\n");
        return find_node(root->left_child, key);
    }
    else
    {
        printf("+ Node found\n+ Node address: %p\n+ Node color: %s\n\n", (void *)root, root->color == red ? "red" : "black");
        return root;
    }
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
            printf("+ Previous node key: %d\n\n", target->key); // CAN BE OPTIMIZED
            return target;
        }
        else{
            while (target->parent != NULL && target != root)    // Stop when root is found
            {
                target = target->parent;
                if (target->key < key)
                {
                    printf("+ Previous node key: %d\n\n", target->key);
                    return target;
                }
            }
            target = NULL;                                      // No previous node found
        }
    }
    else target = NULL;

    if (target != NULL) printf("+ Previous node key: %d\n\n", target->key);
    else printf ("+ Prev node not found\n\n");

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
            printf("+ Next node key: %d\n\n", target->key);     // CAN BE OPTIMIZED
            return target;
        }
        else
        {
            while (target->parent != NULL && target != root)    // Stop searching when arrived to root
            {
                target = target->parent;
                if (target->key > key)
                {
                    printf("+ Next node key: %d\n\n", target->key);
                    return target;
                }
            }
            target = NULL;                                      // No previous node found
        }   
    }
    else target = NULL;

    if (target != NULL) printf("+ Next node key: %d\n\n", target->key);
    else printf("+ Next node not found\n\n");

    return target;
}

void rotate_left(node* root, node parent, node child){

    if(parent->right_child != child) 
    {
        printf("+ Error occurred, rotation not performed\n");
        return;
    }

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

    printf("+ Nodes left-rotated\n\n");
}

void rotate_right (node* root, node parent, node child){

    if(parent->left_child != child) 
    {
        printf("+ Error occurred, rotation not performed\n");
        return;
    }

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

    printf("+ Nodes right-rotated\n\n");
}

void fix_insertion (node* root, node target)
{

    printf("+ Started violations checking\n");
    node parent, grandparent, uncle;

    if ((*root == NULL || target == NULL))                      // Return if tree is not defined or target is not defined
    { 
        printf("+ Tree not found\n\n");
        return;        
    }


    while (target->parent != NULL)                              // target is root
    {
        parent = target->parent;
        grandparent = parent->parent;                           // Can be NULL if parent is root

        if (grandparent == NULL || parent->color == black)                                 
        {
            printf("+ Tree fixed\n\n");
            return;
        }

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

void insert_node (node* root, int key){
    //Every new node inserted starts as red except for root
    
    printf("+ Inserting node: %d\n", key);

    if (*root == NULL)
    { 
        node new_node = (node_t *)malloc(sizeof(node_t));

        new_node->key = key;
        new_node->left_child = NULL;
        new_node->right_child = NULL;
        new_node->parent = NULL;
        new_node->color = black;

        *root = new_node;

        printf("+ Node inserted correctly as root\n+ Node memory address: %p\n\n", (void *)(*root));
        return;
    }

    else
    {
        node parent = NULL;
        node* tmp;
        tmp = root;

        while(*tmp != NULL){
            parent = *tmp;

            if (key > (*tmp)->key)
            {
                printf("+ Right branch taken\n");
                tmp = &((*tmp)->right_child);
            }
            else if (key < (*tmp)->key)
            {
                printf("+ Left branch taken\n");
                tmp = &((*tmp)->left_child);
            }
            else
            {
                printf("+ Found another node with same key\n+ Node not inserted\n\n");
                return;
            }
        }

        node new_node = (node_t *)malloc(sizeof(node_t));

        new_node->key = key;
        new_node->left_child = NULL;
        new_node->right_child = NULL;
        new_node->parent = parent;
        new_node->color = red;

        *tmp = new_node;

        printf("+ Node inserted correctly\n+ Node memory address: %p\n", (void *)(tmp));
        if(parent != NULL) printf ("+ Parent key: %d\n\n", parent->key);

        fix_insertion (root, *tmp); 
    }
}

color_t get_color(node target){
    if (target == NULL || target->color == black)
        return black;
    else
        return red;
}

void fix_removal(node *root, node target)
{
    printf("+ Fixing target: %d\n", target->key);

    node parent = target->parent;
    node sibling;

    if (target == *root){
        target->color = black;
        printf("+ Tree fixed\n\n");
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

        printf("\n+ Parent: %s\n", parent->color == red ? "red" : "black");
        printf("+ Sibling: %s\n", sibling->color == red ? "red" : "black");
        printf("+ Left: %s\n", sibling->left_child->color == red ? "red" : "black");
        printf("+ Right: %s\n\n", sibling->right_child->color == red ? "red" : "black");

        // Sibling has both black children

        if ((get_color(sibling->left_child) == black) && (get_color(sibling->right_child) == black))
        {
            if(sibling->color == black)                         // Black sibling with black children
            {
                if (parent->color == red)                       // Double blackness could be solved pushing up blackness to parent
                {
                    printf("+ Case [1]: pushing up blackness to parent\n");
                    parent->color = black;
                    sibling->color = red;
                }

                else                                            // Parent is also black
                {
                    printf("+ Case [2]: pushing up blackness to black parent\n");
                    sibling->color = red;                       // Push blackness up, parent is now double black
                    fix_removal(root, parent);                  // Fix parent;
                }
            }
            else                                                // Red sibling with black children
            {
                if (parent->color == black)                     // Double blackness could be fixed pushing down blackness from parent
                {
                    printf("+ Case [3]: pushing down blackness to sibling\n");
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
                printf("+ Case [4]: pushing down blackness from sibling to nephew\n");

                sibling->right_child->color = black;            // Push down blackness from sibling to nephew
                rotate_left(root, parent, sibling);
            }

            // Target is right child, sibling is black and furthest nephew is red

            else if(target->parent->right_child == target && sibling->color == black)
            {
                printf("+ Case [5]: switch parent and sibling color, pushing blackness to nephew\n");
            
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
                printf("+ Case [6]: pushing down blackness from sibling to nephew\n");

                sibling->left_child->color = black;             // Push down blacknes from sibling to nephew

                rotate_right(root, parent, sibling);
            }

            // Target is left child, sibling is black and furthest nephew is red

            else if (target->parent->left_child == target && sibling->color == black)
            {
                printf("+ Case [7]: switch parent and sibling color, pushing blackness to nephew\n");

                sibling->right_child->color = black;            // Black pushed to nephew

                rotate_left(root, parent, sibling);
            }

            // In both subcases, color must be switched between parent and sibling

            color_t tmp = parent->color;
            parent->color = sibling->color;
            sibling->color = tmp;
        }
    }


       
}

void remove_node (node* root, int key){                         
    node target = find_node(*root, key);
    int key_copy;

    if (target == NULL)                                         // Target node not found
    {
        printf("+ Node not found\n\n");
        return;
    }

    // Deleting a leaf-node [1]

    if (target->left_child == NULL && target->right_child == NULL)
    {
        if (target->parent == NULL)                             // Target is root
        {
            *root = NULL;
            free(target);
            return;
        }

        else                                                    // Target is not root
        {
            
            printf("+ Left reached\n");
            /////////// Fixing tree before removal ///////////////

            if (target->color == red)                           // Target is a red leaf, no possible violations
                printf("+ Leaf is red, no violations\n\n");

            else
            {
                printf("+ Double blackness violation\n");
                fix_removal(root, target);
            }
            //////////////////////////////////////////////////////

            if(target->parent->left_child == target)            // Target is a left child
                target->parent->left_child = NULL;              // Resetting parent's child pointer
            
            else                                               // Target is a right child
                target->parent->right_child = NULL;
            
        }

        printf("+ Target leaf deleted\n\n");
        free(target);
        
        return;
    }

    // Deleting a central node

    node next = next_node(target, key);                         // Finde next element only inside subtree
    
    if (next != NULL)                                           // Next element found
    {
        key_copy = next->key;
        printf("+ Copied key of next node in order\n");
        remove_node(root, next->key);
    }

    else                                                        // No next element found (left subtree only) [3]
    {
        key_copy = target->left_child->key;
        printf("+ Copied key of left child\n");
        remove_node(root, target->left_child->key);
    }

    target->key = key_copy;                                     // Substituting target node's infos with next node's infos (deleted, so copied)
}

void delete_tree (node root){
    if (root != NULL)
    {
        if (root->left_child != NULL)                           // Free left subtree
            delete_tree(root->left_child);
        
        if (root->right_child != NULL)                          // Free right subtree
            delete_tree(root->right_child);
        
        printf("+ Node deleted - key: %d\n", root->key);
        free(root);                                             // Free root
    }
}

void print_level_order(node root)                               // Remove before flight - Utility
{
    if (root == NULL)
    {
        printf("+ Empty tree\n\n");
        return;
    }

    node queue[1000];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear)
    {
        node current = queue[front++];
        printf("Key: %d Color: %s Parent: %d\n", current->key, current->color == red ? "red" : "black", current->parent != NULL ? current->parent->key : -1);

        
        if (current->left_child)
            queue[rear++] = current->left_child;
        if (current->right_child)
            queue[rear++] = current->right_child;
    }

    printf("\n");
}

/////////////////////////////////////////////////////

int main(void) {
    char inst;                                                  // Char containing instruction to execute
    int key;                                                   
    node root = NULL;                                           // Inizialize NULL root pointer

    insert_node(&root, 15);
    insert_node(&root, 13);
    insert_node(&root, 6);
    insert_node(&root, 14);
    insert_node(&root, 5);
    insert_node(&root, 9);
    insert_node(&root, 12);
    insert_node(&root, 7);
    insert_node(&root, 11);
    insert_node(&root, 8);
    insert_node(&root, 10);
    insert_node(&root, 23);
    insert_node(&root, 21);
    insert_node(&root, 25);
    insert_node(&root, 17);
    insert_node(&root, 24);
    insert_node(&root, 27);
    insert_node(&root, 16);
    insert_node(&root, 19);
    insert_node(&root, 20);

    printf("Instruction list:\n> i - insert\n> r - remove\n> f - find\n> n - next-to\n> p - print\n> q - quit\n\n");

    while (1)
    {
        printf("> "); 
        scanf(" %c", &inst);
        if (inst != 'q' && inst != 'p')
            scanf(" %d", &key);

        switch (inst)
        {
        case 'i':
            printf("\n+ Inserting node - key: %d\n", key);
            printf("+ Root address: %p\n", (void *)root);
            insert_node(&root, key);
            break;

        case 'r':
            printf("\n+ Removing node - key: %d\n", key);
            remove_node(&root, key);
            break;

        case 'f':
            printf("\n+ Searching for node - key: %d\n", key);
            find_node(root, key);
            break;

        case 'n':
            printf("\n+ Finding nodes next to - key: %d\n", key);
            next_node(root, key);
            prev_node(root, key);
            break;

        case 'p':
            print_level_order(root);
            break;

        case 'q':
            printf("\n+ Freeing memory...\n");
            delete_tree(root);
            printf("+ Memory freed, quitting\n");
            return 0;
            break;
        default:
            break;
        }
    }
}
