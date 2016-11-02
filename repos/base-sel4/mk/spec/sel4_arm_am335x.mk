#
# Specifics for the seL4 kernel API arm am335x
#

SPECS += sel4 32bit arm cortex-a8 arm_am335x

include $(call select_from_repositories,mk/spec/32bit.mk)
include $(call select_from_repositories,mk/spec/arm.mk)
include $(call select_from_repositories,mk/spec/arm_v7.mk)
include $(call select_from_repositories,mk/spec/cortex-a8.mk)
include $(call select_from_repositories,mk/spec/sel4.mk)
