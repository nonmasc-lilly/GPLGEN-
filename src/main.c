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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gpl.h"
#include "defs.h"
#include "config.h"

/* TODO: make help menu, and direct user to use -h for help instead of verbose error messages */

static GPLP_ERROR_CODE parameter_string_convert_to_boolean(
                const char *parameter_string,
                const char *parameter_name,
                char parameter_c,
                GPLP_BOOLEAN_T *ret) {
             if(!strcmp(parameter_string,  "true")) *ret = GPLP_TRUE;
        else if(!strcmp(parameter_string, "false")) *ret = GPLP_FALSE;
        else {
                fprintf(
                        stderr,
                        "Error: Booleans may only be \"true\" or \"false\"\n"
                        "  %s Usage: -%c BOOLEAN\n",
                        parameter_name,
                        parameter_c
                );
                return GPLP_NONDESCRIPT_FAILURE;
        }
        return GPLP_SUCCESS;
}

static GPLP_ERROR_CODE parameter_string_convert_to_remark_type(
                const char *parameter_string,
                GPLP_GPL_LANGUAGE_ENUM *ret) {
        uint32_t i;
        for(i=0; i<GPLP_GPL_LANGUAGE__MAX; i++) {
                if(strcmp(parameter_string, GPLP_GPL_LANGUAGE_STRING_VAR[i])) continue;
                *ret = i;
                break;
        }
        if(i != GPLP_GPL_LANGUAGE__MAX) return GPLP_SUCCESS;
        fprintf(
                stderr,
                "Error: Invalid remark type:\n"
                "  Remark Usage: -r <remark>\n"
                "  Remarks:\n"
                "  - \"Ada\"      -->         \"-- commentcontent\"\n"
                "  - \"ABAP\"     -->          \"* commentcontent\"\n"
                "  - \"ABC\"      -->          \"\\ commentcontent\"\n"
                "  - \"ASM\"      -->          \"; commentcontent\"\n"
                "  - \"ALGOL\"    -->   \"comment commentcontent;\"\n"
                "  - \"C\"        -->      \"/* commentcontent */\"\n"
                "  - \"Cxx\"      -->            \"// commentcont\"\n"
                "  - \"Python\"   -->          \"# commentcontent\"\n"
                "  - \"HTML\"     --> \"<!--- commentcontent --->\"\n"
                "  - \"Markdown\" -->  \"[//]: # (commentcontent)\"\n"
                "  - \"VB\"       -->         \"\" commentcontent\"\n"
        );
        return GPLP_NONDESCRIPT_FAILURE;
}

static void file_generate_with_config(const char *config_path, const char *path, GPLP_GPL_LANGUAGE_ENUM language) {
        struct {
                FILE *pointer;
                char *contents;
                uint32_t length;
        } file;
        uint32_t length, i;
        GPLP_CONFIG_TOKEN_DATA *array;
        GPLP_GPL_PROJECT_DATA project_data;
        file.pointer = fopen(config_path, "r");
        if(!file.pointer) {
                fprintf(stderr, "Error: could not open config file %s\n", config_path);
                return;
        }
        fseek(file.pointer, 0L, SEEK_END);
        file.length = ftell(file.pointer);
        fseek(file.pointer, 0L, SEEK_SET);
        file.contents = calloc(1,file.length+1);
        fread(file.contents, 1, file.length, file.pointer);
        fclose(file.pointer);
        gplp_config_string_lex_allocates(file.contents, &length, &array);
        gplp_config_lexed_array_parse(array, length, &project_data);
        gplp_gpl_file_generate_from_structure(path, project_data, language);
        for(i=0; i<length; i++)
                gplp_config_token_destroy(array+i);
        free(array);
        gplp_gpl_project_data_destroy(&project_data);
}

static void help() {
        fprintf(stderr, "GPLGEN+\n"
                        "This program comes with absolutely NO WARRANTY.\n"
                        "This is free software and may be distributed under the\n"
                        "terms and conditions of the GNU General Public License\n"
                        "which can be found at: <https://www.gnu.org/licenses/>\n");
        fprintf(stderr, "=== Usage ===\n"
                        "  gplgp [options]\n"
                        "=== Options ===\n"
                        "  -L <license file> :\n"
                        "    generate a license file named <license file>\n"
                        "  -G <path> <project name> <year> <author> <description>\n"
                        "    generates a file which has the GPL header comment\n"
                        "  -p <true | false>\n"
                        "    set whether to prepend or overwrite any generated non-license files\n"
                        "    true by default\n"
                        "  -s <true | false>\n"
                        "    set whether or not to print the current contents of the file\n"
                        "    true by default\n"
                        "  -r <language>\n"
                        "    set the language which comments will be for. Type \"-r invalid\" for a list\n"
                        "  -U <config path> <file path>"
                        "    like -G, but instead of inputting arguments, they are gained from a config file\n"
                        "=== Config Files ===\n"
                        "  A config file takes a set of statements of the following form\n"
                        "    <option> = <value>\n"
                        "  where option is one of the following:\n"
                        " - project     [string]  : the name of the project\n"
                        " - author      [string]  : the author's name\n"
                        " - description [string]  : the project's description\n"
                        " - year        [number]  : the year of the project's copyright\n"
                        " - prepend     [boolean] : see -p\n"
                        " - standout    [boolean] : see -s\n"
                        "  The option's values have a type (which is listed within braces after the name of the option)\n"
                        "  which must be satisfied by the value, these types are as follows:\n"
                        " - string  : some sentences enclosed with \"\"\n"
                        " - number  : a decimal number, hexadecimal number prefixed with \"$\", or a binary number prefixed with"
                                "\"b\"\n"
                        " - boolean : either \"true\" or \"false\"\n");
}

static GPLP_ERROR_CODE main_error(const char *str) {
        fprintf(stderr, str);
        return GPLP_NONDESCRIPT_FAILURE;
}

int main(int argc, char **argv) {
        uint32_t i;
        GPLP_BOOLEAN_T prepend, standout;
        GPLP_GPL_LANGUAGE_ENUM language;
        prepend = standout = GPLP_TRUE;
        language = GPLP_GPL_LANGUAGE_C;
        for(i=1; i<argc; i++) {
                if(argv[i][0] == '-') switch(argv[i][1]) {
                case 'L':
                        gplp_gpl_license_file_generate(argv[++i]);
                        break;
                case 'G':
                        if(argc < i+5) return main_error(
                                "Error: -G expects the following arguments:\n"
                                "  path, project_name, year, author, desc\n"
                        );
                        gplp_gpl_file_generate_from_arguments(
                                argv[i+1],
                                argv[i+2],
                                argv[i+3],
                                argv[i+4],
                                argv[i+5],
                                prepend,
                                standout,
                                language
                        );
                        i+=5;
                        break;
                case 'p':
                        parameter_string_convert_to_boolean(argv[++i], "Prepend", 'p', &prepend);
                        break;
                case 's':
                        parameter_string_convert_to_boolean(argv[++i], "Standout", 's', &standout);
                        break;
                case 'r':
                        parameter_string_convert_to_remark_type(argv[++i], &language);
                        break;
                case 'U':
                        file_generate_with_config(argv[i+1], argv[i+2], language);
                        i+=2;
                        break;
                case 'h':
                        help();
                        return GPLP_SUCCESS;
                default:
                        fprintf(stderr, "Error: unexpected argument \"%s\" '-h' for help\n\n", argv[i]);
                        return GPLP_NONDESCRIPT_FAILURE;
                } else {
                        fprintf(stderr, "Error: unexpected argument \"%s\" '-h' for help\n\n", argv[i]);
                        return GPLP_NONDESCRIPT_FAILURE;
                }
        }
        return GPLP_SUCCESS;
}

