#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main()
{
    int user_choice;
    printf("Hello!");
    printf("\n Choose the file to be indexed\n");
    printf("\n 0. Small File \n 1. Normal file\n 2. Big file\n 3. Much Bigger File\n");
	printf("Your Choice: ");
    scanf("%d",&user_choice);
    switch(user_choice)
    {
        case 0: fileHandle("snow0.txt"); break;
        case 1: fileHandle("snow1.txt"); break;
        case 2: fileHandle("snow2.txt"); break;
        case 3: fileHandle("snow3.txt"); break;
        default: printf("Invalid choice");
    }


    return 0;
}
