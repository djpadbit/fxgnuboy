MEMORY
{
	m_magic		(rw) : ORIGIN = 0x88040000, LENGTH = 256K
}

SECTIONS
{
	.magic_sec 0x88040000 :
	{
		KEEP(*(.magic_sec))
	} > m_magic
}