#include <stdio.h>


int main()
{
    char const justArray[] = "justArray";
    char const *justPointer = "justPointer";
    char const *Both[2] = {"Both", "Test"};


    printf("%s\n", justArray);
    printf("%s\n", justPointer);
    printf("%s\n", Both[0]);
    printf("%s\n", Both[1]);

    return 0;
}