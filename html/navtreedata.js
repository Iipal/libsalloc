/*
@licstart  The following is the entire license notice for the
JavaScript code in this file.

Copyright (C) 1997-2019 by Dimitri van Heesch

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

@licend  The above is the entire license notice
for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "The Static memory ALLOCator : salloc", "index.html", [
    [ "SALLOC DOCUMENTATIONS", "md_DOCS.html", [
      [ "Why it's here and not in code?", "md_DOCS.html#autotoc_md1", null ],
      [ "Navigation", "md_DOCS.html#autotoc_md2", null ],
      [ "Introduction", "md_DOCS.html#autotoc_md3", [
        [ "Prefer a clang compiler", "md_DOCS.html#autotoc_md4", null ],
        [ "AVOID ANY KIND OF VLA", "md_DOCS.html#autotoc_md5", null ],
        [ "Internal un-definition", "md_DOCS.html#autotoc_md6", null ],
        [ "Other", "md_DOCS.html#autotoc_md7", null ]
      ] ],
      [ "Configuration", "md_DOCS.html#autotoc_md8", null ],
      [ "Attributes", "md_DOCS.html#autotoc_md9", null ],
      [ "Types", "md_DOCS.html#autotoc_md10", null ],
      [ "Public Types", "md_DOCS.html#autotoc_md11", null ],
      [ "Public Macroses", "md_DOCS.html#autotoc_md12", null ],
      [ "Macroses", "md_DOCS.html#autotoc_md13", null ],
      [ "S-Allocators", "md_DOCS.html#autotoc_md14", null ],
      [ "Family of <tt>salloc</tt> functions:", "md_DOCS.html#autotoc_md15", [
        [ "<tt>void *salloc(salloc_t *__s, salloc_size_t __size)</tt>", "md_DOCS.html#autotoc_md16", [
          [ "Params", "md_DOCS.html#autotoc_md17", null ],
          [ "Returns", "md_DOCS.html#autotoc_md18", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md19", null ],
          [ "Family", "md_DOCS.html#autotoc_md20", null ]
        ] ],
        [ "<tt>void *salloc(salloc_t *__s, salloc_size_t __size, salloc_size_t __nmemb)</tt>", "md_DOCS.html#autotoc_md21", [
          [ "Params", "md_DOCS.html#autotoc_md22", null ],
          [ "Returns", "md_DOCS.html#autotoc_md23", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md24", null ],
          [ "Family", "md_DOCS.html#autotoc_md25", null ]
        ] ],
        [ "<tt>void *snalloc(salloc_t *__s, salloc_size_t __size, salloc_size_t __nmemb)</tt>", "md_DOCS.html#autotoc_md26", [
          [ "Params", "md_DOCS.html#autotoc_md27", null ],
          [ "Returns", "md_DOCS.html#autotoc_md28", null ],
          [ "Family", "md_DOCS.html#autotoc_md29", null ]
        ] ]
      ] ],
      [ "Family of <tt>sfree</tt> functions:", "md_DOCS.html#autotoc_md30", [
        [ "<tt>sfree(salloc_t *__s, void *__ptr)</tt>", "md_DOCS.html#autotoc_md31", [
          [ "Params", "md_DOCS.html#autotoc_md32", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md33", null ],
          [ "Family", "md_DOCS.html#autotoc_md34", null ]
        ] ],
        [ "<tt>sfree(void *__ptr)</tt>", "md_DOCS.html#autotoc_md35", [
          [ "Params", "md_DOCS.html#autotoc_md36", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md37", null ],
          [ "Family", "md_DOCS.html#autotoc_md38", null ]
        ] ],
        [ "<tt>spfree(void *__ptr)</tt>", "md_DOCS.html#autotoc_md39", [
          [ "Params", "md_DOCS.html#autotoc_md40", null ],
          [ "Family", "md_DOCS.html#autotoc_md41", null ]
        ] ]
      ] ],
      [ "Additional Functional", "md_DOCS.html#autotoc_md42", null ],
      [ "Family of <tt>salloc_new</tt> functions:", "md_DOCS.html#autotoc_md43", [
        [ "<tt>salloc_t salloc_new(void *buff, salloc_size_t capacity)</tt>", "md_DOCS.html#autotoc_md44", [
          [ "Params", "md_DOCS.html#autotoc_md45", null ],
          [ "Returns", "md_DOCS.html#autotoc_md46", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md47", null ],
          [ "Family", "md_DOCS.html#autotoc_md48", null ]
        ] ],
        [ "<tt>salloc_new(void *buff, salloc_size_t capacity, salloc_t *__s)</tt>", "md_DOCS.html#autotoc_md49", [
          [ "Params", "md_DOCS.html#autotoc_md50", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md51", null ],
          [ "Family", "md_DOCS.html#autotoc_md52", null ]
        ] ],
        [ "<tt>salloc_snew(void *buff, salloc_size_t capacity, salloc_t *__s)</tt>", "md_DOCS.html#autotoc_md53", [
          [ "Params", "md_DOCS.html#autotoc_md54", null ],
          [ "Family", "md_DOCS.html#autotoc_md55", null ]
        ] ]
      ] ],
      [ "Family of <tt>salloc_copy</tt> functions:", "md_DOCS.html#autotoc_md56", [
        [ "<tt>salloc_t *salloc_copy(salloc_t *__dst, salloc_t *__src)</tt>", "md_DOCS.html#autotoc_md57", [
          [ "Params", "md_DOCS.html#autotoc_md58", null ],
          [ "Returns", "md_DOCS.html#autotoc_md59", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md60", null ],
          [ "Family", "md_DOCS.html#autotoc_md61", null ]
        ] ],
        [ "<tt>void *salloc_copy(salloc_t *__dst, void *__src, salloc_size_t __nbytes)</tt>", "md_DOCS.html#autotoc_md62", [
          [ "Params", "md_DOCS.html#autotoc_md63", null ],
          [ "Returns", "md_DOCS.html#autotoc_md64", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md65", null ],
          [ "Family", "md_DOCS.html#autotoc_md66", null ]
        ] ],
        [ "<tt>void *salloc_ncopy(salloc_t *__dst, void *__src, salloc_size_t __nbytes)</tt>", "md_DOCS.html#autotoc_md67", [
          [ "Params", "md_DOCS.html#autotoc_md68", null ],
          [ "Returns", "md_DOCS.html#autotoc_md69", null ],
          [ "Family", "md_DOCS.html#autotoc_md70", null ]
        ] ],
        [ "<tt>void *salloc_copy(salloc_t *__dst, void *__src, salloc_size_t __nbytes, salloc_size_t __offset)</tt>", "md_DOCS.html#autotoc_md71", [
          [ "Params", "md_DOCS.html#autotoc_md72", null ],
          [ "Returns", "md_DOCS.html#autotoc_md73", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md74", null ],
          [ "Family", "md_DOCS.html#autotoc_md75", null ]
        ] ],
        [ "<tt>void *salloc_ncopyo(salloc_t *__dst, void *__src, salloc_size_t __nbytes, salloc_size_t __offset)</tt>", "md_DOCS.html#autotoc_md76", [
          [ "Params", "md_DOCS.html#autotoc_md77", null ],
          [ "Returns", "md_DOCS.html#autotoc_md78", null ],
          [ "Family", "md_DOCS.html#autotoc_md79", null ]
        ] ],
        [ "<tt>salloc_delete(salloc_t *__s)</tt>", "md_DOCS.html#autotoc_md80", [
          [ "Params", "md_DOCS.html#autotoc_md81", null ]
        ] ],
        [ "<tt>salloc_size_t salloc_capacity(salloc_t *__s)</tt>", "md_DOCS.html#autotoc_md82", [
          [ "Params", "md_DOCS.html#autotoc_md83", null ],
          [ "Returns", "md_DOCS.html#autotoc_md84", null ]
        ] ],
        [ "<tt>salloc_size_t salloc_used(salloc_t *__s)</tt>", "md_DOCS.html#autotoc_md85", [
          [ "Params", "md_DOCS.html#autotoc_md86", null ],
          [ "Returns", "md_DOCS.html#autotoc_md87", null ]
        ] ]
      ] ],
      [ "Family of <tt>salloc_unused</tt> functions:", "md_DOCS.html#autotoc_md88", [
        [ "<tt>salloc_ssize_t salloc_unused(salloc_t *__s)</tt>", "md_DOCS.html#autotoc_md89", [
          [ "Params", "md_DOCS.html#autotoc_md90", null ],
          [ "Returns", "md_DOCS.html#autotoc_md91", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md92", null ],
          [ "Family", "md_DOCS.html#autotoc_md93", null ]
        ] ],
        [ "<tt>salloc_ssize_t salloc_unused(salloc_t *__s, salloc_size_t __size)</tt>", "md_DOCS.html#autotoc_md94", [
          [ "Params", "md_DOCS.html#autotoc_md95", null ],
          [ "Returns", "md_DOCS.html#autotoc_md96", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md97", null ],
          [ "Family", "md_DOCS.html#autotoc_md98", null ]
        ] ],
        [ "<tt>salloc_ssize_t salloc_sunused(salloc_t *__s, salloc_size_t __size)</tt>", "md_DOCS.html#autotoc_md99", [
          [ "Params", "md_DOCS.html#autotoc_md100", null ],
          [ "Returns", "md_DOCS.html#autotoc_md101", null ],
          [ "Family", "md_DOCS.html#autotoc_md102", null ]
        ] ],
        [ "<tt>salloc_ssize_t salloc_unused(salloc_t *__s, salloc_size_t __size, salloc_size_t __nmemb)</tt>", "md_DOCS.html#autotoc_md103", [
          [ "Params", "md_DOCS.html#autotoc_md104", null ],
          [ "Returns", "md_DOCS.html#autotoc_md105", null ],
          [ "Overloads", "md_DOCS.html#autotoc_md106", null ],
          [ "Family", "md_DOCS.html#autotoc_md107", null ]
        ] ],
        [ "<tt>salloc_ssize_t salloc_snunused(salloc_t *__s, salloc_size_t __size, salloc_size_t __nmemb)</tt>", "md_DOCS.html#autotoc_md108", [
          [ "Params", "md_DOCS.html#autotoc_md109", null ],
          [ "Returns", "md_DOCS.html#autotoc_md110", null ],
          [ "Family", "md_DOCS.html#autotoc_md111", null ]
        ] ],
        [ "<tt>salloc_trace(salloc_t *__s)</tt>", "md_DOCS.html#autotoc_md112", [
          [ "Params", "md_DOCS.html#autotoc_md113", null ]
        ] ]
      ] ],
      [ "GDI", "md_DOCS.html#autotoc_md114", [
        [ "<tt>salloc_t *__salloc_get_gdi_buffer()</tt>", "md_DOCS.html#autotoc_md115", [
          [ "Returns", "md_DOCS.html#autotoc_md116", null ]
        ] ],
        [ "<tt>void *salloc(salloc_size_t __size)</tt>", "md_DOCS.html#autotoc_md117", null ],
        [ "<tt>void *salloc(salloc_size_t __size, salloc_size_t __nmemb)</tt>", "md_DOCS.html#autotoc_md118", null ],
        [ "<tt>sfree_gdi(void *__gdi_ptr)</tt>", "md_DOCS.html#autotoc_md119", null ],
        [ "<tt>salloc_t *salloc_copy(salloc_t *__src)</tt>", "md_DOCS.html#autotoc_md120", null ],
        [ "<tt>void *salloc_copy(void *__src, salloc_size_t __nbytes)</tt>", "md_DOCS.html#autotoc_md121", null ],
        [ "<tt>void *salloc_copy(void *__src, salloc_size_t __nbytes, salloc_size_t __offset)</tt>", "md_DOCS.html#autotoc_md122", null ],
        [ "<tt>salloc_delete()</tt>", "md_DOCS.html#autotoc_md123", null ],
        [ "<tt>salloc_trace()</tt>", "md_DOCS.html#autotoc_md124", null ],
        [ "<tt>salloc_size_t salloc_capacity()</tt>", "md_DOCS.html#autotoc_md125", [
          [ "Returns", "md_DOCS.html#autotoc_md126", null ]
        ] ],
        [ "<tt>salloc_size_t salloc_used()</tt>", "md_DOCS.html#autotoc_md127", null ],
        [ "<tt>salloc_ssize_t salloc_unused()</tt>", "md_DOCS.html#autotoc_md128", null ],
        [ "<tt>salloc_ssize_t salloc_unused(salloc_size_t __size)</tt>", "md_DOCS.html#autotoc_md129", null ],
        [ "<tt>salloc_ssize_t salloc_unused(salloc_size_t __size, salloc_size_t __nmemb)</tt>", "md_DOCS.html#autotoc_md130", null ]
      ] ],
      [ "End of Docs.", "md_DOCS.html#autotoc_md131", null ]
    ] ],
    [ "SALLOC", "md_README.html", [
      [ "General Idea", "md_README.html#autotoc_md133", null ],
      [ "Internal", "md_README.html#autotoc_md134", null ]
    ] ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Structure Index", "classes.html", null ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';