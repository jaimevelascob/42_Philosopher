CC = gcc 

CFLAGS = -Wall -Wextra -Werror -pthread 
DR = -fsanitize=thread
LEAKS = -fsanitize=address -g3  
SRC = src/philo.c src/philo_utils.c src/philo_actions.c src/philo_exit.c src/philo_init.c
OBJ = $(SRC:.c=.o)
CL = $(CC) $(LEAKS) $(OBJ) -o philo 	 
CW = $(CC) $(CFLAGS) $(OBJ) -o philo 	 
CR = $(CC) $(DR) $(OBJ) -o philo 	 

all: philo

philo: $(OBJ)
	$(CW)	 
%.o : %.c
	gcc -c $< -o $@
clean:
	rm -f philo
fclean: clean
	rm -f $(OBJ)
re: fclean all

.PHONY: clean fclean re 
