#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Order {
    int orderID;
    char customerName[50];
    char cakeType[20];
    int weight;
    int extrasCount;
    char extras[5][30];
    char message[100];
    float cakePrice, extrasPrice, totalPrice;
    struct Order *next;
};

struct Order *head = NULL;
int orderCount = 0;

/* Cake base price for 500g */
int getBasePrice(char cake[]) {
    if (strcmp(cake, "vanilla") == 0) return 300;
    if (strcmp(cake, "chocolate") == 0) return 300;
    if (strcmp(cake, "butterscotch") == 0) return 400;
    if (strcmp(cake, "strawberry") == 0) return 400;
    if (strcmp(cake, "pineapple") == 0) return 500;
    if (strcmp(cake, "red velvet") == 0) return 600;
    if (strcmp(cake, "caramel") == 0) return 800;
    if (strcmp(cake, "pista") == 0) return 900;
    return -1;
}

/* Cake list */
void showCakeList() {
    printf("\nAvailable Cakes (500g base price):\n");
    printf("1. Vanilla        - ₹300\n");
    printf("2. Chocolate      - ₹300\n");
    printf("3. Butterscotch   - ₹400\n");
    printf("4. Strawberry    - ₹400\n");
    printf("5. Pineapple      - ₹500\n");
    printf("6. Red Velvet     - ₹600\n");
    printf("7. Caramel        - ₹800\n");
    printf("8. Pista          - ₹900\n");
}

/* Poppers list */
void showPoppers() {
    printf("\nAvailable Poppers (₹50 each):\n");
    printf("1. Happy Birthday\n");
    printf("2. Congratulations\n");
    printf("3. Anniversary\n");
    printf("4. Best Wishes\n");
    printf("5. Custom Text\n");
}

/* Print receipt */
void printReceipt(struct Order *o) {
    printf("\n====================================");
    printf("\n        CAKE SHOP RECEIPT");
    printf("\n====================================");
    printf("\nOrder ID   : %d", o->orderID);
    printf("\nCustomer   : %s", o->customerName);
    printf("\nCake Type  : %s", o->cakeType);
    printf("\nWeight     : %d grams", o->weight);
    printf("\nMessage    : %s", o->message);

    printf("\n\nPoppers:");
    if (o->extrasCount == 0)
        printf(" None");
    else
        for (int i = 0; i < o->extrasCount; i++)
            printf("\n  - %s", o->extras[i]);

    printf("\n\nCake Price    : ₹%.2f", o->cakePrice);
    printf("\nPoppers Price : ₹%.2f", o->extrasPrice);
    printf("\n------------------------------------");
    printf("\nTOTAL AMOUNT  : ₹%.2f", o->totalPrice);
    printf("\n====================================\n");
}

/* Create order */
struct Order* createOrder() {
    struct Order *o = (struct Order*)malloc(sizeof(struct Order));
    int basePrice, choice;
    char msgChoice[5];

    o->orderID = ++orderCount;

    printf("Enter customer name: ");
    scanf(" %[^\n]", o->customerName);

    do {
        showCakeList();
        printf("Enter cake type (name): ");
        scanf(" %[^\n]", o->cakeType);
        basePrice = getBasePrice(o->cakeType);
        if (basePrice == -1)
            printf("Invalid cake type!\n");
    } while (basePrice == -1);

    do {
        printf("Enter cake weight in grams (min 500): ");
        scanf("%d", &o->weight);
    } while (o->weight < 500);

    do {
        printf("Enter number of poppers (0–5): ");
        scanf("%d", &o->extrasCount);
    } while (o->extrasCount < 0 || o->extrasCount > 5);

    showPoppers();

    for (int i = 0; i < o->extrasCount; i++) {
        printf("Select popper %d (1–5): ", i + 1);
        scanf("%d", &choice);

        switch (choice) {
            case 1: strcpy(o->extras[i], "Happy Birthday"); break;
            case 2: strcpy(o->extras[i], "Congratulations"); break;
            case 3: strcpy(o->extras[i], "Anniversary"); break;
            case 4: strcpy(o->extras[i], "Best Wishes"); break;
            case 5:
                printf("Enter custom text: ");
                scanf(" %[^\n]", o->extras[i]);
                break;
            default:
                strcpy(o->extras[i], "Happy Birthday");
        }
    }

    printf("Do you want to write something on the cake? (yes/no): ");
    scanf(" %s", msgChoice);

    if (strcmp(msgChoice, "yes") == 0) {
        printf("Enter cake message: ");
        scanf(" %[^\n]", o->message);
    } else {
        strcpy(o->message, "No message");
    }

    o->cakePrice = (o->weight / 500.0) * basePrice;
    o->extrasPrice = o->extrasCount * 50;
    o->totalPrice = o->cakePrice + o->extrasPrice;

    o->next = NULL;
    return o;
}

/* Place order */
void placeOrder() {
    struct Order *o = createOrder();

    if (head == NULL)
        head = o;
    else {
        struct Order *t = head;
        while (t->next != NULL)
            t = t->next;
        t->next = o;
    }

    printf("\n✅ Order placed successfully!\n");
    printReceipt(o);
}

/* Display ALL orders */
void displayOrders() {
    if (head == NULL) {
        printf("\nNo orders placed yet.\n");
        return;
    }

    struct Order *t = head;
    while (t != NULL) {
        printReceipt(t);
        t = t->next;
    }
}

/* Cancel order */
void cancelOrder() {
    int id;
    printf("Enter Order ID to cancel: ");
    scanf("%d", &id);

    struct Order *t = head, *p = NULL;

    while (t != NULL && t->orderID != id) {
        p = t;
        t = t->next;
    }

    if (t == NULL) {
        printf("Order not found.\n");
        return;
    }

    if (p == NULL)
        head = t->next;
    else
        p->next = t->next;

    free(t);
    printf("Order cancelled successfully.\n");
}

/* Main */
int main() {
    int choice;

    do {
        printf("\n=== CAKE ORDERING SYSTEM ===");
        printf("\n1. Place Order");
        printf("\n2. Display Orders");
        printf("\n3. Cancel Order");
        printf("\n4. Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: placeOrder(); break;
            case 2: displayOrders(); break;
            case 3: cancelOrder(); break;
            case 4: printf("Thank you!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 4);

    return 0;
}