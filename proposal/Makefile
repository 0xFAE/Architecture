# configuration ---------------------------------------------------#

document := report
latex := pdflatex

# Document recipe -------------------------------------------------#
default:
	$(latex) $(document)
	bibtex $(document)
	$(latex) $(document)
	$(latex) $(document)

# clean up temporary files ----------------------------------------#
clean:
	@echo Cleaning temporary files...
	@rm -f *.out *.aux *.bbl  *.blg *.dvi *.log *.pdf *.ps

#------------------------------------------------------------------#
