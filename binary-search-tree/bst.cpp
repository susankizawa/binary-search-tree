#include "bst.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include "queue.h"
#include "stack.h"
#include "file_system.h"

Node* findMin(Node* root){
    if(root == NULL)
        return root;

    Node* current = root;

    while(1){
        if(current->left)
            current = current->left;
        else
            break;
    }

    return current;
}

Node* findMax(Node* root){
    if(root == NULL)
        return root;

    Node* current = root;

    while(1){
        if(current->right)
            current = current->right;
        else
            break;
    }

    return current;
}

int getHeightNonRecur(Node* root){
    if(root == NULL)
        return -1;

    Queue* q = createQueue();

    enqueue(q, root);
    int height = -1;

    while(!queueIsEmpty(q)){
        int levelSize = q->size;
        for(int i = 0; i < levelSize; i++){
            Node* current = dequeue(q);
            if(current->left)
                enqueue(q, current->left);
            if(current->right)
                enqueue(q, current->right);
        }
        height++;
    }

    free(q);

    return height;
}

int getHeight(Node* root){
    if(root == NULL)
        return -1;

    return 1 + std::max(getHeight(root->left), getHeight(root->right));
}

void updateHeight(Node* node){
    if(node == NULL)
        return;

    int leftHeight = (node->left)? node->left->height : -1;
    int rightHeight = (node->right)? node->right->height : -1;

    node->height = 1 + std::max(leftHeight, rightHeight);

    return;
}

int getBalanceFactor(Node* root){
    if(root == NULL)
        return 0;

    return (getHeight(root->left) - getHeight(root->right));
}

Node* createNode(int id, File* file){
    if(file == NULL){
        printf("error: file cant be NULL\n");
        return NULL;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->id = id;
    newNode->file = file;
    newNode->height = 0;
    newNode->parent = NULL;
    newNode->left = newNode->right = NULL;

    return newNode;
}

Node* findNode(Node* root, int id){
    Node* current = root;

    while(current != NULL){
        if (id < current->id){
            current = current->left;
        } else if(id > current->id){
            current = current->right;
        } else{
            return current;
        }
    }

    // 404: id was not found!
    return NULL;
}

void printNode(Node* n){
    if(n == NULL){
        printf("[ NULL ]");
        return;
    }

    printf("[\n %d ]");
    printf(" id: %d\n", n->id);
    printf(" file:");
    printFile(n->file);
    printf("\n");
    printf(" parent: ");
    if(n->parent)
        printf("%d", n->parent->id);
    else
        printf("NULL");
    printf("\n");
    printf(" left: ");
    if(n->left)
        printf("%d", n->left->id);
    else
        printf("NULL");
    printf("\n");
    printf(" right: ");
    if(n->right)
        printf("%d", n->right->id);
    else
        printf("NULL");
    printf("\n");
    printf("]\n");

    return;
}

Node* insertNodeNonRecur(Node* root, int id, File* file){
    // create newNode with the defined id to be inserted in the tree
    Node* newNode = createNode(id, file);

    // if root is null, return newNode as the root of the new tree
    if(root == NULL)
        return newNode;

    // goes through the tree looking for a free spot for the new node
    Node* current = root;

    while(1){
        // if id is less than the current node's id move left
        // if it's less move right
        // if it's the same id, return the root of the tree without inserting
        if(id < current->id){
            // if left child exists, current is now the left child, else exit the loop
            if(current->left)
                current = current->left;
            else
                break;
        } else if(id > current->id){
            // if right child exists, current is now the right child, else exit the loop
            if(current->right)
                current = current->right;
            else
                break;
        } else {
            // frees newNode to have no duplicates
            free(newNode);
            return root;
        }
    }

    // inserts new node in the free spot
    newNode->parent = current;
    if(id < current->id){
        current->left = newNode;
    } else {
        current->right = newNode;
    }

    root = balanceTree(root);

    return root;
}

Node* insertNode(Node* root, int id, File* file){
    // if root is NULL, we found the spot for the new node
    if(root == NULL){
        Node* newNode = createNode(id, file);
        return newNode;
    }

    // if id < root's id, go left
    // if id > root's id, go right
    // if id == root's id, do nothing (no duplicates)
    if(id < root->id){
        root->left = insertNode(root->left, id, file);
        // updates parent
        if(root->left->parent == NULL)
            root->left->parent = root;
    } else if(id > root->id){
        root->right = insertNode(root->right, id, file);
        //updates parent
        if(root->right->parent == NULL)
            root->right->parent = root;
    }

    root = balanceTree(root);

    return root;
}

Node* removeNodeNonRecur(Node* root, int id){
    if(root == NULL)
        return root;

    // finds node to be removed and it's parent
    Node* node = findNode(root, id);

    if(node == NULL)
        return root;

    Node* parent = node->parent;

    // Case 1: No children
    if(node->left == NULL && node->right == NULL){
        // if parent exists, update parent
        // else node must be root since it doesnt have a parent, therefore remove root
        if(parent != NULL){
            if(parent->left == node)
                parent->left = NULL;
            else
                parent->right = NULL;
        } else{
            root = NULL;
        }

        delete node->file;

        free(node);
    }
    // Case 2: 1 child
    else if(node->left == NULL || node->right == NULL){
        Node* child = (node->left) ? node->left : node->right;

        child->parent = node->parent;

        // if parent exists, update parent
        // else node must be root since it doesnt have a parent, therefore replace root
        if(parent != NULL){
            if(parent->left == node)
                parent->left = child;
            else
                parent->right = child;
        } else{
            root = child;
        }

        delete node->file;

        free(node);
    }
    // Case 3: 2 children
    else{
        // finds the in-order predecessor (max id in left subtree)and it's parent
        Node* predecessor = findMax(node->left);

        // updates parent
        if(predecessor->parent == node)
            node->left = predecessor->left;
        else
            predecessor->parent->right = predecessor->left;

        // update the left child's parent pointer
        if(predecessor->left != NULL)
            predecessor->left->parent = predecessor->parent;

        node->id = predecessor->id;
        node->file = predecessor->file;

        delete predecessor->file;

        free(predecessor);
    }

    root = balanceTree(root);

    return root;
}

Node* removeNode(Node* root, int id){
    if(root == NULL)
        return root;

    // traverse to find the target node
    if(id < root->id){
        root->left = removeNode(root->left, id);
    }
    else if(id > root->id){
        root->right = removeNode(root->right, id);
    }
    // found the target node - handle deletion based on number of children
    else{
        // Case 1: No children
        if(root->left == NULL && root->right == NULL){
            delete root->file;
            free(root);
            return NULL;
        }
        // Case 2: 1 child
        else if(root->left == NULL || root->right == NULL){
            Node* child = (root->left) ? root->left : root->right;
            child->parent = root->parent;
            delete root->file;
            free(root);
            return child;
        }
        // Case 3: 2 children
        else{
            // find the in-order predecessor (max id in left subtree)
            Node* predecessor = findMax(root->left);

            // replace current node's id with predecessor's id
            root->id = predecessor->id;

            // remove the predecessor from left subtree
            root->left = removeNode(root->left, predecessor->id);
        }
    }

    root = balanceTree(root);

    return root;
}

Node* rotateLeft(Node* p){
    if(p == NULL || p->right == NULL)
        return p;

    Node* z = p->right;
    Node* T2 = z->left;

    // rotates
    z->left = p;
    p->right = T2;

    // updates nodes' parents
    z->parent = p->parent;
    p->parent = z;
    if(T2 != NULL)
        T2->parent = p;

    updateHeight(p);
    updateHeight(z);

    return z;
}

Node* rotateRight(Node* p){
    if(p == NULL || p->left == NULL)
        return p;

    Node* z = p->left;
    Node* T2 = z->right;

    // rotates
    z->right = p;
    p->left = T2;

    // updates nodes' parents
    z->parent = p->parent;
    p->parent = z;
    if(T2 != NULL)
        T2->parent = p;

    updateHeight(p);
    updateHeight(z);

    return z;
}

Node* doubleRotateLeft(Node* root){
    if(root == NULL || root->right == NULL)
        return root;

    root->right = rotateRight(root->right);
    return rotateLeft(root);
}

Node* doubleRotateRight(Node* root){
    if(root == NULL || root->left == NULL)
        return root;

    root->left = rotateLeft(root->left);
    return rotateRight(root);
}

int rotations = 0;

Node* balanceTree(Node* root){
    if(root == NULL)
        return root;

    root->left = balanceTree(root->left);
    root->right = balanceTree(root->right);

    int BF = getBalanceFactor(root);

    if(BF > 1){
        int leftChildBF = getBalanceFactor(root->left);

        if(leftChildBF < 0){
            rotations++;
            return doubleRotateRight(root);
        } else{
            rotations++;
            return rotateRight(root);
        }
    } else if(BF < -1){
        int rightChildBF = getBalanceFactor(root->right);

        if(rightChildBF > 0){
            rotations++;
            return doubleRotateLeft(root);
        } else{
            rotations++;
            return rotateLeft(root);
        }
    }

    updateHeight(root);
    if(rotations > 0){
        printf("Debug: %d rotations done\n", rotations);
        rotations = 0;
    }

    return root;
}

void preOrderPrintNonRecur(Node* root){
    if(root == NULL)
        return;

    // creates and initializes a new stack
    Stack* s = createStack();

    Node* current = root;

    while(current != NULL || !stackIsEmpty(s)){
        // goes through the left path
        while(current != NULL){
            // prints and pushes current node then goes left
            printNode(current);
            push(s, current);
            current = current->left;
        }

        // pops current node we're at
        Node* popped = pop(s);

        // move right
        current = popped->right;
    }

    printf("\n");

    freeStack(s);

    return;
}

void preOrderPrint(Node* root){
    if(root == NULL)
        return;

    printNode(root);
    preOrderPrint(root->left);
    preOrderPrint(root->right);

    return;
}

void inOrderPrintNonRecur(Node* root){
    if(root == NULL)
        return;

    // creates and initializes a new stack
    Stack* s = createStack();

    Node* current = root;

    while(current != NULL || !stackIsEmpty(s)){
        // goes through the left path
        while(current != NULL){
            push(s, current);
            current = current->left;
        }

        // pops and prints current node we're at
        Node* popped = pop(s);

        printNode(popped);

        // after moving back a node by popping it, move right
        current = popped->right;
    }

    printf("\n");

    freeStack(s);

    return;
}

void inOrderPrint(Node* root){
    if(root == NULL)
        return;

    inOrderPrint(root->left);
    printNode(root);
    inOrderPrint(root->right);

    return;
}

void postOrderPrintNonRecur(Node* root){
    if(root == NULL)
        return;

    // creates and initializes 2 new stacks
    Stack* s1 = createStack();
    Stack* s2 = createStack();

    push(s1, root);

    while(!stackIsEmpty(s1)){
        Node* current = pop(s1);
        push(s2, current);
        if(current->left)
            push(s1, current->left);
        if(current->right)
            push(s1, current->right);
    }

    while(!stackIsEmpty(s2)){
        Node* item = pop(s2);
        printNode(item);
    }

    printf("\n");

    freeStack(s1);
    freeStack(s2);

    return;
}

void postOrderPrint(Node* root){
    if(root == NULL)
        return;

    postOrderPrint(root->left);
    postOrderPrint(root->right);
    printNode(root);

    return;
}

void levelOrderPrint(Node* root){
    if(root == NULL)
        return;

    Queue* q = createQueue();

    enqueue(q, root);

    while(!queueIsEmpty(q)){
        Node* current = dequeue(q);
        if(current->left)
            enqueue(q, current->left);
        if(current->right)
            enqueue(q, current->right);

        printNode(current);
    }

    printf("\n");

    free(q);

    return;
}
