CPP = g++

RM = rm -f

CPPFLAGS = -Wall -g

PERMISSIONS = -Wno-multichar #-Wwrite-strings -fpermissive -Wunused-variable -Wsign-compare

LIBS = -lz -lpng

SRCS = *.cpp

MAIN = ultimateconverter

all:
		$(CPP) $(CPPFLAGS) $(PERMISSIONS) $(LIBS) $(SRCS) -o $(MAIN)

clean:
		$(RM) $(MAIN)

