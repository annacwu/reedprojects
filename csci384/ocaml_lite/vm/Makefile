# This makefile is partially addapted from https://stackoverflow.com/a/20830354

FLAGS = -Wall -Wextra

.PHONY: release debug profile clean prep

all: release

release: FLAGS += -O3
release: OUTDIR = release
release: prep $(OUTDIR)/ovm

debug: FLAGS += -g -DDEBUG
debug: OUTDIR = debug
debug: prep $(OUTDIR)/ovm

profile: FLAGS += -g -O3
profile: OUTDIR = profile
profile: prep $(OUTDIR)/ovm

SRCS = alloc.c machine_state.c readfile.c
OBJS = $(SRCS:.c=.o)
OUTOBJS = $(addprefix $(OUTDIR)/, $(OBJS))
EXE = $(OUTDIR)/ovm

prep:
	mkdir -p $(OUTDIR)

$(OUTDIR)/ovm: main.c stack.h types.h $(OUTOBJS)
	$(CC) $(FLAGS) -o $(OUTDIR)/ovm main.c $(OUTOBJS)

$(OUTDIR)/machine_state.o: machine_state.c stack.h types.h
	$(CC) $(FLAGS) -o $(OUTDIR)/machine_state.o -c machine_state.c

$(OUTDIR)/alloc.o: alloc.c alloc.h stack.h types.h
	$(CC) $(FLAGS) -o $(OUTDIR)/alloc.o -c alloc.c

$(OUTDIR)/readfile.o: readfile.c readfile.h types.h
	$(CC) $(FLAGS) -o $(OUTDIR)/readfile.o -c readfile.c

clean:
	rm -rf debug/ release/ profile/
