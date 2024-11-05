<!---
    GPLGEN+, An improvement on GPLGEN, a GPL license/header generator 
    Copyright (C) 2024 Lilly H. St Claire
             This program is free software: you can redistribute it and/or modify
             it under the terms of the GNU General Public License as published by
             the Free Software Foundation, either version 3 of the License, or (at
             your option) any later version.
             This program is distributed in the hope that it will be useful, but
             WITHOUT ANY WARRANTY; without even the implied warranty of
             MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
             General Public License for more details.
             You should have received a copy of the GNU General Public License
             along with this program.  If not, see <https://www.gnu.org/licenses/>.
--->

# GPLGEN+
## Lilly H. St Claire

GPLGEN+ is an improvement on GPLGEN, it essentially revamps the codebase and fixes some of the annoying features.
It has the following ABNF in respect to its command line call:

```ABNF
command         = "gplgp" *SP *(options *SP)
options         = license | generate | create_config | use_config | prepend | standout | remark
license         = "-L" *SP file
generate        = "-G" *SP file *SP project *SP year *SP author *SP desc
file            = STRING
config_file     = file
project         = STRING
author          = STRING
desc            = STRING
year            = INTEGER
sub             = BOOLEAN
prepend         = "-p" *SP BOOLEAN
standout        = "-s" *SP BOOLEAN
remark          = "-r" *SP remark_type
create_config   = "-C" *SP config_file *SP project *SP year *SP author *SP desc
use_config      = "-U" *SP config_file *SP file

remark_type =
    "Ada"      ; -- commentcont
  | "ABAP"     ; * commentcont
  | "ABC"      ; \ commentcont
  | "ASM"      ; ; commentcont
  | "ALGOL"    ; comment commentcont
  | "C"        ; /* commentcont */
  | "Cxx"      ; // commentcont
  | "Python"   ; # commentcont
  | "HTML"     ; <!--- commentcont --->
  | "Markdown" ; [//]: # (commentcont)

STRING   = DQUOTE *(VCHAR / SP) DQUOTE | *%x21-7E
INTEGER  = *DIGIT | "$" *HEXDIG | "b" *BIT
BOOLEAN  = "true" | "false"
```

We may input our config file with the following syntax:

```
config    = *statement
statement = *SP prim *SP [comment] *SP NL
prim      = set
set       = option *SP "=" *SP value
option    = "project" | "author" | "year" | "description" | "prepend" | "standout"
value     = STRING | INTEGER | BOOLEAN
comment   = ";" *(VCHAR | SP)

STRING  = DQUOTE *(VCHAR | SP) DQUOTE | *%x21-7E
INTEGER = *DIGIT | "$" *HEXDIG | "b" *BIT
BOOLEAN = "true" | "false"
```
