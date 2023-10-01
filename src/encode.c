#include <stdio.h>
#include <list.h>
#include <tree.h>
#include <encode.h>

void print(BitHuff table[],int frequency[]){
    for(int i = 0; i < 256;i++){
        if(frequency[i] > 0){
            printf("\n%c %d\n",i,table[i].bitH);
        } 
    }
}
void build_table(Node* tree_node,BitHuff table[],BitHuff code)
{         
    if(isLeaf(tree_node))
    {
        table[(int) getByteFromVoidPointer(tree_node->byte)] = code;
        return;
    }
    else
    {
        code.size++;
        code.bitH <<= 1;
        if(tree_node->left != NULL)
        build_table(tree_node->left, table, code);
        code.bitH++;
        if(tree_node->right != NULL)
        build_table(tree_node->right, table, code);
    }        
}
int is_bit_i_set(unsigned char c, int i)
{
unsigned char mask = 1 << i;
return mask & c;
}

int trashsize(int frequency[],BitHuff table[]){
    long long int totalBits = 0;
    for(int i = 0;i < 256;i++){
        if(frequency[i] == 0) continue;
        totalBits += frequency[i] * table[i].size;
    }
    int trash = (8 - (totalBits % 8));
    return trash;
}

void setFirstByte(FILE *file,int trashSize,int treeSize){
    unsigned char byte_completo = 0x00;
    byte_completo = byte_completo | (trashSize << 5);
    byte_completo = byte_completo | (treeSize >> 8);
    fwrite(&byte_completo,sizeof(unsigned char),1,file);
}
void setSecondByte(FILE *file,int treesize){
    unsigned char byte_completo = 0x00;
    byte_completo = byte_completo | treesize;
    fwrite(&byte_completo,sizeof(unsigned char),1,file);
}
void setTree(FILE *file,struct Node *bt)
{
    if (bt != NULL) {
    unsigned char x = getByteFromVoidPointer(bt->byte);
    if(((x == '*') || (x == '\\')) && isLeaf(bt)){
        unsigned char y = '\\';
        fwrite(&y,sizeof(unsigned char),1,file);
    }
    fwrite(&x,sizeof(unsigned char),1,file);
    setTree(file,bt->left);
    setTree(file,bt->right);
    }
}
void printBytes(FILE *fileIn,FILE *fileOut,BitHuff table[],int trashSize){
    short buffer_size = 0, caractere;
    unsigned char buffer = 0; 
    unsigned short int aux, code_tmp = 0;
    while((caractere = getc(fileIn)) != EOF)
    {        
        if((table[caractere].size + buffer_size) <= 8)
        {
            buffer <<= table[caractere].size;
            buffer = buffer | table[caractere].bitH;
            buffer_size += table[caractere].size;
        }        
        else 
        {
            aux = table[caractere].size;                         
            buffer <<= (8 - buffer_size);
            code_tmp = table[caractere].bitH >> (table[caractere].size - (8 - buffer_size));
            aux -= (8 - buffer_size);            
            buffer = buffer | code_tmp;            
            fprintf(fileOut, "%c", buffer);
            buffer = 0;
            buffer_size = aux;                
            code_tmp = table[caractere].bitH << (16 - buffer_size);
            code_tmp >>= (16 - buffer_size);
            buffer = code_tmp;                       
        }
        
        if(buffer_size == 8)
        {            
            fprintf(fileOut, "%c", buffer);
            buffer = 0;
            buffer_size = 0;
        }
    }    

    if(buffer_size > 0 && buffer_size < 8)
    {         
        buffer <<= trashSize;        
        fprintf(fileOut, "%c", buffer);
        buffer = 0;
        buffer_size = 0;
    }
}
