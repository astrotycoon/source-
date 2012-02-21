# Copyright (c) 2008, Björn Rehm (bjoern@shugaa.de)
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

SET(libsrcs
    dll_list.c 
    dll_iterator.c
    dll_util.c)
 
ADD_LIBRARY(dll SHARED ${libsrcs})

INSTALL(TARGETS dll
    DESTINATION lib)

# Install all libdll header files excluding the _prv.h ones
INSTALL(DIRECTORY . 
    DESTINATION include/dll
    FILES_MATCHING REGEX "dll_([^_]+).h")

# This is the same but does not require cmake 2.6
#INSTALL(FILES dll_list.h DESTINATION include/)
#INSTALL(FILES dll_util.h DESTINATION include/)

