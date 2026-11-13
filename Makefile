CC=g++

CFLAGS=-ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts\
-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self\
-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default \
-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix\
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs\
-Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow \
-flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla \
-fsanitize=address,leak,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

DEFINES=


all: make_tree.o tree.o run_tree.o tree_to_graphviz.o read_tree.o akinator.o
	@$(CC) $(CFLAGS) $(DEFINES) make_tree.o tree.o run_tree.o tree_to_graphviz.o read_tree.o akinator.o -o tree.out
	@rm -f make_tree.o tree.o run_tree.o tree_to_graphviz.o read_tree.o akinator.o

make_tree.o:
	@$(CC) $(CFLAGS) $(DEFINES) -c Tree/make_tree.cpp -o make_tree.o

tree.o:
	@$(CC) $(CFLAGS) $(DEFINES) -c Tree/tree.cpp -o tree.o

run_tree.o:
	@$(CC) $(CFLAGS) $(DEFINES) -c Tree/run_tree.cpp -o run_tree.o

tree_to_graphviz.o:
	@$(CC) $(CFLAGS) $(DEFINES) -c Tree/tree_to_graphviz.cpp -o tree_to_graphviz.o

read_tree.o:
	@$(CC) $(CFLAGS) $(DEFINES) -c Tree/read_tree.cpp -o read_tree.o

akinator.o:
	@$(CC) $(CFLAGS) $(DEFINES) -c Tree/akinator.cpp -o akinator.o
