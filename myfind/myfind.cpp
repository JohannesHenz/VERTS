#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* Include Datei für getopt(); getopt.h geht auch, ist aber älter. */
#include <unistd.h>
#include <string.h>


/* Hilfsfunktion */
void print_usage(char *programm_name)
{
    printf("Usage: %s [-R] [-i] [dateiname]\n\n", programm_name);
    return;
}

/* Entry Point */
int main(int argc, char *argv[])
{
    int c;
    char *current_dateiname;
    char *alle_dateinamen[argc];
    char *programm_name;
    unsigned short Counter_Option_R = 0;
    unsigned short Counter_Option_i = 0;


    programm_name = argv[0];

    while ((c = getopt(argc, argv, "Rif:")) != EOF)
    {
        switch (c)
        {
        case '?':
            fprintf(stderr, "%s error: Unknown option.\n", programm_name);
            print_usage(programm_name);
            exit(1);
            /* Das break ist nach exit() eigentlich nicht mehr notwendig. */
            break;
        case 'i': //case insensitive search 
            Counter_Option_i++;
           // print_usage(programm_name);
            break;
        case 'R': //search recursivly
            Counter_Option_R++;
            //print_usage(programm_name);
            break;
        default:
            /* assert() dient nur zu Debugzwecken und sollte nur dort eingesetzt sein,
               wo etwas sicher niemals passieren darf. 0 ist in C immer gleich "logisch falsch". */
            assert(0);
        }
    }
    if ((Counter_Option_R > 1) || (Counter_Option_i > 1))
    {
        fprintf(stderr, "%s Fehler: Optionen wurden mehrfach verwendet.\n", programm_name);
        exit(1);
    }

    if (optind < argc)
    {
        printf("Target Files: ");
        while (optind < argc)
        {
            current_dateiname = argv[optind];
            printf("%s", current_dateiname);
           alle_dateinamen[optind] = current_dateiname;
            optind++;
        }
        printf("\n");
        
    }
   

    //printf("Es wurden %u Argumente angeben.\n", argc);
    printf("Recursive Search Modus ist");
    if (Counter_Option_R == 0)
    {
        printf(" nicht");
    }
    printf(" gesetzt.\n");

    printf("Case Insensitive Modus ist");
    if (Counter_Option_i == 0)
    {
        printf(" nicht");
    }
    printf(" gesetzt.\n");
    /* In main() sind return() und exit() gleichwertig. */
    return (0);
}