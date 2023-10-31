# Planet Drop Game
## Goal
The goal of this project is to create my own adaptation of the Japanese Suika game using only my own (personally-created) assets. This includes sounds, artwork, and game mechanics, etc.

## The Program
### Suika Game
The Japanese Suika game is a game in which the user drops different fruits into a container. When two of the same fruit touch, they combine into a single, larger fruit. There are ten fruits ranging from a cherry to a watermelon; the watermelon is the only fruit that does not combine with another instance of itself to form a larger fruit. The player loses the game when any fruit pokes above the top opening of the container.   

Each time two fruits combine, the user's score is increased. Additionally, a high score is recorded at the end of each highest scoring game. Lastly, there is an image displaying the next fruit to be dropped in the top-right corner of the screen.

### Planet Drop
In order to create my Planet Drop game, I took inspiration for multiple game mechanics and HUD decisions from the Suika game. Instead of ten different fruits, my game uses ten planetary bodies (and the moon). These bodies are in order of size, similar to the Suika game, and any time two of the same planet touch, they combine to form the next biggest planet. The planets available to the player to drop are randomized among the first five planets, utilizing a Queue structure of planet objects. By utilizing the queue, I am able to display the next drop that the player will have available to them. Unlike the Suika game, the players of Planet Drop do not fail if the planets poke above the top of the container. Instead, the planets must fall out of the container into the lava in order for the player to lose.   

The HUD of my Planet Drop game is very similar to the Suika game in the placement of the scores, image for the next drop, and order of drops by size. I have made an attempt to create a responsive HUD for different resolutions that may be used for playing my game.   

To play the game, unzip the PlanetDrop.zip and open the Planet Drop.exe application inside the uncompressed folder.

### Further Improvements
The following is a list of potential improvements for the game:  
- Additional sounds
  - Planets colliding
  - Planets combining
  - Ambient background noise
- Impoved artwork
  - Container
  - Some of the planets
  - Artwork for HUD
- Optimize the size of the planets and container for the optimal experience