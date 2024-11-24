#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Implement binomial heap and its all operations.

struct Node {
  int data, degree;
  struct Node *child, *sibling, *parent;
};

struct Node *newNode(int key) {
  struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
  temp->data = key;
  temp->degree = 0;
  temp->child = temp->parent = temp->sibling = NULL;
  return temp;
}

struct Node *mergeBinomialTrees(struct Node *b1, struct Node *b2) {
  if (b1->data > b2->data) {
    struct Node *temp = b1;
    b1 = b2;
    b2 = temp;
  }
  b2->parent = b1;
  b2->sibling = b1->child;
  b1->child = b2;
  b1->degree++;
  return b1;
}

struct Node *mergeBinomialHeap(struct Node *b1, struct Node *b2) {
  if (b1 == NULL)
    return b2;
  if (b2 == NULL)
    return b1;
  struct Node *res = NULL;
  struct Node *prev = NULL, *temp = NULL;
  while (b1 != NULL && b2 != NULL) {
    if (b1->degree <= b2->degree) {
      temp = b1;
      b1 = b1->sibling;
    } else {
      temp = b2;
      b2 = b2->sibling;
    }
    if (res == NULL) {
      res = temp;
    } else {
      prev->sibling = temp;
    }
    prev = temp;
  }
  if (b1 != NULL) {
    prev->sibling = b1;
  } else {
    prev->sibling = b2;
  }
  return res;
}

struct Node *unionBinomialHeap(struct Node *h1, struct Node *h2) {
  if (h1 == NULL && h2 == NULL)
    return NULL;
  struct Node *res = mergeBinomialHeap(h1, h2);
  struct Node *prev = NULL, *curr = res, *next = curr->sibling;
  while (next != NULL) {
    if ((curr->degree != next->degree) ||
        ((next->sibling != NULL) && (next->sibling)->degree == curr->degree)) {
      prev = curr;
      curr = next;
    } else {
      if (curr->data <= next->data) {
        curr->sibling = next->sibling;
        curr = mergeBinomialTrees(curr, next);
      } else {
        if (prev == NULL) {
          res = next;
        } else {
          prev->sibling = next;
        }
        curr = mergeBinomialTrees(next, curr);
      }
    }
    next = curr->sibling;
  }
  return res;
}

void printTree(struct Node *h) {
  while (h) {
    printf("%d ", h->data);
    printTree(h->child);
    h = h->sibling;
  }
}

void printHeap(struct Node *h) {
  while (h) {
    printTree(h);
    h = h->sibling;
  }
}

struct Node *insert(struct Node *h, int key) {
  struct Node *temp = newNode(key);
  return unionBinomialHeap(h, temp);
}

struct Node *getMin(struct Node *h) {
  if (h == NULL)
    return NULL;
  struct Node *min_node_prev = NULL;
  struct Node *min_node = h;
  int min = h->data;
  struct Node *curr = h;
  while (curr->sibling != NULL) {
    if ((curr->sibling)->data < min) {
      min = (curr->sibling)->data;
      min_node = curr->sibling;
      min_node_prev = curr;
    }
    curr = curr->sibling;
  }
  return min_node;
}

struct Node *extractMin(struct Node *h) {
  if (h == NULL)
    return NULL;
  struct Node *min_node_prev = NULL;
  struct Node *min_node = h;
  int min = h->data;
  struct Node *curr = h;
  while (curr->sibling != NULL) {
    if ((curr->sibling)->data < min) {
      min = (curr->sibling)->data;
      min_node = curr->sibling;
      min_node_prev = curr;
    }
    curr = curr->sibling;
  }
  if (min_node_prev == NULL) {
    h = min_node->sibling;
  } else {
    min_node_prev->sibling = min_node->sibling;
  }
  struct Node *new_heap = NULL;
  struct Node *temp = min_node->child;
  while (temp) {
    struct Node *next = temp->sibling;
    temp->sibling = new_heap;
    temp->parent = NULL;
    new_heap = temp;
    temp = next;
  }
  h = unionBinomialHeap(h, new_heap);
  return min_node;
}

struct Node *decreaseKey(struct Node *h, int old_val, int new_val) {
  struct Node *temp = NULL;
  struct Node *prev = NULL;
  struct Node *curr = h;
  while (curr) {
    if (curr->data == old_val) {
      curr->data = new_val;
      temp = curr;
      break;
    }
    prev = curr;
    curr = curr->sibling;
  }
  if (temp == NULL)
    return h;
  while (prev && temp->data < prev->data) {
    int z = temp->data;
    temp->data = prev->data;
    prev->data = z;
    temp = prev;
    prev = prev->parent;
  }
  return h;
}

struct Node *deleteKey(struct Node *h, int key) {
  if (h == NULL)
    return NULL;
  h = decreaseKey(h, key, INT_MIN);
  return extractMin(h);
}

int main() {
  struct Node *h = NULL;
  h = insert(h, 10);
  h = insert(h, 20);
  h = insert(h, 30);
  h = insert(h, 40);
  h = insert(h, 50);
  printf("Heap elements after insertion:\n");
  printHeap(h);
  struct Node *min_node = getMin(h);
  printf("\nMinimum element of heap: %d\n", min_node->data);
  h = extractMin(h);
  printf("Heap elements after extraction of minimum element:\n");
  printHeap(h);
  h = decreaseKey(h, 40, 5);
  printf("\nHeap elements after decreasing key 40 to 5:\n");
  printHeap(h);
  h = deleteKey(h, 5);
  printf("\nHeap elements after deleting key 5:\n");
  printHeap(h);
  return 0;
}
