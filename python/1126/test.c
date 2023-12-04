#include <stdio.h>

int main()
{
    int loopCount;
    scanf("%d", &loopCount);
    for(int y = 1; y<=loopCount; y++)
    {
        for(int x = 1; x<=y; x++)
        {
            printf('*');
        }
        printf('\n');
    }
}