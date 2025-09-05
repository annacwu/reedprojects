from typing import Optional, Generator
from data import Player, generateData, generatePlayers
from pprint import pp  # built-in pretty print formatting for convenience
from copy import deepcopy
from irving import irving
import time

"""
This is our "main" function that gets called. It'll find all the worst matches
for this round, truncate preferences based on that, run our static matcher
on the preferences, and then move to the next round. Rounds should always be
a positive natural number, and when it's 1 this is our last round.
"""


def matcher(
    prefs: dict[Player, list[Player]], rounds: int
) -> dict[Player, Optional[Player]]:
    assert rounds >= 1
    round_counter = 1
    res: dict[Player, Optional[Player]] = {}
    # when we've truly matched someone, we remove them from prefs
    # (their entry and everyone's preference lists)

    while round_counter < rounds:
        # worst has all players, not just this round
        worst = worst_matches(prefs, rounds, round_counter)

        # truncated has only players for this round
        truncated = truncate_prefs(prefs, worst, round_counter)

        # (and then so does round_matching)
        round_matching = irving(truncated, False)

        for removed_player, their_match in round_matching.items():
            # if they were actually matched,
            if their_match is not None:
                # remove them from prefs and everyone's pref lists
                del prefs[removed_player]
                for other_player_prefs in prefs.values():
                    if removed_player in other_player_prefs:
                        other_player_prefs.remove(removed_player)
        res.update(round_matching)
        round_counter += 1

    final_match = irving(prefs, False)
    res.update(final_match)
    return res


def make_prefs(
    pref_ref: dict[Player, list[Player]],
    later: list[Player],
    maybe_now: list[Player],
    now_flags: list[bool],
) -> tuple[dict[Player, list[Player]], dict[Player, list[Player]]]:
    now_dict: dict[Player, list[Player]] = {}
    not_now_dict: dict[Player, list[Player]] = {
        player: deepcopy(pref_ref[player]) for player in later
    }
    i = 0
    while i < len(now_flags):
        p = maybe_now[i]
        if now_flags[i]:
            now_dict[p] = deepcopy(pref_ref[p])
        else:
            not_now_dict[p] = deepcopy(pref_ref[p])
        i += 1
    return now_dict, not_now_dict


# the nasty thing that lets us iterate over the powerset with special restrictions
def split_powerset(
    prefs: dict[Player, list[Player]], k: Player, this_round: int
) -> Generator[tuple[dict[Player, list[Player]], dict[Player, list[Player]]], None, None]:
    always_later = []
    could_be_now = []
    for player in prefs:
        if player.arrival_round > this_round or player == k:
            always_later.append(player)
        else:
            could_be_now.append(player)
    # keep list of booleans representing whether they're entering now
    flags = [False] * len(could_be_now)
    yield make_prefs(prefs, always_later, could_be_now, flags)
    while not all(flags):
        idx = 0
        while flags[idx]:
            flags[idx] = False
            idx += 1
        flags[idx] = True
        yield make_prefs(prefs, always_later, could_be_now, flags)


# small helper function to get the worse match with a given list
# (handles Nones and such properly)
def worse_match(
    p_prefs: list[Player], opt1: Optional[Player], opt2: Optional[Player]
) -> Player:
    if opt1 not in p_prefs or opt2 not in p_prefs:
        return None
    pos1 = p_prefs.index(opt1)
    pos2 = p_prefs.index(opt2)
    if pos1 > pos2:
        # worst is largest index
        return pos1
    else:
        return pos2


def worst_matches(
    prefs: dict[Player, list[Player]], rounds: int, this_round: int
) -> dict[Player, Optional[Player]]:
    if rounds == this_round:
        return irving(prefs, True)
    # else recurse for every possible set of players
    # who could choose to match in this round vs a later round
    ret: dict[Player, Optional[Player]] = {}
    for k in prefs:
        # split_powerset handles round arrival for us
        for curr_players, later_players in split_powerset(prefs, k, this_round):
            # get worst-possible matching for those who are staying
            worst_curr = irving(curr_players, True)

            # find all unmatched in this round and move them to a later round
            unmatched = {p for p, match in worst_curr.items() if match is None}
            for p in unmatched:
                del worst_curr[p]
                later_players[p] = prefs[p]

            worst_later = worst_matches(later_players, rounds, this_round + 1)

            # we assume that worst_curr and worst_later have disjoint keys,
            # so we can just stick them together
            worst_later.update(worst_curr)

            # now update our eventual return dict with the recursed-worst match
            for p, match in worst_later.items():
                ret[p] = worse_match(prefs[p], match, ret.get(p))

    return ret


# returns dict of preferences, "truncated" in two ways: with the shorter
# preference list and also only containing people relevant to this round
# note that `worst` has all players, not just this round
def truncate_prefs(
    prefs: dict[Player, list[Player]], worst: dict[Player, Optional[Player]], round: int
) -> dict[Player, list[Player]]:
    prefs = deepcopy(prefs)
    # need this intermediary list because we can't modify prefs while iterating over it
    to_be_removed = [p for p in prefs if p.arrival_round > round]
    for player in to_be_removed:
        del prefs[player]
    for p1, p1_prefs in prefs.items():
        p1_worst = worst[p1]
        if p1_worst is not None:
            # assume that p1_worst is in p1's list
            del p1_prefs[p1_prefs.index(p1_worst) :]
    return prefs


if __name__ == "__main__":
    PLAYER_COUNT = 10
    ROUNDS = 3
    all_players: list[Player] = generatePlayers(PLAYER_COUNT, ROUNDS)
    player_dict: dict[Player, list[Player]] = generateData(all_players)
    pp(player_dict)
    start = time.time()
    matches = matcher(player_dict, ROUNDS)
    end = time.time()
    pp(matches)
    print(f"Execution time: {end - start:.4f} seconds")
