#include <assert.h>
#include <stdlib.h>
#include "amn_def.h"
#include "rb_tree.h"

static inline int rb_is_root(rb_node__t* node){
   return NULL == node->parent;
}

static inline rb_node__t* new_node(const void* item, rb_color__t color, rb_node__t* left, rb_node__t* right, rb_node__t* parent
                   , void *new_item(const void* d)){
   rb_node__t* node = (rb_node__t*)malloc(sizeof(rb_node__t));
   if (node){
      node->item = new_item(item);
      node->color = color;
      node->left = left;
      node->right = right;
      node->parent = parent;
   }
   return node;
}


static void rotate_left(rb_tree__t* tree, rb_node__t* node){
   assert(node != NULL && node->right != NULL);
   rb_node__t* p = node;
   rb_node__t* q = node->right;
   rb_node__t* parent = node->parent;

   if (!rb_is_root(p)){
      if (parent->left == p)
         parent->left = q;
      else
         parent->right = q;
   } else
      tree->root = q;
   q->parent = parent;
   p->parent = q;
   p->right = q->left;
   if (NULL != p->right)
      p->right->parent = p;
   q->left = p;
}

static void rotate_right(rb_tree__t* tree, rb_node__t* node){
   assert(node != NULL && node->left != NULL);
   rb_node__t* p = node;
   rb_node__t* q = node->left;
   rb_node__t* parent = node->parent;

   if (!rb_is_root(p)){
      if (parent->left == p)
         parent->left = q;
      else
         parent->right = q;
   } else
      tree->root = q;
   q->parent = parent;
   p->parent = q;
   p->left = q->right;
   if (NULL != p->left)
      p->left->parent = p;
   q->right = p;
}
/*Insert item to tree
 return NULL is malloc fail
 if duplicate item is found, return the node address of
 duplicate item, otherwise return new node address*/
rb_node__t* rb_insert__r(rb_tree__t* tree, const void* item){
   rb_node__t* node;
   rb_node__t* node_new;
   rb_node__t* p;           /*Traverse iterator*/
   rb_node__t* parent = NULL;
   int is_left = 0;


   assert(tree != NULL && item != NULL);

   p = tree->root;

   while(p != NULL){
      int cmp_result = tree->compare(item, p->item);

      if(cmp_result == 0)
       {

         return p;
       }

      parent = p;

      if (cmp_result < 0){
         is_left = 1;
         p = p->left;
      }else{
         is_left = 0;
         p = p->right;
      }

   }

   node_new = new_node(item, RB_RED, NULL, NULL, parent, tree->new_item);
   if (node_new == NULL)
      return NULL;

   node = node_new;
   if((parent != NULL)&&(!rb_is_root(node))){
      if(is_left)
         parent->left = node;
      else
         parent->right = node;
   }
   else{
      tree->root = node;
   }



   while(parent != NULL && parent->color == RB_RED){
      rb_node__t* grandpa = parent->parent;
      rb_node__t* uncle;
      if( grandpa == NULL)
         break;
      if(grandpa->left == parent){
         uncle = grandpa->right;
         if(uncle && uncle->color == RB_RED){
            parent->color = RB_BLACK;
            uncle->color = RB_BLACK;
            grandpa->color = RB_RED;
            node = grandpa;
         }
         else{
            if(node == parent->right){
               rotate_left(tree, parent);
               node = parent;
               parent = node->parent;
            }
            parent->color = RB_BLACK;
            grandpa->color = RB_RED;
            rotate_right(tree, grandpa);
         }
      }
      else{
         uncle = grandpa->left;
         if(uncle && uncle->color == RB_RED){
            parent->color = RB_BLACK;
            uncle->color = RB_BLACK;
            grandpa->color = RB_RED;
            node = grandpa;
         }
         else{
            if(node == parent->left){
               rotate_right(tree, parent);
               node = parent;
               parent = node->parent;
            }
            parent->color = RB_BLACK;
            grandpa->color = RB_RED;
            rotate_left(tree, grandpa);
         }
      }
      parent = node->parent;
   }
   tree->root->color = RB_BLACK;
   tree->node_num++;

   return node_new;
}

inline rb_node__t* rb_first(rb_node__t *node){
   if (node == NULL){
      return NULL;
   }

   while(node->left != NULL)
      node = node->left;
   return node;
}

inline rb_node__t* rb_last(rb_node__t *node){
   if (node == NULL){
      return NULL;
   }
   while(node->right != NULL)
      node = node->right;
   return node;
}

rb_node__t* rb_next(const rb_node__t *node){
   if (node == NULL){
      return NULL;
   }
   rb_node__t* parent = node->parent;
   if(node->right != NULL)
      return rb_first(node->right);
   while(parent != NULL && parent->right == node){
      node = parent;
      parent = node->parent;
   }
   return parent;
}

rb_node__t* rb_prev(const rb_node__t *node){

   if (node == NULL){
      return NULL;
   }

   rb_node__t* parent = node->parent;
   if(node->left != NULL)
      return rb_last(node->left);
   while(parent != NULL && parent->left == node){
      node = parent;
      parent = node->parent;
   }
   return parent;
}


void rb_destory_subtree(rb_node__t *node, void destory(void * item))
{
   if (node == NULL)
      return;

   rb_destory_subtree(node->left, destory);
   rb_destory_subtree(node->right, destory);

   destory(node->item);
   free(node);

}


void rb_destory__r(rb_tree__t* rb_tree)
{
   rb_destory_subtree(rb_tree->root, rb_tree->destory);

   rb_tree->root = NULL;
   rb_tree->node_num = 0;
}



#ifdef AMN_UT

#else
/*below functions are not used in AMN code, just tst functions*/

rb_node__t* rb_find__r(rb_tree__t* tree, const void* item){
   assert(tree != NULL && item != NULL);
   rb_node__t* p = tree->root;
   while(p!=NULL){
      int cmp_result = tree->compare(item, p->item);
      if(cmp_result == 0)
         return p;
      else if (cmp_result > 0)
         p = p->right;
      else
         p = p->left;
   }
   return NULL;
}

void rb_del_node__r(rb_tree__t* tree, rb_node__t* node){
   assert(tree != NULL && node != NULL);
   rb_node__t* parent = node->parent;
   rb_node__t* left = node->left;
   rb_node__t* right = node->right;
   rb_node__t* next;
   rb_color__t color;

   if(left == NULL)
      next = right;
   else if (right == NULL)
      next = left;
   else
      next = rb_first(right);

   if(parent != NULL){
      if(parent->left == node)
         parent->left = next;
      else
         parent->right = next;
   }
   else
      tree->root = next;



   if(left != NULL && right != NULL){
      color = next->color;
      next->color = node->color;
      next->left = left;
      left->parent = parent;
      if(next != right){
         parent = next->parent;
         next->parent = node->parent;
         node = next->right;
         parent->left = node;
         next->right=right;
         right->parent=next;
      }
      else{
         next->parent = parent;
         parent = next;
         node = next->right;
      }
   }
   else{
      color = node->color;
      node = next;
   }

   if(node != NULL)
      node->parent=parent;

  /*node_num -- ?*/

   if(color == RB_RED)
      return;
   if(node != NULL && node->color == RB_RED){
      node->color = RB_BLACK;
      return;
   }

   do{
      if(node == tree->root)
         break;

      if(node == parent->left){
         rb_node__t* sibling = parent->right;
         if(sibling->color == RB_RED){
            sibling->color = RB_BLACK;
            parent->color = RB_RED;
            rotate_left(tree, parent);
            sibling = parent->right;
         }
         if((sibling->right == NULL || sibling->right->color == RB_BLACK)&&
            (sibling->left == NULL || sibling->left->color == RB_BLACK)){
            sibling->color = RB_RED;
            node = parent;
            parent = parent->parent;
            continue;
         }
         if(sibling->right ==NULL || sibling->right->color == RB_BLACK){
            sibling->left->color = RB_BLACK;
            sibling->color = RB_RED;
            rotate_right(tree, sibling);
            sibling = parent->right;
         }
         sibling->color = parent->color;
         parent->color = RB_BLACK;
         sibling->right->color = RB_BLACK;
         rotate_left(tree, parent);
         node = tree->root;
         break;
      }
      else{
         rb_node__t* sibling = parent->left;
         if(sibling->color == RB_RED){
            sibling->color = RB_BLACK;
            parent->color = RB_RED;
            rotate_right(tree, parent);
            sibling = parent->left;
         }
         if((sibling->left == NULL || sibling->left->color == RB_BLACK)&&
            (sibling->right == NULL || sibling->right->color == RB_BLACK)){
            sibling->color = RB_RED;
            node = parent;
            parent = parent->parent;
            continue;
         }
         if(sibling->left == NULL || sibling->left->color == RB_BLACK){
            sibling->right->color = RB_BLACK;
            sibling->color = RB_RED;
            rotate_left(tree, sibling);
            sibling = parent->left;
         }
         sibling->color = parent->color;
         parent->color = RB_BLACK;
         sibling->left->color = RB_BLACK;
         rotate_right(tree, parent);
         node = tree->root;
         break;
      }
   }while(node->color == RB_BLACK);

   if(node != NULL)
      node->color = RB_BLACK;

}


void print_tree(rb_node__t *node)
{

   if (node == NULL)
      return;

   print_tree(node->right);

   dxsyslog_r(LOG_TYPE_T_NOTICE_C, (unsigned char*)"amn: pr tree id %s" , ((object_value_t*)(node->item))->value[0]) ;

   print_tree(node->left);

}

#endif



