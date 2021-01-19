#include "hash.h"


/**
* @input - A number of buckets, the size to make the hash table.
* Should assign space for the hash_struct pointer, all buckets, and should
* initialize all entries as KEY = -1, VALUE = -1 to begin.
* 
*/

hash_struct* initTable(int num_of_buckets){
  hash_struct* table = malloc(sizeof (hash_struct));
  table->buckets = malloc(sizeof(data_entry) * num_of_buckets); 
  table->num_of_buckets = num_of_buckets;
  for(int i = 0; i < num_of_buckets; i++) {
    data_entry* entry = malloc (sizeof (data_entry));
    entry->key = -1;
    entry->value = -1;
    table->buckets[i] = entry;
  }
  return table;
}


/*
* @input : Some key value.
* @returns : The key value modulated by the size of the hash table.
*/

int hashcode(hash_struct* table, int key){
  return abs(key % (table->num_of_buckets)); 
}

/*
* @input : Some key value, table pointer.
* @returns : The data entry if some matching (key, value) pair exists, otherwise returns NULL.
*/
data_entry* get(hash_struct* table, int key){
  int index = hashcode(table, key);
  for (int i = index; i < table->num_of_buckets; i++) {
    if (table->buckets[i]->key == key) {
      if(key != -1 || table->buckets[i]->value != -1) {
        return table->buckets[i];
      }
    }
  }
  for (int i = 0; i < index; i++) {
    if (table->buckets[i]->key == key) {
      if(key != -1 || table->buckets[i]->value != -1) {
        return table->buckets[i];
      }
    }
  }
  return NULL;
}

/*
* @input : Some key value, table pointer.
* @returns : True only if a valid key exists that maps to this value.
*/
bool contains(hash_struct* table, int key){
  int index = hashcode(table, key);
  for (int i = index; i < table->num_of_buckets; i++) {
    if (table->buckets[i]->key == key) {
      if (key != -1 || table->buckets[i]->value != -1) {
        return true;
      }
    }
  }
  for (int i = 0; i < index; i++) {
    if (table->buckets[i]->key == key) {
      if (key != -1 || table->buckets[i]->value != -1) {
        return true;
      }
    }
  }
  return false;
}

/*
* @input : Some key integer.
* @input : Some value,.
* @returns : void. Places (key, value) pairing into map.
* Replaces value if another pairing with this key already exists.
* Do nothing if the table is full!
*/
void put(hash_struct* table, int key, int value){
  if (isFull(table)) {
    return;
  }
  int index = hashcode(table, key);
  if(key == -1 && value == -1) {
    return;
  }
  for (int i = index; i < table->num_of_buckets; i++) {
    if (table->buckets[i]->key == key || (table->buckets[i]->key == -1 && table->buckets[i]->value == -1)) {
      table->buckets[i]->key = key;
      table->buckets[i]->value = value;
      return;
    }
  }
  for (int i = 0; i < index; i++) {
    if (table->buckets[i]->key == key || (table->buckets[i]->key == -1 && table->buckets[i]->value == -1)) {
      table->buckets[i]->key = key;
      table->buckets[i]->value = value;
      return;
    }
  }
}

/*
* @returns : The number of valid (key, value) pairings in the table.
*/
int size(hash_struct* table){
  int counter = 0;
  for(int i = 0; i < table->num_of_buckets; i++) {
    if(!(table->buckets[i]->key == -1 && table->buckets[i]->value == -1)) {
      counter++;
    }
  }
  return counter;
}

/*
* Iterates through the table and resets all entries.
*/
void clear(hash_struct* table){
  for(int i = 0; i < table->num_of_buckets; i++) {
    table->buckets[i]->key = -1;
    table->buckets[i]->value = -1;
  }
}

/*
* @returns : true, only if the table contains 0 valid (key, value) pairings.
*/
bool isEmpty(hash_struct* table){
  for(int i = 0; i < table->num_of_buckets; i++) {
    if(table->buckets[i]->key != -1 || table->buckets[i]->value != -1) {
      return false;
    }
  }
  return true;
}


/*
* @returns : true, only when the table is filled entirely with VALID values.
*/
bool isFull(hash_struct* table){
  for(int i = 0; i < table->num_of_buckets; i++) {
    if(table->buckets[i]->key == -1 && table->buckets[i]->value == -1) {
      return false;
    }
  }
  return true;
}


/*
* @input : Some key value.
* @returns : void. if a pair exists for this key, reinitialize this entry.
*/
void removeEntry(hash_struct* table, int key){
  int index = hashcode(table, key);
  for (int i = index; i < table->num_of_buckets; i++) {
    if (table->buckets[i]->key == key) {
      table->buckets[i]->key = -1;
      table->buckets[i]->value = -1;
      return;
    }
  }
  for (int i = 0; i < index; i++) {
    if (table->buckets[i]->key == key) {
      table->buckets[i]->key = -1;
      table->buckets[i]->value = -1;
      return;
    }
  }
}


/*
* Debugging function.
* Iterates through the hashTable and prints all NON-NULL (key, value) pairings.
* Print statement should be of the form "(key1, value1), (key2, value2), ...."
*/
void printEntries(hash_struct* table){
  for(int i = 0; i < table->num_of_buckets; i++) {
    if(table->buckets[i]->key != -1 || table->buckets[i]->value != -1) {
      printf("(%d, %d), ", table->buckets[i]->key, table->buckets[i]->value);
    }
  }
  printf("\n");
}

/*
* Debugging function.
* Iterates though the hashTable and prints ALL entries in order.
* If a value is not valid, you will print "EMPTY" for the value.
* Entry print Format: "(INDEX: tableIndex, KEY: key, VALUE: value)"
* Example :  "(INDEX: 0, KEY: 0, VALUE: 3), (INDEX: 1, KEY: EMPTY, VALUE: EMPTY), (INDEX: 2, KEY: 2, VALUE: 49), ..."
*/
void printFullTable(hash_struct* table){
  for(int i = 0; i < table->num_of_buckets; i++) {
    printf("(INDEX: %d, KEY: %d, VALUE: %d), ", i, table->buckets[i]->key, table->buckets[i]->value);
  }
  printf("\n");
}


/**
* Should free all space consumed by the hash table.
*/
void done(hash_struct* table){
  for(int i = 0; i < table->num_of_buckets; i++) {
    free(table->buckets[i]);
  }
  free(table->buckets);
  free(table);
}