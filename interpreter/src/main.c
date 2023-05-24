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

/*
________________________________________________________________________

    Grammatica generatrice del linguaggio WHILE.
    Fonte: "Semantica del linguaggio WHILE", Simona Ronchi Della Rocca
________________________________________________________________________

    Il linguaggio WHILE è generato dalla seguente grammatica:

    <variabile>     ::= X <indice>
    <indice>        ::= <digit> | <indice> <digit>
    <assegnazione>  ::= <variabile> := 0 |
                        <variabile> := succ(<variabile>) |
                        <variabile> := pred(<variabile>)
    <test>          ::= <variabile> != <variabile>
    <istruzione>    ::= <assegnazione> | while <test> do <istruzione> |
                        while <test> do <programma>
    <sequenza>      ::= <istruzione> | <istruzione> ; <sequenza>
    <programma>     ::= begin end | begin <sequenza> end
________________________________________________________________________
*/

#include "main.h"

#include <stdio.h>
#include <string.h>

#include "reader.h"

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

static bool interpreta_assegnazione(void);
static bool interpreta_test(bool *value);
static bool interpreta_while(void);
static bool interpreta_istruzione(void);
static bool interpreta_programma(void);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

static bool decodifica_variabile(int *index) {
    char *variable = read_next_token(true);

    int len = strlen(variable);
    if(len < 2)
        goto name_not_valid;

    if(variable[0] != 'x')
        goto name_not_valid;

    char *end;
    *index = strtol(&variable[1], &end, 10);
    if(end == &variable[1])
        goto name_not_valid;

    // Reinserisci qualunque carattere dopo il nome della variabile
    int number_length = 1;
    for(int n = *index; n > 9; n /= 10)
        number_length++;
    unread_chars(len - 1 - number_length);

    return true;

    name_not_valid:
    fprintf(
        stderr,
        "Errore: una variabile del tipo 'xi' era attesa, "
        "ma e' stato trovato '%s'\n", variable
    );
    return false;
}

static bool assegna_zero(int index_left) {

    return true;
}

static bool assegna_succ_o_pred(int index_left, bool is_succ) {
    read_next_token_n(true, 1);
    if(!assert_token("("))
        return false;

    int index_right;
    if(!decodifica_variabile(&index_right))
        return false;

    int value_right;
    // TODO get value_right

    if(is_succ)
        value_right++;
    else
        value_right--;

    // TODO assign to index_left

    read_next_token_n(true, 1);
    if(!assert_token(")"))
        return false;

    return true;
}

static bool interpreta_assegnazione(void) {
    puts("reading assign"); // DEBUG

    int index_left;
    if(!decodifica_variabile(&index_left))
        return false;

    read_next_token_n(true, 2);
    if(!assert_token(":="))
        return false;

    // Determina quale assegnamento va eseguito
    read_next_token_n(true, 1);
    if(check_token("0"))
        return assegna_zero(index_left);

    unread_chars(1);

    read_next_token_n(true, 4);
    if(check_token("succ"))
        return assegna_succ_o_pred(index_left, true);
    else if(check_token("pred"))
        return assegna_succ_o_pred(index_left, false);

    unread_latest_token();
    char *token = read_next_token(true);

    fprintf(
        stderr,
        "Errore: '0', 'succ' o 'pred' erano attesi, "
        "ma e' stato trovato '%s'\n", token
    );
    return false;
}

static bool interpreta_test(bool *value) {
    // TODO
    return true;
}

static bool interpreta_while(void) {
    puts("reading while"); // DEBUG

    read_next_token(true);
    if(!assert_token("while"))
        return false;

    while(true) {
        bool test_value;
        if(!interpreta_test(&test_value))
            return false;

        /*if(!test_value)*/
            /*break;*/

        read_next_token(true);
        if(!assert_token("do"))
            return false;

        read_next_token(false);
        if(check_token("begin")) {
            if(!interpreta_programma())
                return false;
        } else {
            if(!interpreta_istruzione())
                return false;
        }
    }
    return true;
}

static bool interpreta_istruzione(void) {
    read_next_token(false);

    if(check_token("while"))
        return interpreta_while();

    // Se non è un 'while', è un'assegnazione
    return interpreta_assegnazione();
}

static bool interpreta_programma(void) {
    puts("reading program"); // DEBUG

    read_next_token(true);
    if(!assert_token("begin"))
        return false;

    read_next_token(true);
    if(check_token("end"))
        return true;

    // Interpreta una sequenza fino a quando non si incontra 'end'
    unread_latest_token();
    while(true) {
        puts("new inst"); // DEBUG
        if(!interpreta_istruzione())
            return false;

        read_next_token(true);
        if(check_token("end"))
            break;
        else if(!assert_token(";"))
            return false;
    }

    read_next_token(true);
    if(!assert_token("end"))
        return false;
    return true;
}

int main(int argc, char *argv[]) {
    reader_init();
    reader_set_input(stdin);

    interpreta_programma();
}
