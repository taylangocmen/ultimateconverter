C = gcc

RM = rm -f

FLAGS = -Wall -g

PERMISSIONS = -Wno-multichar #-Wno-write-strings -Wno-unused-variable #-fpermissive -Wno-sign-compare

LIBS = -lpng

SRCS = *.c

MAIN = ultimateconverter

all:
		$(C) $(FLAGS) $(PERMISSIONS) $(LIBS) $(SRCS) -o $(MAIN)

clean:
		$(RM) $(MAIN)

