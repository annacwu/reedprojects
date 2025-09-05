# Non-bipartite Dynamic Matching
An adaptation of [Laura Doval's 2022 Dynamic Stability](https://doi.org/10.3982/TE4187) algorithm for nonbipartite environments using Irving's algorithm in place of Gale-Shapley.

## `data.py`

This file generates data based on [Reed College first-year demographics](https://www.reed.edu/ir/firstyearstuethnic.html) for gender and sexuality distributions in a realistic queer stable matching setting. It defines the following structures:

`Player` class: 

Each Player object represents an individual with:

- `name`: A randomly generated full name (using `Faker`)
- `gender`: One of 'M', 'F', or 'X' (nonbinary)
- `sexuality`: One of 'straight', 'gay', 'lesbian', or 'bi', assigned probabilistically based on gender
- `arrival_round`: The period in which the player becomes active in the dynamic matching algorithm in `doval.py`
- `id`: A unique identifier used for hashing and comparison

`generatePlayers(numPlayers, numPeriods)`:

Generates a list of `Player` objects based on:

- A target number of players
- Random assignment of arrival rounds from 1 to `numPeriods`, where `numPeriods` represents how many rounds of dynamic matching there will be as per Doval's environment definition.

`generateData(players)`:

Generates a dictionary mapping each player to a randomized preference list of other compatible players:

- Compatibility is based on the player’s sexuality and gender, and includes all possible compatible matches in random shuffled order

## `doval.py`

This file contains the altered dynamic matching algorithm. `matcher(prefs, rounds)` serves as the main function, and calls various helper functions to carry out Doval's algorithm. Most of the implementation stays as true to Doval's description of the algorithm as possible, with the primary change being that where she calls Gale-Shapley, we call our implementation of Irving's algorithm. Additionally, the `worst_matches` function is used to calculate conjectures at the start, which takes a slightly different approach than the original algorithm by implementing recursive methods of running Irving's and truncating preferences for increased efficiency.

## `irving.py`
Irving's is a slighly altered version of the [original Irving's algorithm](https://uvacs2102.github.io/docs/roomates.pdf) as well, also implementing a recursive call to repeat the round 1 reduction in between cycle reductions until a stable match is found or players preference lists contain 1 person or less. It is wrapped in the `irving(prefs, worst)` function, though the actual algorithm is implemented in `irving_full(prefs, worst)`. This ensures that in practice, the algorithm outputs something we can work with from period to period in the `matcher` function. 

