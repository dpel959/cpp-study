import csv
from collections import defaultdict
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


CURRENT_DIR = Path(__file__).resolve().parent
CSV_PATH = CURRENT_DIR / "pathfinding_results.csv"
OUTPUT_PATH = CURRENT_DIR / "pathfinding_benchmark.png"

ALGORITHMS = ["BFS", "Dijkstra", "AStarClosed", "AStarNoClosed"]
LABELS = {
    "BFS": "BFS",
    "Dijkstra": "Dijkstra",
    "AStarClosed": "A* (Closed)",
    "AStarNoClosed": "A* (No Closed)",
}
COLORS = {
    "BFS": "#7aa2f7",
    "Dijkstra": "#bb9af7",
    "AStarClosed": "#9ece6a",
    "AStarNoClosed": "#2ac3de",
}


def load_means():
    values = defaultdict(lambda: defaultdict(list))

    with CSV_PATH.open(encoding="utf-8", newline="") as csv_file:
        for row in csv.DictReader(csv_file):
            key = (int(row["board_size"]), row["algorithm"])
            values[key]["expanded_nodes"].append(int(row["expanded_nodes"]))
            values[key]["elapsed_ns"].append(int(row["elapsed_ns"]))

    return {
        key: {metric: sum(samples) / len(samples) for metric, samples in metrics.items()}
        for key, metrics in values.items()
    }


def main():
    means = load_means()
    board_sizes = sorted({board_size for board_size, _ in means})

    plt.style.use("dark_background")
    figure, axes = plt.subplots(1, 2, figsize=(16, 7.5), dpi=120)
    figure.patch.set_facecolor("#111827")

    chart_specs = [
        ("expanded_nodes", "Average expanded nodes", "Nodes"),
        ("elapsed_ns", "Average execution time", "Nanoseconds"),
    ]

    for axis, (metric, title, y_label) in zip(axes, chart_specs):
        axis.set_facecolor("#111827")
        for algorithm in ALGORITHMS:
            samples = [means[(size, algorithm)][metric] for size in board_sizes]
            axis.plot(
                board_sizes,
                samples,
                marker="o",
                linewidth=2.5,
                markersize=7,
                label=LABELS[algorithm],
                color=COLORS[algorithm],
            )

        axis.set_title(title, fontsize=16, pad=14, fontweight="bold")
        axis.set_xlabel("Board size", fontsize=11)
        axis.set_ylabel(y_label, fontsize=11)
        axis.set_xticks(board_sizes)
        axis.grid(alpha=0.18, linestyle="--")
        axis.legend(frameon=False, fontsize=10)

    figure.suptitle(
        "8-direction pathfinding benchmark (300 fixed-seed mazes per size)",
        fontsize=19,
        fontweight="bold",
        y=0.98,
    )
    figure.tight_layout(rect=(0, 0, 1, 0.94))
    figure.savefig(OUTPUT_PATH, bbox_inches="tight", facecolor=figure.get_facecolor())
    print(OUTPUT_PATH)


if __name__ == "__main__":
    main()
