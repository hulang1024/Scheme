
#ifndef HASHTABLE_H
#define HASHTABLE_H

/* key相等比较函数 */
typedef int (*hashtable_key_equal_fn)(void *x, void *y);

/* 哈希函数 */
typedef int (*hashtable_hash_fn)(void *key);

struct hashtable;
typedef struct hashtable hashtable;

struct hashtable_entry;
typedef struct hashtable_entry hashtable_entry;

struct hashtable_iter;
typedef struct hashtable_iter hashtable_iter;

hashtable* hashtable_new(int size, hashtable_key_equal_fn, hashtable_hash_fn);
void hashtable_set(hashtable *, void *key, void *val);
void* hashtable_get(hashtable *, void *key);
int hashtable_count(hashtable *);

hashtable_iter* hashtable_iter_get(hashtable *);
hashtable_iter* hashtable_iter_next(hashtable *, hashtable_iter *);
void* hashtable_iter_key(hashtable *, hashtable_iter *);

#endif