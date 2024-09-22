# Name of the binary
BINARY = myShell

# Object files
OBJS   = myShell.o

# Compiler flags (for C)
CFLAGS = -Werror -Wall

# Linker flags (empty for now)
LFLAGS =

# all target: builds all important targets
all: ${BINARY}

# Links the binary
${BINARY}: ${OBJS}
	gcc ${LFLAGS} -o ${BINARY} ${OBJS}

# Compiles a source-file (any file with file extension .c) into an object-file
%.o: %.c
	gcc -c ${CFLAGS} -o $@ $<

# Run the shell
run: ${BINARY}
	./${BINARY}

# Delete all build artifacts
clean:
	rm -rf ${BINARY} ${OBJS}

# all and clean are "phony" targets
.PHONY: all clean
