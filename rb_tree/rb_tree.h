#ifndef RB_TREE_H
#define RB_TREE_H

#include <stddef.h>
#define MAX_TREE_HEIGHT 100

typedef enum rb_color{
   RB_BLACK,
   RB_RED
}rb_color__t;

typedef struct rb_node{
   void* item;
   rb_color__t color;
   struct rb_node* left;
   struct rb_node* right;
   struct rb_node* parent;
}rb_node__t;

typedef struct rb_tree{
   int (*compare)(const void* d1, const void* d2);
   void (*destory)(void *d);
   void *(*new_item)(const void* d2);
   rb_node__t* root;
   size_t node_num;
}rb_tree__t;

rb_tree__t* rb_create__r(int (*compare)(const void*, const void*),
                          void (*destory)(void*));
void rb_destory__r(rb_tree__t*);
rb_node__t* rb_find__r(rb_tree__t* tree, const void* item);
rb_node__t* rb_insert__r(rb_tree__t* tree, const void* item);
rb_node__t* rb_first(rb_node__t *node);
rb_node__t* rb_last(rb_node__t *node);
rb_node__t* rb_next(const rb_node__t *node);
void print_tree(rb_node__t* node);

#endif
