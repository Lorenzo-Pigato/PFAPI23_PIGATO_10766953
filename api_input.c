#include <stdio.h>

#define BUFF_SIZE   20
#define MAX_SIZE    514                                     // Station_id   Car_Number  512 cars

int find_station (void){
    return 0;
}

int add_car (void){
    return 0;
}

int find_car(void){
    return 1;
}

int main()
{
    char buffer [BUFF_SIZE];
    int input [MAX_SIZE];

    while (scanf(" %s", buffer) == 1 && !feof(stdin)){       // Data collected, no EOF
        switch (buffer[0]) {
            case 'a':
                
                int i = 0;
                while (buffer[i] != '-') i++;
                
                if (buffer[i+1] == 's') {                  // Add a station
                    
                    if(scanf(" %d %d", &input[0], &input[1])){          
                        for (i = 0; i < input[1]; i++)
                            if (!scanf("%d", &input[i+2])) return 1;
                    }
                    else return 1;
                    
                    if (find_station()) 
                        printf("non aggiunta\n");
                    else 
                        printf("aggiunta\n");
                
                }
                    
                else if (buffer[i+1] == 'a') {
                    if (!scanf(" %d %d", &input[0], &input[1])) 
                        return 1;

                    if (find_station()){
                        if(add_car())
                            printf("aggiunta\n");
                    }
                    else
                        printf("non aggiunta\n");
                }
                break;

            case 'd':
                if (scanf(" %d", &input[0])){
                    if (find_station())
                        printf("demolita\n");
                    
                    else
                        printf("non demolita\n");
                }
                break;

            case 'r':
                if (scanf(" %d %d", &input[0], &input[1])){
                    if (find_station() && find_car())
                            printf("rottamata\n");
                        
                    else
                        printf("non rottamata\n");
                }
                break;

            case 'p':
                if (scanf(" %d %d", &input[0], &input[1]))
                {
                    if (find_station() && find_station())
                        printf("pianifica\n");

                    else
                        printf("non pianifica\n");
                }
                break;

            default:
                return 1;
            }
    }

    return 0;
}
