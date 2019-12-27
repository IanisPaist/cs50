#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int get_new_number(int k, int original_char);


int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        char numbers[10][1] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
        int n = strlen(argv[1]);
        bool check = false;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < 10; j++)
            {           
                if (argv[1][i] != numbers[j][0])
                {
                    check = false;
                }
                else
                {
                    check = true;
                    break;
                }
            }   
            if (check == false)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }    
        }
    }
    int k = atoi(argv[1]);
 
    //ask user for text
    char *plaintext = get_string("plaintext: ");
    char *chiphertext = malloc(strlen(plaintext) + 1);
    
    //add + k to every letter of user's input
    for (int i = 0, r = strlen(plaintext) + 1; i < r; i++)
    {
        if isalpha(plaintext[i])
        {
            chiphertext[i] = get_new_number(k, plaintext[i]);
        }    
        else
        {
            chiphertext[i] = plaintext[i];
        }    
    }  
    printf("ciphertext: %s\n", chiphertext);
    return 0;
}





int get_new_number(int k, int original_char)
{
    int real_k = 0;
    if (k > 25)
    {
        int k_times = k / 25;
        real_k = k - k_times * 25; //15
    }
        
    else
    {
        real_k = k + 2;
    }

    int startnum = 0;
    int endnum = 0;
        
    if islower(original_char)
    {
        startnum = 97;
        endnum = 122;
    }
    else
    {
        startnum = 65;
        endnum = 90;
    }
    
    if (original_char + real_k - 2 > endnum)
    {
        return startnum - 1 + original_char + real_k - 2 - endnum;    
    }
    else
    {
        return original_char + real_k - 2;
    }
}
