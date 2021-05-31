NAME = webserv
CC = clang++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -I./srcs -fsanitize=address
SRC_NAME = main.cpp \
			ServerManager.cpp \
			fdmanager/FileDiscriptorManager.cpp \
			file/File.cpp \
			server/Connection.cpp \
			server/CGIResponse.cpp \
			server/ErrorResponse.cpp \
			server/GETResponse.cpp \
			server/HEADResponse.cpp \
			server/OPTIONSResponse.cpp \
			server/POSTResponse.cpp \
			server/PUTResponse.cpp \
			server/DELETEResponse.cpp \
			server/Request.cpp \
			server/Response.cpp \
			server/ResponseFactory.cpp \
			server/Server.cpp \
			server/ServerComponent.cpp \
			socket/Socket.cpp \
			config/CommonDirective.cpp \
			config/ConfigManager.cpp \
			config/ConfigParser.cpp \
			config/ConfigValidator.cpp \
			config/LocationConfig.cpp \
			config/ServerConfig.cpp \
			logger/Logger.cpp \
			utils/Config.cpp \
			utils/Time.cpp \
			utils/String.cpp \
			utils/Auth.cpp \
			utils/HTTP.cpp

SRC_DIR = ./srcs/
SRC = $(addprefix $(SRC_DIR), $(SRC_NAME))

OBJ_NAME = $(SRC_NAME:.cpp=.o)
OBJ_DIR = ./objs/
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_NAME))

DIRNAME = $(shell dirname $1)

all : $(NAME)

$(NAME) : $(OBJ)
		$(CC) $(CFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
		@mkdir -p $(call DIRNAME, $@) 2> /dev/null || true
		$(CC) $(CFLAGS) -o $@ -c $^

clean :
		rm -rf $(OBJ)

fclean : clean
		rm -rf $(NAME)

re : fclean all

.PHONY : clean fclean re all
