# BareDSP - Audio Processing in Pure C

This is an **audio processing project written entirely in C**, with no external libraries or dependencies.

My goal is to explore and implement core audio DSP concepts from scratch, while maintaining full (and maybe too much) control over performance and memory.

This project does not currently aim to be a complete DSP library, nor does it claim to be performant in any sense, because it serves as a sandbox for learning and iterating on low-level audio concepts.

Feel free to fork, study, or contribute ideas — this is a hobby project with an no real vision.

## What I'm currently working on

- **FFT** - Fast Fourier Transform, implemented from scratch (Currently only DFT is implemented, which is slow but works)
- ~~**STFT** - Short-Time Fourier Transform, with a set window function and variable window size and hop length~~ Done! Here's an STFT of my beautiful voice!<br><image src=meta/stft.png width="512">
- **Bitmap image export** - To visualize output data (Currently uses matplotlib, and I hate python.)
- ~~**Chromagram generation** - For pitch class profiles and harmonic content~~ I also have that now! You can even use it to tell I sing waaay off-key 🫢 <br><image src=meta/chroma.png width="512">

## Potential future Ideas

These may be added later as the project evolves and I get bored:

- Chord recognition using the Viterbi algorithm, focused on major/minor detection and based on harmonic trends and data from [Hooktheory Trends](https://www.hooktheory.com/trends)
- Maybe X11 API integration? Will be used for simple visual output




