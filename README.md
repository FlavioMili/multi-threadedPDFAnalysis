# PDF Analyzer: Single-threaded vs Multithreaded

This project demonstrates the performance difference between single-threaded and multithreaded implementations for analyzing text data extracted from a PDF file. The analysis includes statistics like the total number of words, the longest word, and the average word length.

## Files
- **`SinglePDFAnalyzer.cpp`**: Single-threaded implementation of the PDF analyzer.
- **`MultiPDFAnalyzer.cpp`**: Multithreaded implementation of the PDF analyzer.
- **`text.pdf`**: Sample PDF file used for testing, almost 7000 pages of random characters.

## Features
- Extract text from a PDF using the **Poppler** library.
- Analyze the extracted text to compute:
  - **Total word count**
  - **Longest word**
  - **Average word length**
- Compare performance between single-threaded and multithreaded approaches.

## Requirements
- **CMake** (3.10 or higher)
- **Poppler** library (with C++ bindings)
- A C++ compiler supporting **C++17**

## Performance comparison
The single-threaded implementation needs around 18 seconds in order to print the statistics, versus the 5 seconds of the multi-threaded implementation, this translates as a 360% improvement.
