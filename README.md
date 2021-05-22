# 6.851 Fractional Cascading Coding Project 
## By: Isabelle Quaye, Ricardo Gayle, Shawn Imm, and Temi Omitoogun

This project implements fractional cascading over graphs, as described in
"[Fractional Cascading I. A Data Structuring Technique](https://www.cs.princeton.edu/courses/archive/spring12/cos423/bib/FractionalCascading1.pdf)"
by Chazelle and Guibas (1986).

We also implement two applications from
"[Fractional Cascading II. Applications](https://www.cs.princeton.edu/courses/archive/spring12/cos423/bib/FractionalCascading2.pdf)":

* Intersecting a Polygonal Path with a Line (Section 3)
* Visualization of hive graph from Orthogonal Range Search (Section 5)

## Usage

Build the C++ portion of the code via `make` (requires GCC).

To run the Python visualizations (also spawned by the C++ code),
you need the following dependencies:

* NumPy (`pip install numpy`)
* Matplotlib (`pip install matplotlib`)
