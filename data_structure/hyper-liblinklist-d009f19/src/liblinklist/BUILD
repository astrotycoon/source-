libexpbuf - Build and Install instructions.

Building and installing the library is fairly simple.
Assuming that you have obtained and extracted the source code.
(the latest can be downloaded from http://hyper-active.com.au/libexpbuf/)

As a regular user, issue the following command
	
	make

Which should provide an output something like:

	gcc -c -fPIC expbuf.c -o libexpbuf.o
	gcc -shared -Wl,-soname,libexpbuf.so.1 -o libexpbuf.so.1.0.1 libexpbuf.o
	
Then, as root (by using su or sudo), issue the following command:
	
	make install
	
Which will copy the files into the appropriate locations.  
After installation you can begin using it immediately.

