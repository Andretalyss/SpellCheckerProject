/*
    Developed by: André Talysson (Computer Engineer Student at UFPB);
                  Fernando Marques (Computer Engineer Student at UFPB);
    

    SpellChecking with table hash for the discipline of data structures;
    Colisions solucioned with chaining;

*/

#include "struct.h"
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <locale.h>

//Thread with rusage
#undef getrusage
double calculate(const struct rusage* b, const struct rusage* a);


int main(){
    setlocale(LC_ALL,"");
    node *elementos[MAX_BUCKETS];

    struct rusage before,after;
    double time_load = 0.0;
    double time_to_check = 0.0;

    //Initialize of the array of pointers;
    Iniciar(elementos);

    //Reading the archive with dictionary;
    getrusage(RUSAGE_SELF, &before);
    if(!LerDicionari(elementos))
        return 0;
    getrusage(RUSAGE_SELF, &after);

    time_load = calculate(&before,&after);

    int Fail_Words = 0;
    int Total_Words = 0;
    int ind = 0;

    wint_t caract;
    wchar_t auxiliar[MAX_WORD]; //used because special words occupy 2 bytes of char and transfer to unicode table;
    

    FILE *arq = fopen("textoforcheck.txt", "r");
    if(!arq)
        return 0;

    //Reading text for checking.
    while(caract != WEOF){
        caract = fgetwc(arq);

        //here we look for words in the alphabet,hyphen or single quotes. 
        if(isalpha(caract) ||  ((caract == '-' || caract == '\'') && ind > 0)){
            auxiliar[ind] = caract;
            ind++;

            //If max size of word has been occupied;
            if(ind > MAX_WORD){
                while ((caract = fgetwc(arq)) != WEOF && isalpha(caract));
                ind = 0;
            }
        }
        
        //here we look for numbers and ignore;
        else if(isdigit(caract)){
            while ((caract = fgetwc(arq)) != WEOF && isalnum(caract));
            ind = 0;
        }

        //Here, words can come accentuated, then we check and fix it.
        else if(RemoveAccents(auxiliar,ind,&caract)){
            auxiliar[ind] = caract;
            ind++;
        }

        //If the others if before not true, is because the word is complete, we start the process of checking.
        else if(ind > 0){
            auxiliar[ind] = '\0'; //assign the end caractere of string;
            Total_Words++;

            //transform the words for lowercase;
            for(int i=0;i<wcslen(auxiliar);i++){
                auxiliar[i]=tolower(auxiliar[i]);
            }

            //Is called the function of checking;
            getrusage(RUSAGE_SELF, &before);
            int check = WordCheck(elementos, auxiliar);
            getrusage(RUSAGE_SELF, &after);

            time_to_check += calculate(&before,&after);

            if(!check){
                Fail_Words++;
                //printf("palavras incorretas: %ls\n", auxiliar);
            }

            ind = 0; 
        }

    }

    fclose(arq);
    printf("\nPalavras erradas= %d\nPalavras no texto= %d\n", Fail_Words, Total_Words);
    //printf("Time to read dict = %.0lf miliseconds\n", time_load*1000);
    printf("Time to check the words = %.0lf miliseconds\n\n", time_to_check*1000);

    return 0;
}

double calculate(const struct rusage* b, const struct rusage* a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
        
    }
}