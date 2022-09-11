CC = gcc -lpthread

CFLAGS = -Wall -Wextra -Werror
LEAKS = -fsanitize=thread 
# LEAKS = -fsanitize=address -g3 
SRC = src/philo.c src/philo_utils.c src/philo_actions.c src/philo_exit.c src/philo_init.c
	 
OBJ = $(SRC:.c=.o)
all: philo

philo: $(OBJ)
	$(CC) $(LEAKS) $(OBJ) -o philo 
%.o : %.c
	gcc -c $< -o $@
clean:
	rm -f philo
fclean: clean
	rm -f $(OBJ)
re: fclean all

.PHONY: clean fclean re 
