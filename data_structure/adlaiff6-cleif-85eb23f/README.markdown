cleif
=====

Some crap I wrote.  The 'c' stands for the programming language, and the word
'crap'.

Concept
-------

This is a small collection of simple, but efficient data structures in C.  I'm
writing them slowly, whenever I get a chance (usually once in a while I'll take
a night and hack out another one), for a few reasons: 1) I like to practice my C
sometimes, 2) coding data structures is _far_ better than just reading about
them, and 3) I'm sick of learning other peoples' stupid APIs.

Warning
-------

With that in mind, you probably won't want to use these, because they'll likely
have vastly different usage patterns (I change how I like to do things
whimsically and based on BAC), and because there's no way in hell I'm going to
test them completely or maintain them reliably.

Usage
-----

Include the headers you need.  Some of the things here (`klist` and `withstmt`)
are just headers, and you don't need to link anything else in.  For the others,
you'll need to compile the relevant objects (just `make` should be enough) and
then link them to your program (find the objects in `.objs/`).  Soon, I'll add a
library target as well.

Descriptions
------------

### `klist`

A pretty direct conceptual copy of the linux kernel's `list.h`.  There are a few
API changes, so it will be incompatible.  You probably don't want to use it.

Fairly well tested.

### `skiplist`

A reference implementation of a skiplist.  This is probably the most interesting
one so far, and it's actually fairly fast (though it calls `malloc` each time,
so you can't pool memory), as you can see with `make run_skiplist_perf`.

Fairly well tested.

### `tree`

An unbalanced binary tree.  This is pretty much a piece of crap.  The API is
pretty bad, and since it's not balanced, there's not much point in using it.
I'll write a red/black tree (or maybe a splay tree since I've been writing so
many red/black trees lately and they're getting boring) soon enough, and that
one will be better.

Not well tested, since it sucks so much.

### `withstmt`

Pretty useful if you're using locks, though you'll probably want to customize
it.  For example, if you're using pthread locks, you should do something like
the following:

    #define with_rdlock(lock, block)            \
            with((lock),                        \
                 pthread_rwlock_rdlock,         \
                 pthread_rwlock_rdunlock,       \
                 (block))

    #define with_wrlock(lock, block)            \
            with((lock),                        \
                 pthread_rwlock_wrlock,         \
                 pthread_rwlock_wrunlock,       \
                 (block))

License
-------

You can use this for whatever you want, commercial, non-commercial,
anti-commercial, whatever.  If you feel like crediting me, that's cool,
otherwise, no big deal.  It would be nice if you didn't use this software for
anything evil, but I have no way of enforcing that, so I guess you can go ahead
and do that too.  If you do use it, even if you don't want to credit me, it
would be nice if you'd drop me a note (<leif.walsh@gmail.com>) and let me know
how you're using it and if you like it.

That said, I don't expect anyone to use it.

Bugs
----

Yes.  Bug reports can be emailed, or if you just want to fork it and send me a
pull request, that's fine too.