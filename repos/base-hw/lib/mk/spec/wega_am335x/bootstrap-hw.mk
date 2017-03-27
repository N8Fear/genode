INC_DIR += $(BASE_DIR)/../base-hw/src/core/include/spec/wega_am335x
INC_DIR += $(BASE_DIR)/../base-hw/src/core/include/spec/cortex_a8

#SRC_CC  += bootstrap/spec/arm_v6/cpu.cc
#SRC_CC  += bootstrap/spec/rpi/platform.cc
#SRC_CC  += core/spec/rpi/pic.cc

include $(BASE_DIR)/../base-hw/lib/mk/spec/arm/bootstrap-hw.inc

