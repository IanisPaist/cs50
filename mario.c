#include <cs50.h>
#include <stdio.h>

void build(int);
void print_hash(int);

int main(void)
{
    int height = 0;
    do 
    {
        height = get_int("Height: \n");    
    }        
    while (height <1 || height > 8);
    
    build(height);
}

void build(int n)
{
    int i = 0;
    int j = 0;
    
    for (i = 1; i <= n; i++)
    {
         int blank_count = n - i;
         for (j = 0; j < blank_count;j++) 
             {
                 printf(" ");
             }
        
            print_hash(i); 
        
            printf("  ");
         
            print_hash(i);
        
    printf("\n");
    }
                
}

void print_hash(int hash_count)
{
    int i = 0;
    for (i = 0; i < hash_count; i++)
            {
                printf("#");
            }
 }
