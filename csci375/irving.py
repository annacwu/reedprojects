from data import Player
from typing import Optional
from copy import deepcopy


##### HELPER FUNCTIONS


def still_first_round(
    desires: dict[Player, tuple[Optional[Player], Optional[Player]]],
) -> bool:
    for player, (wants, _) in desires.items():
        if wants is None:
            return player
    return None


def will_take(
    wanter: Player, wanteds_prefs: list[Player], wanteds_curr: Player
):
    if wanter not in wanteds_prefs:
        return False
    if wanteds_curr is None:
        return True
    curr_idx = wanteds_prefs.index(wanteds_curr)
    wanter_idx = wanteds_prefs.index(wanter)
    return wanter_idx < curr_idx


# WARN modifies prefs
# WARN assumes both in each other's lists
def both_reject(prefs: dict[Player, list[Player]], p1: Player, p2: Player):
    prefs[p1].remove(p2)
    prefs[p2].remove(p1)


##### MAIN CODE


# runs the first round of Irving's algorithm and returns
# the shorter preference lists after removing invalid matches
# WARN modifies prefs
def round1(prefs: dict[Player, list[Player]]):
    # first is who they want, second is who wants them
    desires: dict[Player, tuple[Optional[Player], Optional[Player]]] = {
        p: (None, None) for p in prefs
    }
    while (needs_match := still_first_round(desires)) is not None:
        if prefs[needs_match] == []:
            desires[needs_match] = needs_match, needs_match
            continue
        next_best_choice = prefs[needs_match][0]
        if will_take(
            needs_match,
            prefs[next_best_choice],
            desires[next_best_choice][1],
        ):
            old_match = desires[next_best_choice][1]
            if old_match is not None:
                both_reject(prefs, next_best_choice, old_match)
                desires[old_match] = (None, desires[old_match][1])
            desires[needs_match] = (next_best_choice, desires[needs_match][1])
            desires[next_best_choice] = (desires[next_best_choice][0], needs_match)
        else:
            both_reject(prefs, needs_match, next_best_choice)
    # now cut down the preference lists
    for player in prefs:
        prop_to_them = desires[player][1]
        if prop_to_them == player:
            del prefs[player][:]
            continue
        last_wanted = prefs[player].index(prop_to_them)
        for other in prefs[player][last_wanted + 1 :]:
            # safe to "modify list we're iterating over"
            # because taking a slice (in the for statement) is copying
            both_reject(prefs, other, player)


def irving_full(
    prefs: dict[Player, list[Player]], worst: bool
) -> dict[Player, list[Player]]:
    round1(prefs)  # modifies via chaotic proposals

    if all(map(lambda x: len(x) < 2, prefs.values())):
        return prefs
    # else find a cycle, remove prefs, and recurse
    # add `worst` logic some other time, for now we just do something
    #
    # ASSUME this always works (algo says it should)
    curr_a = None
    for p in prefs:
        if len(prefs[p]) > 1:
            curr_a = p
            break
    if curr_a is None:
        assert False, "Something went wrong :("

    # make a- and b-lists, append until we cycle
    a_lst = []
    b_lst = []
    while curr_a not in a_lst:
        a_lst.append(curr_a)
        prev_b = prefs[curr_a][1]
        b_lst.append(prev_b)
        curr_a = prefs[prev_b][-1]

    # remove the "leading tail" so we only have the cycle part
    cycle_head_idx = a_lst.index(curr_a)
    del a_lst[:cycle_head_idx]
    del b_lst[:cycle_head_idx]

    # now all b_i reject a_(i+1) (modding appropriately)
    for i, b in enumerate(b_lst[:-1]):
        both_reject(prefs, b, a_lst[i + 1])
    both_reject(prefs, b_lst[-1], a_lst[0])

    # then we recurse!
    return irving_full(prefs, worst)


# documentation is for nerds
def irving(
    prefs: dict[Player, list[Player]], worst: bool
) -> dict[Player, Optional[Player]]:
    prefs = deepcopy(prefs)

    # remove invalid pairs due to incomplete prefs
    # and some people not being around for this round
    for wanter in prefs:
        for wanted in prefs[wanter][:]:
            if wanted not in prefs or wanter not in prefs[wanted]:
                prefs[wanter].remove(wanted)

    full_match = irving_full(prefs, worst)
    return {
        player: (maybe_other[0] if maybe_other != [] else None)
        for player, maybe_other in full_match.items()
    }


if __name__ == "__main__":
    # exam problem
    a = Player("A", "horse", "yes", 1)
    b = Player("B", "horse", "yes", 1)
    c = Player("C", "horse", "yes", 1)
    d = Player("D", "horse", "yes", 1)
    e = Player("E", "horse", "yes", 1)
    s = Player("S", "horse", "yes", 1)
    p = dict()
    p[a] = [c, d, b, s, e]
    p[b] = [s, e, d, a, c]
    p[c] = [b, d, e, a, s]
    p[d] = [e, b, c, s, a]
    p[e] = [c, a, b, d, s]
    p[s] = [e, a, c, d, b]
    # round1(p)
    # breakpoint()
