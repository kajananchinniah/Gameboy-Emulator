# Gameboy Emulator

## Note on implementation:
Originally, the plan was to write this in modern C++. That clearly did not pan out. The gameboy instruction set is quite
large so I ended up implementing it a pretty messy way. In hindsight, with better plan, there are lots of chunks that I
could have reused (e.g. some opcodes do basically the same thing but slightly differently). I did do some code reuse
(which is why my raw opcode count is much less than 512), in hindsight, I should have reused a lot more things.

Suggested CPU implementations:
- Hashmap that maps opcode -> a struct consisting of instruction. This struct would contain a field the "type of access" (e.g.
  use a register as an address vs read from memory vs just use a register directly), as well as a generic implementation
  for the instruction itself. I'm in too deep to do this though.

## Note on Tests:
Originally I had planned on unit testing every function I wrote via GTest. While this approach is fine, I realized that
it was simpler to just rely on tools that were specifically created for testing gameboys (e.g. test ROMs). Due to this,
the testing part of this project was mostly abandoned.

## Testing ROMs Acknowledgements
For CPU instructions, I used blargg's tests. This can be found at: https://gbdev.gg8.se/wiki/articles/Test_ROMs or https://github.com/retrio/gb-test-roms

## Debugging Acknowledgements
To get started on debugging my opcodes, I used BGB (https://bgb.bircd.org/) as a dissassembler on blargg's tests. Then I compared the output of the disassembler with my program (by printing out the status of my registers). This helped me find minor bugs that really messed up everyhing. (For a while my program was stuck on an infinite loop due to a bug :)). Once I got past the requirements for blargg's tests initialization, I shifted to using it exclusively.

## References
- Pandocs: https://gbdev.io/pandocs/
  - Contains everything you need to know about the gameboy
- gbops: https://izik1.github.io/gbops/index.html
  - Accurate opcode table for the gameboy (made planning easier)
- Gameboy CPU Manual: http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
  - Manual on the gameboy CPU
- Gameboy Complete Technical Reference: https://gekkio.fi/files/gb-docs/gbctr.pdf
  - In progress (as of Aug 21st 2021) technical reference that I used
- Took the Gameboy power up sequence from: https://gbdev.gg8.se/wiki/articles/Power_Up_Sequence
  - Note: this may affect compatability with all games
- Stackoverflow for half carry: https://stackoverflow.com/questions/57958631/game-boy-half-carry-flag-and-16-bit-instructions-especially-opcode-0xe8
  - I had trouble understanding how the half carry flag worked so I read this to understand
