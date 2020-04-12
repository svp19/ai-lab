# High-Low Card Game MDP

The game High-Low is a card game played with an infinite deck containing threetypes of cards:  2,3,4. 
You start with a 3 showing(,ie, the start state), and say eitherhigher or lower. 
Then, a new card is flipped; if you say higher and the new card is higher in value than your current card, you win the points shown on the new card.
Similarly, if you say lower and the new card is lower in value than your current card,you win the points shown on the new card. 
If the new card is the same value as yourcurrent card, you don’t get any points. Otherwise, the you get a penalty for guessing wrong. 
Your current card is discarded and the new card becomes your current card.

* Source code is in `src/`

* Example inputs in `inputs/`

* Report in `report.pdf`

## Steps to run 

./run.sh \<path-to-input-file\>

Example -

`./run.sh inputs/1`

