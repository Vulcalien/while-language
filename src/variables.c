/* Copyright 2023 Vulcalien
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "variables.h"

#include <stdio.h>

#define TABLE_SIZE (256)
struct Variable {
    int value;
    int index;
    struct Variable *next;
};

static struct Variable *table[TABLE_SIZE] = { 0 };

int get_variable(unsigned int index) {
    struct Variable *var = table[index % TABLE_SIZE];
    while(var && var->index != index)
        var = var->next;

    // 0 è il valore di default di una variabile,
    // in questa implementazione
    if(!var)
        return 0;

    return var->value;
}

void set_variable(unsigned int index, int value) {
    struct Variable **var = &table[index % TABLE_SIZE];
    while(*var && (*var)->index != index)
        var = &(*var)->next;

    if(!(*var)) {
        *var = malloc(sizeof(struct Variable));
        (*var)->index = index;
        (*var)->next  = NULL;
    }

    #ifdef DEBUG
        printf("\033[1;31mDEBUG: x%d = %d\033[0m\n", index, value);
    #endif
    (*var)->value = value;
}
