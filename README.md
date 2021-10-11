# Gameboy Emulator
This is my messy implementation of a gameboy emulator using C++. I've been working on this on and off (whenever I had
time, but sometimes I was too busy to work on it). This is why my coding style may seem different depending on which
file you look at. I generally tried to refactor after finishing a subset of emulator though.

## Notes on implementation
Originally, the plan was to write this in modern C++. That clearly did not pan out. The gameboy instruction set is quite
large so I ended up implementing it a pretty messy way. In hindsight, with better plan, there are lots of chunks that I
could have reused (e.g. some opcodes do basically the same thing but slightly differently). I did do some code reuse
(which is why my raw opcode count is much less than 512), in hindsight, I should have reused a lot more things.

Suggested CPU implementations:
- Hashmap that maps opcode -> a struct consisting of instruction. This struct would contain a field the "type of access" (e.g.
  use a register as an address vs read from memory vs just use a register directly), as well as a generic implementation
  for the instruction itself. I'm in too deep to do this though.

I also realized that I rely on the MMU for handling interrupts, timers and PPU registers. This isn't necessarily a bad thing, since they are located on specific memory addresses, but in hindsight, I think it would have been cleaner to have separated out these functionalities into where they should go (e.g. have an interrupt manager, timer manager and put the relevant PPU functions into the PPU). I may refactor later to do this.

My emulator is clock accurate on the opcode level but isn't clock accurate in terms of memory accesses or interrupts.
Doing it at the whole interrupt level might be something I explore if it's necessary (I have it implemented but it's
unused because I can't run the interrupt timing test for some reason), but doing it at the memory access
/ actual subopcode level is a bit inconvenient since I didn't realize this was necessary. In hindsight, I should have
read the entire pandoc instead of just reading as I implemented.

## Note on Tests
Originally I had planned on unit testing every function I wrote via GTest. While this approach is fine, I realized that
it was simpler to just rely on tools that were specifically created for testing gameboys (e.g. test ROMs). Due to this,
the testing part of this project was mostly abandoned.

## Testing ROMs Acknowledgements
For CPU instructions, I used blargg's tests. This can be found at: https://gbdev.gg8.se/wiki/articles/Test_ROMs or https://github.com/retrio/gb-test-roms

I currently pass the individual CPU instruction tests and the instruction timing test.

## Debugging Acknowledgements
To get started on debugging my opcodes, I used BGB (https://bgb.bircd.org/) as a dissassembler on blargg's tests. Then I compared the output of the disassembler with my program (by printing out the status of my registers). This helped me find minor bugs that really messed up everyhing such as accidental infinite loops due to mixing up the order of registers in some instructions. Once I got past the requirements for blargg's tests initialization, I shifted to using it exclusively.

## References
- Pandocs: https://gbdev.io/pandocs/
  - Contains everything you need to know about the gameboy
- gbops: https://izik1.github.io/gbops/index.html
  - Accurate opcode table for the gameboy (made planning easier)
- Gameboy CPU Manual: http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
  - Manual on the gameboy CPU
- Gameboy Complete Technical Reference: https://gekkio.fi/files/gb-docs/gbctr.pdf
  - In progress (as of Aug 21st 2021) technical reference that I used
- Stackoverflow for half carry: https://stackoverflow.com/questions/57958631/game-boy-half-carry-flag-and-16-bit-instructions-especially-opcode-0xe8
  - I had trouble understanding how the half carry flag worked so I read this to understand
- Cinoop: https://cturt.github.io/cinoop.html
  - Used it for reference to inspire implementation (specifically registers)
- Codeslinger: http://www.codeslinger.co.uk/pages/projects/gameboy/beginning.html
  - High level tutorial for writing a gameboy emulator that I used when I was stuck and wanted another perspective on
    details of things
- Random NES website: http://archive.nes.science/nesdev-forums/f20/t15944.xhtmlhttp://archive.nes.science/nesdev-forums/f20/t15944.xhtml
  - Contains an explanation of DAA + implementation of DAA instruction that I copied for my DAA instruction
- Eric's blog: https://ehaskins.com/2018-01-30%20Z80%20DAA/
  - Contains an explanation of both what BCD is & what DAA is (more detailed) + implementation of DAA instruction
- The Gameboy Emulator Development Guide: https://hacktixme.ga/GBEDG/
  - At the time of use, it's incomplete but contains a really good PPU reference that explains it with visuals, etc.
    This helped me understand what to do (as I found it pretty confusing). (Link to PPU reference:
    https://hacktixme.ga/GBEDG/ppu)
- POKEGB: a gameboy emulator that only plays Pokemon blue: https://binji.github.io/posts/pokegb/
  - This inspired me to do all my rendering at the end of a scanline, rather than doing pixel fifo. I found the pixel
    fifo algorithm to be relatively complicated (and given how busy I've been, I didn't have enough time to understand
    the algorithm details, although I have a high level understanding, so I may revisit it later)
