#!/usr/bin/env python3
"""Drive the manual Truco frontend through a complete seeded match."""

import argparse
from collections import deque
import queue
import re
import subprocess
import sys
import threading

RANK_STRENGTH = {
    rank: index
    for index, rank in enumerate(("4", "5", "6", "7", "Q", "J", "K", "A", "2", "3"))
}
SUIT_STRENGTH = {"diamonds": 0, "spades": 1, "hearts": 2, "clubs": 3}


def weakest_card_index(line, manilha):
    cards = re.findall(r"\[(\d+)\] ([^/ ]+)/([^ ]+)", line)
    if not cards:
        raise RuntimeError(f"could not parse the current player's cards: {line}")

    def strength(card):
        index, rank, suit = card
        if rank == manilha:
            return (1, SUIT_STRENGTH[suit], int(index))
        return (0, RANK_STRENGTH[rank], int(index))

    return min(cards, key=strength)[0]


def stream_lines(stream, output):
    try:
        for line in stream:
            output.put(line.rstrip("\n"))
    finally:
        output.put(None)


def run_case(executable, eleven_choice):
    process = subprocess.Popen(
        [executable],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1,
    )
    lines = queue.Queue()
    reader = threading.Thread(
        target=stream_lines, args=(process.stdout, lines), daemon=True
    )
    reader.start()

    transcript = deque(maxlen=300)
    card_turns = 0
    tried_invalid_card = False
    saw_invalid_card = False
    tried_raise = False
    saw_raise_accept = False
    saw_raise_refusal = False
    saw_eleven = False
    saw_decline = False
    saw_accept = False
    saw_iron = False
    saw_inspection = False
    saw_partner_signal = False
    hidden_plays = 0
    signal_sent_this_hand = False
    manilha = None
    selected_card = None
    try:
        while True:
            try:
                line = lines.get(timeout=20)
            except queue.Empty:
                raise RuntimeError("interactive match timed out waiting for output")
            if line is None:
                break
            transcript.append(line)
            if line.startswith("Partner signal code: 42"):
                saw_partner_signal = True
            elif line.startswith("Player ") and "plays a card face down." in line:
                hidden_plays += 1
            if line.startswith("Seed ("):
                process.stdin.write("2024\n")
                process.stdin.flush()
            elif line.startswith("Iron-hand point award"):
                saw_iron = True
                process.stdin.write("1\n")
                process.stdin.flush()
            elif line.startswith("Team ") and "at 11:" in line:
                saw_eleven = True
                if not saw_inspection:
                    raise RuntimeError("hand of eleven was offered before inspection")
                if eleven_choice == "decline":
                    process.stdin.write("n\n")
                    saw_decline = True
                else:
                    process.stdin.write("y\n")
                    saw_accept = True
                process.stdin.flush()
                saw_inspection = False
            elif line.startswith("Team ") and "inspects partner" in line:
                saw_inspection = True
            elif line.startswith("Accept? (y/n):"):
                if eleven_choice == "decline":
                    process.stdin.write("n\n")
                    saw_raise_refusal = True
                else:
                    process.stdin.write("y\n")
                    saw_raise_accept = True
                process.stdin.flush()
            elif re.match(r"Hand \d+ ", line):
                signal_sent_this_hand = False
                match = re.search(r"manilha ([^,]+),", line)
                if match is None:
                    raise RuntimeError(f"could not parse hand header: {line}")
                manilha = match.group(1)
            elif line.startswith("Player ") and "cards:" in line:
                selected_card = weakest_card_index(line, manilha)
            elif line.startswith("Action [card"):
                if selected_card is None:
                    raise RuntimeError("action prompt had no parsed card choice")
                if not signal_sent_this_hand:
                    process.stdin.write("signal 42\n")
                    signal_sent_this_hand = True
                elif not tried_invalid_card:
                    process.stdin.write("card 99\n")
                    tried_invalid_card = True
                elif not tried_raise:
                    process.stdin.write("raise\n")
                    tried_raise = True
                elif card_turns >= 4 and hidden_plays == 0:
                    process.stdin.write(f"card {selected_card} hidden\n")
                    card_turns += 1
                else:
                    process.stdin.write(f"card {selected_card}\n")
                    card_turns += 1
                process.stdin.flush()
            elif line == "Invalid card choice; try again.":
                saw_invalid_card = True
            if line.startswith("Match winner "):
                break

        process.stdin.close()
        exit_code = process.wait(timeout=5)
        stderr = process.stderr.read()
        if exit_code != 0:
            raise RuntimeError(f"interactive match exited {exit_code}: {stderr}")
        final_line = next(
            (line for line in reversed(transcript)
             if line.startswith("Match winner ")),
            None,
        )
        match = re.fullmatch(
            r"Match winner ([01]); final score (\d+)-(\d+); hands (\d+)\.",
            final_line or "",
        )
        if match is None:
            raise RuntimeError("match did not print a valid final result")
        winner, score_zero, score_one, hands = map(int, match.groups())
        if max(score_zero, score_one) != 12 or winner != (
            0 if score_zero == 12 else 1
        ):
            raise RuntimeError(f"invalid terminal score: {final_line}")
        if card_turns == 0 or hands == 0:
            raise RuntimeError("the scripted match did not play any cards")
        if not saw_eleven:
            raise RuntimeError("script did not exercise hand of eleven")
        if eleven_choice == "decline" and not saw_decline:
            raise RuntimeError("script did not exercise declining at eleven")
        if eleven_choice == "accept" and not saw_accept:
            raise RuntimeError("script did not exercise accepting at eleven")
        if eleven_choice == "decline" and not saw_iron:
            raise RuntimeError("decline case did not reach an iron hand")
        if not tried_invalid_card or not saw_invalid_card:
            raise RuntimeError("invalid card selection was not rejected")
        if not tried_raise:
            raise RuntimeError("script did not request a raise")
        if not saw_partner_signal:
            raise RuntimeError("script did not deliver a signal to a partner")
        if hidden_plays == 0:
            raise RuntimeError("script did not play any card face down")
        if eleven_choice == "accept" and not saw_raise_accept:
            raise RuntimeError("script did not exercise an accepted raise")
        if eleven_choice == "decline" and not saw_raise_refusal:
            raise RuntimeError("script did not exercise a refused raise")
        print(
            f"PASS interactive {eleven_choice} case: {final_line}; "
            f"card selections={card_turns}; iron={saw_iron}"
        )
        return 0
    except (BrokenPipeError, OSError, RuntimeError, subprocess.TimeoutExpired) as error:
        process.kill()
        process.wait()
        print(f"FAIL interactive match: {error}", file=sys.stderr)
        print("\n".join(transcript), file=sys.stderr)
        return 1


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--executable", required=True)
    args = parser.parse_args()
    for choice in ("accept", "decline"):
        if run_case(args.executable, choice) != 0:
            return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
