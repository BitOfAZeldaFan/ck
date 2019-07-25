#!/bin/bash
set -e

GRN='\033[0;32m'
RED='\033[0;31m'
BLK='\033[0m'

INFO="${GRN}Info:${BLK}"
WARN="${RED}Warning:${BLK}"

VERSION="C/K Builder Tool v5.0"
OPTIONS=()
TARGET=()
ROOT="."
OBJ="obj"
BIN="bin"
IMAGE="test.img"

CARCH="-m32 -mtune=i386"                   # Architecture options
CSTD="-std=c99 -nostdlib "                 # C Language options
CEMBED="-ffreestanding -fno-exceptions"    # Embedded options
CINC="-Isrc/include -Isrc/asm"             # File inclusion options
CWARN="-Wall -Wextra -Werror -Wno-unused"  # Warning options

LARCH="-m elf_i386 -A i386"   # Architecture options
LMAP="-Map kernel.map"        # Output map generation
LSCRIPT="-T linker.ld"        # Linker script options
LLIBS="-Llib"                 # Library options

LFLAGS=" $LARCH $LMAP $LSCRIPT $LLIBS "     # Linker options

CFLAGS=" $CARCH $CSTD $CEMBED $CINC $CWARN -Wfatal-errors " 

#QFLAGS=" -ctrl-grab -m 16 -d cpu_reset "
QFLAGS=" "

SOURCES=(" src src/asm ")

function printhelp {
	printf "\n
$VERSION\n
Usage: build -option [files]\n
Options:\n
-c    	Compile C source code in /src/core\n
-c-all	Compile everything in /src/core\n
-a    	Assemble ASM source code in /src\n
-m		Compile the main.c file in /src\n
-l		Link together all files in /obj\n
-make	Assemble, compile, and link Everything\n
-lib      Compiles the contents of a target folder into a linkable library\n
-h    	Display this help text\n"
}

function main {
	for i in $@; do
		case "$i" in
			"-c") OPTIONS+=" -c " ;;
			"-a") OPTIONS+=" -a " ;;
			"-h") OPTIONS+=" -h " ;;
			"-l") OPTIONS+=" -l " ;;
			"-lib") OPTIONS+=" -lib " ;;
			"-make") OPTIONS+=" -make " ;;
			"-run") OPTIONS+=" -run " ;;
			"-dev") OPTIONS+=" -dev " ;;
			*) TARGET+=" $i " ;;
		esac
	done
	
	for i in $OPTIONS; do
		case $i in
			"-c") compile;;
			"-a") assemble ;;
			"-h") printhelp ;;
			"-l") link ;;
			"-lib") make_library ;;
			"-make") make_all;;
			"-run") run ;;
			"-dev") DEV="1" ;;
			*) printf "$WARN Unknown option '$i'\n" ;;
		esac
	done
}

function make_all {
     rm obj/*.o || printf "$INFO No object files to delete\n"
	TARGET+=$(ls -R | grep "\.c\|\.asm")
	assemble
	compile
	link
}

function make_library {
   
   for LIB in $TARGET; do
      SRC=$(ls src/$LIB | grep "\.c")
      for i in $SRC; do
         NAME=`echo "$i" | cut -d'.' -f1`
         gcc -c src/$LIB/$NAME.c -o src/$LIB/obj/$NAME.o $CFLAGS
      done
      printf "$INFO Compiling lib$LIB.a\n"
      ar r lib/lib$LIB.a src/$LIB/obj/*.o
   done
}

function assemble {
	for i in $TARGET; do
		NAME=`echo "$i" | cut -d'.' -f1`
		if [ -f src/asm/$NAME.asm ]; then
			printf "$INFO Assembling $i\n"
			nasm -f elf src/asm/$NAME.asm -o $OBJ/$NAME.o
		fi
	done
}

function compile {
	for DIR in $SOURCES; do
		for SRC in $TARGET; do
			NAME=`echo "$SRC" | cut -d'.' -f1`
			if [ -f $DIR/$NAME.c ]; then
				printf "$INFO Compiling $SRC\n"
				gcc -c $DIR/$NAME.c -o $OBJ/$NAME.o $CFLAGS
			fi
		done
	done
}

function link {
     OBJFILES=""
     LIBFILES=""
     
     
     # Make sure that there are object files to link before linking them.
	# If there are no object files, skip it.
	if [ "$(ls -A obj)" ]; then
	     printf "$INFO Linking object files:\n"
	     ls obj/
	     OBJFILES=" obj/*.o "
	else printf "$WARN No object files to link\n" 
	fi
     
	
	# Make sure that there are libaries to link before linking them.
	# If there are no libraries, skip it.
	if [ "$(ls -A lib)" ]; then
	     printf "$INFO Linking libraries:\n"
	     ls lib/
	     LIBFILES=" lib/*.a "
	else printf "$WARN No libraries to link\n"
	fi
	
	# Calculate the old size of the kernel binary, just for comparative sake.
	# This doesn't really effect compilation. I'm just curious
	printf "\n"
	if [ -f "bin/coffee" ]; then
		OLDSIZE=$(stat -c%s "bin/coffee")
	else
		OLDSIZE=0
	fi
	
	#printf "$INFO using linker flags \"$LFLAGS\"\n"
	
	#### ACTUAL LINKER COMMAND ################################################
	ld -o bin/coffee $OBJFILES $LFLAGS $LIBFILES
	###########################################################################
	NEWSIZE=$(stat -c%s "bin/coffee")
	
	printf "$INFO Generated binary bin/coffee ($NEWSIZE bytes)\n"
	printf "$INFO (Net change: $(( $NEWSIZE - $OLDSIZE )) bytes)\n"
}

function run {
   printf "$INFO Executing C/K with QEMU...\n"
   qemu-system-i386 -kernel bin/coffee $QFLAGS
}


main $@


 