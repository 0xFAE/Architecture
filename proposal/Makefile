# configuration ---------------------------------------------------#

document := root
latex := latex
bibtex := bibtex

# Document recipe -------------------------------------------------#

$(document).pdf: $(document).tex
	$(latex) $(document)
	$(bibtex) $(document)
	$(latex) $(document)
	$(latex) $(document)
	dvips -t letter -Ppdf $(document)
	ps2pdf $(document).ps

# clean up temporary files ----------------------------------------#
.PHONY: clean
clean:
	@echo Cleaning temporary files...
	@rm -f *.out *.aux *.bbl *.dvi *.log *.pdf *.ps

#------------------------------------------------------------------#
