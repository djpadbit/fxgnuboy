/* Copied from gint's source code. Temporary, will get removed when BFile_Find... are added to gint */
	.global	_BFile_FindFirst
	.global	_BFile_FindNext
	.global	_BFile_FindClose

# int BFile_FindFirst(const uint16_t *path, int *findhandle, uint16_t *foundfile, bfile_info *fileinfo)
_BFile_FindFirst:
	mov.l	1f, r0
	mov.l	syscall_table, r1
	jmp	@r1
	nop
1:	.long	0x043b

# int BFile_FindNext(int findhandle, uint16_t *foundfile, bfile_info *fileinfo)
_BFile_FindNext:
	mov.l	1f, r0
	mov.l	syscall_table, r1
	jmp	@r1
	nop
1:	.long	0x043c

# int BFile_FindClose(int findhandle)
_BFile_FindClose:
	mov.l	1f, r0
	mov.l	syscall_table, r1
	jmp	@r1
	nop
1:	.long	0x043d


	.align 4

syscall_table:
	.long	0x80010070
