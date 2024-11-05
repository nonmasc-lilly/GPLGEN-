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
#ifndef X__GPLP_CONFIG_H__X
#define X__GPLP_CONFIG_H__X
#include "gpl.h"

typedef uint8_t GPLP_CONFIG_TOKEN_ENUM; enum {
        GPLP_CONFIG_TOKEN_NULL          =       0x00,
        GPLP_CONFIG_TOKEN_IDENTIFIER    =       0x01,
        GPLP_CONFIG_TOKEN_NUMBER        =       0x02,
        GPLP_CONFIG_TOKEN_STRING        =       0x03,
        GPLP_CONFIG_TOKEN_BOOLEAN       =       0x04,
        GPLP_CONFIG_TOKEN_EQUAL         =       0x05,
        GPLP_CONFIG_TOKEN__MAX
};

static const char *GPLP_CONFIG_TOKEN_STRING_VAR[GPLP_CONFIG_TOKEN__MAX] = {
        "NULL",
        "IDENTIFIER",
        "NUMBER",
        "STRING",
        "BOOLEAN",
        "EQUAL"
};

typedef struct {
        GPLP_CONFIG_TOKEN_ENUM type;
        uint32_t line;
        union {
                char *string;
                uint32_t number;
                GPLP_BOOLEAN_T boolean;
        } value;
} GPLP_CONFIG_TOKEN_DATA;

void gplp_config_string_get_token_type(const char *string, GPLP_CONFIG_TOKEN_ENUM *ret);
void gplp_config_token_create(GPLP_CONFIG_TOKEN_ENUM type, const char *string, uint32_t line, GPLP_CONFIG_TOKEN_DATA *ret);
GPLP_ERROR_CODE gplp_config_string_lex_allocates(const char *string, uint32_t *rlength, GPLP_CONFIG_TOKEN_DATA **ret);
void gplp_config_token_array_represent(const GPLP_CONFIG_TOKEN_DATA *array, uint32_t array_length);
void gplp_config_token_destroy(GPLP_CONFIG_TOKEN_DATA *token);

GPLP_ERROR_CODE gplp_config_lexed_array_parse(const GPLP_CONFIG_TOKEN_DATA *array, uint32_t length, GPLP_GPL_PROJECT_DATA *ret);


#endif
