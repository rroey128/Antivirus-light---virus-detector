
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char *scannedFileName;

typedef struct virus
{
    unsigned short SigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

typedef struct link link;
struct link
{
    link *nextVirus;
    virus *vir;
};

void PrintHex(char *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        fprintf(stdout, "%02X ", (unsigned char)buffer[i]);
    }
    printf("\n");
}

virus *readVirus(FILE *file)
{
    char arr[2];
    fread(arr, 1, 2, file); 
    virus *nextVirus = malloc(sizeof(virus));
    nextVirus->SigSize = *(short *)arr;
    fread(nextVirus->virusName, 1, sizeof(nextVirus->virusName), file);
    nextVirus->sig = malloc(nextVirus->SigSize);
    if (fread(nextVirus->sig, 1, nextVirus->SigSize, file) != nextVirus->SigSize)
    { // make sure we read all the bytes
        free(nextVirus->sig);
        free(nextVirus);
        return NULL;
    }
    return nextVirus;
}

void printVirus(virus *virus, FILE *output)
{
    fprintf(output, "Virus name: %s\n", virus->virusName);
    fprintf(output, "Virus size: %d\n", virus->SigSize);
    fprintf(output, "signature: \n");
    for (int i = 0; i < virus->SigSize; i++)
    {
        fprintf(output, "%02X ", virus->sig[i]);
    }
    fprintf(output, "\n");
}

void list_print(link *virus_list, FILE *output)
{
    link *next = virus_list;
    while (next != NULL)
    {
        printVirus(next->vir, output);
        next = next->nextVirus;
    }
}

void list_printWrapper(link **virus_list)
{
    list_print(*virus_list, stdout);
}

link *list_append(link *virus_list, virus *data)
{
    link *head = (link *)malloc(sizeof(link));
    if (virus_list == NULL)
    {
        head->vir = data;
        head->nextVirus = NULL;
    }
    else
    {
        head->vir = data;
        head->nextVirus = virus_list;
    }
    return head;
}

void list_free(link *virus_list)
{
    if (virus_list == NULL)
    {
        return;
    }
    list_free(virus_list->nextVirus);
    free(virus_list->vir->sig);
    free(virus_list->vir);
    free(virus_list);
}

void quit(link **head)
{
    list_free(*head);
    exit(0);
}

void loadSignatures(link **head)
{
    char fileName[100];
    FILE *file;

    printf("please enter a file name : \n");
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    file = fopen(fileName, "r+");
    if (file == NULL)
    {
        printf("ERROR - failed to load file\n");
        return;
    }

    char arr[5];
    fread(arr, 1, 4, file);
    arr[4] ='\0';
    char *toCompare = "VISL";
    if (strcmp(arr, toCompare) == 0) {

    while (1)
    {
        virus *nextVirus = readVirus(file);
        if (nextVirus == NULL)
            break;
        *head = list_append(*head, nextVirus);
    }
    fclose(file);
    }

    else{
        printf("this isn't the correct file !");
        fclose(file);
    }
        
}

void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
    for (link *curr = virus_list; curr != NULL; curr = curr->nextVirus)
    {
        for (int i = 0; i < size; i++)
        {
            if (memcmp(buffer + i, curr->vir->sig, curr->vir->SigSize) == 0)
            {
                printf("Virus found at byte %u\n", i);
                printf("Virus name: %s\n", curr->vir->virusName);
                printf("Virus size: %d\n", curr->vir->SigSize);
            }
        }
    }
}

void deteceVirus(link **head)
{
    char buffer[10000];
    FILE *file = fopen(scannedFileName, "r+");
    if (file == NULL)
    {
        printf("ERROR - failed to load file\n");
        return;
    }
    int size = fread(buffer, 1, 10000, file);
    detect_virus(buffer, size, *head);
    fclose(file);
}

void neutralize_virus(char *fileName, int signatureOffset)
{
    FILE *file = fopen(fileName, "r+");
    if (file == NULL)
    {
        printf("ERROR - failed to load file");
        return;
    }
    fseek(file, signatureOffset, SEEK_SET);
    fwrite("\xC3", 1, 1, file);
    fclose(file);
}

void fixFile(link **head)
{
    char buffer[10000];
    FILE *file = fopen(scannedFileName, "r+");
    if (file == NULL)
    {
        printf("ERROR - failed to load file\n");
        return;
    }
    int size = fread(buffer, 1, sizeof(buffer), file);
    for (link *curr = *head; curr != NULL; curr = curr->nextVirus)
    {
        for (int i = 0; i < size; i++)
        {
            if (memcmp(buffer + i, curr->vir->sig, curr->vir->SigSize) == 0)
            {
                neutralize_virus(scannedFileName, i);
            }
        }
    }
    fclose(file);
}

struct fun_desc
{
    char *name;
    void (*fun)(link **head);
};

struct fun_desc menu[] = {
    {"Load signatures", loadSignatures},
    {"Print signatures", list_printWrapper},
    {"Detect viruses", deteceVirus},
    {"Fix file", fixFile},
    {"Quit", quit},
    {NULL, NULL}};

int main(int argc, char **argv)
{
    scannedFileName = argv[1];
    link *head = NULL;

    while (1)
    {
        printf("Please choose a function (ctrl^D for exit):\n");
        int i = 0;
        while (menu[i].name != NULL)
        {
            printf("%d) %s\n", i, menu[i].name);
            i++;
        }

        char input[100];
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;
        int num = atoi(input);
        printf("option : %d\n\n", num);
        if (num >= 0 && num <= i)
        {
            printf("Within bounds\n");
            menu[num].fun(&head);
            printf("DONE.\n\n");
        }
        else
        {
            printf("Not within bounds\n");
            break;
        }
    }

    return 0;
}