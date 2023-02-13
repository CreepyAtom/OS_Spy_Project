CC=gcc
CFLAGS=-Wall -Wextra -pedantic -O2 -g

# Compilation under MacOS X or Linux
UNAME=$(shell uname -s)
ifeq ($(UNAME),Darwin)
    CPPFLAGS=-D_REENTRANT -I./include -I/usr/local/Cellar/ncurses/6.3/include
    LDFLAGS=-L/usr/local/Cellar/ncurses/6.3/lib -lncurses -lpthread -lm
endif
ifeq ($(UNAME),Linux)
    CPPFLAGS=-D_REENTRANT -I./include
    LDFLAGS=-lncurses -lpthread -lrt -lm
endif

.PHONY: all clean distclean test


test: bin/test_enemy
all: bin/monitor bin/timer bin/spy_simulation bin/citizen_manager

# ----------------------------------------------------------------------------
# MONITOR
# ----------------------------------------------------------------------------
bin/monitor: src/monitor/main.o \
             src/monitor/monitor.o \
             src/monitor/monitor_common.o \
             src/common/logger.o \
             src/map.o
	$(CC) $^ -o $@ $(LDFLAGS)

src/monitor/main.o: src/monitor/main.c include/monitor.h include/monitor_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

src/monitor/monitor.o: src/monitor/monitor.c include/monitor.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

src/monitor/monitor_common.o: src/monitor/monitor_common.c include/monitor_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

src/map.o: src/map.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c





# test
bin/test_enemy: src/enemy_country.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

bin/citizen_manager: src/citizen_manager.o src/character.o src/logger.o src/utils.o
	$(CC) $^ -o $@ $(LDFLAGS)

src/citizen_manager.o: src/citizen_manager.c include/character.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

src/character.o: src/character.c include/character.h include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

src/logger.o: src/logger.c include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

src/utils.o: src/utils.c include/utils.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

bin/spy_simulation: src/spy_simulation.c include/semaphore_name.h
	$(CC) $^ -o $@ $(LDFLAGS)

bin/timer: src/timer.o
	$(CC) $^ -o $@ $(LDFLAGS)

src/timer.o: src/timer.c include/semaphore_name.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# COMMON OBJECTS FILES
# ----------------------------------------------------------------------------

src/common/logger.o: src/common/logger.c include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# Map
src/map.o: src/map.c include/map.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c


# ----------------------------------------------------------------------------
# CLEANING
# ----------------------------------------------------------------------------
clean:
	rm src/monitor/*.o src/common/*.o src/*.o

distclean: clean
	rm bin/monitor
	rm bin/timer
	rm bin/spy_simulation

