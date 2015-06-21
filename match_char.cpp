#include <stdio.h>
using namespace std;
int match(const char* const,char* const);
/*int main()
{
    char regexp[20], ID[20];
    while(scanf("%s%s", regexp, ID) != EOF)
    {
        printf("regexp is %s and ID is %s\n", regexp, ID);
        if(match(regexp, ID))
            puts("Match");
        else
            puts("Not match");
    }
}*/
int match(const char* const regexp,char* const ID)
{
    if(*regexp == '\0')
        return (*ID == '\0');
    if(*regexp == '?')
    {
        if(*ID == '\0')
            return 0;
        return match(regexp + 1, ID + 1);
    }
    if(*regexp == '*')
    {
        char *position = ID;
        while(*position != '\0')
        {
            if(match(regexp + 1, position))
                return 1;
            position++;
        }
        return match(regexp + 1, position); // *position = '\0' 
    }
    if(*regexp != *ID)
        return 0;
    return match(regexp + 1, ID + 1);
}
