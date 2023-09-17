#include <stdio.h>
#include <stdlib.h>

#define MAX 100

struct Node {
    unsigned char byte;
    int frequency;
    struct Node* next;
};

/* OBJETIVO: Inserir na lista encadeada list a frequencia e o byte
             de forma ordenada na ordem crescente.*/
void insertionFrequency(struct Node** list, int frequency[])
{
    for (int i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            // Criar no
            struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
            newNode->byte = (unsigned char)i;
            newNode->frequency = frequency[i];
            newNode->next = NULL;

            // Inserir de forma ordenada
            if (*list == NULL || frequency[i] < (*list)->frequency)
            {
                newNode->next = *list;
                *list = newNode;
            }
            else
            {
                struct Node* current = *list;
                while (current->next != NULL && frequency[i] >= current->next->frequency)
                {
                    current = current->next;
                }
                newNode->next = current->next;
                current->next = newNode;
            }
        }
    }
}

int main()
{
    FILE *archive;
    char file_path[MAX]; // Arrey que armazena o nome do arquivo
    printf("Digite o caminho da pasta do arquivo: ");
    scanf("%s", file_path);

    // Lista encadeada para armazenar as frequencias em ordem crescente
    struct Node* list_frequency = NULL;

    // Abre o arquivo para leitura
    archive = fopen(file_path, "rb");

    // Caso de erro na leitura do arquivo
    if (archive == NULL)
    {
        perror("Erro ao abrir o arquivo");
    }
    
    unsigned char byte; // Variavel que armazenara cada bit temporariamente
    int frequency[256] = {0}; // Arrey que armazena a frequencia de cada byte

    // Le cada byte do arquivo
    while (fread(&byte, sizeof(unsigned char), 1, archive) == 1)
    {
        frequency[byte]++;
    }

    insertionFrequency(&list_frequency, frequency);

    // Exibir a tabela de frequência
    printf("Tabela de Frequencia:\n");
    for (int i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            printf("Byte 0x%02X: %d vezes\n", i, frequency[i]);
        }
    }
    
    // Fecha o arquivo
    fclose(archive);

    // Exibir a lista encadeada
    printf("\nLista encadeada ordenada das frequencias:\n");
    struct Node* current = list_frequency;
    while (current != NULL)
    {
        printf("[%c, %d] -> ", current->byte, current->frequency);
        current = current->next;
    }
    
    //Liberar a memoria
    struct Node* previour;
    while (list_frequency != NULL)
    {
        previour = list_frequency;
        list_frequency = list_frequency->next;
        free(previour);
    }
    
    return 0;
}