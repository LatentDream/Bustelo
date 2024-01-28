#include <stdio.h>
#include <stdlib.h>


struct node {
    int data;
    struct node *next;
};


void createList(int n, struct node **head);
void display(struct node * head);


int main(int argc, char *argv[]) {
    int n;
    struct node* head = NULL;

    printf("Enter the total number of nodes:");
    scanf("%d", &n);

    createList(n, &head);

    printf("\nData in the list");
    display(head);

    return EXIT_SUCCESS;
}


void createList(int n, struct node **head) {

    struct node *newNode, *temp;

    int data;

    *head = (struct node *)malloc(sizeof(struct node));

    if(head == NULL) {
        printf("Unable to allocate memory");
        exit(0);
    }

    printf("Enter some data to be stored:");
    scanf("%d", &data);
    

    (*head)->data = data;
    (*head)->next = NULL;

    // Create the followings node
    temp = *head;
    for(int i=2; i<=n; i++) {
        newNode = (struct node *)malloc(sizeof(struct node));
        if(newNode == NULL) {
            printf("Unable to allocate memory");
            break;
        }

        printf("Enter some data to be stored:");
        scanf("%d", &data);

        newNode->data = data;
        newNode->next = NULL;

        temp->next = newNode;
        temp = temp->next;
    }

}


void display(struct node* head) {
    
    struct node *temp;

    // Return if list is empty
    if (head == NULL) {
        printf("\nEmpty list");
        return;
    }

    temp = head;
    while(temp != NULL) {
        printf("Data = %d\n", temp->data);
        temp = temp->next;
    }
}
