TARGET=talk

build/${TARGET}.pdf: ${TARGET}.tex
	mkdir -p build
	lualatex --output-directory=build --shell-escape $^

clean:
	rm -rf build
	rm -f ${TARGET}.pyg
	rm -rf _minted-talk

open: build/${TARGET}.pdf
	open build/${TARGET}.pdf
