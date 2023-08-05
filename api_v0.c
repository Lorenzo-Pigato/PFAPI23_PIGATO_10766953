#include <stdio.h>

typedef struct station_tree {                                       
    int station_id;                                                 //max-heap size
    int cars;
    station_t* left_child;
    station_t* righ_child;
    carpark_t* station_carpark;                                     //max-heap of station's cars
}station_t;

typedef struct carpark_max_heap {
    int car_id;
    station_t* connects_to;
    carpark_t* first_child;
    carpark_t* second_child;
}carpark_t;

void add_station(void);
void add_car(void);
void rm_station(void);
void rm_car(void);
void calc_path(void);



int main(void)
{
    char buffer [18];

    while (scanf("%s", buffer) == 1){
        switch (buffer[0]) {
            case 'a':
                int i = 0;
                while (buffer[i] != '-') i++;
                if (buffer[i+1] == 's') add_station();
                else if (buffer[i+1] == 'a') add_car(); 
                break;

            case 'd': 
                rm_station();
                break;

            case 'r': 
                rm_car();
                break;

            case 'p':
                calc_path();
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