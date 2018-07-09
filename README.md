HT2T(1)                                                                                  General Commands Manual                                                                                  HT2T(1)

NAME
       ht2t - extract pure text from html files

SYNOPSIS
       ht2t [options] [-f <input_file> | -d <input_directory>] [-o <output_file>]

DESCRIPTION
       ht2t Hypertext to text converter. The command parses an html file and outputs the textual content only in plain text format.  The output can be used for reading it with text-to-speech to visual‐
       ly impaired users or to postprocess it with text analysis software.

       ht2t is designed to accept input from wget (either in a pipe, or, in case of wget -r, from a directory tree) and give output to espeak (see examples).

       The input in case of -f (or when using the standard input) may contain a sequence of web pages, which are processed separately and concatenated to the output.

       The input is converted to UTF-8 before processing, if a meta-tag designating the charset ist present. Some character entities (&....;) are converted to UTF-8 too.

OPTIONS
       -f <input_file>
              Process <input_file>. If none of -f or -d is present, standard input is used.

       -d <input_directory>
              Process <input_directory> recursively. All files with the extensions .html and .htm are processed and concatenated to the output.

       -o <output_file>
              If present, the output is written to <output_file> instead of stdout.

       -h     Prints a short usage instruction and exits.

       -v     Adds debugging information to the output. Up to three 'v' are possible to increase the amount of information. This can result in quite a lot of output!

EXAMPLES
       This example reads the wikipedia entry about "Recitation":

       wget -O - https://en.wikipedia.org/wiki/Recitation | ht2t | espeak -v english  -g5 -b1

       The German article in a kind of German language:

       wget -O - https://de.wikipedia.org/wiki/Rezitation | ht2t | espeak -v german-mbrola-3 -p65 -g5 -b1

AUTHOR
       Written by Thomas Heckmann

COPYRIGHT
       Copyright © 2018 by the author. Under BSD license.  This is free software: you are free to change and redistribute it.  There is NO WARRANTY, to the extent permitted by law.

SEE ALSO
       wget(1), espeak(1)

  
