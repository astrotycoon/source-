;-----------------------------------------
;
;  Audio Extract Program, MBoo Project
;
;-----------------------------------------

.386
.model flat, stdcall
option casemap:none

include windows.inc
include masm32.inc
includelib masm32.lib
include kernel32.inc
includelib kernel32.lib

.data
HelloWord BYTE 'hello, world!', 0
.code
start:
	invoke StdOut, addr HelloWord
	invoke ExitProcess,0
end start
