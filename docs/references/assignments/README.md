# Historical Assignment Statements

The PDFs in this directory are preserved byte-for-byte from the repository
baseline. Their accessible text was reviewed locally with a temporary `pypdf`
installation; no PDF extraction dependency was added to the project. Exercise
records link to their source PDFs.

| File | Original path | Reviewed content / status |
| --- | --- | --- |
| `lista-1.pdf` | `Lista_1/Lista_1 (1).pdf` | Methodology, nine required integer/array exercises, and six complementary exercises. Required items 1–9 are catalogued. |
| `lista-2-ex02-array-fill-1.pdf` | `Lista_2/ex02 - Array_fill_1.pdf` | URI 1173: ten doubled values, exact `N[i] = X` format, input less than 50. |
| `lista-2-ex05-fibonacci-array.pdf` | `Lista_2/ex05 - Fibonacci Array.pdf` | Multiple test cases, indices 0–60, exact `Fib(N) = X` format. |
| `lista-2-ex07-array-fill-iii.pdf` | `Lista_2/ex07 - Array Fill III.pdf` | 100 successive halvings, four decimal places. |
| `lista-2-ex08-array-fill-iv.pdf` | `Lista_2/ex08 - Array Fill IV.pdf` | Fifteen values into reusable even/odd buffers of five; print full buffers and odd leftovers first. |
| `lista-2-ex10-line-in-array.pdf` | `Lista_2/ex10 - Line in Array.pdf` | URI Line in Array: row 0–11 and sum/mean over the green region illustrated in the PDF. Floating-point values and one decimal place. |
| `lista-2-ex13-below-main-diagonal.pdf` | `Lista_2/ex13 - Below the Main Diagonal.pdf` | Sum/mean of the 66 floating-point values strictly below the diagonal; one decimal place. |
| `lista-2-matrix-fill-i.pdf` | `Lista_2/matrizFill I.pdf` | URI 1435, multiple sizes through zero; concentric matrix and width-three fields. |
| `lista-2-matrix-fill-iii.pdf` | `Lista_2/matrizFill III.pdf` | URI 1557, sizes through 15 and zero sentinel; powers of two with computed field width. |
| `lista-4.pdf` | `Lista_4/lista4.pdf` | Three scanned image pages, transcribed by visual review as 12 linked-list exercises; all twelve now have tested implementations. |
| `trabalho-1.pdf` | `Trabalho/Trabalho1.pdf` | Reviewed: linked-list structures and a Truco Paulista game specification. Core game rules, tested state transitions, automatic and user-controlled frontends are summarized in `specifications.md`; ambiguities left by the assignment are recorded as explicit policies or unresolved caller choices. |

The matrix exercises' code and smoke fixtures follow the assignment statement.
For `Line in Array`, the region boundaries were read from the source figure;
its visible sample output is also retained in the PDF.
