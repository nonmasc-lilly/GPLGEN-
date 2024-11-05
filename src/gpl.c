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

GPLP_ERROR_CODE gplp_gpl_str_convert_to_uint(const char *str, uint32_t *ret) {
        char *eptr;
        switch(*str) {
        case 'b':
                *ret = strtol(str+1, &eptr, 0x02);
                if(*str && !*eptr) return GPLP_SUCCESS;
                break;
        case '$':
                *ret = strtol(str+1, &eptr, 0x10);
                if(*str && !*eptr) return GPLP_SUCCESS;
                break;
        default:
                *ret = strtol(str, &eptr, 0xA);
                if(*str && !*eptr) return GPLP_SUCCESS;
                break;
        }
        return GPLP_NONDESCRIPT_FAILURE;
}

GPLP_ERROR_CODE gplp_gpl_license_file_generate(const char *path) {
        FILE *fp;
        fp = fopen(path, "w");
        if(!fp) {
                fprintf(stderr, "Error: Could not open %s\n", path);
                return GPLP_NONDESCRIPT_FAILURE;
        }
        gplp_gpl_license_fprint(fp);
        fclose(fp);
        return GPLP_SUCCESS;
}

/* TODO: abstract into two functions
 * gplp_gpl_file_generate_from_args
 * gplp_gpl_file_generate_from_data
 *
 * in order to abstract gplp_config_file_generate
*/
GPLP_ERROR_CODE gplp_gpl_file_generate_from_arguments(
                const char *path,
                const char *project_name,
                const char *year_str,
                const char *author,
                const char *desc,
                GPLP_BOOLEAN_T prepend,
                GPLP_BOOLEAN_T standout,
                GPLP_GPL_LANGUAGE_ENUM language) {
        GPLP_GPL_PROJECT_DATA project_data;
        project_data.project_name = malloc(strlen(project_name)+1);
        strcpy(project_data.project_name, project_name);
        project_data.author = malloc(strlen(author)+1);
        strcpy(project_data.author, author);
        project_data.description = malloc(strlen(desc)+1);
        strcpy(project_data.description, desc);
        project_data.prepend = prepend;
        project_data.standout = standout;
        if(gplp_gpl_str_convert_to_uint(year_str, &project_data.year)) {
                fprintf(stderr, "Error: invalid integer \"%s\"", year_str);
                return GPLP_NONDESCRIPT_FAILURE;
        }
        return gplp_gpl_file_generate_from_structure(path, project_data, language);
}

GPLP_ERROR_CODE gplp_gpl_file_generate_from_structure(
                const char *path,
                GPLP_GPL_PROJECT_DATA project_data,
                GPLP_GPL_LANGUAGE_ENUM language) {
        struct {
                uint32_t length;
                FILE *pointer;
                char *content;
        } file;
        file.content = NULL;
        if(project_data.standout || project_data.prepend) {
                file.pointer = fopen(path, "r");
                if(!file.pointer && errno != ENOENT) {
                        fprintf(stderr, "Error: Could not open \"%s\" for reading (in order to standout)\n", path);
                        return GPLP_NONDESCRIPT_FAILURE;
                } else if(file.pointer) {
                        fseek(file.pointer, 0L, SEEK_END);
                        file.length = ftell(file.pointer);
                        fseek(file.pointer, 0L, SEEK_SET);
                        file.content = calloc(1,file.length+1);
                        fread(file.content, 1, file.length, file.pointer);
                        fclose(file.pointer);
                        if(project_data.standout)
                                printf("%s\n", file.content);
                }
        }
        file.pointer = fopen(path, "w");
        if(!file.pointer) {
                fprintf(stderr, "Error: Could not open \"%s\"\n", path);
                return GPLP_NONDESCRIPT_FAILURE;
        }
        gplp_gpl_header_write(language, project_data.project_name, project_data.description, project_data.year, project_data.author, file.pointer);
        fclose(file.pointer);
        if(file.content && project_data.prepend) {
                file.pointer = fopen(path, "a");
                fprintf(file.pointer, file.content);
                fclose(file.pointer);
        }
        return GPLP_SUCCESS;
}

void gplp_gpl_header_write(
                GPLP_GPL_LANGUAGE_ENUM language,
                const char *project_name,
                const char *description,
                uint32_t    year,
                const char *author,
                FILE *file) {

        fprintf(
                file,
                GPLP_GPL_FORMAT_STRING_VAR,
                GPLP_GPL_LANGUAGE_COMMENT_BEGIN_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                project_name,
                description,
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                year,
                author,
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[language],
                GPLP_GPL_LANGUAGE_COMMENT_END_STRING_VAR[language]
        );
}

void gplp_gpl_project_data_destroy(GPLP_GPL_PROJECT_DATA *project_data) {
        free(project_data->project_name);
        free(project_data->author);
        free(project_data->description);
}
