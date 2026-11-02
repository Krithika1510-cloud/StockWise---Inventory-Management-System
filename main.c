/* StockWise - Inventory Management System (C console version)
   Demonstrates product CRUD, Linear Search, Binary Search,
   Merge Sort, and Binary Search Tree (BST).
   Compile: gcc main.c -o stockwise
   Run: ./stockwise
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PRODUCTS 100

typedef struct { int sku; char name[50], category[40]; int quantity; float price; } Product;
typedef struct Node { int sku; struct Node *left, *right; } Node;
Product products[MAX_PRODUCTS];
int count = 0;

int findSKU(int sku) {
    for (int i=0; i<count; i++) if (products[i].sku == sku) return i;
    return -1;
}
void showProduct(Product p) {
    printf("%-7d %-20s %-15s %-8d Rs.%-10.2f Rs.%.2f\n", p.sku, p.name, p.category, p.quantity, p.price, p.quantity*p.price);
}
void display(void) {
    if (!count) { puts("Inventory is empty."); return; }
    printf("\n%-7s %-20s %-15s %-8s %-13s %s\n", "SKU", "Product", "Category", "Qty", "Unit Price", "Total Value");
    for (int i=0; i<count; i++) showProduct(products[i]);
}
void addProduct(void) {
    if (count == MAX_PRODUCTS) { puts("Inventory is full."); return; }
    Product p;
    printf("Enter SKU: "); scanf("%d", &p.sku);
    if (findSKU(p.sku) != -1) { puts("SKU already exists."); return; }
    printf("Enter product name (one word): "); scanf("%49s", p.name);
    printf("Enter category (one word): "); scanf("%39s", p.category);
    printf("Enter quantity: "); scanf("%d", &p.quantity);
    printf("Enter unit price: "); scanf("%f", &p.price);
    if (p.quantity < 0 || p.price < 0) { puts("Quantity and price cannot be negative."); return; }
    products[count++] = p; puts("Product added successfully.");
}
void updateProduct(void) {
    int sku; printf("Enter SKU to update: "); scanf("%d", &sku);
    int i=findSKU(sku); if (i<0) { puts("Product not found."); return; }
    printf("New name (one word): "); scanf("%49s", products[i].name);
    printf("New category (one word): "); scanf("%39s", products[i].category);
    printf("New quantity: "); scanf("%d", &products[i].quantity);
    printf("New unit price: "); scanf("%f", &products[i].price);
    if (products[i].quantity<0 || products[i].price<0) { puts("Invalid quantity or price."); return; }
    puts("Product updated successfully.");
}
void deleteProduct(void) {
    int sku; printf("Enter SKU to delete: "); scanf("%d", &sku);
    int i=findSKU(sku); if (i<0) { puts("Product not found."); return; }
    for (; i<count-1; i++) products[i]=products[i+1];
    count--; puts("Product deleted successfully.");
}
void linearSearch(void) {
    int sku; printf("Enter SKU to search: "); scanf("%d", &sku);
    for (int i=0; i<count; i++) if (products[i].sku==sku) { puts("Product found:"); showProduct(products[i]); return; }
    puts("Product not found.");
}
int cmpSKU(const void *a, const void *b) {
    const Product *x=a, *y=b; return (x->sku>y->sku)-(x->sku<y->sku);
}
void binarySearch(void) {
    Product a[MAX_PRODUCTS]; memcpy(a, products, count*sizeof(Product));
    qsort(a, count, sizeof(Product), cmpSKU);
    int sku; printf("Enter SKU to search: "); scanf("%d", &sku);
    int low=0, high=count-1;
    while (low<=high) {
        int mid=low+(high-low)/2;
        if (a[mid].sku==sku) { puts("Product found:"); showProduct(a[mid]); return; }
        if (a[mid].sku<sku) low=mid+1; else high=mid-1;
    }
    puts("Product not found.");
}
void merge(Product a[], int l, int m, int r) {
    Product temp[MAX_PRODUCTS]; int i=l, j=m+1, k=0;
    while(i<=m && j<=r) temp[k++]=(a[i].sku<=a[j].sku)?a[i++]:a[j++];
    while(i<=m) temp[k++]=a[i++]; while(j<=r) temp[k++]=a[j++];
    for(i=0;i<k;i++) a[l+i]=temp[i];
}
void mergeSort(Product a[], int l, int r) {
    if(l>=r) return; int m=l+(r-l)/2;
    mergeSort(a,l,m); mergeSort(a,m+1,r); merge(a,l,m,r);
}
void sortProducts(void) {
    if(!count) { puts("Inventory is empty."); return; }
    mergeSort(products,0,count-1); puts("Sorted by SKU using Merge Sort:"); display();
}
Node *newNode(int sku) {
    Node *n=malloc(sizeof(Node)); if(!n) { puts("Memory allocation failed."); exit(1); }
    n->sku=sku; n->left=n->right=NULL; return n;
}
Node *insertBST(Node *root,int sku) {
    if(!root) return newNode(sku);
    if(sku<root->sku) root->left=insertBST(root->left,sku);
    else if(sku>root->sku) root->right=insertBST(root->right,sku);
    return root;
}
void inorder(Node *root) { if(root) { inorder(root->left); printf("%d ",root->sku); inorder(root->right); } }
int searchBST(Node *root,int sku) {
    while(root) { if(root->sku==sku) return 1; root=(sku<root->sku)?root->left:root->right; }
    return 0;
}
void freeBST(Node *root) { if(root) { freeBST(root->left); freeBST(root->right); free(root); } }
void bstOperations(void) {
    Node *root=NULL; for(int i=0;i<count;i++) root=insertBST(root,products[i].sku);
    if(!root) { puts("Inventory is empty."); return; }
    printf("BST inorder traversal: "); inorder(root); putchar('\n');
    int sku; printf("Enter SKU to search in BST: "); scanf("%d",&sku);
    printf(searchBST(root,sku)?"SKU found in BST.\n":"SKU not found in BST.\n"); freeBST(root);
}
void dashboard(void) {
    int units=0, low=0; float value=0;
    for(int i=0;i<count;i++) { units+=products[i].quantity; value+=products[i].quantity*products[i].price; if(products[i].quantity<=10) low++; }
    printf("\n======= STOCKWISE DASHBOARD =======\nProducts: %d\nUnits in stock: %d\nInventory value: Rs. %.2f\nLow-stock products: %d\n",count,units,value,low);
}
int main(void) {
    products[0]=(Product){1001,"Notebook","Stationery",50,45.0f};
    products[1]=(Product){1002,"Pen","Stationery",100,10.0f};
    products[2]=(Product){1003,"Mouse","Electronics",15,450.0f};
    products[3]=(Product){1004,"Keyboard","Electronics",8,750.0f};
    products[4]=(Product){1005,"Bottle","Accessories",25,120.0f}; count=5;
    int choice;
    do {
        printf("\n========== STOCKWISE ==========\n1. Dashboard\n2. Display products\n3. Add product\n4. Update product\n5. Delete product\n6. Linear search\n7. Binary search\n8. Merge sort by SKU\n9. BST operations\n0. Exit\nEnter choice: ");
        if(scanf("%d",&choice)!=1) { puts("Invalid input."); break; }
        switch(choice) {
            case 1: dashboard(); break; case 2: display(); break; case 3: addProduct(); break;
            case 4: updateProduct(); break; case 5: deleteProduct(); break; case 6: linearSearch(); break;
            case 7: binarySearch(); break; case 8: sortProducts(); break; case 9: bstOperations(); break;
            case 0: puts("Thank you for using StockWise."); break; default: puts("Invalid choice.");
        }
    } while(choice!=0);
    return 0;
}
