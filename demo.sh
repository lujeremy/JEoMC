#!/bin/sh

# Regression testing script for JEoMC
# Step through a list of files
#  Compile, run, and check the output of each expected-to-work test
#  Compile and check the error of each expected-to-fail test

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     OS=Linux;;
    Darwin*)    OS=Mac;;
    CYGWIN*)    OS=Cygwin;;
    MINGW*)     OS=MinGw;;
    *)          OS="UNKNOWN:${unameOut}"
esac

# generate C linkers based on OS
if [ ${OS} = "Mac" ]; then
    # Mac OSX
    printf "\nYou are currently running on ${OS}, appropriate C flags will be generated\n\n"
    FLAGS="-framework OpenGL -framework GLUT -lglfw -lGLEW"

    # Path to the LLVM interpreter
    LLI="/usr/local/opt/llvm@8/bin/lli"

    # Path to the LLVM compiler
    LLC="/usr/local/opt/llvm@8/bin/llc"
elif [ ${OS} = "Linux" ]; then
    # Assume Linux.
    printf "\nYou are currently running on ${OS}, appropriate C flags will be generated\n\n"
    FLAGS="-lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm -lGLEW"

    # Path to the LLVM interpreter
    LLI="lli"

    # Path to the LLVM compiler
    LLC="llc"
else
    printf "\nYou are currently running on ${OS}, we do not support this system\n\n"
fi

# Path to the C compiler
CC="cc"

# Path to the jeomc compiler.  Usually "./jeomc.native"
# Try "_build/jeomc.native" if ocamlbuild was unable to create a symbolic link.
JEOMC="./jeomc.native"

# Set time limit for all operations
ulimit -t 30

globallog=testall.log
rm -f $globallog
error=0
globalerror=0

keep=0

Usage() {
    echo "Usage: testall.sh [options] [.j files]"
    echo "-k    Keep intermediate files"
    echo "-h    Print this help"
    exit 1
}

SignalError() {
    if [ $error -eq 0 ] ; then
  echo "FAILED"
  error=1
    fi
    echo "  $1"
}

# Run <args>
# Report the command, run it, and report any errors
Run() {
    echo $* 1>&2
    eval $* || {
  SignalError "$1 failed on $*"
  return 1
    }
}

Check() {
    error=0
    basename=`echo $1 | sed 's/.*\\///
                             s/.j//'`
    reffile=`echo $1 | sed 's/.j$//'`
    basedir="`echo $1 | sed 's/\/[^\/]*$//'`/."

    echo -n "$basename..."

    echo 1>&2
    echo "###### Testing $basename" 1>&2

    generatedfiles=""

    generatedfiles="$generatedfiles ${basename}.ll ${basename}.s ${basename}.exe ${basename}.out" &&
    Run "$JEOMC" "$1" ">" "${basename}.ll" &&
    Run "$LLC" "-relocation-model=pic" "${basename}.ll" ">" "${basename}.s" &&
    Run "$CC" "-o" "${basename}.exe" "${basename}.s" "jeomc_draw.o" "$FLAGS" &&
    Run "./${basename}.exe" > "${basename}.out" &&

    # Report the status and clean up the generated files

    if [ $error -eq 0 ] ; then
        if [ $keep -eq 0 ] ; then
            rm -f $generatedfiles
        fi
        echo "OK"
        echo "###### SUCCESS" 1>&2
    else
        echo "###### FAILED" 1>&2
        globalerror=$error
    fi
}

while getopts kdpsh c; do
      case $c in
    k) # Keep intermediate files
        keep=1
        ;;
    h) # Help
        Usage
        ;;
      esac
done

shift `expr $OPTIND - 1`

LLIFail() {
  echo "Could not find the LLVM interpreter \"$LLI\"."
  echo "Check your LLVM installation and/or modify the LLI variable in testall.sh"
  exit 1
}

which "$LLI" >> $globallog || LLIFail

if [ $# -ge 1 ]
then
    files=$@
else
    files="demo/*.j"
fi

for file in $files
do
    case $file in
  **)
      Check $file 2>> $globallog
      ;;
  *)
      echo "unknown file type $file"
      globalerror=1
      ;;
    esac
done

exit $globalerror
