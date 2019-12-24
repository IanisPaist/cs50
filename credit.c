#include <stdio.h>
#include <cs50.h>

int count_digits();
int number_toarray();

int main(void)
{
    long card_number = get_long_long("input here: \n");
    int number_count = count_digits(card_number);
    int check_sum = 0;
    
    // check amex = 15, mc = 16, visa = 13 or 16
    // anything else = invalid
    
    if (number_count != 15 && number_count != 13 && number_count != 16 ) 
    {
        printf("INVALID\n");
    }
    else
    {
        int card_number_array[number_count];
        int i = 0;
        int j = 0;
        for (i=0;i<number_count;i++)
        {
           card_number_array[i] = card_number % 10;
           card_number = card_number / 10;
        }
        
        int count_other_digits = number_count / 2;
        int multiplied_by_two[count_other_digits];
        int not_multiplied[number_count - count_other_digits];
        for (i=1, j = 0; i < number_count;i = i+2, j++)
        {
            multiplied_by_two[j] = card_number_array[i] * 2; 
        }
        
        for(i=0,j=0; i<number_count; i = i+2, j++)
        {
            not_multiplied[j] = card_number_array[i];
        }
        
        int sum_of_multiplied = 0;
            for (i=0; i < count_other_digits; i++)
            {
                if (multiplied_by_two[i] % 10 > 1 || multiplied_by_two[i] == 10)
                {
                    int left_side = multiplied_by_two[i] / 10;
                    int right_side = multiplied_by_two[i] % 10;
                    sum_of_multiplied = sum_of_multiplied + left_side + right_side;
                    
                }
                else
                {
                    sum_of_multiplied = sum_of_multiplied + multiplied_by_two[i]; 
                    
                }
            }
                int sum_of_not_multiplied = 0;
                for (i=0; i < (number_count - count_other_digits);i++)
                {
                    sum_of_not_multiplied = sum_of_not_multiplied + not_multiplied[i];
                }
        
        int total_sum = sum_of_multiplied + sum_of_not_multiplied;
        printf("%i\n", total_sum);
        if (total_sum % 10 != 0)
        {
            printf("INVALID\n");
        }
        else
        {
            if(number_count == 15 && card_number_array[number_count-1] == 3 && (card_number_array[number_count-2] == 4 || card_number_array[number_count-2] == 7) )
            {
                printf("AMEX\n");
            }
            else if (number_count == 13 || (number_count == 16 && card_number_array[number_count-1] == 4))
            {
                printf("VISA\n");
            }
            else if (number_count == 16 && card_number_array[number_count-1] == 5 && (card_number_array[number_count-2] == 1 || card_number_array[number_count-2] == 2 || card_number_array[number_count-2] == 3 || card_number_array[number_count-2] == 4 || card_number_array[number_count-2] == 5) )
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }    
        
    }
    
}


int count_digits(long card_number)
{
    int count = 0;
    while (card_number != 0)
    {
        card_number = card_number / 10;
        count = count + 1;
    }
    return count;
}
