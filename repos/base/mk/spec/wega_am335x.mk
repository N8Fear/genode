#
# \brief  Build-system configurations for Phytec Wega AM335x
# \author Hinnerk van Bruinehsen
# \date   2017-03-27
#

# denote wich specs are also fullfilled by this spec
SPECS += arm_v7a gpio
#usb framebuffer gpio

# add repository relative include paths
REP_INC_DIR += include/spec/wega_am335x

# include implied specs
include $(BASE_DIR)/mk/spec/arm_v7a.mk
