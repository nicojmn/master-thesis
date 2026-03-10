#!/usr/bin/env python3

import subprocess
import re
import csv
import os
import argparse

from datetime import datetime
from typing import Tuple

parser = argparse.ArgumentParser(
    description="Run transports (DAS / TCP / RDMA) benchmark"
)
parser.add_argument("program", help="Path to program to benchmark", metavar="PROGRAM")
parser.add_argument(
    "--output", default="./out/", help="Directory to put output csv files"
)
parser.add_argument(
    "--runs", type=int, default=10, help="Number of runs for each transport"
)
parser.add_argument(
    "--configs",
    nargs="+",
    required=True,
    help="JSON configs for DAS and / or (TCP and RDMA)",
)

parser.add_argument(
    "program_args",
    nargs=argparse.REMAINDER,
    help="Additional arguments to pass to the program being benchmarked",
    metavar="ARGS",
)

args = parser.parse_args()

config = {}

if len(args.configs) == 1:
    config["das"] = args.configs[0]
elif len(args.configs) == 2:
    config["tcp"] = args.configs[0]
    config["rdma"] = args.configs[1]
elif len(args.configs) == 3:
    config["das"] = args.configs[0]
    config["tcp"] = args.configs[1]
    config["rdma"] = args.configs[2]
else:
    raise ValueError("Invalid number of configs provided")

if args.output:
    os.makedirs(args.output, exist_ok=True)


def parse_output(string: str) -> Tuple[str, str]:
    w = re.search(r"Write I/O time : (\d+) us", string)
    r = re.search(r"Read I/O time : (\d+) us", string)

    w_time = w.group(1) if w else "ERR"
    r_time = r.group(1) if r else "ERR"

    return w_time, r_time


def run_benchmark(transport: str, n=args.runs) -> None:
    conf = config[transport]
    cmd = ["sudo", args.program] + args.program_args + ["-c", conf]

    filename = os.path.join(
        args.output,
        f"{transport}-{n}-{datetime.now().strftime('%Y-%m-%d_%H-%M-%S')}.csv",
    )

    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Read", "Write"])
        for _ in range(n):
            res = subprocess.run(cmd, capture_output=True, text=True)
            print(res.stderr)
            w, r = parse_output(res.stderr)
            writer.writerow([w, r])

        print(f"Wrote benchmark to {filename}")


for transport in config.keys():
    run_benchmark(transport)
