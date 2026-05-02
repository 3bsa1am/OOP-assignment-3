# OOP Assignment 3

Course assignment repository for an Object-Oriented Programming (OOP) project. It includes a written report (LaTeX/TeX), generated artifacts (PDF/PS), and implementation/demo materials (web + small amount of C++/JS).

## What’s in this repo

- **Documentation/report**: LaTeX/TeX sources and compiled outputs.
- **Web content**: HTML/CSS/JavaScript pages for presenting or demoing the work.
- **Source code**: C++ (and some JS) used in the assignment implementation.

## Languages

According to GitHub language stats:

- HTML (~62.9%)
- TeX (~23.9%)
- JavaScript (~4.7%)
- C++ (~3.4%)
- PostScript (~3.0%)
- CSS (~2.0%)

## Getting started

### Clone

```bash
git clone https://github.com/3bsa1am/OOP-assignment-3.git
cd OOP-assignment-3
```

### Build the report (LaTeX)

Install a LaTeX distribution (TeX Live / MiKTeX). Then, from the directory containing the main `.tex` file:

```bash
pdflatex main.tex
# run again if references are used
pdflatex main.tex
```

If the report uses BibTeX:

```bash
pdflatex main.tex
bibtex main
pdflatex main.tex
pdflatex main.tex
```

### View the web pages

If the repo contains an `index.html` (often under a `web/` folder), open it directly in a browser, or serve it locally.

Example (Python):

```bash
# run inside the folder that contains index.html
python -m http.server 8000
```

Then visit `http://localhost:8000`.

### Build / run the C++ code (if present)

```bash
# example only; adjust filenames as needed
g++ -std=c++17 -O2 -o app main.cpp
./app
```

## Notes

- Generated artifacts (PDF/PS) may be present in the repo; if you rebuild the report, outputs may change based on your TeX setup.
- This repository is primarily for academic/educational use.

## License

No license is currently specified. If you intend to reuse or redistribute any part of this repository, please contact the repository owner.
