#include <list.h>
#include <tree.h>
#include <string.h>
#include <stdlib.h>

#ifndef ENCODE_H
#define ENDODE_H

typedef struct bithuff{
    unsigned int bitH;
    unsigned int size;
}BitHuff;

void print(BitHuff table[],int frequency[]);

void build_table(Node* tree_node,BitHuff table[],BitHuff code);
// Achar o tamanho do lixo
int trashsize(int frequency[],BitHuff table[]);

void setFirstByte(FILE *file,int trashSize,int treeSize);

void setSecondByte(FILE *file,int treesize);

void setTree(FILE *file,struct Node *bt);

void printBytes(FILE *fileIn,FILE *fileOut,BitHuff table[],int trashSize);

#endif
