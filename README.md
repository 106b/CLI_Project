Usage: Basic Unix-like Commands

pwd 0 "Print Working Directory" — Prints the full path name of the working directory.

cd 1

"Change Directory" — Changes the working directory to that specified by the
argument, which must be one of the following:
• the name of a directory in the working directory;
• the string "..", meaning the parent directory (the root is its own parent);
or
• the string "/", meaning the root directory.

ls 0 or 1

"LiSt files" — If given without arguments, prints the names of files in the
working directory in alphabetical order; if given with the name of a text file in the
working directory, prints that name; if given with the name of a directory in the
working directory, prints the names of files in that directory in alphabetical order
by name; otherwise prints nothing.

cat ≥ 1 

"conCATenate" — Prints the contents of the text files named by the arguments.

mkdir 1 

"MaKe DIRectory" — Creates a directory with the given name. The working
directory must not already contain a file with that name.

rmdir 1

"ReMove DIRectory" — Removes the directory with the given name. The
working directory must contain the named directory, and the named directory
must be empty.

create 1

Creates a "text file" with the given name, then reads the contents of the file from
standard input. The working directory must not already contain a file with the
given name.

rm 1 

"ReMove" — Removes the named text file from the working directory. The

working directory must contain the named file.

cp 2

"CoPy" — Creates a copy of the first argument in the working directory; the
second argument specifies the name of the copied file. If the first argument names
a directory, the copy will contain copies of the directory's files and subdirectories.
(I.e. it is a deep copy.) The working directory must contain the file named by the
first argument, and must not already contain a text file whose name is the second
argument.

mv 2

"MoVe" — If the second argument names a directory, moves the first argument
into the directory; the directory must not already contain a file whose name is the
first argument. If the working directory does not contain a file whose name is the
second argument, changes the name of the file named by the first argument to be
the second argument. The working directory must not contain a file whose name
is the second argument.
