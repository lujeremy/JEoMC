# "make test" Compiles everything and runs the regression tests
.PHONY : test
test : all testall.sh
	./testall.sh

# "make all" builds the executable as well as the "draw" library designed
# to test linking external code
.PHONY : all
all : jeomc.native draw.o draw2.o
# draw.o

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

# Testing the "draw" example
draw : draw.c
	cc draw.c -o draw -DBUILD_TEST -framework OpenGL -framework GLUT -lglfw

draw2 : draw2.c
	cc draw2.c -o draw2 -DBUILD_TEST -framework OpenGL -framework GLUT -lglfw -lGLEW

##############################

