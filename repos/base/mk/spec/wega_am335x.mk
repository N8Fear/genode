#
# \brief  Build-system configurations for Phytec Wega AM335x
# \author Hinnerk van Bruinehsen
# \date   2017-03-27
#

# denote wich specs are also fullfilled by this spec
SPECS += am335x
#usb framebuffer gpio

# add repository relative include paths
REP_INC_DIR += include/spec/wega_am335x

# include implied specs
include $(BASE_DIR)/mk/spec/am335x.mk
