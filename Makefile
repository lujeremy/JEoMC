all : calc.out

# "ocamlbuild calc.native" will also build the calculator

calc : parser.cmo scanner.cmo calc.cmo
	ocamlc -w A -o calc $^

%.cmo : %.ml
	ocamlc -w A -c $<

%.cmi : %.mli
	ocamlc -w A -c $<

scanner.ml : scanner.mll
	ocamllex $^

parser.ml parser.mli : parser.mly
	ocamlyacc $^

calc.out : calc calc.tb
	./calc < calc.tb > calc.out

# Depedencies from ocamldep
calc.cmo : scanner.cmo parser.cmi ast.cmi
calc.cmx : scanner.cmx parser.cmx ast.cmi
parser.cmo : ast.cmi parser.cmi
parser.cmx : ast.cmi parser.cmi
scanner.cmo : parser.cmi
scanner.cmx : parser.cmx


##############################

ZIPFILES = README Makefile zipWith3.ml zipWith3.tb wordcount.mll \
	scanner.mll ast.mli parser.mly calc.ml calc.tb

hw1.zip : $(ZIPFILES)
	cd .. && zip hw1/hw1.zip $(ZIPFILES:%=hw1/%)

.PHONY : clean
clean :
	rm -rf zipWith3.out wordcount.ml wordcount wordcount.out \
	*.cmi *.cmo parser.ml parser.mli scanner.ml calc.out calc
