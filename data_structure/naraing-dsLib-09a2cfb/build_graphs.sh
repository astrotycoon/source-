#!/bin/bash
lst_files=*.lst
out_dir=graphs

echo "Creating graphs from .lst files"

[ ! -d $out_dir ] && echo "Creating directory $out_dir" && mkdir $out_dir

for f in $lst_files
do
  echo Graphing "$f"
  dot -Tpng $f > $out_dir/$f.png
done
