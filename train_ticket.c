#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max_seat 5
#define n_lenth 50
int seats[max_seat];
typedef struct pass{
    char name[n_lenth];
    int seat;
    struct pass *next;
} pass;
pass *booked=NULL;
typedef struct wait {
    char name[n_lenth];
    struct wait *next;
} wait;
wait *front=NULL,*rear = NULL;
typedef struct cancel {
    char name[n_lenth];
    int seat;
    struct cancel*next;
} cancel;
cancel *top=NULL;
void add_booked(char name[],int seat) {
    pass *new_name=malloc(sizeof(pass));
    strcpy(new_name->name,name);
    new_name->seat=seat;
    new_name->next=booked;
    booked=new_name;
}
void remove_booked(int seatNumber,char passengerName[]) {
    pass *previousNode=NULL;
    pass *currentNode=booked;
    while (currentNode){
        if (currentNode->seat==seatNumber) {
            strcpy(passengerName,currentNode->name);
            if (previousNode)
            previousNode->next=currentNode->next;
            else{
            booked=currentNode->next;
            free(currentNode);
            return;
            }
        }
        previousNode=currentNode;
        currentNode=currentNode->next;
    }
}
void add_waiting(char name[]) {
    wait *new_name = malloc(sizeof(wait));
    strcpy(new_name->name, name);
    new_name->next = NULL;
    if (rear) rear->next = new_name;
    else front = new_name;
    rear = new_name;
}
void remove_waiting(char name[]) {
    if (!front) return;
    wait *temp = front;
    strcpy(name, temp->name);
    front = front->next;
    if (!front) rear = NULL;
    free(temp);
}
// stack use kora hoiche
void push_cancel(char name[],int seat) {
    cancel*new_name=malloc(sizeof(cancel));
    strcpy(new_name->name,name);
    new_name->seat=seat;
    new_name->next=top;
    top = new_name;
}
// stack er pop function banao hoise seat er index cancel korar jonno
int pop_cancel(char name[]){
    if (!top)
    return -1;
    cancel*temp=top;
    strcpy(name,temp->name);
    int seat=temp->seat;
    top=top->next;
    free(temp);
    return seat;
}
void book_ticket(){
    char name[n_lenth];
    printf("Enter name: ");
    fgets(name,n_lenth,stdin);
    name[strcspn(name,"\n")]=0;
    int seat=-1;
    for (int i=0;i<max_seat;i++) {
        if(seats[i]==0){
            seat=i+1;
            seats[i]=1;
            break;
        }
    }
    if (seat != -1) {
        add_booked(name, seat);
        printf("Seat %d booked for %s\n", seat, name);
    } else {
        add_waiting(name);
        printf("No seats available, %s added to waiting list\n", name);
    }
}
void cancel_ticket(){ // user seat number dibe nijer ta dekhe
    int seat;
    printf("Enter seat number to cancel: ");
    scanf("%d",&seat);
    getchar();
    if (seat < 1||seat>max_seat||seats[seat-1]==0) {
        printf("Invalid seat number\n");
        return;
    }
    char name[n_lenth];
    remove_booked(seat, name);
    seats[seat - 1] = 0;
    push_cancel(name, seat);
    printf("Seat %d cancelled for %s\n", seat, name);
    if (front){
        char wname[n_lenth];
        remove_waiting(wname);
        seats[seat - 1] = 1;
        add_booked(wname, seat);
        printf("%s moved from waiting list to seat %d\n", wname, seat);
    }
}
void restore_ticket(){
    char name[n_lenth];
    int seat = pop_cancel(name);
    if (seat==-1) {
        printf("No cancelled tickets to restore\n");
        return;
    }
    if (seats[seat-1]==0) {
        seats[seat-1]=1;
        add_booked(name, seat);
        printf("Seat %d restored for %s\n",seat,name);
    } else {
        printf("Seat %d is already taken, cannot restore\n", seat);
    }
}
void display_booked() {
    pass*currentPassenger=booked;
    if(!currentPassenger) {
        printf("No booked passengers\n");
        return;
    }
    printf("Booked Passengers:\n");
    while (currentPassenger) {
        printf("Seat %d: %s\n",currentPassenger->seat,currentPassenger->name);
        currentPassenger=currentPassenger->next;
    }
}

void display_waiting() {
    wait *currentWaitingPassenger = front;
    if (!currentWaitingPassenger) {
    printf("No one in waiting list\n");
    return;
    }
    printf("Waiting List:\n");
    while (currentWaitingPassenger) {
    printf("%s\n", currentWaitingPassenger->name);
    currentWaitingPassenger = currentWaitingPassenger->next;
    }
}
// User Menu
int main() {
    int choice;
    for (int i = 0; i < max_seat; i++)
    seats[i]=0;// initial seat 0 theke 4 porjonto
    while (1) {// user er kache nicher printf er vitorer message gulo show korbe
    printf("\n\t\t\t\t\tTrain Ticket Reservation \n");
    printf("\t\t\t\t\t1. Book Ticket\n");
    printf("\t\t\t\t\t2. Cancel Ticket\n");
    printf("\t\t\t\t\t3. Restore Cancelled Ticket\n");
    printf("\t\t\t\t\t4. Display Booked Passengers\n");
    printf("\t\t\t\t\t5. Display Waiting List\n");
    printf("\t\t\t\t\t0. Exit\n");
    printf("\t\t\t\t\tEnter choice: ");
    scanf("%d", &choice);
    getchar();// getchar user kora hoise scanf er por option choose korar jonno jeno space thake.
        switch (choice) {//akekta function switch case diye define kora hoise
            case 1: book_ticket(); break;
            case 2: cancel_ticket(); break;
            case 3: restore_ticket(); break;
            case 4: display_booked(); break;
            case 5: display_waiting(); break;
            case 0: exit(0);
            default: printf("Invalid choice\n");
        }
    }
}
