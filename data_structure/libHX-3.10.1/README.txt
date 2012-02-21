
libHX is a C library (with some C++ bindings available) that provides data
structures and functions commonly needed, such as maps, deques, linked lists,
string formatting and autoresizing, option and config file parsing, type
checking casts and more.

libHX aids in quickly writing up C and C++ data processing programs, by
consolidating tasks that often happen to be open-coded, such as (simple) config
file reading, option parsing, directory traversal, and others, into a library.
The focus is on reducing the amount of time (and secondarily, the amount of
code) a developer has to spend for otherwise implementing such.


Components (by all means not all)

  • Documentation (see doc/libHX_Documentation.pdf)

  • maps/sets (HXmap_*)

    Originally created to provide a data structure like Perl's associative
    arrays. Multiple models and underlying storage data structures are
    available (unordered hash-based map, ordered rbtree).

  • linked lists (HXdeque_*, HXlist_*, HXclist_*)

    Doubly-linked lists are suitable for both providing stack and queue
    functionality. Different implementations are available for use, depending
    on situation.

  • directory handling (HXdir_*)

    HXdir provides for opendir-readdir-closedir semantics. Windows uses a
    different kind, so it had to be naturally covered up. On the other hand,
    Solaris's readdir() implementation is nasty in terms of memory management.
    HXdir covers up these discrepancies and provides a sane Linux-style
    readdir.

    Convenience functions mkdir (create all missing parents), rrmdir (rm -Rf)
    are also available.

  • string formatter with placeholders (HXformat_*)

    HXformat is something in the direction of printf(), but the argument list
    is not implemented by means of varargs, so is flexible even beyond compile
    time. You can change the format string — in fact, just let the user
    configuration provide it — without having to worry about argument
    evaluation problems. Positional and optional arguments are simply freely
    choosable.

  • memory containers, auto-sizing string ops (HXmc_*)

    At the cost of slightly increased number memory allocations as you work
    with the buffers, the hmc collection of functions provide scripting-level
    semantics for strings. Appending to a string is simply hmc_strcat(&s,
    "123") [cf. $s .= "123"], without having to worry about overflowing a
    buffer.

  • option parsing (HXoption_*)

    Put blunt, libpopt failed to do some elementary things and there was no
    maintainer to fix it. Well, it's packaged with rpm which already diverged
    in all distros.
    HXoption is table- and callback-based, much like popt.

  • shellconfig parser (HXshconfig_*)

    Parsers shconfig files. Their format is a subset of shell code. Files in /
    etc/sysconfig are commonly shconfig-style.

  • common string operations

    basename, chomp, dirname, getl(ine), split, strlower/-upper, str*trim,
    strsep, strsep2, etc.

This page was last modified: 2011-01-15 16:41 UTC                libHX.sf.net

# Generated using `w3m -dump libhx.sf.net` and slightly editing it.
