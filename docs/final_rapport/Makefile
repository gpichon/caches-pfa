all: rapport

rapport: rapport.tex rapport.bib
	pdflatex rapport.tex
	bibtex rapport
	pdflatex rapport.tex
	pdflatex rapport.tex

clean: 
	rm -f *.aux *.log *~ *.bbl *.blg *.dvi *.toc *.pdf
