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

#ifndef X__GPLP_GPL_H__X
#define X__GPLP_GPL_H__X
#include "defs.h"

typedef struct {
        GPLP_BOOLEAN_T prepend, standout;
        uint32_t year;
        char *project_name, *author, *description;
} GPLP_GPL_PROJECT_DATA;

static const char *GPLP_GPL_FORMAT_STRING_VAR =
        "%s\n"
        "%s  %s, %s%s\n"
        "%s  Copyright (C) %d %s%s\n"
        "%s          This program is free software: you can redistribute it and/or modify%s\n"
        "%s          it under the terms of the GNU General Public License as published by%s\n"
        "%s          the Free Software Foundation, either version 3 of the License, or (at%s\n"
        "%s          your option) any later version.%s\n"
        "%s          This program is distributed in the hope that it will be useful, but%s\n"
        "%s          WITHOUT ANY WARRANTY; without even the implied warranty of%s\n"
        "%s          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU%s\n"
        "%s          General Public License for more details.%s\n"
        "%s          You should have received a copy of the GNU General Public License%s\n"
        "%s          along with this program. If not, see <https://www.gnu.org/licenses>.%s\n"
        "%s\n";

typedef uint8_t GPLP_GPL_LANGUAGE_ENUM; enum {
        GPLP_GPL_LANGUAGE_ADA           =       0x00,
        GPLP_GPL_LANGUAGE_ABAP          =       0x01,
        GPLP_GPL_LANGUAGE_ABC           =       0x02,
        GPLP_GPL_LANGUAGE_ASM           =       0x03,
        GPLP_GPL_LANGUAGE_ALGOL         =       0x04,
        GPLP_GPL_LANGUAGE_C             =       0x05,
        GPLP_GPL_LANGUAGE_CXX           =       0x06,
        GPLP_GPL_LANGUAGE_PYTHON        =       0x07,
        GPLP_GPL_LANGUAGE_HTML          =       0x08,
        GPLP_GPL_LANGUAGE_MARKDOWN      =       0x09,
        GPLP_GPL_LANGUAGE_VISUAL_BASIC  =       0x0A,
        GPLP_GPL_LANGUAGE__MAX
};

static const char *GPLP_GPL_LANGUAGE_STRING_VAR[GPLP_GPL_LANGUAGE__MAX] = {
        "Ada",
        "ABAP",
        "ABC",
        "Assembly",
        "ALGOL",
        "C",
        "Cxx",
        "Python",
        "HTML",
        "Markdown",
        "VB",
};

static const char *GPLP_GPL_LANGUAGE_COMMENT_BEGIN_STRING_VAR[GPLP_GPL_LANGUAGE__MAX] = {
        "--",           /*      Ada             */
        "*",            /*      ABAP            */
        "\\",           /*      ABC             */
        ";",            /*      Assembly        */
        "comment",      /*      ALGOL           */
        "/*",           /*      C               */
        "//",           /*      C++             */
        "#",            /*      Python          */
        "<!---",        /*      HTML            */
        "",             /*      Markdown        */
        "\""            /*      Visual Basic    */
};
static const char *GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_STRING_VAR[GPLP_GPL_LANGUAGE__MAX] = {
        "--",           /*      Ada             */
        "*",            /*      ABAP            */
        "\\",           /*      ABC             */
        ";",            /*      Assembly        */
        "comment",      /*      ALGOL           */
        "*",            /*      C               */
        "//",           /*      C++             */
        "#",            /*      Python          */
        "",             /*      HTML            */
        "[//]: # (",    /*      Markdown        */
        "\""            /*      Visual Basic    */
};
static const char *GPLP_GPL_LANGUAGE_COMMENT_CONTINUE_END_STRING_VAR[GPLP_GPL_LANGUAGE__MAX] = {
        "",             /*      Ada             */
        "",             /*      ABAP            */
        "",             /*      ABC             */
        "",             /*      Assembly        */
        ";",            /*      ALGOL           */
        "",             /*      C               */
        "",             /*      C++             */
        "",             /*      Python          */
        "",             /*      HTML            */
        ")",            /*      Markdown        */
        ""              /*      Visual Basic    */
};
static const char *GPLP_GPL_LANGUAGE_COMMENT_END_STRING_VAR[GPLP_GPL_LANGUAGE__MAX] = {
        "--",           /*      Ada             */
        "*",            /*      ABAP            */
        "\\",           /*      ABC             */
        ";",            /*      Assembly        */
        "comment;",     /*      ALGOL           */
        "*/",           /*      C               */
        "//",           /*      C++             */
        "#",            /*      Python          */
        "--->",         /*      HTML            */
        "",             /*      Markdown        */
        "\""            /*      Visual Basic    */
};

void gplp_gpl_header_write(
        GPLP_GPL_LANGUAGE_ENUM language,
        const char *project_name,
        const char *description,
        uint32_t    year,
        const char *author,
        FILE *file
);

GPLP_ERROR_CODE gplp_gpl_str_convert_to_uint(
        const char *str,
        uint32_t *ret
);
GPLP_ERROR_CODE gplp_gpl_license_file_generate(
        const char *path
);
GPLP_ERROR_CODE gplp_gpl_file_generate_from_arguments(
        const char *path,
        const char *project_name,
        const char *year_str,
        const char *author,
        const char *desc,
        GPLP_BOOLEAN_T prepend,
        GPLP_BOOLEAN_T standout,
        GPLP_GPL_LANGUAGE_ENUM language
);
GPLP_ERROR_CODE gplp_gpl_file_generate_from_structure(
        const char *path,
        GPLP_GPL_PROJECT_DATA project_data,
        GPLP_GPL_LANGUAGE_ENUM language
);
void gplp_gpl_project_data_destroy(GPLP_GPL_PROJECT_DATA *project_data);

void gplp_gpl_license_fprint(
        FILE *file
);

#endif
