#!/bin/bash

rm *.aux  *.log *.out  *.toc *.bbl *.blg *.run.xml *.bcf

# pdflatex rapport.tex
# biber rapport
# pdflatex rapport
# pdflatex rapport
