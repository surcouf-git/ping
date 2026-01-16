NAME= ft_ping

COMPILER= gcc
C_FLAGS= -Wall -Wextra -g

SOURCES=	src/*.c \
			main.c

INCLUDES= -I include/

NAME: all

all:
	$(COMPILER) $(C_FLAGS) $(INCLUDES) $(SOURCES) -o ft_ping

exe: all
	@./ft_ping

fclean:
	rm -rf *.o ft_ping