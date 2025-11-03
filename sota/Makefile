all: pdf clean

pdf:
	@echo Compiling PDF document
	latexmk -cd -pdf -lualatex -silent -jobname=sota  main.tex

clean:
	@echo Cleaning files
	rm -f .aux *.log *.out *.toc *.lof *.lot *.bbl *.blg *.fls *.fdb_latexmk *.synctex.gz *.aux *.maf *.mtc0 *run.xml main-blx.bib