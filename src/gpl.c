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
#include <errno.h>
#include "gpl.h"

void gplp_str_to_uint(const char *str, uint32_t *ret, uint32_t *error_code) {
        char *eptr;
        *error_code = 0;
        switch(*str) {
        case 'b':
                *ret = strtol(str+1, &eptr, 0x02);
                if(*str && !*eptr) return;
                break;
        case '$':
                *ret = strtol(str+1, &eptr, 0x10);
                if(*str && !*eptr) return;
                break;
        default:
                *ret = strtol(str, &eptr, 0xA);
                if(*str && !*eptr) return;
                break;
        }
        *error_code = 1;
        return;
}

void gplp_generate_license_file(const char *path) {
        FILE *fp;
        fp = fopen(path, "w");
        if(!fp) {
                fprintf(stderr, "Error: Could not open %s\n", path);
                return;
        }
        gplp_license_fprint_license(fp);
        fclose(fp);
}

void gplp_generate_file(
                uint32_t argc,
                char **argv,
                uint32_t index,
                GPLP_BOOLEAN prepend,
                GPLP_BOOLEAN standout,
                GPLP_LANGUAGE_TYPE_ENUM language) {
        const char      *path,
                        *project_name,
                        *year_str,
                        *author,
                        *desc;
        char            *file_content;
        uint32_t        year,
                        error_code,
                        file_length;
        FILE            *fp;
        if(index+5 > argc) goto generic_error;
        path            = argv[index+1];
        project_name    = argv[index+2];
        year_str        = argv[index+3];
        author          = argv[index+4];
        desc            = argv[index+5];
        gplp_str_to_uint(year_str, &year, &error_code);
        if(error_code) {
                fprintf(stderr, "Error: invalid integer \"%s\"", year_str);
                goto generic_error;
        }
        if(standout) {
                fp = fopen(path, "r");
                if(!fp && errno != ENOENT) {
                        fprintf(stderr, "Error: Could not open \"%s\" for reading (in order to standout)\n");
                        goto generic_error;
                } else if(fp) {
                        fseek(fp, 0L, SEEK_END);
                        file_length = ftell(fp);
                        fseek(fp, 0L, SEEK_SET);
                        file_content = calloc(1,file_length+1);
                        fread(file_content, 1, file_length, fp);
                        fclose(fp);
                        printf("%s\n", file_content);
                        free(file_content);
                }
        }
        fp = fopen(path, "w");
        if(!fp) {
                fprintf(stderr, "Error: Could not open \"%s\"\n", path);
                goto generic_error;
        }
        printf("he\n");
        gplp_write_header(fp, language, project_name, desc, year, author);
        printf("he\n");
        fclose(fp);
        return;

generic_error:
        fprintf(stderr, "Error: Usage of -G is as follows:\n"
                        "  -G <path> <project name> <year> <author> <description>\n");
        return;
}

void gplp_write_header(
                FILE *file,
                GPLP_LANGUAGE_TYPE_ENUM language,
                const char *project_name,
                const char *description,
                uint32_t    year,
                const char *author) {

        fprintf(file, GPLP_HEADER_FORMAT_STRING,
                GPLP_LANGUAGE_COMMENT_BEGIN_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                project_name,
                description,
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                year,
                author,
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_STR[language],
                GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[language],
                GPLP_LANGUAGE_COMMENT_END_STR[language]
        );
}
