#
# \brief  Build-system configurations specific to phyBOARD Wega-AM335X
# \author Hinnerk van Bruinehsen
# \date   2017-03-23
#

# denote wich specs are also fullfilled by this spec
SPECS += cortex_a8
# not implemented: gpio framebuffer nic ....

# add repository relative include paths
REP_INC_DIR += include/spec/wega_am335x

# include implied specs
include $(call select_from_repositories,mk/spec/cortex_a8.mk)

