#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int key;
    int range;
} node_t;

typedef node_t* node;

typedef struct path_nd
{
    node station;
    
    int clicks;                                                         // Distance from origin
    struct path_nd* link;                                               // Best connection for path
} path_node;


void find_path(path_node* list, int size)
{
    printf("list size: %d\n", size);

    for (int crnt = 0; crnt < size -1; crnt++)
    {
        int next = crnt + 1;

        while(next < size && (list[next].station->key) - (list[crnt].station->key) <= list[crnt].station->range)
        {
            printf("%d %d\n", list[crnt].station->key, list[next].station->key);
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

    printf("Path: ");
    if(list[size-1].link == NULL)
    {
        printf("No path found\n");
        return;
    }

    int path_size = list[size -1].clicks;
    int path [path_size];

    path_node sttn = list[size -1];
    
    for (int i = path_size - 1; i >= 0; i--)
    {
        path[i] = sttn.station->key;
        sttn = *(sttn.link);
    }

    printf("%d ", list[0].station->key);
    for (int i = 0; i < path_size; i++)
        printf("%d ", path[i]);
    
    
}

int main()
{
    node station = NULL;
    path_node path_list [100];
    int i = 0;

    printf("key range\n");
    while (!feof(stdin) && i < 100)
    {
        station = (node)malloc(sizeof(node_t));
        scanf("%d %d", &(station->key), &(station->range));
        path_list[i].station = station;
        path_list[i].clicks = 0;
        path_list[i].link = NULL;

        i++;
    }

    find_path(path_list, i);
    
    free(station);
    return 0;
}
