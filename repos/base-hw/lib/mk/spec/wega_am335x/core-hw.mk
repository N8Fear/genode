#
# \brief  Build config for Genodes core process
# \author Hinnerk van Bruinehsen
# \date   2017-03-27
#

# add include paths
INC_DIR += $(REP_DIR)/src/core/include/spec/wega_am335x
#INC_DIR += $(REP_DIR)/src/core/include/spec/pl011

# add C++ sources
SRC_CC += platform_services.cc

# include less specific configuration
include $(REP_DIR)/lib/mk/spec/cortex_a8/core-hw.inc
