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
#ifndef WHILE_READER
#define WHILE_READER

#include "main.h"

#include <stdio.h>

extern char *latest_token;

extern void reader_init(void);
extern void reader_set_input(FILE *in);

extern char *read_next_token(bool consume_token);
extern char *read_next_token_n(bool consume_token, int n);

extern void unread_chars(int count);
extern void unread_latest_token(void);

extern int  reader_get_position(void);
extern void reader_set_position(int position);

extern bool check_token(char *expected_token);
extern bool assert_token(char *expected_token);

#endif // WHILE_READER
