include $(GENODE_DIR)/repos/base/recipes/src/base_content.inc


TIMER_SRC := main.cc target.inc pit include

content: src/drivers/timer
src/drivers/timer:
	mkdir -p $@
	cp -r $(addprefix $(GENODE_DIR)/repos/os/$@/,$(TIMER_SRC)) $@

content: include/spec/x86_32/trace/timestamp.h include/spec/x86_64/trace/timestamp.h

include/spec/%/trace/timestamp.h:
	mkdir -p $(dir $@)
	cp $(GENODE_DIR)/repos/os/$@ $@


content: README
README:
	cp $(REP_DIR)/recipes/src/base-sel4/README $@

content: lib/import etc include/sel4
lib/import etc include/sel4:
	$(mirror_from_rep_dir)

content: src/kernel/sel4
src/kernel:
	$(mirror_from_rep_dir)

KERNEL_PORT_DIR := $(call port_dir,$(REP_DIR)/ports/sel4)

src/kernel/sel4: src/kernel
	cp -r $(KERNEL_PORT_DIR)/src/kernel/sel4/* $@


content:
	for spec in x86_32 x86_64 arm; do \
	  mv lib/mk/spec/$$spec/ld-sel4.mk lib/mk/spec/$$spec/ld.mk; \
	  done;
	sed -i "s/ld-sel4/ld/"          src/lib/ld/sel4/target.mk
	if ( !spec==wega_am335x); then
	  sed -i "s/pit_timer_drv/timer/" src/drivers/timer/pit/target.mk;
	fi

