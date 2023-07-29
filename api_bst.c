//This program will be used to implement a bst and test its functionalities

#include <stdio.h>
#include <stdlib.h>

typedef struct bst_node {
    int key;
    struct bst_node* left_child;
    struct bst_node* right_child;
} node_t;

void insert_node(node_t** root, int key){
    if(*root == NULL) {                                                      //If BST is empty, new inserted key is the actual Root
        node_t* node = (node_t*)malloc(sizeof(node_t));
        
        node->key = key;
        node->left_child = NULL;
        node->right_child = NULL;

        *root = node;
        printf("+ Node inserted correctly\n+ Node memory address: %p\n\n", (void*)node);
    }
    else {
        if (key > (*root)->key){
            printf("+ Right branch taken\n");
            insert_node( &((*root)->right_child), key);
        }
        else if (key < (*root)->key){
            printf("+ Left branch taken\n");
            insert_node( &((*root) ->left_child), key);
        }
        else
            printf("+ Found another node with same key\n+ Node not inserted\n\n");
    }
}

node_t* get_node (node_t* root, int key){
    if (root == NULL){
        printf("+ Node not found\n\n");
        return NULL;
    }
    else if(root->key < key){
        printf("+ Right branch taken\n");
        get_node(root->right_child, key);
    }
    else if(root->key > key){
        printf("+ Left branch taken\n");
        get_node(root->left_child, key);
    }
    else{
        printf("+ Node found\n+ Node address: %p\n\n", (void*)root);
        return root;
    }
}

node_t* get_parent (node_t* root, int key){
    if(root->key == key) return NULL;
    if(root->left_child->key == key || root->right_child->key == key) return root;
    else if (root->key < key) get_parent(root->right_child, key);  
    else if (root->key > key) get_parent(root->left_child, key);
}

node_t* remove_node(node_t* root, int key){
    node_t* node = get_node(root, key);
    if(node->right_child == NULL && node->left_child != NULL){
        node_t* tmp = node->left_child;
        node->key = node->left_child->key;
        node->right_child = node->left_child->right_child;
        node->left_child = node->left_child->left_child;
        free(tmp);
    }
    else if (node->left_child == NULL && node->right_child != NULL){
        node_t *tmp = node->right_child;
        node->key = node->right_child->key;
        node->left_child = node->right_child->left_child;
        node->right_child = node->right_child->right_child;
        free(tmp);
    }
    else if(node->left_child == NULL && node->right_child == NULL){
        node_t* parent = get_parent(root, key);
        if(parent->left_child->key == key){
            parent->left_child == NULL;
            free(node);
        }
        else if (parent->right_child->key == key){
            parent->right_child == NULL;
            free(node);
        }
        else{
            free(node);
        }
    }
    else{
        node_t* left_tree = node->left_child;
        node_t* right_tree = node->right_child;
        
        while(right_tree->left_child != NULL)
            right_tree = right_tree->left_child;
        
        right_tree->left_child = left_tree;

        node_t* tmp = node->right_child;
        
        node->key = node->right_child->key;
        node->left_child = node->right_child->left_child;
        node->right_child = node->right_child->right_child;

        free(tmp);
    }
}

void delete_tree(node_t* root){
    if(root != NULL){
        if (root->left_child != NULL)
        {
            printf("+ Left branch taken\n");
            delete_tree(root->left_child);
        }
        if (root->right_child != NULL)
        {
            printf("+ Right branch taken\n");
            delete_tree(root->right_child);
        }

        free(root);
        printf("+ Node deleted\n");
    }
}


int main(void)
{
    char inst;
    int key;
    node_t* root = NULL;                                                    //Inizialize NULL root pointer

    printf("Instruction list:\n> i - insert\n> r - remove\n> f - find\n> q - quit\n\n");
    
    while (1)    
    {   
        printf("> ");
        scanf(" %c", &inst);
        if (inst != 'q') scanf(" %d", &key);
        
        switch (inst)
        {
        case 'i':
            printf("\n+ Inserting node - key: %d\n", key);
            printf("+ Root address: %p\n", (void*)root);
            insert_node(&root, key);
            break;

        case 'r':
            printf("\n+ Removing node - key: %d\n", key);
            remove_node(root, key);
            break;

        case 'f':
            printf("\n+ Searching for node - key: %d\n", key);
            get_node(root, key);
            break;
        
        case 'q':
            printf("\n+ Freeing memory\n");
            delete_tree(root);
            printf("+ Quitting...\n");
            return 0;
            break;
        default:
            break;
        }
    }
}

