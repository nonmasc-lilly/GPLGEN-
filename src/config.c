/*
*   GPLGEN+, An improvement on GPLGEN, a GPL license/header generator 
*   Copyright (C) 2024 Lilly H. St Claire
*            This program is free software: you can redistribute it and/or modify
*            it under the terms of the GNU General Public License as published by
*            the Free Software Foundation, either version 3 of the License, or (at
*            your option) any later version.
*            This program is distributed in the hope that it will be useful, but
*            WITHOUT ANY WARRANTY; without even the implied warranty of
*            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*            General Public License for more details.
*            You should have received a copy of the GNU General Public License
*            along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gpl.h"
#include "config.h"

void gplp_config_string_get_token_type(const char *str, GPLP_CONFIG_TOKEN_ENUM *ret) {
        uint32_t tmp;
        if(!*str) {
                *ret = GPLP_CONFIG_TOKEN_NULL;
                return;
        }
        if(!gplp_gpl_str_convert_to_uint(str, &tmp)) {
                *ret = GPLP_CONFIG_TOKEN_NUMBER;
                return;
        }
        if(!strcmp(str, "true") || !strcmp(str, "false")) {
                *ret = GPLP_CONFIG_TOKEN_BOOLEAN;
                return;
        }
        *ret = GPLP_CONFIG_TOKEN_IDENTIFIER;
}

void gplp_config_token_create(GPLP_CONFIG_TOKEN_ENUM type, const char *string, uint32_t line, GPLP_CONFIG_TOKEN_DATA *ret) {
        ret->type = type;
        ret->line = line;
        switch(type) {
        case GPLP_CONFIG_TOKEN_IDENTIFIER:
        case GPLP_CONFIG_TOKEN_STRING:
                ret->value.string = malloc(strlen(string)+1);
                strcpy(ret->value.string, string);
                break;
        case GPLP_CONFIG_TOKEN_NUMBER:
                gplp_gpl_str_convert_to_uint(string, &ret->value.number);
                break;
        case GPLP_CONFIG_TOKEN_BOOLEAN:
                if(!strcmp(string, "true")) ret->value.boolean = GPLP_TRUE;
                else if(!strcmp(string, "false")) ret->value.boolean = GPLP_FALSE;
        }
}

#define IS_SPACE(s) (s == ' ' || s == '\n' || s == '\t')

GPLP_ERROR_CODE gplp_config_string_lex_allocates(const char *string, uint32_t *rlength, GPLP_CONFIG_TOKEN_DATA **ret) {
        struct {
                char *contents;
                uint32_t length;
        } buffer;
        GPLP_CONFIG_TOKEN_ENUM type;
        uint32_t i, tmp_value_int, line;
        char *expr;
        buffer.contents = calloc(1,1);
        buffer.length   = 0;
        *ret = malloc(1);
        *rlength = 0;
        line = 0;
        for(i=0; string[i]; i++) {
                if(string[i] == '\n') line++;
                if(!IS_SPACE(string[i]) && string[i] != '"' && string[i] != '=' && string[i] != ';') {
                        buffer.contents = realloc(buffer.contents, ++buffer.length + 1);
                        buffer.contents[buffer.length-1] = string[i];
                        buffer.contents[buffer.length]   = 0;
                        continue;
                }
                if(string[i] == ';') {
                        while(string[i] != '\n') ++i;
                        continue;
                }
                if(string[i] == '=') {
                        *ret = realloc(*ret, ++*rlength * sizeof(GPLP_CONFIG_TOKEN_DATA));
                        (*ret)[*rlength - 1].type = GPLP_CONFIG_TOKEN_EQUAL;
                        (*ret)[*rlength - 1].line = line;
                        continue;
                }
                if(IS_SPACE(string[i]) || string[i] == '"') {
                        gplp_config_string_get_token_type(buffer.contents, &type);
                        if(type == GPLP_CONFIG_TOKEN_NULL && IS_SPACE(string[i])) continue;
                        if(type == GPLP_CONFIG_TOKEN_NULL && string[i] == '"') goto string_continue;
                        *ret = realloc(*ret, ++*rlength * sizeof(GPLP_CONFIG_TOKEN_DATA));
                        gplp_config_token_create(type, buffer.contents, line, *ret + *rlength - 1);
                        buffer.length = *(buffer.contents = realloc(buffer.contents, 1)) = 0;
                        if(IS_SPACE(string[i])) continue;
                }
        string_continue:
                for(++i; string[i] && string[i] != '"'; i++) {
                        if(string[i] < 0x20 && string[i] != '\t') break;
                        buffer.contents = realloc(buffer.contents, ++buffer.length + 1);
                        buffer.contents[buffer.length-1] = string[i];
                        buffer.contents[buffer.length]   = 0;
                }
                if(string[i] != '"') {
                        fprintf(stderr, "Config Error: invalid string on line %d\n", line);
                        return GPLP_NONDESCRIPT_FAILURE;
                }
                *ret = realloc(*ret, ++*rlength * sizeof(GPLP_CONFIG_TOKEN_DATA));
                gplp_config_token_create(GPLP_CONFIG_TOKEN_STRING, buffer.contents, line, *ret + *rlength - 1);
                buffer.length = *(buffer.contents = realloc(buffer.contents, 1)) = 0;
        }
        gplp_config_string_get_token_type(buffer.contents, &type);
        if(type != GPLP_CONFIG_TOKEN_NULL) {
                *ret = realloc(*ret, ++*rlength * sizeof(GPLP_CONFIG_TOKEN_DATA));
                gplp_config_token_create(type, buffer.contents, line, *ret + *rlength - 1);
                buffer.length = *(buffer.contents = realloc(buffer.contents, 1)) = 0;
        }
        free(buffer.contents);
        return GPLP_SUCCESS;
}

#undef IS_SPACE

void gplp_config_token_array_represent(const GPLP_CONFIG_TOKEN_DATA *array, uint32_t array_length) {
        uint32_t i;
        for(i=0; i<array_length; i++) {
                fprintf(stderr, "%15d| %15s", i, GPLP_CONFIG_TOKEN_STRING_VAR[array[i].type]);
                switch(array[i].type) {
                case GPLP_CONFIG_TOKEN_IDENTIFIER:
                        fprintf(stderr, ", %30s|\n", array[i].value.string);
                        break;
                case GPLP_CONFIG_TOKEN_STRING:
                        fprintf(stderr, ", \"%28s\"|\n", array[i].value.string);
                        break;
                case GPLP_CONFIG_TOKEN_NUMBER:
                        fprintf(stderr, ", %30u|\n", array[i].value.number);
                        break;
                case GPLP_CONFIG_TOKEN_BOOLEAN:
                        fprintf(stderr, ", %30s|\n", array[i].value.boolean ? "true" : "false");
                        break;
                default:
                        fprintf(stderr, "  %30s|\n", " ");
                        break;
                }
        }
}

static GPLP_ERROR_CODE gplp_config_lexed_array_parse_error_static(const char *str, GPLP_CONFIG_TOKEN_DATA token) {
        fprintf(stderr, "Config Error: %s beginning with %s on line %d\n", str, GPLP_CONFIG_TOKEN_STRING_VAR[token.type], token.line);
        return GPLP_NONDESCRIPT_FAILURE;
}

GPLP_ERROR_CODE gplp_config_lexed_array_parse(const GPLP_CONFIG_TOKEN_DATA *array, uint32_t length, GPLP_GPL_PROJECT_DATA *ret) {
        uint32_t index;
        index = 0;
        ret->project_name = NULL;
        ret->author = NULL;
        ret->description = NULL;
        ret->year = 0;
        ret->standout = GPLP_TRUE;
        ret->prepend = GPLP_TRUE;
        while(1) {
                if(index >= length) break;
                if(array[index].type != GPLP_CONFIG_TOKEN_IDENTIFIER || index+1 >= length || index+2 >= length || array[index+1].type != GPLP_CONFIG_TOKEN_EQUAL) return gplp_config_lexed_array_parse_error_static(
                        "invalid statement",
                        array[index]
                );
                index += 2;
                if(!strcmp(array[index-2].value.string, "project")) {
                        if(array[index].type != GPLP_CONFIG_TOKEN_STRING) return gplp_config_lexed_array_parse_error_static(
                                "statement expects string argument",
                                array[index-2]
                        );
                        ret->project_name = calloc(1,strlen(array[index].value.string)+1);
                        strcpy(ret->project_name, array[index].value.string);
                } else if(!strcmp(array[index-2].value.string, "author")) {
                        if(array[index].type != GPLP_CONFIG_TOKEN_STRING) return gplp_config_lexed_array_parse_error_static(
                                "statement expects string argument",
                                array[index-2]
                        );
                        ret->author = calloc(1,strlen(array[index].value.string)+1);
                        strcpy(ret->author, array[index].value.string);
                } else if(!strcmp(array[index-2].value.string, "description")) {
                        if(array[index].type != GPLP_CONFIG_TOKEN_STRING) return gplp_config_lexed_array_parse_error_static(
                                "statement expects string argument",
                                array[index-2]
                        );
                        ret->description = calloc(1,strlen(array[index].value.string)+1);
                        strcpy(ret->description, array[index].value.string);
                } else if(!strcmp(array[index-2].value.string, "year")) {
                        if(array[index].type != GPLP_CONFIG_TOKEN_NUMBER) return gplp_config_lexed_array_parse_error_static(
                                "statement expects numeric argument",
                                array[index-2]
                        );
                        ret->year = array[index].value.number;
                } else if(!strcmp(array[index-2].value.string, "prepend")) {
                        if(array[index].type != GPLP_CONFIG_TOKEN_BOOLEAN) return gplp_config_lexed_array_parse_error_static(
                                "statement expects boolean argument",
                                array[index-2]
                        );
                        ret->prepend = array[index].value.boolean;
                } else if(!strcmp(array[index-2].value.string, "standout")) {
                        if(array[index].type != GPLP_CONFIG_TOKEN_BOOLEAN) return gplp_config_lexed_array_parse_error_static(
                                "statement expects boolean argument",
                                array[index-2]
                        );
                        ret->standout = array[index].value.boolean;
                }
                index += 1;
        }
        if(!ret->project_name) return gplp_config_lexed_array_parse_error_static(
                "project name is missing!",
                array[0]
        );
        if(!ret->author) return gplp_config_lexed_array_parse_error_static(
                "author name is missing!",
                array[0]
        );
        if(!ret->description) return gplp_config_lexed_array_parse_error_static(
                "project description is missing!",
                array[0]
        );
}

void gplp_config_token_destroy(GPLP_CONFIG_TOKEN_DATA *token) {
        switch(token->type) {
        case GPLP_CONFIG_TOKEN_IDENTIFIER:
        case GPLP_CONFIG_TOKEN_STRING:
                free(token->value.string);
        }
}
