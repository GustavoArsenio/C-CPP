# Legacy ZIP Archive Review

The archive `Listas_Gustavo_Arsenio_de_Sousa_74830.zip` contains 30 entries,
all under `Lista_1/` and `Lista_2/`. A byte-for-byte comparison against the
tracked loose files found 26 identical entries and four source-file
differences:

- `Lista_1/Ex5.c`: the archive uses `%d` for floating-point values, while the
  loose tracked file uses `%.2f`; the latter is a correction to the format
  mismatch.
- `Lista_2/ex05.c`: only whitespace and final-newline differences.
- `Lista_2/ex07.c`: only whitespace differences.
- `Lista_2/matrizFill III.c`: only whitespace differences.

The archive contains no unique assignment PDF, fixture, or implementation.
Its only semantic difference is a less-correct format string. The loose
historical sources and all PDFs have separate migration records, so the ZIP is
classified `DEPRECATE` and need not be copied as another binary archive.
