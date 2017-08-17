TARGET   = gpio_drv
REQUIRES = wega_am335x
SRC_CC   = main.cc
LIBS     = base
INC_DIR += $(PRG_DIR)

vpath main.cc $(PRG_DIR)
