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

static void parameter_boolean(
                GPLP_BOOLEAN *par,
                const char *arg,
                const char *parameter_name,
                char parameter_c) {
        if(!strcmp(arg, "true")) *par = GPLP_TRUE;
        else if(!strcmp(arg, "false")) *par = GPLP_FALSE;
        else {
                fprintf(stderr, "Error: Booleans may only be \"true\" or \"false\"\n"
                                "  %s Usage: -%c BOOLEAN\n",
                        parameter_name,
                        parameter_c
                );
                return;
        }
}

static void parameter_remark(GPLP_LANGUAGE_TYPE_ENUM *ret, const char *arg) {
        uint32_t i;
        for(i=0; i<GPLP_LANGUAGE_TYPE__MAX; i++) {
                if(strcmp(arg, GPLP_LANGUAGE_TYPE_STR[i])) continue;
                *ret = i;
                break;
        }
        if(i == GPLP_LANGUAGE_TYPE__MAX)
                fprintf(stderr, "Error: Invalid remark type:\n"
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
}

int main(int argc, char **argv) {
        uint32_t i;
        GPLP_BOOLEAN            prepend  = GPLP_TRUE,
                                standout = GPLP_TRUE;
        GPLP_LANGUAGE_TYPE_ENUM language = GPLP_LANGUAGE_TYPE_C;
        for(i=1; i<argc; i++) if(argv[i][0] == '-') switch(argv[i][1]) {
                case 'L': gplp_generate_license_file(argv[++i]);   break;
                case 'G': gplp_generate_file(argc, argv, i, prepend, standout, language); i+=5; break;
                case 'p':
                        parameter_boolean(&prepend, argv[++i], "Prepend", 'p');
                        break;
                case 's':
                        parameter_boolean(&standout, argv[++i], "Standout", 's');
                        break;
                case 'r':
                        parameter_remark(&language, argv[++i]);
                        break;
                default: goto error;
        } else goto error;
        return 0;
error:
        fprintf(stderr, "Error: unexpected argument \"%s\" '-h' for help\n\n", argv[i]);
        return 0;
}

