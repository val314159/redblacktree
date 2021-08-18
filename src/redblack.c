#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define SI static inline

enum nodeColor{ RED = 0, BLACK = 1 };

struct rbNode{
  int data, color;
  struct rbNode *link[2];
};

struct rbTree{
  int size;
  struct rbNode *root;
};

SI struct rbNode *createNode(int data, int color){
  struct rbNode *newnode;
  newnode = (struct rbNode *)malloc(sizeof(struct rbNode));
  newnode->data = data;
  newnode->color = color;
  newnode->link[0] = newnode->link[1] = NULL;
  return newnode;
}

int insertion(struct rbTree *t, int data){
  int h = ((int)log2(t->size+1))<<1;
  int d[h]; struct rbNode *s[h], *p = t->root;
  if (p == NULL){
    t->root = createNode(data, BLACK);
    return 1;
  }
  s[0] = p; d[0] = 0;
  for(h = 1; ; h++){
    if (p->data == data){
      printf("Duplicates Not Allowed!!\n");
      return 0;
    }
    int n = (data > p->data) ? 1 : 0;
    s[h] = p; d[h] = n;
    p = p->link[n];
    if (p == NULL){
      s[h++]->link[n] = createNode(data, RED);
      break;
    }
  }

  struct rbNode *x, *y;
  while ((h >= 3) && (s[h - 1]->color == RED)){
    if (d[h - 2] == 0){
      y = s[h - 2]->link[1];
      if (y != NULL && y->color == RED){
        s[--h]->color = y->color = BLACK;
        s[--h]->color = RED;
	continue;
      }
      if (d[h - 1] == 0){
	y = s[h - 1];
      } else{
	x = s[h - 1];
	y = s[h - 2]->link[0] = x->link[1];
	x->link[1] = y->link[0]; y->link[0] = x;
      }
      ;;x = s[h - 2];
      ;;x->link[0] = y->link[1]; y->link[1] = x;
    } else{
      y = s[h - 2]->link[0];
      if ((y != NULL) && (y->color == RED)){
        s[--h]->color = RED;
        s[--h]->color = y->color = BLACK;
	continue;
      }
      if (d[h - 1] == 1){
	y = s[h - 1];
      } else{
	x = s[h - 1];
	y = s[h - 2]->link[1] = x->link[0];
	x->link[0] = y->link[1]; y->link[1] = x;
      }
      ;;x = s[h - 2];
      ;;x->link[1] = y->link[0]; y->link[0] = x;
    }
    x->color = RED;
    y->color = BLACK;
    if    (x == t->root)  t->root = y;
    else s[h - 3]->link[d[h - 3]] = y;
    break;
  }
  if(t->root->color == RED){
    printf("_ROOT IS RED, MEANS RUN SON! "
	   "NUMBERS ADD UP TO NOTHIN\n");
    exit(1);
  }
  t->size++;
  return 1;
}

int deletion(struct rbTree *t, int data){
  int h = ((int)log2(t->size+1))<<1;
  int d[h]; struct rbNode *s[h], *p = t->root;
  if (p == NULL){
    printf("Data not available\n");
    return 0;
  }
  
  for (h = 0; ; h++){
    if (data == p->data){
      ;
      break;
    }
    int n = (data > p->data) ? 1 : 0;
    s[h] = p; d[h] = n;
    p = p->link[n]; 
    if (p == NULL){
      printf("Data not available\n");
      return 0;
    }
  }
 
  struct rbNode *x, *y;
  x = p->link[1];
  if (x == NULL){
    if (p == t->root){
      t->root = p->link[0];
    } else{
      s[h - 1]->link[d[h - 1]] = p->link[0];
    }
  } else if (x->link[0] == NULL){
    x->link[0] = p->link[0];
    if (x->color == RED){
      if(p->color == BLACK){
	x->color = BLACK;
	p->color = RED;
      }
    }else{
      if(p->color == RED){
	x->color = RED;
	p->color = BLACK;
      }
    }
    if (p == t->root){
      t->root = x;
    } else{
      s[h - 1]->link[d[h - 1]] = x;
    }
    d[h] = 1; s[h] = x; h++;
  } else{
    int i = h++;
    while(1){
      d[h] = 0; s[h] = x; h++;
      y = x->link[0];
      if (y->link[0] == NULL)
	break;
      x = y;
    }
    d[i] = 1; s[i] = y;
    if (i > 0)
      s[i - 1]->link[d[i - 1]] = y;
    y->link[0] = p->link[0];
    x->link[0] = y->link[1];
    y->link[1] = p->link[1];
    if (p == t->root){
      t->root = y;
    }
    if (y->color == RED){
      if(p->color == BLACK){
	y->color = BLACK;
	p->color = RED;
      }
    }else{
      if(p->color == RED){
	y->color = RED;
	p->color = BLACK;
      }
    }
  }
  if (h > 0 && p->color == BLACK){
    while (1){
      p = s[h - 1]->link[d[h - 1]];
      if (p != NULL && p->color == RED){
        p->color = BLACK;
        break;
      }
      if (h < 2)
        break;
      if (d[h - 2] == 0){
        y = s[h - 1]->link[1];
        if (y == NULL)
          break;
        if (y->color == RED){
          s[h - 1]->color = RED;
          y->color = BLACK;
          s[h - 1]->link[1] = y->link[0];
          y->link[0] = s[h - 1];
          if (s[h - 1] == t->root){
            t->root = y;
          } else{
            s[h - 2]->link[d[h - 2]] = y;
          }
          d[h] = 0; s[h] = s[h - 1]; s[h - 1] = y; h++;
          y = s[h - 1]->link[1];
        }
        if ((y->link[0]==NULL || y->link[0]->color == BLACK) &&
	    (y->link[1]==NULL || y->link[1]->color == BLACK)){
          y->color = RED;
        } else{
          if(y->link[1]==NULL || y->link[1]->color == BLACK){
            x = y->link[0];
            y->color = RED;
            x->color = BLACK;
            y->link[0] = x->link[1];
            x->link[1] = y;
            y = s[h - 1]->link[1] = x;
          }
          y->color = s[h - 1]->color;
          s[h - 1]->color = BLACK;
          y->link[1]->color = BLACK;
          s[h - 1]->link[1] = y->link[0];
          y->link[0] = s[h - 1];
          if (s[h - 1] == t->root){
            t->root = y;
          } else{
            s[h - 2]->link[d[h - 2]] = y;
          }
          break;
        }
      } else{
        y = s[h - 1]->link[0];
        if (y == NULL)
          break;
        if (y->color == RED){
          s[h - 1]->color = RED;
          y->color = BLACK;
          s[h - 1]->link[0] = y->link[1];
          y->link[1] = s[h - 1];
          if (s[h - 1] == t->root){
            t->root = y;
          } else{
            s[h - 2]->link[d[h - 2]] = y;
          }
          d[h] = 1; s[h] = s[h - 1]; s[h - 1] = y; h++;
	  y = s[h - 2]->link[0];
        }
        if ((y->link[0]==NULL || y->link[0]->color == BLACK) &&
	    (y->link[1]==NULL || y->link[1]->color == BLACK)){
          y->color = RED;
        } else{
          if(y->link[0]==NULL || y->link[0]->color == BLACK){
            x = y->link[1];
            y->color = RED;
            x->color = BLACK;
            y->link[1] = x->link[0];
            x->link[0] = y;
            y = s[h - 1]->link[0] = x;
          }
          y->color = s[h - 1]->color;
          s[h - 1]->color = BLACK;
          y->link[0]->color = BLACK;
          s[h - 1]->link[0] = y->link[1];
          y->link[1] = s[h - 1];
          if (s[h - 1] == t->root){
            t->root = y;
          } else{
            s[h - 2]->link[d[h - 2]] = y;
          }
          break;
        }
      }
      h--;
    }
  }
  free(p);
  t->size--;
  return 1;
}

void inorderTraversal(struct rbNode *node){
  if (node != NULL){
    inorderTraversal(node->link[0]);
    printf("%d, ", node->data);
    inorderTraversal(node->link[1]);
  }
}

void driver(){
  struct rbTree tree ={ 0, NULL };
  char buf[16];
  while(fgets(buf, sizeof(buf), stdin)){
    switch (buf[0]){
    case '+':
      insertion(&tree, atoi(buf+1));
      break;
    case '-':
      ;deletion(&tree, atoi(buf+1));
      break;
    case'\r':
    case'\n':
      inorderTraversal(tree.root);
      printf("\n");
      break;
    case 'x':
      exit(0);
    default:
      printf("Not available\n");
      break;
    }
  }
}

int main(){
  driver();
}
