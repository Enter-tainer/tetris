OUTPUT_ARCH( "riscv" )
ROM_SIZE = 0x4000;
RAM_SIZE = 0x2000;
STACK_SIZE = 0x1500;
MEMORY {
  rom (rx) : ORIGIN = 0x00000000, LENGTH = ROM_SIZE
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = RAM_SIZE
}
ENTRY(_start)

SECTIONS
{
  .text : 
  {
    . = ALIGN(4);
    *(.text.start);
    *(.text .text.*);
  } > rom
  .data :
  {
    . = ALIGN(4);
    *(.data .data*);
    *(.rodata .rodata*);
    *(.bss .bss.*)
  } > ram
  .stack (NOLOAD):
  {
    . = ALIGN(4);
    . = . + STACK_SIZE;
    . = ALIGN(4);
    __stacktop = .;
  } > ram
}
