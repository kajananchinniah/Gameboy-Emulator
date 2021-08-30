# Gameboy Emulator

## Note on Tests:
Originally I had planned on unit testing every function I wrote via GTest. While this approach is fine, I realized that
it was simpler to just rely on tools that were specifically created for testing gameboys (e.g. test ROMs). Due to this,
the testing part of this project was mostly abandoned.

## Testing ROMs Acknowledgements
For CPU instructions, I used blargg's tests. This can be found at: https://gbdev.gg8.se/wiki/articles/Test_ROMs or https://github.com/retrio/gb-test-roms

## Debugging Acknowledgements
To debug my opcodes, I used BGB (https://bgb.bircd.org/) as a dissassembler on blargg's tests. Then I compared the output of the disassembler with my program (by printing out the status of my registers). This helped me find minor bugs that really messed up everyhing. (For a while my program was stuck on an infinite loop due to a bug :))

## References
- Pandocs: https://gbdev.io/pandocs/
  - Contains everything you need to know about the gameboy
- gbops: https://izik1.github.io/gbops/index.html
  - Accurate opcode table for the gameboy (made planning easier)
- Gameboy CPU Manual: http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
  - Manual on the gameboy CPU
- Gameboy Complete Technical Reference: https://gekkio.fi/files/gb-docs/gbctr.pdf
  - In progress (as of Aug 21st 2021) technical reference that I used
- Codeslinger: http://www.codeslinger.co.uk/pages/projects/gameboy/hardware.html
  - Took the gameboy initialization from this
