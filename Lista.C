#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOK_LENGTH 256
#define MAX_AUTHOR_LENGTH 256


typedef struct {
    char title[MAX_BOOK_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
} Book;


typedef struct Node {
    Book data;
    struct Node* next;
} Node;


Node* createNode(Book book) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Erro ao alocar memória para o novo nó.\n");
        exit(1);
    }
    newNode->data = book;
    newNode->next = NULL;
    return newNode;
}


void displayBook(Book book) {
    printf("Livro: %s; Autor: %s\n", book.title, book.author);
}


void displayQueue(Node* front) {
    Node* current = front;
    while (current != NULL) {
        displayBook(current->data);
        current = current->next;
    }
}


Node* enqueue(Node* front, Book book) {
    Node* newNode = createNode(book);
    if (front == NULL) {
        return newNode;
    } else {
        Node* current = front;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        return front;
    }
}


Node* dequeue(Node* front) {
    if (front == NULL) {
        printf("A fila de livros está vazia.\n");
    } else {
        Node* temp = front;
        front = front->next;
        free(temp);
    }
    return front;
}

void displayStack(Node* top) {
    Node* current = top;
    while (current != NULL) {
        displayBook(current->data);
        current = current->next;
    }
}

Node* push(Node* top, Book book) {
    Node* newNode = createNode(book);
    newNode->next = top;
    return newNode;
}


Node* pop(Node* top) {
    if (top == NULL) {
        printf("A pilha de livros está vazia.\n");
    } else {
        Node* temp = top;
        top = top->next;
        free(temp);
    }
    return top;
}


Node* readBooksFromFile(const char* filename) {
    Node* front = NULL;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        exit(1);
    }
    char line[MAX_BOOK_LENGTH + MAX_AUTHOR_LENGTH + 3];
    while (fgets(line, sizeof(line), file)) {
        Book book;
        char *title, *author;
        title = strtok(line, ";");
        author = strtok(NULL, ";");
        if (title != NULL && author != NULL) {
            strcpy(book.title, title);
            strcpy(book.author, author);
            front = enqueue(front, book);
        }
    }
    fclose(file);
    return front;
}

void writeBooksToFile(const char* filename, Node* front) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        exit(1);
    }
    Node* current = front;
    while (current != NULL) {
        fprintf(file, "%s;%s\n", current->data.title, current->data.author);
        current = current->next;
    }
    fclose(file);
}

int main() {
    Node* queueFront = readBooksFromFile("fila_de_livros.txt");
    Node* stackTop = readBooksFromFile("pilha_de_livros.txt");

    int choice;
    char temp[256]; // Variável temporária para ler títulos e autores

    while (1) {
        printf("\nMenu:\n");
        printf("1. Exibir fila de livros\n");
        printf("2. Inserir novo livro na fila\n");
        printf("3. Inserir novo livro na pilha\n");
        printf("4. Remover livro da fila (FIFO)\n");
        printf("5. Remover livro da pilha (LIFO)\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Fila de Livros:\n");
                displayQueue(queueFront);
                break;
            case 2:
                printf("Inserir novo livro na fila:\n");
                Book newBook;
                printf("Título: ");
                getchar(); // Consumir a nova linha pendente
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = 0; // Remover a nova linha do final
                strncpy(newBook.title, temp, sizeof(newBook.title));

                printf("Autor: ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = 0; // Remover a nova linha do final
                strncpy(newBook.author, temp, sizeof(newBook.author));

                queueFront = enqueue(queueFront, newBook);
                writeBooksToFile("fila_de_livros.txt", queueFront);
                break;
            case 3:
                printf("Inserir novo livro na pilha:\n");
                Book newStackBook;
                printf("Título: ");
                getchar(); // Consumir a nova linha pendente
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = 0; // Remover a nova linha do final
                strncpy(newStackBook.title, temp, sizeof(newStackBook.title));

                printf("Autor: ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = 0; // Remover a nova linha do final
                strncpy(newStackBook.author, temp, sizeof(newStackBook.author));

                stackTop = push(stackTop, newStackBook);
                writeBooksToFile("pilha_de_livros.txt", stackTop);
                break;
            case 4:
                printf("Remover livro da fila (FIFO):\n");
                queueFront = dequeue(queueFront);
                writeBooksToFile("fila_de_livros.txt", queueFront);
                break;
            case 5:
                printf("Remover livro da pilha (LIFO):\n");
                stackTop = pop(stackTop);
                writeBooksToFile("pilha_de_livros.txt", stackTop);
                break;
            case 6:
                printf("Saindo do programa.\n");
                exit(0);
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}