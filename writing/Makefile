all: clean pdf

pdf:
	@echo Compiling PDF document
	git rev-parse --short HEAD > commit.tex
	latexmk -pdf -silent  main.tex

clean:
	@echo Cleaning files
	rm -f .aux *.log *.out *.toc *.lof *.lot *.bbl *.blg *.fls *.fdb_latexmk *.synctex.gz *.aux *.maf *.mtc0 *.mtc *run.xml main-blx.bib

.PHONY : pdf
