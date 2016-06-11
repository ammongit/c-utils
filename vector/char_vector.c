/*
 * char_vector.c
 *
 * libaisvector
 * Copyright (c) 2016 Ammon Smith
 *
 * This file is available to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdlib.h>

#include "char_vector.h"

#define VECTOR_DEFAULT_CAPACITY 20

char_vector char_vector_init()
{
    return char_vector_init_c(VECTOR_DEFAULT_CAPACITY);
}

char_vector char_vector_init_c(size_t capacity)
{
    char_vector v = malloc(sizeof(struct char_vector_st));

    if (v == NULL) {
        return NULL;
    }

    if (capacity == 0) {
        free(v);
        return NULL;
    }

    v->array = malloc(capacity * sizeof(char));
    v->capacity = capacity;
    v->length = 0;

    return v;
}

void char_vector_destroy(char_vector v)
{
    if (v) {
        free(v->array);
        free(v);
    }
}

void char_vector_clear(char_vector v)
{
    v->length = 0;
}

bool char_vector_append(char_vector v, char item)
{
    if (v->length >= v->capacity) {
        if (!char_vector_resize(v, v->capacity * 2 + 1)) {
            return false;
        }
    }

    v->array[v->length++] = item;
    return true;
}

bool char_vector_prepend(char_vector v, char item)
{
    if (v->length >= v->capacity) {
        if (!char_vector_resize(v, v->capacity * 2)) {
            return false;
        }
    }

    int i;
    for (i = v->length - 1; i >= 0; i--) {
        v->array[i + 1] = v->array[i];
    }

    v->array[0] = item;
    v->length++;
    return true;
}

bool char_vector_insert(char_vector v, size_t index, char item)
{
    if (v->length >= v->capacity) {
        if (!char_vector_resize(v, v->capacity * 2)) {
            return false;
        }
    }

    size_t i;
    for (i = v->length - 1; i > index; i--) {
        v->array[i] = v->array[i + 1];
    }

    v->array[index] = item;
    v->length++;
    return true;
}

bool char_vector_expand(char_vector v, size_t increase, char item)
{
    if (v->length + increase >= v->capacity) {
        if (!char_vector_resize(v, v->length + increase + 4)) {
            return false;
        }
    }

    size_t i;
    for (i = 0; i < increase; i++) {
        v->array[v->length + i] = item;
    }

    v->length += increase;
    return true;
}

bool char_vector_shrink(char_vector v, size_t decrease)
{
    if (decrease > v->length) {
        return false;
    }

    v->length -= decrease;
    return true;
}

char char_vector_get(char_vector v, size_t index, bool *success)
{
    if (index >= v->length) {
        if (success) {
            *success = false;
        }

        return -1;
    }

    if (success) {
        *success = true;
    }

    return v->array[index];
}

bool char_vector_set(char_vector v, size_t index, char item)
{
    if (index >= v->length) {
        return false;
    }

    v->array[index] = item;
    return true;
}

bool char_vector_remove(char_vector v, size_t index)
{
    size_t i;
    for (i = index; i < v->length; i++) {
        v->array[i] = v->array[i + 1];
    }

    v->length--;
    return true;
}

char_vector char_vector_resize(char_vector v, size_t new_capacity)
{
    if (v->length > new_capacity) {
        return NULL;
    }

    char *new_array = malloc(new_capacity * sizeof(char));
    if (new_array == NULL) {
        return NULL;
    }

    size_t i;
    for (i = 0; i < v->length; i++) {
        new_array[i] = v->array[i];
    }

    free(v->array);
    return v;
}

void char_vector_for_each(char_vector v, void (*f)(size_t, char))
{
    size_t i;
    for (i = 0; i < v->length; i++) {
        f(i, v->array[i]);
    }
}

char *char_vector_to_string(char_vector v)
{
    char *str = malloc(v->length + 1);

    if (str == NULL) {
        return NULL;
    }

    size_t i;
    for (i = 0; i < v->length; i++) {
        str[i] = v->array[i];
    }
    str[v->length] = '\0';

    return str;
}

char *char_vector_to_buffer(char_vector v)
{
    char *buf = malloc(v->length);

    if (buf == NULL) {
        return NULL;
    }

    size_t i;
    for (i = 0; i < v->length; i++) {
        buf[i] = v->array[i];
    }

    return buf;
}

