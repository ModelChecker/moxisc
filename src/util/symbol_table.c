#include <string.h>

#include "util/hash.h"
#include "symbol_table.h"


/**
 * Compute the hash symbol for `symbol` relative to the size of `table`. Since `table`'s size is a
 * power of 2, we just return the lowest `log2(table->size)-1` bits of `symbol`'s hash.
*/
uint32_t compute_symbol_table_entry_hash(symbol_table_t *table, char *symbol)
{
    uint32_t mask = table->size - 1;
    return mask & djb2_hash_symbol(symbol);
}


/**
 * 
*/
void init_symbol_table(symbol_table_t *table, uint32_t size)
{
    if (size <= 0) {
        table->size = DEFAULT_SYMBOL_TABLE_SIZE;
    } else {
        table->size = size;
    }

    table->data = malloc(table->size * sizeof(symbol_table_entry_t *)); 
    
    uint32_t i;
    for(i = 0; i < table->size; ++i) {
        table->data[i] = NULL;
    }
}


/**
 * 
*/
void delete_symbol_table(symbol_table_t *table)
{
    uint32_t i;
    symbol_table_entry_t *cur, *next;

    for(i = 0; i < table->size; ++i) {
        cur = table->data[i];
        if (cur == NULL) {
            continue;
        }

        next = cur->next;
        while (cur != NULL) {
            free(cur);
            cur = next;
            next = next->next;
        }
    }

    free(table->data);
}


/**
 * Returns the value for `symbol` if `symbol` is present in `table`. Otherwise, returns -1.
*/
int64_t symbol_table_find(symbol_table_t *table, char *symbol)
{
    uint32_t hash;
    
    hash = compute_symbol_table_entry_hash(table, symbol);

    if (table->data[hash] == NULL) {
        return -1;
    }

    symbol_table_entry_t *cur;
    for(cur = table->data[hash]; cur != NULL; cur = cur->next) {
        if (!strcmp(cur->symbol, symbol)) {
            return (int64_t) cur->value;
        }
    }

    return -1;
}


/**
 * 
*/
void symbol_table_add(symbol_table_t *table, char *symbol, uint32_t value)
{
    symbol_table_entry_t *entry;
    uint32_t hash;

    entry = malloc(sizeof(symbol_table_entry_t));
    entry->symbol = symbol;
    entry->value = value;
    entry->next = NULL;

    hash = compute_symbol_table_entry_hash(table, symbol);

    if (table->data[hash] == NULL) {
        table->data[hash] = entry;
        return;
    }

    symbol_table_entry_t *cur;
    for(cur = table->data[hash]; cur->next != NULL; cur = cur->next);
    cur->next = entry;
}


/**
 * Returns the value of `symbol` on success, -1 on failure.
*/
int64_t symbol_table_remove(symbol_table_t *table, char *symbol)
{
    uint32_t hash;
    
    hash = compute_symbol_table_entry_hash(table, symbol);

    if (table->data[hash] == NULL) {
        return -1;
    }

    symbol_table_entry_t **cur, *prev;

    cur = &table->data[hash];
    prev = NULL;

    for(cur = &table->data[hash]; *cur != NULL; prev = *cur, cur = &(*cur)->next) {
        if(strcmp((*cur)->symbol, symbol)) {
            continue;
        }

        if (prev == NULL) {
            *cur = NULL;
        } else {
            prev->next = (*cur)->next;
            free(*cur);
        }

        return (int64_t) (*cur)->value;
    }

    return -1;
}