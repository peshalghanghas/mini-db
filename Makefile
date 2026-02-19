CC = gcc
CFLAGS = -Wall -Wextra -g

OBJS = main.o storage.o hash.o

mini-db: $(OBJS)
	$(CC) $(CFLAGS) -o mini-db $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o mini-db
