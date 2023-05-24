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
static int content_length = 0;

static FILE *input;

void reader_init(void) {
    latest_token = malloc(TOKEN_MAX_SIZE * sizeof(char));
}

void reader_set_input(FILE *in) {
    input = in;
}

static char read_char(void) {
    static char *content = NULL;

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

void unread_chars(int count) {
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

char *read_next_token_n(bool consume_token, int n) {
    int old_reading_index = reading_index;

    skip_whitespaces();

    int i = 0;
    while(i < TOKEN_MAX_SIZE - 1 && i < n) {
        char c = read_char();

        if(isspace(c)) {
            unread_char();
            break;
        } else {
            latest_token[i] = c;
        }

        i++;
    }
    latest_token[i] = '\0';

    if(!consume_token)
        reading_index = old_reading_index;

    return latest_token;
}

char *read_next_token(bool consume_token) {
    return read_next_token_n(consume_token, TOKEN_MAX_SIZE - 1);
}

void unread_latest_token(void) {
    unread_chars(strlen(latest_token));
}

int reader_get_position(void) {
    return reading_index;
}

void reader_set_position(int position) {
    #ifdef DEBUG
        printf("\033[1;32mDEBUG: jumping to %d\033[0m\n", position);
    #endif

    bool is_valid = false;
    if(position < 0)
        position = 0;
    else if(position >= content_length)
        position = content_length - 1;
    else
        is_valid = true;

    if(!is_valid) {
        fputs(
            "Errore dell'interprete: salto a posizione invalida",
            stderr
        );
    }

    reading_index = position;
}

bool check_token(char *expected_token) {
    if(strncmp(expected_token, latest_token, TOKEN_MAX_SIZE) != 0)
        return false;
    return true;
}

bool assert_token(char *expected_token) {
    if(check_token(expected_token)) {
        #ifdef DEBUG
            printf(
                "\033[1;32mDEBUG: '%s' asserted successfully\033[0m\n",
                expected_token
            );
        #endif
        return true;
    }

    fprintf(
        stderr,
        "Errore: '%s' era atteso, ma e' stato trovato '%s'\n",
        expected_token, latest_token
    );
    return false;
}
