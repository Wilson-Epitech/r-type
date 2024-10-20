##
## R_TYPE PROJECT, 2024
## Makefile
## File description:
## Makefile
##

re: fclean all

all:
	make -C src/network/server
	make -C src/network/client

clean:
	make clean -C src/network/server
	make clean -C src/network/client

fclean:
	make fclean -C src/network/server
	make fclean -C src/network/client

.PHONY :  all clean fclean re
