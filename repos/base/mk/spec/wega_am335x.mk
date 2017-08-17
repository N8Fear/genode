#
# \brief  Build-system configurations for Phytec Wega AM335x
# \author Hinnerk van Bruinehsen
# \date   2017-03-27
#

# denote wich specs are also fullfilled by this spec
SPECS += arm_v6 cortex_a8 gpio
#usb framebuffer gpio

# add repository relative include paths
REP_INC_DIR += include/spec/wega_am335x

# include implied specs
include $(call select_from_repositories,mk/spec/cortex_a8.mk)
