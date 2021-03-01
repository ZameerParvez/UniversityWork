<!-- <style>
    img {
        max-width: 100%;
        max-height: 500px;
    }
    figure {
        max-width: 70%;
        text-align: center;
        margin-left: auto;
        margin-right: auto;
    }
</style> -->

# University Coursework

This repo contains a sample of some of the work I did at university

## Graphics

[To the Graphics Project](Graphics/README.md)

## Games Simulations

- This project is a demo for simple game physics, and builds upon the universities existing framework
- Features:
  - Fixed time step physics loop
  - Running animations for player, coins and robots
  - Axis aligned collision detection and resolution, for quadrilaterals and circles
  - Momentum physics for elastic collisions
  - Spring physics for balloons
  - Boid flocking for the robots
  - Quad trees for broad phase collision detection
  - A collision volume structure which could support nested collision volumes, for more precise collision detection
  - Debugging output for collision volumes and quad tree leaves
- Usage:
  - Open the solutions file in visual studio
  - Set the start up project to "CSC3222 2019"
  - Then build and run through visual studio
  - The only controls are the arrow keys to move the player around

<figure>
  <img src="Games Simulations/Screenshots/CollisionVolumesAndQuadTree.png" alt="Image showing debug output for collision volumes and the quad tree leaves"/>
  <figcaption><em>
  This image shows the collision volumes, outlined by the blue and red squares/circles.
  It also shows the leaves of the quad tree with the light blue outline.
  </em></figcaption>
</figure>

## Cryptography Work

- Frequency analysis on cypher text produced by a shift cypher, using chi squared test to find a key that produces a distribution of characters most similar to a large sample of English text
- Cryptanalysis on cypher text produced by a Vigenere cypher, by calculating the mean [index of coincidence](https://en.wikipedia.org/wiki/Index_of_coincidence) for all key sizes within a set range, which involves splitting the cypher text into columns. Then for each column find the key used for it's shift using frequency analysis
- Built an adversary in a [chosen plain text](https://en.wikipedia.org/wiki/Chosen-plaintext_attack) oracle experiment, where the adversary always wins. This is to show that if the IV is predictable then AES CBC mode is not [IND-CPA](https://en.wikipedia.org/wiki/Ciphertext_indistinguishability#Indistinguishability_under_chosen-plaintext_attack_(IND-CPA)) secure
- Implemented [Goldwasser-Micali](https://en.wikipedia.org/wiki/Goldwasser%E2%80%93Micali_cryptosystem) (GM) asymmetric encryption scheme

## CSS Box Model Tutorial

- This is a simple website detailing what the box model for css is
- It also contains an interactive box written in js, to demonstrate the box model, and a tutorial on how to make it

[The Site](CSS%20Box%20Model%20Tutorial/Website-HTML.html)

## ASM Work

- This is a collection of solutions to problems given a the first year module
- It uses a reduced 8086 instruction set simulator http://www.softwareforeducation.com/sms32v50/
- The final coursework was to code a lift controller, which prints some information about the lifts state, and allows the lift to be interrupted to move down at any time, within 256 bytes of memory, including the instructions

<figure>
  <img src="ASM work/Screenshots/ASM-Memory.png" alt="Memory layout of ASM"/>
  <figcaption><em>
  This is an image of the memory layout, as well as the simulated lift and display.
  The blank area at the bottom of the memory address is the memory mapped to the display.
  The blue highlighted square indicates the current top of the stack, which grows backwards from the end of the non-display memory.
  </em></figcaption>
</figure>
