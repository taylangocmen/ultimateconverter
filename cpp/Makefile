CPP = g++

RM = rm -f

CPPFLAGS = -Wall -g -O3

PERMISSIONS = -Wno-multichar -Wno-write-strings -Wno-unused-variable #-fpermissive -Wno-sign-compare

LIBS = -lz -lpng

SRCS = *.cpp

MAIN = ultimateconverter

all:
		$(CPP) $(CPPFLAGS) $(PERMISSIONS) $(LIBS) $(SRCS) -o $(MAIN)

clean:
		$(RM) $(MAIN)

