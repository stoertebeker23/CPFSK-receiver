project = presentation

project : $(project).bib $(project).tex
	pdflatex -shell-escape $(project) 
	bibtex $(project)
	pdflatex -shell-escape $(project) 
	pdflatex -shell-escape $(project) 

clean:
	rm -f $(project).aux $(project).bbl $(project).blg $(project).dvi $(project).fdb_latexmk $(project).log $(project).nav $(project).out $(project).pdf $(project).snm $(project).toc missfont.log

fresh:
	make clean && make
