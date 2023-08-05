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

/* aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n
Aggiunge una stazione, posta alla distanza indicata, dotata di numero-auto, dall’autonomia indicata.
Ad esempio:
aggiungi-stazione 10 3 100 200 300
aggiunge una stazione a distanza 10 dall’inizio dell’autostrada, con un parco di tre veicoli con autonomia
100, 200 e 300 km rispettivamente. Se esiste già una stazione alla distanza indicata, il comando non fa nulla.
Stampa attesa come risposta: aggiunta oppure non aggiunta.
• demolisci-stazione distanza
Rimuove la stazione posta alla distanza indicata, se essa esiste.
Stampa attesa come risposta: demolita oppure non demolita.
• aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere
Se la stazione esiste, aggiunge un’auto alla stessa. È possibile avere più auto con la stessa autonomia.
Stampa attesa come risposta: aggiunta oppure non aggiunta.
• rottama-auto distanza-stazione autonomia-auto-da-rottamare
Rimuove un’automobile dalla stazione indicata, se la stazione esiste ed è dotata di almeno un’automobile
con l’autonomia indicata.
Stampa attesa come risposta: rottamata oppure non rottamata.
• pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo
Richiede di pianificare il percorso con i vincoli sopra indicati.
Stampa attesa come risposta: le tappe in ordine di percorrenza, rappresentate con la distanza delle stazioni
da inizio autostrada, separate da spazi e alla fine seguite da un a-capo. Devono essere incluse partenza e
arrivo; se coincidono la stazione viene stampata una sola volta. Se il percorso non esiste, stampa nessun
percorso. L’azione di*/