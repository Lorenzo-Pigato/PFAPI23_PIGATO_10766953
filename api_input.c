// Code to read user input

#include <stdio.h>

int main()
{
    while (1)
    {
        char istruzione[20];     // per contenere la stringa di istruzione
        int num_arg;             // per contenere il numero di argomenti
        char argomenti[100][20]; // per contenere gli argomenti

        // Leggi l'input dall'utente
        int result = scanf("%s %d", istruzione, &num_arg);

        // Verifica se il file di input è terminato o se c'è stato un errore
        if (result == EOF)
        {
            break; // Esci dal ciclo se il file di input è terminato
        }
        else if (result != 2)
        {
            printf("Errore: input non valido.\n");
            return 1;
        }

        // Verifica se la stringa di istruzione è valida
        if (strcmp(istruzione, "aggiungi-stazione") != 0 &&
            strcmp(istruzione, "aggiungi-auto") != 0 &&
            strcmp(istruzione, "pianifica-percorso") != 0 &&
            strcmp(istruzione, "rottama-auto") != 0 &&
            strcmp(istruzione, "demolisci-stazione") != 0)
        {
            printf("Errore: istruzione non valida.\n");
            return 1;
        }

        // Verifica se il numero di argomenti è valido
        if (num_arg < 0 || num_arg > 100)
        {
            printf("Errore: numero di argomenti non valido.\n");
            return 1;
        }

        // Leggi gli argomenti
        for (int i = 0; i < num_arg; i++)
        {
            scanf("%s", argomenti[i]);
        }

        // Esegui l'azione corrispondente all'istruzione con gli argomenti
        if (strcmp(istruzione, "aggiungi-stazione") == 0)
        {
            if (num_arg == 2)
            {
                // Esegui l'azione per aggiungere una stazione con due argomenti
            }
            else
            {
                printf("Istruzione non eseguita: numero di argomenti non corretto.\n");
            }
        }
        else if (strcmp(istruzione, "aggiungi-auto") == 0)
        {
            if (num_arg == 3)
            {
                // Esegui l'azione per aggiungere un'auto con tre argomenti
            }
            else
            {
                printf("Istruzione non eseguita: numero di argomenti non corretto.\n");
            }
        }
        else if (strcmp(istruzione, "pianifica-percorso") == 0)
        {
            if (num_arg == 1)
            {
                // Esegui l'azione per pianificare un percorso con un argomento
            }
            else
            {
                printf("Istruzione non eseguita: numero di argomenti non corretto.\n");
            }
        }
        // Continua con gli altri casi per le altre istruzioni...
    }

    return 0;
}

#include <stdio.h>

int main(void)
{
    char buffer [18];
    int base_arg = 0;
    int input_arg [512];

    while (scanf("%s", buffer) == 1){                       // Data collected, no EOF
        switch (buffer[0]) {
            case 'a':
                
                int i = 0;
                while (buffer[i] != '-') i++;
                
                if (buffer[i+1] == 's'){                    // Add a station
                    if(scanf("%d", &base_arg))              // Get number of expected args        
                        
                            if(!scanf("%d", &input_arg[i])){
                                printf("Non-aggiunta\n");
                                break;
                            }
                        }
                        
                    
                };
                else if (buffer[i+1] == 'a') add_car(); 
                break;

            case 'd': 
                "demolita\n"
                break;

            case 'r': 
                "rottamata\n"
                break;

            case 'p':
                "pianifica\n"
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