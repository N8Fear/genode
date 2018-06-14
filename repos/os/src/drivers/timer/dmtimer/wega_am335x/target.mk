TARGET   = wega_am335x_timer_drv
REQUIRES = wega_am335x
INC_DIR += $(REP_DIR)/src/drivers/timer/dmtimer
SRC_CC  += time_source.cc
SRC_CC  += timer.cc

include $(REP_DIR)/src/drivers/timer/target.inc

vpath time_source.cc $(REP_DIR)/src/drivers/timer/dmtimer
