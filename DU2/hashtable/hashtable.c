/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  for(int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL;
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int hash = get_hash(key);
  ht_item_t *item = (*table)[hash];
  //ht_item_t *tmp = (*table)[hash];
  while(item != NULL){
    if (!strcmp(item->key, key)){
      break;
    }
    item = item->next;
  }
  return item;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  int hash = get_hash(key);
  ht_item_t *tmp = (*table)[hash];
  ht_item_t *new = malloc(sizeof(ht_item_t));
  new->key = key;
  new->value = value;
  new->next = NULL;
  if((*table)[hash] == NULL){
    (*table)[hash] = new;
    return;
  }
  if(!strcmp((*table)[hash]->key, key)){
    (*table)[hash]->value = value;
    free(new);
    return;
  }
  while(tmp->next != NULL){
    if(strcmp(tmp->next->key, key)){
      tmp->next->value = value;
      free(new);
      return;
    }
    tmp = tmp->next;
  }
  tmp->next = new;
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  int hash = get_hash(key);
  ht_item_t *tmp = (*table)[hash];
  float *value;
  while(tmp != NULL){
    if(!strcmp(tmp->key, key)){
      value = &tmp->value;
    }
    tmp = tmp->next;
  }
  return value;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int hash = get_hash(key);
  ht_item_t *item = (*table)[hash];
  if(item == NULL){
    return;
  }
  if(item->next == NULL){
    if(!strcmp(item->key, key)){
      free(item);
      return;
    }
  }

  ht_item_t *tmp;
  while(item->next != NULL){
    if(!strcmp(item->next->key, key)){
      tmp = item->next;
      item->next = item->next->next;
      free(tmp);
      return;
    }
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  for(int i = 0; i < HT_SIZE; i++){
    while((*table)[i] != NULL){
      if((*table)[i]->next == NULL){
        free((*table)[i]);
        (*table)[i] = NULL;
        break;
      }
      ht_item_t *tmp = (*table)[i];
      while(tmp->next->next != NULL){
        tmp = tmp->next;
      }
      free(tmp->next);
      tmp->next = NULL;
    }
  }
}
