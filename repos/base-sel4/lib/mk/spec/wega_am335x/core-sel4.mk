SRC_CC += $(addprefix spec/arm/, boot_info.cc thread.cc platform.cc \
                                 platform_pd.cc vm_space.cc)

include $(REP_DIR)/lib/mk/core-sel4.inc

vpath platform_services.cc $(GEN_CORE_DIR)
