# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: mvan-wij <mvan-wij@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/10/27 13:16:39 by mvan-wij      #+#    #+#                  #
#    Updated: 2023/03/14 12:00:53 by mvan-wij      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

################################################################################

BLUE_FG := \e[34m
RESET_COLOR := \e[0m
GREEN_FG := \e[32m
CYAN_FG := \e[36m
CLEAR_REST_OF_LINE := \e[K
print_prefix = printf "$(CYAN_FG)%-$(PROJECT_SPACING)s$(RESET_COLOR) $(GREEN_FG)%-$(RULE_SPACING)s$(RESET_COLOR) : " "[$(1)]" "$(2)"
exec_no_nl = printf "$(1)$(CLEAR_REST_OF_LINE)\n"; $(1); printf "\e[A"

################################################################################

PROJECT		:= ex00-bitcoin-exchange
NAME		:= btc
NAME_BONUS	:= btc

LIBFT		:=
MINILIBX	:=

CC			:= c++
CFLAGS		+= -Wall -Wextra -Werror -std=c++98 -pedantic $(if $(DEBUG), -g3) $(if $(SANITIZE), -fsanitize=address -g3)

################################################################################

USER_LIBS +=
SYSTEM_LIBS +=
FRAMEWORKS +=
HEADERS := $(wildcard *.h *.hpp)
SOURCES := $(wildcard *.cpp)

################################################################################

ifdef BONUS
NAME	:= $(NAME_BONUS)
HEADERS +=
SOURCES	+=
CFLAGS	+= -DBONUS
endif

################################################################################

OBJDIR := obj
OBJECTS := $(addprefix $(OBJDIR)/, $(SOURCES:cpp=o))
INCLUDE_FLAGS += $(addprefix -I, $(sort $(dir $(HEADERS))))
LIB_FLAGS += $(addprefix -L, $(sort $(dir $(USER_LIBS)))) $(addprefix -l, $(SYSTEM_LIBS) $(patsubst lib%,%,$(basename $(notdir $(USER_LIBS))))) $(addprefix -framework , $(FRAMEWORKS))

export CFLAGS := $(sort $(CFLAGS))
export LIB_FLAGS := $(LIB_FLAGS)

DATA_FILE := .make_data.txt
MAKE_DATA := $(CFLAGS) $(LIB_FLAGS) $(INCLUDE_FLAGS) $(sort $(OBJECTS))
ifneq ($(shell echo $(MAKE_DATA)), $(shell cat "$(DATA_FILE)" 2> /dev/null))
PRE_RULES := clean
endif

################################################################################

all: $(PRE_RULES) $(NAME)

$(NAME): $(USER_LIBS) $(OBJECTS)
	@$(call print_prefix,"$(PROJECT)","make")
	$(CC) $(CFLAGS) $(OBJECTS) $(LIB_FLAGS) -o $(NAME)
	@$(call print_prefix,"$(PROJECT)","make")
	@printf "$(BLUE_FG)$(NAME)$(RESET_COLOR) created\n"
	@echo $(MAKE_DATA) > $(DATA_FILE)


$(OBJDIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	@$(call print_prefix,"$(PROJECT)","make")
	@$(call exec_no_nl,$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(INCLUDES) -c $< -o $@)


debug:
	@$(MAKE) DEBUG=1

clean:
	@$(call print_prefix,"$(PROJECT)","$@")
	rm -rf $(OBJDIR)

fclean: clean
	@$(call print_prefix,"$(PROJECT)","$@")
	rm -f $(NAME)
	@$(call print_prefix,"$(PROJECT)","$@")
	rm -f $(DATA_FILE)

re: fclean all

run: $(NAME)
	@./$(NAME) $(ARGS)

.PHONY: all debug clean fclean re sources

################################################################################
