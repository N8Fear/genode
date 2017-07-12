PLAT := am335x
ARCH := arm

SEL4_ARCH := aarch32
PLAT_BOARD := /wega_am335x
SEL4_WORDBITS := 32

include $(REP_DIR)/lib/mk/syscall-sel4.inc
