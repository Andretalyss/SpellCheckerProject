#include "struct.h"
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Init the array of pointers;
void Iniciar(node **elementos){
    for(int i=0;i<MAX_BUCKETS;i++){
        elementos[i]=NULL;
    }
}

//Hash function djb2;
unsigned long FuncaoHash(wchar_t *word){
	unsigned long hash = 5381;
    int aux;

    while (aux = *word++){
        hash *= 33;
        hash += aux;
    }

    int bucket = hash % MAX_BUCKETS;
    return bucket;
}

//Checking the word in the dict;
int WordCheck(node **elementos, wchar_t *word){
    int bucket = FuncaoHash(word);

    node *aux = elementos[bucket];

    while(aux){
        if(!wcscmp(aux->word, word))
            return 1;
        
            
        aux = aux->next;
    }

    return 0;

}

//Remove Accents;
int RemoveAccents(wchar_t *word, int indice, wint_t *caract){
    word[indice] = *caract;
    wchar_t vogalCom[] = L"áéíóúÁÉÍÓÚãẽĩõũÃẼĨÕŨàÀâêîôûÂÊÎÔÛçÇ";
    wchar_t vogalSem[] = L"aeiouaeiouaeiouaeiouaaaeiouaeioucc";
    
    wchar_t *p;
    p = wcschr(vogalCom, word[indice]);

    if(p!=NULL){
        int ind = -1;
        for(int i=0;i<33;i++){
            if(!wcscmp(&p[0], &vogalCom[i])){
                ind = i;
                break;
             }
        }
    
        if(ind == -1)
            return 0;
    
        *caract = vogalSem[ind];

        return 1;

    }else{ 
        return 0;
    }
}

//Read the dict for the array;
int LerDicionari(node **elementos){
    FILE *dict = fopen("dict2.txt", "r");
    if(!dict)
        return 0;

    wchar_t auxiliar[MAX_WORD];

    while(1){
        fgetws(auxiliar, MAX_WORD, dict);
        if(feof(dict))
            break;

        if(auxiliar[wcslen(auxiliar)-1]=='\n')
            auxiliar[wcslen(auxiliar)-1]='\0';
        
        for(int i=0;i<wcslen(auxiliar);i++){
            auxiliar[i]=tolower(auxiliar[i]);
        }

        int ind = FuncaoHash(auxiliar);

        if(elementos[ind] != NULL){
            node *aux = elementos[ind];

            aux = (node*) malloc(sizeof(node));
            aux->next = elementos[ind];
            elementos[ind] = aux;
            wcscpy(aux->word, auxiliar);
        }else{
            elementos[ind] = (node*) malloc(sizeof(node));
            elementos[ind]->next = NULL;
            wcscpy(elementos[ind]->word, auxiliar);
        }
    }

    fclose(dict);
    return 1;
}
