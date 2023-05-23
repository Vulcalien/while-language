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

#include "reader.h"

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

static bool interpreta_assegnazione(void);
static bool interpreta_test(bool *value);
static bool interpreta_while(void);
static bool interpreta_istruzione(void);
static bool interpreta_programma(void);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

static bool interpreta_assegnazione(void) {
    // TODO
    return true;
}

static bool interpreta_test(bool *value) {
    // TODO
    return true;
}

static bool interpreta_while(void) {
    read_next_token();
    if(!assert_token("while"))
        return false;

    while(true) {
        bool test_value;
        if(!interpreta_test(&test_value))
            return false;

        if(!test_value)
            break;

        read_next_token();
        if(!assert_token("do"))
            return false;

        read_next_token();
        if(check_token("begin")) {
            unread_latest_token();

            if(!interpreta_programma())
                return false;
        } else {
            unread_latest_token();

            if(!interpreta_istruzione())
                return false;
        }
    }
    return true;
}

static bool interpreta_istruzione(void) {
    read_next_token();

    if(check_token("while")) {
        unread_latest_token();
        return interpreta_while();
    }

    // Se non è un 'while', è un'assegnazione
    unread_latest_token();
    return interpreta_assegnazione();
}

static bool interpreta_programma(void) {
    read_next_token();
    if(!assert_token("begin"))
        return false;

    read_next_token();
    if(check_token("end"))
        return true;

    // Interpreta una sequenza fino a quando non si incontra 'end'
    unread_latest_token();
    while(true) {
        interpreta_istruzione();

        read_next_token();
        if(check_token("end"))
            break;
        else if(!assert_token(";"))
            return false;
    }

    read_next_token();
    if(!assert_token("end"))
        return false;
    return true;
}

int main(int argc, char *argv[]) {
    reader_set_input(stdin);

    interpreta_programma();
}
