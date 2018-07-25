#
# \brief Build-system for the TI AM335x series CPUs
# \author Hinnerk van Bruienhsen <hvbruinehsen@stackptr.de>
# \date 2018-07-13
#

SPECS += arm_v7a gpio cpsw

# Add repository relative include paths
REP_INC_DIR += include/spec/am335x

# include implied specs
include $(BASE_DIR)/mk/spec/arm_v7a.mk
