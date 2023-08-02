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

const node_t leaf_s = {-1, black, NULL, NULL, NULL};            //Definition of a NULL LEAF
const node_t* const leaf = &leaf_s; 

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
        printf("+ Node found\n+ Node address: %p\n\n", (void *)root);
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
            while (target->parent != NULL && target != NULL)    // Stop when root is found
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

void check_tree(node root)
{
    // Root has to be black
    if (root->parent == NULL && root->color == red)
    {
        printf("+ Recolored tree's root\n");
        root->color = black;
    }

    printf("+ Tree fixed\n\n");

    // Sibling of parent node is RED and parent has NO CHILD-> RECOLORING
    // RECOLORING inverts colors of PARENT - UNCLE - GRANDPA
    // Must check if other rotations or recoloring is needed ascending to root

    // Sibling of parent node is BLACK -> ROTATION
    // left heavy -> right rotation
    // right heavy -> left rotation
    // left-right sit -> left rotate parent -> right rotate grand parent
    // right-left sit -> right rotate parent -> left rotate grand parent
}

void insert_node (node* root, int key){
    //Every new node inserted starts as red except for root
    
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

    }
}

void remove_node (node* root, int key){                         // MAYBE OPTIMIZABLE WHEN TREE IS  R&B
    node target = find_node(*root, key);

    if (target == NULL)                                         // Target node not found
    {
        printf("+ Node not removed\n\n");
        return;
    }

    // Deleting a leaf-node [1]

    if (target->left_child == NULL && target->right_child == NULL){
        if(target->parent != NULL){                             // Target is not root
            if(target->parent->left_child == target){           // Target is a left child
                printf("+ Left leaf removed\n");
                target->parent->left_child = NULL;              // Resetting parent's child pointer
            }
            else{                                               // Target is a right child
                printf("+ Right leaf removed\n");
                target->parent->right_child = NULL;
            }
        }
        
        if(target->color != red) check_tree(*root);             // Removing a red leaf implies no violation
        
        free(target);
        
        return;
    }

    // Deleting a central node

    node next = next_node(target, key);

    if (target->right_child != next)                            // next node is not target's child and neither an ancestor [2]
    {
        next->left_child = target->left_child;                  // Acquiring target's left child, if any
        target->left_child->parent = next;

        if(next->parent->left_child == next)                    // Next node's parent child pointer deletion
            next->parent->left_child = NULL;
        else
            next->parent->right_child = NULL;

        node tmp = next;                                        // OPTIMIZABLE -> use next

        while(tmp->right_child != NULL)                         // Find max of right subtree
            tmp = tmp->right_child;

        tmp->right_child = target->right_child;
        target->right_child->parent = next;
    }

    else if (next == NULL)                                      // No next element found (left subtree only) [3]
    {
        next = target->left_child;
    }

    else if (next == target->right_child)                       // Next node has no left tree -> Next element is target's right child [4]
    {                       
        next->left_child = target->left_child;                  // Acquiring target's left subtree, if any
    }

    next->parent = target->parent;                              // Acquiring target's parent, if any
    if (target->parent != NULL)
    {
        if (target->parent->left_child == target)               // Changing parent's child pointer to new node
            target->parent->left_child = next;
        else
            target->parent->right_child = next;
    }
    else
    {                                                           // Must change root reference
        *root = next;
        (*root)->parent = NULL;
        printf("+ New root key: %d\n", (*root)->key);
    }

    printf("+ Target Node deleted: %d\n+ Substituted with node: %d\n", target->key, next->key);
    printf("+ Left child: %p\n+ Right child: %p\n+ Parent key: %d\n\n", next->left_child, next->right_child, next->parent == NULL ? -1 : next->parent->key);

    free(target);

    //check_tree(root);                                         // Maintain Red and Black structure
}

/*
list* prev_nodes_in_range (node root, int key, int range){

}

list* next_nodes_in_range(node root, int key, int range){
    if (root == NULL) return NULL;

    node next = root->right_child;

    while (next->left_child != NULL){
        
        next = next->left_child;
    }

    return next;
}
*/

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

/////////////////////////////////////////////////////

int main(void)
{
    char inst;                                                  // Char containing instruction to execute
    int key;  
    int range;                                                  
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

    /*insert_node(&root, 11);
    insert_node(&root, 17);
    insert_node(&root, 14);
    insert_node(&root, 13);
    insert_node(&root, 15);*/

    printf("Instruction list:\n> i - insert\n> r - remove\n> f - find\n> g - graph\n> q - quit\n\n");

    while (1)
    {
        printf("> "); 
        scanf(" %c", &inst);
        if (inst != 'q')
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

        case 'g':
            scanf(" %d", &range);
            printf("\n+ Creating reachability graph for node - key: %d - range: %d\n", key, range);
            next_node(root, key);
            prev_node(root, key);
            //list lst = get_nodes_in_range(root, key, range);
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
