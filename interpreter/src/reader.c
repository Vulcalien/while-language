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
#include "reader.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define CONTENT_BLOCK_SIZE (4 * 1024)
#define TOKEN_MAX_SIZE (16 * 1024)

char *latest_token = NULL;

static int reading_index = 0;
/*static int reading_line = 0;*/

static FILE *input;

void reader_set_input(FILE *in) {
    input = in;
}

static char read_char(void) {
    static char *content = NULL;
    static int content_length = 0;

    // If there is no content left to read, read from 'input'
    if(reading_index == content_length) {
        // Check if the 'content' buffer needs to be resized
        if(content_length % CONTENT_BLOCK_SIZE == 0) {
            content = realloc(
                content, content_length + CONTENT_BLOCK_SIZE
            );
        }
        content[content_length++] = getc(input);
    }

    return content[reading_index++];
}

static void unread_chars(int count) {
    reading_index -= count;
    if(reading_index < 0)
        reading_index = 0;
}

static void unread_char(void) {
    unread_chars(1);
}

static void skip_whitespaces(void) {
    char c;
    do {
        c = read_char();
    } while(isspace(c));
    unread_char();
}

char *read_next_token(void) {
    if(!latest_token)
        latest_token = malloc(TOKEN_MAX_SIZE * sizeof(char));

    skip_whitespaces();

    int i = 0;
    while(i < TOKEN_MAX_SIZE - 1) {
        char c = read_char();

        if(isspace(c))
            break;
        else
            latest_token[i] = c;

        i++;
    }
    latest_token[i] = '\0';
    return latest_token;
}

void unread_latest_token(void) {
    unread_chars(strlen(latest_token));
}

bool check_token(char *expected_token) {
    if(strncmp(expected_token, latest_token, TOKEN_MAX_SIZE) != 0)
        return false;
    return true;
}

bool assert_token(char *expected_token) {
    if(check_token(expected_token))
        return true;

    fprintf(
        stderr,
        "Errore: '%s' era atteso, ma e' stato trovato '%s'\n",
        expected_token, latest_token
    );
    return false;
}
