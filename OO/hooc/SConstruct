#! /usr/bin/env python
# SConstruct

files = ['main.c', 'ChildPlayer.c', 'ConcretePlayer.c', 'MyPlayer.c', 'logger.c', 'Object.c', 'Object.h']
flags = '-Wall -Werror -g'

Program('main', files, CPPFLAGS=flags)
Program('sample', ['sample.c', 'callback.c', 'callback.h'], CPPFLAGS=flags);
