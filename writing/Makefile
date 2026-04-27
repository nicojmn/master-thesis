all: pdf

pdf:
	@echo Compiling PDF document
	latexmk -pdf -silent -shell-escape main.tex
	makeglossaries main
	latexmk -pdf -silent -shell-escape main.tex
	@echo Done !
clean:
	@echo Cleaning files
	rm -f .aux *.log *.out *.toc *.lof *.lot *.bbl *.blg *.fls *.fdb_latexmk *.synctex.gz *.aux *.maf *.mtc0 *.mtc *run.xml main-blx.bib

.PHONY : pdf
