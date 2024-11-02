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

#ifndef X__GPLGENP_MAIN__X
#define X__GPLGENP_MAIN__X
#include <stdio.h>
#include "license.h"

static const char *GPLP_HEADER_FORMAT_STRING =
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

typedef uint8_t GPLP_BOOLEAN;
#define GPLP_TRUE  1
#define GPLP_FALSE 0
typedef uint8_t GPLP_LANGUAGE_TYPE_ENUM; enum {
        GPLP_LANGUAGE_TYPE_ADA,
        GPLP_LANGUAGE_TYPE_ABAP,
        GPLP_LANGUAGE_TYPE_ABC,
        GPLP_LANGUAGE_TYPE_ASM,
        GPLP_LANGUAGE_TYPE_ALGOL,
        GPLP_LANGUAGE_TYPE_C,
        GPLP_LANGUAGE_TYPE_CXX,
        GPLP_LANGUAGE_TYPE_PYTHON,
        GPLP_LANGUAGE_TYPE_HTML,
        GPLP_LANGUAGE_TYPE_MARKDOWN,
        GPLP_LANGUAGE_TYPE_VISUAL_BASIC,
        GPLP_LANGUAGE_TYPE__MAX
};

static const char *GPLP_LANGUAGE_TYPE_STR[GPLP_LANGUAGE_TYPE__MAX] = {
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

static const char *GPLP_LANGUAGE_COMMENT_BEGIN_STR[GPLP_LANGUAGE_TYPE__MAX] = {
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
static const char *GPLP_LANGUAGE_COMMENT_CONTINUE_STR[GPLP_LANGUAGE_TYPE__MAX] = {
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
static const char *GPLP_LANGUAGE_COMMENT_CONTINUE_END_STR[GPLP_LANGUAGE_TYPE__MAX] = {
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
static const char *GPLP_LANGUAGE_COMMENT_END_STR[GPLP_LANGUAGE_TYPE__MAX] = {
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

void gplp_write_header(
        FILE *file,
        GPLP_LANGUAGE_TYPE_ENUM language,
        const char *project_name,
        const char *description,
        uint32_t    year,
        const char *author
);

void gplp_str_to_uint(const char *str, uint32_t *ret, uint32_t *error_code);
void gplp_generate_license_file(const char *path);
void gplp_generate_file(
        uint32_t argc,
        char **argv,
        uint32_t index,
        GPLP_BOOLEAN prepend,
        GPLP_BOOLEAN standout,
        GPLP_LANGUAGE_TYPE_ENUM language
);

#endif
