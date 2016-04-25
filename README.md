# doubly-linked-list-in-Cpp

This program (in C++) implements a doubly-linked list using a single "pointer value" per node. This is realized by using bitwise exclusive-or (XOR). Here XOR is used to encode a linked pointer traversal:
1) the single "pointer value" stored in a node will be the XOR result of the preceding and following nodes'addresses, 
2) the single "pointer value" is no longer a pointer, rather it is an integer result of having XOR'd the previous node's memory address with the next node's memory address.

Usage

Makefile is provided (requires compiler support for C++14).

Can also use the executable main in bin/.
