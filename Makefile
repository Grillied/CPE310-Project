# Created by: Evan Anderson
# MakeFile for easier compilation of project

cc = gcc


SRCS_TOP    = $(wildcard *.c)
SRCS_FUNCS  = $(wildcard MIPS_Translatron_3000/functions/*.c)
SRCS_UTIL   = $(wildcard MIPS_Translatron_3000/*.c) 
SRCS        = $(SRCS_TOP) $(SRCS_FUNCS) $(SRCS_UTIL)
OBJDIR = build
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

TARGET = MIPS.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(cc) -o $@ $^

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)