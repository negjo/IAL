/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if(tree == NULL){
    return false;
  }
  else{
    bst_node_t *node;
    node = tree;
    while(node != NULL){
      if(node->value == value){
        return true;
      }
      else if(value > node->value){
        node = node->right;
      }
      else{
        node = node->left;
      }
    }
    return false;
  }
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *node;
  if(tree == NULL){
    node = malloc(sizeof(bst_node_t));
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    *tree = node;
    return;
  }
  node = &tree;
  while(true){
    if(node->key == key){
      node->value = value;
      return;
    }
    if(value < node->value){
      if(node->left == NULL){
        node->left = malloc(sizeof(bst_node_t));
        node->left->key = key;
        node->left->value = value;
        node->left->left = NULL;
        node->left->right = NULL;
        return;
      }
      else{
        node = node->left;
        continue;
      }
    }
    else if(value > node->value){
      if(node->right == NULL){
        node->right = malloc(sizeof(bst_node_t));
        node->right->key = key;
        node->right->value = value;
        node->right->left = NULL;
        node->right->right = NULL;
        return;
      }
      else{
        node = node->right;
        continue;
      }
    }
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *node = &tree;
  while(node->right != NULL){
    node = node->right;
  }
  target->key = node->key;
  target->value = node->value;
  free(node);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *node = &tree;
  bst_node_t *prev = NULL;
  int way; //0 = left, 1 = right
  while(node->key != key){
    prev = node;
    if(key < node->key){
      way = 0;
      node = node->left;
    }
    else if(key > node->key){
      way = 1;
      node = node->right;
    }
    if(node == NULL){
      return;
    }
  }

  if(node->left == NULL && node->right == NULL){
    free(node);
    if(way == 0){
      prev->left = NULL;
    }
    else{
      prev->right = NULL;
    }
    node = NULL;
    return;
  }
  else if(node->left != NULL && node->right != NULL){
    bst_replace_by_rightmost(node, &node->left);
  }
  else if(node->left == NULL){
    bst_node_t *tmp = node->right;
    node->key = node->right->key;
    node->value = node->right->value;
    node->left = node->right->left;
    node->right = node->right->right;
    free(tmp);
  }
  else if(node->right == NULL){
    bst_node_t *tmp = node->left;
    node->key = node->left->key;
    node->value = node->left->value;
    node->left = node->left->left;
    node->right = node->left->right;
    free(tmp);
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  do{
    if(*tree != NULL){
      if(!stack_bst_empty(&stack)){
        *tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
      }
    }
    else{
      if((*tree)->right != NULL){
        stack_bst_push(&stack,(*tree)->right);
      }
      bst_node_t *tmp = *tree;
      *tree = (*tree)->left;
      free(tmp);
    }
  } while((*tree != NULL) || (!stack_bst_empty(&stack)));
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_preorder(tree, &stack);
  while(!stack_bst_empty(&stack)){
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_leftmost_preorder(tree, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tree, &stack);
  while(!stack_bst_empty(&stack)){
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  bool fromLeft;
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bool_t bool_stack;
  stack_bool_init(&bool_stack);
  bst_leftmost_postorder(tree, &stack, &bool_stack);
  while(!stack_bst_empty(&stack)){
    tree = stack_bst_top(&stack);
    fromLeft = stack_bool_top(&bool_stack);
    stack_bool_pop(&bool_stack);
    if(fromLeft){
      stack_bool_push(&bool_stack, false);
      bst_leftmost_postorder(tree, &stack, &bool_stack);
    }
    else{
      stack_bst_pop(&stack);
      bst_print_node(tree);
    }
  }
}
