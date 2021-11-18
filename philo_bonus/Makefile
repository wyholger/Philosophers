SRCS		=	main.c\
				parsing.c\
				time.c\
                init_forks_and_philo_struct.c\
                end_program.c\
                write_msg.c\
                ft_itoa.c\
                actions.c

OBJS		= 	${SRCS:.c=.o}

CC			= gcc

FLAGS		= -Wall -Wextra -Werror -O3

RM			= rm -f

NAME		= philo_bonus

HDR			= ../includes/philo_bonus.h

all: $(NAME) $(OBJS) $(HDR)

$(NAME):	$(OBJS)
		$(CC) $(FLAGS) $(SRCS) -o $(NAME)

%.o: %.c $(HDR)
		$(CC) $(FLAGS) -c $< -o $@

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all clean