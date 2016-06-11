/*
 * char_vector.h
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
#ifndef __CHAR_VECTOR_H
#define __CHAR_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

struct char_vector_st {
    char *array;
    size_t length;
    size_t capacity;
};

typedef struct char_vector_st *char_vector;

char_vector char_vector_init();
char_vector char_vector_init_c(size_t capacity);
void char_vector_destroy(char_vector v);
void char_vector_clear(char_vector v);
bool char_vector_append(char_vector v, char item);
bool char_vector_prepend(char_vector v, char item);
bool char_vector_insert(char_vector v, size_t index, char item);
bool char_vector_expand(char_vector v, size_t increase, char item);
bool char_vector_shrink(char_vector v, size_t decrease);
char char_vector_get(char_vector v, size_t index, bool *success);
bool char_vector_set(char_vector v, size_t index, char item);
bool char_vector_remove(char_vector v, size_t index);
char_vector char_vector_resize(char_vector v, size_t new_capacity);
void char_vector_for_each(char_vector v, void (*f)(size_t, char));
char *char_vector_to_string(char_vector v);
char *char_vector_to_buffer(char_vector v);

#define char_vector_is_empty(v)         ((v)->size == 0)

#endif /* __CHAR_VECTOR_H */

