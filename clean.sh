#!/bin/bash
DIR=~/Desktop/SScript
cd $DIR
say 'creating files'
touch 0{a..z}
say "showing files"
ls
say "Cleaning files. "
find . -empty -delete
say "Done "
ls 
