#!/bin/bash
#remove latex temp files
exts="aux bbl blg brf fdb_latexmk fls idx ilg ind lof log lol lot out toc synctex.gz"
for ext in $exts; do
       rm  *.$ext
done

