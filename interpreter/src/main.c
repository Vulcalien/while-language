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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 *                                                                     *
 *    Implementazione in C di un interprete per il linguaggio WHILE.   *
 *                                                                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 */

#include "main.h"

#include <stdio.h>

#include "reader.h"

int main(int argc, char *argv[]) {
    reader_set_input(stdin);

    read_next_token();
    assert_token("begin");
}
