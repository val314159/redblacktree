#include <math.h>
#include <stdio.h>
#include <stdlib.h>

enum nodeColor{ RED, BLACK };

#define true  1
#define false 0

struct rbNode {
  int data, color;
  struct rbNode *link[2];
};

struct rbTree {
  int size;
  struct rbNode *root;
};

struct rbNode *createNode(int data, int color) {
  struct rbNode *newnode;
  newnode = (struct rbNode *)malloc(sizeof(struct rbNode));
  newnode->data = data;
  newnode->color = color;
  newnode->link[0] = newnode->link[1] = NULL;
  return newnode;
}

int insertion(struct rbTree *tree,
	       int data) { 
  int ht = ((int)log2(tree->size+1))<<1;
  struct rbNode *stack[ht], *p, *x, *y;
  int d[ht];

  if (!tree->root) {
    tree->root = createNode(data, BLACK);
    tree->size = 1;
    return 1;
  }
  ht = 1;
  stack[0] = p = tree->root;
  d[0] = 0;
  while (1) {
    if (p->data == data) {
      printf("Duplicates Not Allowed!!\n");
      return 0;
    }
    int n = (data > p->data) ? 1 : 0;
    stack[ht] = p;
    d[ht++] = n;
    p = p->link[n];
    if(p == NULL){
      stack[ht - 1]->link[n] = createNode(data, RED);
      break;
    }
  }
  while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
    if (d[ht - 2] == 0) {
      y = stack[ht - 2]->link[1];
      if (y != NULL && y->color == RED) {
        stack[--ht]->color = RED;
        stack[--ht]->color = y->color = BLACK;
	continue;
      }
      if (d[ht - 1] == 0) {
	y = stack[ht - 1];
      } else {
	x = stack[ht - 1];
	stack[ht - 2]->link[0] = y = x->link[1];
	x->link[1] = y->link[0]; y->link[0] = x;
      }
      x = stack[ht - 2];
      x->link[0] = y->link[1]; y->link[1] = x;
    } else {
      y = stack[ht - 2]->link[0];
      if ((y != NULL) && (y->color == RED)) {
        stack[--ht]->color = RED;
        stack[--ht]->color = y->color = BLACK;
	continue;
      }
      if (d[ht - 1] == 1) {
	y = stack[ht - 1];
      } else {
	x = stack[ht - 1];
	stack[ht - 2]->link[1] = y = x->link[0];
	x->link[0] = y->link[1]; y->link[1] = x;
      }
      x = stack[ht - 2];
      x->link[1] = y->link[0]; y->link[0] = x;
    }
    x->color = RED;
    y->color = BLACK;
    if (x == tree->root) {
      tree->root = y;
    } else {
      stack[ht - 3]->link[d[ht - 3]] = y;
    }
    break;
  }
  //root->color = BLACK;
  if(tree->root->color == RED){
    printf("WRONG COLOR IN ROOT\n");
    exit(1);
  }    
  tree->size++;
  return 1;
}

int deletion(struct rbTree *tree,
	      int data) {
  int ht = ((int)log2(tree->size+1))<<1;
  struct rbNode *stack[ht], *p, *x, *y;
  int d[ht];
  ht = 0;
  p = tree->root;
  
  while (1) {
    if (p == NULL) {
      printf("Data not available\n");
      return 0;
    }
    if (data == p->data)
      break;
    int n = (data > p->data) ? 1 : 0;
    stack[ht] = p;
    d[ht++] = n;
    p = p->link[n];
  }

  x = p->link[1];
  if (x == NULL) {
    if (p == tree->root) {
      tree->root = p->link[0];
    } else {
      stack[ht - 1]->link[d[ht - 1]] = p->link[0];
    }
  } else if (x->link[0] == NULL) {
    x->link[0] = p->link[0];

    enum nodeColor color;
    color = x->color;
    x->color = p->color;
    p->color = color;

    if (p == tree->root) {
      tree->root = x;
    } else {
      stack[ht - 1]->link[d[ht - 1]] = x;
    }

    d[ht] = 1;
    stack[ht++] = x;
  } else {
    int i = ht++;
    while (1) {
      d[ht] = 0;
      stack[ht++] = x;
      y = x->link[0];
      if (!y->link[0])
	break;
      x = y;
    }

    d[i] = 1;
    stack[i] = y;
    if (i > 0)
      stack[i - 1]->link[d[i - 1]] = y;

    y->link[0] = p->link[0];

    x->link[0] = y->link[1];
    y->link[1] = p->link[1];

    if (p == tree->root) {
      tree->root = y;
    }

    enum nodeColor color;
    color = y->color;
    y->color = p->color;
    p->color = color;
  }

  if (ht > 0 && p->color == BLACK) {
    while (1) {
      p = stack[ht - 1]->link[d[ht - 1]];
      if (p && p->color == RED) {
        p->color = BLACK;
        break;
      }

      if (ht < 2)
        break;

      if (d[ht - 2] == 0) {
        y = stack[ht - 1]->link[1];

        if (!y)
          break;

        if (y->color == RED) {
          stack[ht - 1]->color = RED;
          y->color = BLACK;
          stack[ht - 1]->link[1] = y->link[0];
          y->link[0] = stack[ht - 1];

          if (stack[ht - 1] == tree->root) {
            tree->root = y;
          } else {
            stack[ht - 2]->link[d[ht - 2]] = y;
          }
          d[ht] = 0;
          stack[ht] = stack[ht - 1];
          stack[ht - 1] = y;
          ht++;

          y = stack[ht - 1]->link[1];
        }

        if ((!y->link[0] || y->link[0]->color == BLACK) &&
	    (!y->link[1] || y->link[1]->color == BLACK)) {
          y->color = RED;
        } else {
          if(!y->link[1] || y->link[1]->color == BLACK) {
            x = y->link[0];
            y->color = RED;
            x->color = BLACK;
            y->link[0] = x->link[1];
            x->link[1] = y;
            y = stack[ht - 1]->link[1] = x;
          }
          y->color = stack[ht - 1]->color;
          stack[ht - 1]->color = BLACK;
          y->link[1]->color = BLACK;
          stack[ht - 1]->link[1] = y->link[0];
          y->link[0] = stack[ht - 1];
          if (stack[ht - 1] == tree->root) {
            tree->root = y;
          } else {
            stack[ht - 2]->link[d[ht - 2]] = y;
          }
          break;
        }
      } else {
        y = stack[ht - 1]->link[0];
        if (!y)
          break;

        if (y->color == RED) {
          stack[ht - 1]->color = RED;
          y->color = BLACK;
          stack[ht - 1]->link[0] = y->link[1];
          y->link[1] = stack[ht - 1];

          if (stack[ht - 1] == tree->root) {
            tree->root = y;
          } else {
            stack[ht - 2]->link[d[ht - 2]] = y;
          }
          d[ht] = 1;
          stack[ht] = stack[ht - 1];
          stack[ht - 1] = y;
          ht++;

          y = stack[ht - 1]->link[0];
        }
        if ((!y->link[0] || y->link[0]->color == BLACK) &&
	    (!y->link[1] || y->link[1]->color == BLACK)) {
          y->color = RED;
        } else {
          if(!y->link[0] || y->link[0]->color == BLACK) {
            x = y->link[1];
            y->color = RED;
            x->color = BLACK;
            y->link[1] = x->link[0];
            x->link[0] = y;
            y = stack[ht - 1]->link[0] = x;
          }
          y->color = stack[ht - 1]->color;
          stack[ht - 1]->color = BLACK;
          y->link[0]->color = BLACK;
          stack[ht - 1]->link[0] = y->link[1];
          y->link[1] = stack[ht - 1];
          if (stack[ht - 1] == tree->root) {
            tree->root = y;
          } else {
            stack[ht - 2]->link[d[ht - 2]] = y;
          }
          break;
        }
      }
      ht--;
    }
  }
  free(p);
  tree->size--;
  return 1;
}

void inorderTraversal(struct rbNode *node) {
  if (node) {
    inorderTraversal(node->link[0]);
    printf("%d, ", node->data);
    inorderTraversal(node->link[1]);
  }
}

void driver(){
  struct rbTree tree;
  char b[12];
  while(fgets(b,sizeof(b),stdin)){
    switch(b[0]){
    case '+':
      insertion(&tree, atoi(b+1));
      break;
    case '-':
      ;deletion(&tree, atoi(b+1));
      break;
    case'\n':
    case'\r':
      inorderTraversal(tree.root);
      printf("\n");
      break;
    default:
      printf("unknown: [%s]\n", b);
    }
  }
}

int main() {
  driver();
}
