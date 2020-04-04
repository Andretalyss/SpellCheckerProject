#ifndef STRUCT_H
#define STRUCT_H

#include <wchar.h>

#define MAX_WORD 42
#define MAX_BUCKETS 104683

typedef struct node{
    wchar_t word[MAX_WORD];
    struct node *next;

}node;

void Iniciar(node **elementos);
int LerDicionari(node **elementos);
unsigned long FuncaoHash(wchar_t *str);
int FuncaoHash2(const char *word);
int WordCheck(node **elementos, wchar_t *word);
int RemoveAccents(wchar_t *word, int indice, wint_t *caract);

#endif