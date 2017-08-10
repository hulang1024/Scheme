#include <stdlib.h>
#include "hashtable.h"

struct hashtable_entry {
    void *key;
    void *val;
};

struct hashtable {
    int size;
    int count;
    hashtable_entry *array;
    
    hashtable_key_equal_fn key_equal_fn;
    hashtable_hash_fn hash_fn;
};

static void reset(hashtable *ht, int resize)
{
    ht->size = resize;
    ht->count = 0;
    
    ht->array = (hashtable_entry *)malloc(ht->size * sizeof(hashtable_entry));
    int i;
    for (i = 0; i < resize; i++)
        ht->array[i].key = NULL;
}

static void grow(hashtable *ht)
{
    hashtable_entry *old = ht->array;
    int oldsize = ht->size;
    int i;
    
    reset(ht, oldsize * 2);
    
    for (i = 0; i < oldsize; i++) {
        if (old[i].key)
            hashtable_set(ht, old[i].key, old[i].val);
    }
}

hashtable* hashtable_new(int size, hashtable_key_equal_fn key_equal_fn, hashtable_hash_fn hash_fn)
{
    hashtable *ht = malloc(sizeof(hashtable));
    ht->key_equal_fn = key_equal_fn;
    ht->hash_fn = hash_fn;

    reset(ht, size);
 
    return ht;
}


void hashtable_set(hashtable *ht, void *key, void *val)
{
    int i = ht->hash_fn(key) % ht->size;
    
    while (1) {
        if (!ht->array[i].key)
            break;
        if (ht->key_equal_fn(ht->array[i].key, key)) {
            ht->array[i].val = val;
            return;
        }
        i = (i + 1) % ht->size;
    }
    
    ht->array[i].key = key;
    ht->array[i].val = val;
    
    ht->count++;
    
    if (ht->count > ht->size * 0.5)
        grow(ht);
}

void* hashtable_get(hashtable *ht, void *key)
{
    int i = ht->hash_fn(key) % ht->size;
    
    while (1) {
        if (!ht->array[i].key)
            return NULL;
        if (ht->key_equal_fn(ht->array[i].key, key))
            return ht->array[i].val;
        i = (i + 1) % ht->size;
    }
}

int hashtable_count(hashtable *ht)
{
    return ht->count;
}


struct hashtable_iter {
    int pos;
};

hashtable_iter* hashtable_iter_get(hashtable *ht)
{
    hashtable_iter *iter = malloc(sizeof(hashtable_iter));
    iter->pos = 0;
    
    return iter;
}

hashtable_iter* hashtable_iter_next(hashtable *ht, hashtable_iter *iter)
{
    iter->pos++;
    if (iter->pos >= ht->size)
        return NULL;
    else
        return iter;
}

void* hashtable_iter_key(hashtable *ht, hashtable_iter *iter)
{
    return ht->array[iter->pos].key;
}
