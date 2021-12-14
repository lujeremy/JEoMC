# "make test" Compiles everything and runs the regression tests
.PHONY : test
test : all testall.sh
	./testall.sh

# "make all" builds the executable as well as the "draw" library designed
# to test linking external code
.PHONY : all
all : jeomc.native draw.o draw2.o

# "ocamlbuild calc.native" will also build the calculator
#before: opam config exec --
jeomc.native :
	opam exec -- \
	ocamlbuild -use-ocamlfind jeomc.native

# "make clean" removes all generated files
.PHONY : clean
clean :
	ocamlbuild -clean
	rm -rf testall.log ocamlllvm *.diff *.err *.ll *.s *.exe *.out
	rm -f *.o draw draw2 img.png


##############################

# Not actually necessary
# draw : draw.c
# 	cc -o draw -DBUILD_TEST draw.c
#
# draw2 : draw2.c
# 	cc -o draw2 -DBUILD_TEST draw2.c
