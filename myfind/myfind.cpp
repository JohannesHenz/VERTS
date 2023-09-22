#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* Include Datei für getopt(); getopt.h geht auch, ist aber älter. */
#include <unistd.h>
#include <string.h>

/* Verbose Flag wird global gesetzt, damit der komplette Code es sehen kann. */
//unsigned short opt_verbose = 0;

/* Hilfsfunktion */
void print_usage(char *programm_name)
{
    printf("Usage: %s [-h] [-v] [-f dateiname]\n\n", programm_name);
    return;
}

/* Entry Point */
int main(int argc, char *argv[])
{
    int c;
    char *dateiname;
    char *programm_name;
    unsigned short Counter_Option_R = 0;
    unsigned short Counter_Option_i = 0;

    programm_name = argv[0];

    while ((c = getopt(argc, argv, "hvf:")) != EOF)
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
            Counter_Option_R++;
            print_usage(programm_name);
            exit(0);
            break;
        case 'R': //search recursivly
            Counter_Option_i++;
           // opt_verbose = 1;
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
        printf("ARGV Elemente ohne Optionen: ");
        while (optind < argc)
        {
            printf("%s ", argv[optind++]);
        }
        printf("\n");
    }

    /* if ( optind >= argc ) {
        fprintf(stderr, "Fehler: Es wurden Optionen, die Argumente erwarten, ohne Argument angegeben.\n");
        exit(EXIT_FAILURE);
    } */

    printf("Es wurden %u Argumente angeben.\n", argc);
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