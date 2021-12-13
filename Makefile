# "make test" Compiles everything and runs the regression tests
.PHONY : test
test : all testall.sh
	./testall.sh

# "make all" builds the executable as well as the "draw" library designed
# to test linking external code
.PHONY : all
all : jeomc.native draw.o
# draw.o

# "ocamlbuild calc.native" will also build the calculator

jeomc.native :
	opam config exec -- \
	ocamlbuild -use-ocamlfind jeomc.native

# "make clean" removes all generated files
.PHONY : clean
clean :
	ocamlbuild -clean
	rm -rf testall.log ocamlllvm *.diff
	rm *.o

# Testing the "draw" example
draw : draw.c
	cc -o draw -DBUILD_TEST draw.c

##############################

