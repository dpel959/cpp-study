from __future__ import annotations

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import FancyArrowPatch, Rectangle


ROOT = Path(__file__).resolve().parent
ASSET_DIR = ROOT / "assets"
RESULT_PATH = ROOT / "benchmark-results.csv"
REMOVAL_RESULT_PATH = ROOT / "removal-results.csv"
LOOKUP_RESULT_PATH = ROOT / "lookup-results.csv"


def read_results() -> list[dict[str, float]]:
    with RESULT_PATH.open(encoding="utf-8", newline="") as csv_file:
        rows = []
        for row in csv.DictReader(csv_file):
            rows.append(
                {
                    "capacity": float(row["capacity"]),
                    "occupancy": float(row["occupancy_percent"]),
                    "linear": float(row["linear_scan_ns"]),
                    "tracked": float(row["tracked_slot_ns"]),
                    "ratio": float(row["ratio"]),
                }
            )
        return rows


def read_removal_results() -> list[dict[str, float]]:
    with REMOVAL_RESULT_PATH.open(encoding="utf-8", newline="") as csv_file:
        rows = []
        for row in csv.DictReader(csv_file):
            rows.append(
                {
                    "capacity": float(row["capacity"]),
                    "occupancy": float(row["occupancy_percent"]),
                    "erase": float(row["find_erase_ns"]),
                    "swap": float(row["swap_pop_ns"]),
                    "ratio": float(row["ratio"]),
                }
            )
        return rows


def read_lookup_results() -> list[dict[str, float]]:
    with LOOKUP_RESULT_PATH.open(encoding="utf-8", newline="") as csv_file:
        rows = []
        for row in csv.DictReader(csv_file):
            rows.append(
                {
                    "capacity": float(row["capacity"]),
                    "occupancy": float(row["occupancy_percent"]),
                    "pointer": float(row["pointer_scan_ns"]),
                    "handle": float(row["handle_lookup_ns"]),
                    "ratio": float(row["ratio"]),
                }
            )
        return rows


def format_nanoseconds(value: float) -> str:
    if value >= 1000.0:
        return f"{value / 1000.0:.2f} us"
    return f"{value:.0f} ns"


def generate_benchmark_chart(rows: list[dict[str, float]]) -> None:
    labels = [
        f"{int(row['capacity']):,}\n{int(row['occupancy'])}%" for row in rows
    ]
    linear_values = [row["linear"] for row in rows]
    tracked_values = [row["tracked"] for row in rows]
    ratios = [row["ratio"] for row in rows]

    figure, (latency_axis, ratio_axis) = plt.subplots(
        2,
        1,
        figsize=(12, 8),
        gridspec_kw={"height_ratios": [2.2, 1.4]},
    )
    figure.subplots_adjust(left=0.08, right=0.98, top=0.88, bottom=0.12, hspace=0.55)
    figure.patch.set_facecolor("#f7f8fa")
    figure.suptitle(
        "Inventory Add Performance: Linear Scan vs Free-slot Stack",
        fontsize=18,
        fontweight="bold",
    )
    figure.text(
        0.5,
        0.945,
        "MSVC 19.44 | C++17 | Release x64 /O2 | median of 5 runs",
        ha="center",
        fontsize=10,
        color="#4b5563",
    )

    positions = np.arange(len(rows))
    bar_width = 0.36
    linear_color = "#64748b"
    tracked_color = "#2563eb"

    latency_axis.set_facecolor("#ffffff")
    linear_bars = latency_axis.bar(
        positions - bar_width / 2,
        linear_values,
        bar_width,
        label="Linear scan",
        color=linear_color,
    )
    tracked_bars = latency_axis.bar(
        positions + bar_width / 2,
        tracked_values,
        bar_width,
        label="Free-slot stack + occupied vector",
        color=tracked_color,
    )
    latency_axis.set_yscale("log")
    latency_axis.set_ylabel("Add latency (ns/op, log scale)")
    latency_axis.set_xticks(positions, labels)
    latency_axis.set_xlabel("Capacity and occupancy")
    latency_axis.grid(axis="y", which="both", alpha=0.18)
    latency_axis.legend(frameon=False, ncol=2, loc="upper left")
    latency_axis.spines[["top", "right"]].set_visible(False)

    for bars, values in (
        (linear_bars, linear_values),
        (tracked_bars, tracked_values),
    ):
        for bar, value in zip(bars, values):
            latency_axis.text(
                bar.get_x() + bar.get_width() / 2,
                value * 1.13,
                format_nanoseconds(value),
                ha="center",
                va="bottom",
                fontsize=8,
                color="#111827",
            )

    ratio_colors = ["#dc2626" if ratio < 1.0 else tracked_color for ratio in ratios]
    ratio_axis.set_facecolor("#ffffff")
    ratio_bars = ratio_axis.bar(positions, ratios, 0.58, color=ratio_colors)
    ratio_axis.axhline(1.0, color="#111827", linewidth=1.0)
    ratio_axis.set_ylabel("Speed ratio (x)")
    ratio_axis.set_xticks(positions, labels)
    ratio_axis.grid(axis="y", alpha=0.18)
    ratio_axis.spines[["top", "right"]].set_visible(False)

    for bar, ratio in zip(ratio_bars, ratios):
        ratio_axis.text(
            bar.get_x() + bar.get_width() / 2,
            bar.get_height() + max(ratios) * 0.018,
            f"{ratio:.2f}x",
            ha="center",
            va="bottom",
            fontsize=9,
            fontweight="bold",
            color="#111827",
        )

    figure.text(
        0.5,
        0.025,
        "Independent algorithm models | Ratio above 1.0x means tracked slots are faster | Absolute values vary by hardware",
        ha="center",
        fontsize=9,
        color="#4b5563",
    )

    figure.savefig(
        ASSET_DIR / "benchmark-comparison.png",
        dpi=180,
        facecolor=figure.get_facecolor(),
    )
    plt.close(figure)


def draw_slot_row(axis: plt.Axes, y: float, occupied_count: int) -> None:
    slot_width = 0.075
    start_x = 0.08

    for index in range(10):
        occupied = index < occupied_count
        rectangle = Rectangle(
            (start_x + index * slot_width, y),
            slot_width - 0.006,
            0.11,
            facecolor="#bfdbfe" if occupied else "#f1f5f9",
            edgecolor="#475569",
            linewidth=1.0,
        )
        axis.add_patch(rectangle)
        axis.text(
            start_x + index * slot_width + (slot_width - 0.006) / 2,
            y + 0.055,
            str(index),
            ha="center",
            va="center",
            fontsize=8,
            color="#0f172a",
        )


def generate_design_diagram() -> None:
    figure, axes = plt.subplots(1, 2, figsize=(12, 5), constrained_layout=True)
    figure.patch.set_facecolor("#f7f8fa")
    figure.suptitle("How the Empty Slot Is Found", fontsize=18, fontweight="bold")

    linear_axis, tracked_axis = axes
    for axis in axes:
        axis.set_xlim(0, 1)
        axis.set_ylim(0, 1)
        axis.axis("off")
        axis.set_facecolor("#ffffff")

    linear_axis.set_title("Linear scan baseline", fontsize=14, fontweight="bold")
    linear_axis.text(
        0.5,
        0.82,
        "Check slots from the beginning until an empty slot is found",
        ha="center",
        fontsize=10,
        color="#475569",
    )
    draw_slot_row(linear_axis, 0.48, 8)

    for index in range(8):
        start_x = 0.115 + index * 0.075
        end_x = start_x + 0.052
        linear_axis.add_patch(
            FancyArrowPatch(
                (start_x, 0.43),
                (end_x, 0.43),
                arrowstyle="->",
                mutation_scale=8,
                color="#dc2626",
                linewidth=1.0,
            )
        )

    linear_axis.text(
        0.5,
        0.25,
        "Worst-case lookup: O(N)",
        ha="center",
        fontsize=13,
        fontweight="bold",
        color="#991b1b",
    )

    tracked_axis.set_title("Portfolio implementation", fontsize=14, fontweight="bold")
    tracked_axis.text(
        0.5,
        0.82,
        "Pop an index from the free-slot stack",
        ha="center",
        fontsize=10,
        color="#475569",
    )
    draw_slot_row(tracked_axis, 0.48, 8)

    stack_box = Rectangle(
        (0.12, 0.16),
        0.23,
        0.2,
        facecolor="#dbeafe",
        edgecolor="#2563eb",
        linewidth=1.4,
    )
    tracked_axis.add_patch(stack_box)
    tracked_axis.text(0.235, 0.315, "Free-slot stack", ha="center", fontsize=10)
    tracked_axis.text(0.235, 0.245, "top -> 9", ha="center", fontsize=11, fontweight="bold")
    tracked_axis.text(0.235, 0.19, "next -> 8", ha="center", fontsize=9)
    tracked_axis.add_patch(
        FancyArrowPatch(
            (0.35, 0.27),
            (0.79, 0.47),
            arrowstyle="->",
            mutation_scale=14,
            color="#2563eb",
            linewidth=2.0,
        )
    )
    tracked_axis.text(
        0.68,
        0.25,
        "Lookup: O(1)",
        ha="center",
        fontsize=13,
        fontweight="bold",
        color="#1d4ed8",
    )

    figure.text(
        0.5,
        0.025,
        "The tracked design uses extra memory; whether it is faster depends on inventory scale.",
        ha="center",
        fontsize=10,
        color="#4b5563",
    )
    figure.savefig(
        ASSET_DIR / "design-comparison.png",
        dpi=180,
        facecolor=figure.get_facecolor(),
    )
    plt.close(figure)


def generate_removal_chart(rows: list[dict[str, float]]) -> None:
    labels = [
        f"{int(row['capacity']):,}\n{int(row['occupancy'])}%" for row in rows
    ]
    erase_values = [row["erase"] for row in rows]
    swap_values = [row["swap"] for row in rows]
    ratios = [row["ratio"] for row in rows]
    positions = np.arange(len(rows))
    bar_width = 0.36

    figure, (latency_axis, ratio_axis) = plt.subplots(
        2,
        1,
        figsize=(12, 8),
        gridspec_kw={"height_ratios": [2.2, 1.4]},
    )
    figure.subplots_adjust(left=0.08, right=0.98, top=0.88, bottom=0.12, hspace=0.55)
    figure.patch.set_facecolor("#f7f8fa")
    figure.suptitle(
        "Occupied-slot Removal: Find + Erase vs Swap-and-pop",
        fontsize=18,
        fontweight="bold",
    )
    figure.text(
        0.5,
        0.945,
        "Middle occupied slot | MSVC 19.44 | C++17 | Release x64 /O2 | median of 5 runs",
        ha="center",
        fontsize=10,
        color="#4b5563",
    )

    erase_color = "#64748b"
    swap_color = "#7c3aed"
    latency_axis.set_facecolor("#ffffff")
    erase_bars = latency_axis.bar(
        positions - bar_width / 2,
        erase_values,
        bar_width,
        label="std::find + vector::erase",
        color=erase_color,
    )
    swap_bars = latency_axis.bar(
        positions + bar_width / 2,
        swap_values,
        bar_width,
        label="Reverse lookup + swap-and-pop",
        color=swap_color,
    )
    latency_axis.set_yscale("log")
    latency_axis.set_ylabel("Removal latency (ns/op, log scale)")
    latency_axis.set_xticks(positions, labels)
    latency_axis.set_xlabel("Capacity and occupancy")
    latency_axis.grid(axis="y", which="both", alpha=0.18)
    latency_axis.legend(frameon=False, ncol=2, loc="upper left")
    latency_axis.spines[["top", "right"]].set_visible(False)

    for bars, values in ((erase_bars, erase_values), (swap_bars, swap_values)):
        for bar, value in zip(bars, values):
            latency_axis.text(
                bar.get_x() + bar.get_width() / 2,
                value * 1.14,
                format_nanoseconds(value),
                ha="center",
                va="bottom",
                fontsize=8,
                color="#111827",
            )

    ratio_axis.set_facecolor("#ffffff")
    ratio_bars = ratio_axis.bar(positions, ratios, 0.58, color=swap_color)
    ratio_axis.axhline(1.0, color="#111827", linewidth=1.0)
    ratio_axis.set_ylabel("Speed ratio (x)")
    ratio_axis.set_xticks(positions, labels)
    ratio_axis.grid(axis="y", alpha=0.18)
    ratio_axis.spines[["top", "right"]].set_visible(False)

    for bar, ratio in zip(ratio_bars, ratios):
        ratio_axis.text(
            bar.get_x() + bar.get_width() / 2,
            bar.get_height() + max(ratios) * 0.018,
            f"{ratio:.2f}x",
            ha="center",
            va="bottom",
            fontsize=9,
            fontweight="bold",
            color="#111827",
        )

    figure.text(
        0.5,
        0.025,
        "Container-maintenance cost only | Ratio above 1.0x means swap-and-pop is faster | Absolute values vary by hardware",
        ha="center",
        fontsize=9,
        color="#4b5563",
    )
    figure.savefig(
        ASSET_DIR / "removal-benchmark.png",
        dpi=180,
        facecolor=figure.get_facecolor(),
    )
    plt.close(figure)


def draw_value_row(
    axis: plt.Axes,
    values: list[int | None],
    y: float,
    highlight_index: int | None = None,
    highlight_color: str = "#fde68a",
) -> None:
    slot_width = 0.095
    start_x = 0.11

    for index, value in enumerate(values):
        color = highlight_color if index == highlight_index else "#dbeafe"
        if value is None:
            color = "#f1f5f9"
        rectangle = Rectangle(
            (start_x + index * slot_width, y),
            slot_width - 0.008,
            0.10,
            facecolor=color,
            edgecolor="#475569",
            linewidth=1.0,
        )
        axis.add_patch(rectangle)
        axis.text(
            start_x + index * slot_width + (slot_width - 0.008) / 2,
            y + 0.05,
            "" if value is None else str(value),
            ha="center",
            va="center",
            fontsize=10,
            color="#0f172a",
        )


def generate_removal_diagram() -> None:
    figure, axes = plt.subplots(1, 2, figsize=(12, 6), constrained_layout=True)
    figure.patch.set_facecolor("#f7f8fa")
    figure.suptitle("Removing Slot 3 from the Occupied-slot Vector", fontsize=18, fontweight="bold")

    erase_axis, swap_axis = axes
    for axis in axes:
        axis.set_xlim(0, 1)
        axis.set_ylim(0, 1)
        axis.axis("off")

    erase_axis.set_title("std::find + vector::erase", fontsize=14, fontweight="bold")
    erase_axis.text(0.11, 0.78, "Before", fontsize=10, color="#475569")
    draw_value_row(erase_axis, [0, 1, 2, 3, 4, 5, 6, 7], 0.66, 3, "#fecaca")
    erase_axis.text(
        0.5,
        0.51,
        "4, 5, 6, 7 shift one position to the left",
        ha="center",
        fontsize=10,
        color="#991b1b",
    )
    erase_axis.add_patch(
        FancyArrowPatch(
            (0.5, 0.62),
            (0.5, 0.44),
            arrowstyle="->",
            mutation_scale=14,
            color="#dc2626",
            linewidth=1.8,
        )
    )
    erase_axis.text(0.11, 0.41, "After", fontsize=10, color="#475569")
    draw_value_row(erase_axis, [0, 1, 2, 4, 5, 6, 7, None], 0.29)
    erase_axis.text(
        0.5,
        0.13,
        "Order preserved | O(N)",
        ha="center",
        fontsize=13,
        fontweight="bold",
        color="#991b1b",
    )

    swap_axis.set_title("Reverse lookup + swap-and-pop", fontsize=14, fontweight="bold")
    swap_axis.text(0.11, 0.78, "Before", fontsize=10, color="#475569")
    draw_value_row(swap_axis, [0, 1, 2, 3, 4, 5, 6, 7], 0.66, 3, "#fecaca")
    swap_axis.text(
        0.5,
        0.51,
        "Move the last value (7) into slot 3, then pop the back",
        ha="center",
        fontsize=10,
        color="#5b21b6",
    )
    swap_axis.add_patch(
        FancyArrowPatch(
            (0.5, 0.62),
            (0.5, 0.44),
            arrowstyle="->",
            mutation_scale=14,
            color="#7c3aed",
            linewidth=1.8,
        )
    )
    swap_axis.text(0.11, 0.41, "After", fontsize=10, color="#475569")
    draw_value_row(swap_axis, [0, 1, 2, 7, 4, 5, 6, None], 0.29, 3, "#ddd6fe")
    swap_axis.text(
        0.5,
        0.13,
        "Order changed | O(1)",
        ha="center",
        fontsize=13,
        fontweight="bold",
        color="#5b21b6",
    )

    figure.text(
        0.5,
        0.02,
        "O(1) requires a slot-to-vector-position lookup table; swap-and-pop alone does not remove the search cost.",
        ha="center",
        fontsize=10,
        color="#4b5563",
    )
    figure.savefig(
        ASSET_DIR / "removal-design-comparison.png",
        dpi=180,
        facecolor=figure.get_facecolor(),
    )
    plt.close(figure)


def generate_lookup_chart(rows: list[dict[str, float]]) -> None:
    labels = [f"{int(row['capacity']):,}\n{int(row['occupancy'])}%" for row in rows]
    pointer_values = [row["pointer"] for row in rows]
    handle_values = [row["handle"] for row in rows]
    ratios = [row["ratio"] for row in rows]
    positions = np.arange(len(rows))
    bar_width = 0.36

    figure, (latency_axis, ratio_axis) = plt.subplots(
        2, 1, figsize=(12, 8), gridspec_kw={"height_ratios": [2.2, 1.4]}
    )
    figure.subplots_adjust(left=0.08, right=0.98, top=0.88, bottom=0.12, hspace=0.55)
    figure.patch.set_facecolor("#f7f8fa")
    figure.suptitle(
        "Item Lookup: Pointer Linear Scan vs Generational Handle",
        fontsize=18,
        fontweight="bold",
    )
    figure.text(
        0.5,
        0.945,
        "Deterministic occupied-slot queries | MSVC 19.44 | C++17 | Release x64 /O2 | median of 5 runs",
        ha="center",
        fontsize=10,
        color="#4b5563",
    )

    pointer_color = "#64748b"
    handle_color = "#059669"
    latency_axis.set_facecolor("#ffffff")
    pointer_bars = latency_axis.bar(
        positions - bar_width / 2,
        pointer_values,
        bar_width,
        label="Item* linear scan",
        color=pointer_color,
    )
    handle_bars = latency_axis.bar(
        positions + bar_width / 2,
        handle_values,
        bar_width,
        label="Index + generation handle",
        color=handle_color,
    )
    latency_axis.set_yscale("log")
    latency_axis.set_ylabel("Lookup latency (ns/op, log scale)")
    latency_axis.set_xticks(positions, labels)
    latency_axis.set_xlabel("Capacity and occupancy")
    latency_axis.grid(axis="y", which="both", alpha=0.18)
    latency_axis.legend(frameon=False, ncol=2, loc="upper left")
    latency_axis.spines[["top", "right"]].set_visible(False)

    for bars, values in ((pointer_bars, pointer_values), (handle_bars, handle_values)):
        for bar, value in zip(bars, values):
            latency_axis.text(
                bar.get_x() + bar.get_width() / 2,
                value * 1.14,
                format_nanoseconds(value),
                ha="center",
                va="bottom",
                fontsize=8,
                color="#111827",
            )

    ratio_axis.set_facecolor("#ffffff")
    ratio_bars = ratio_axis.bar(positions, ratios, 0.58, color=handle_color)
    ratio_axis.axhline(1.0, color="#111827", linewidth=1.0)
    ratio_axis.set_ylabel("Speed ratio (x)")
    ratio_axis.set_xticks(positions, labels)
    ratio_axis.grid(axis="y", alpha=0.18)
    ratio_axis.spines[["top", "right"]].set_visible(False)

    for bar, ratio in zip(ratio_bars, ratios):
        ratio_axis.text(
            bar.get_x() + bar.get_width() / 2,
            bar.get_height() + max(ratios) * 0.018,
            f"{ratio:.2f}x",
            ha="center",
            va="bottom",
            fontsize=9,
            fontweight="bold",
            color="#111827",
        )

    figure.text(
        0.5,
        0.025,
        "Lookup cost only | Ratio above 1.0x means handles are faster | Absolute values vary by hardware",
        ha="center",
        fontsize=9,
        color="#4b5563",
    )
    figure.savefig(
        ASSET_DIR / "lookup-benchmark.png",
        dpi=180,
        facecolor=figure.get_facecolor(),
    )
    plt.close(figure)


def generate_handle_diagram() -> None:
    figure, axes = plt.subplots(1, 2, figsize=(12, 5.5), constrained_layout=True)
    figure.patch.set_facecolor("#f7f8fa")
    figure.suptitle("Finding an Item and Rejecting a Stale Reference", fontsize=18, fontweight="bold")

    scan_axis, handle_axis = axes
    for axis in axes:
        axis.set_xlim(0, 1)
        axis.set_ylim(0, 1)
        axis.axis("off")

    scan_axis.set_title("Item* linear scan", fontsize=14, fontweight="bold")
    draw_value_row(scan_axis, [10, 20, 30, 40, 50, 60, 70, 80], 0.55, 6, "#fecaca")
    scan_axis.text(
        0.5,
        0.42,
        "Compare each pointer until Item 70 is found",
        ha="center",
        fontsize=10,
        color="#991b1b",
    )
    scan_axis.text(
        0.5,
        0.23,
        "Lookup O(N)",
        ha="center",
        fontsize=14,
        fontweight="bold",
        color="#991b1b",
    )

    handle_axis.set_title("ItemHandle { slot, generation }", fontsize=14, fontweight="bold")
    handle_axis.text(
        0.25,
        0.66,
        "Handle\n{ 6, 3 }",
        ha="center",
        va="center",
        fontsize=13,
        fontweight="bold",
        bbox={"boxstyle": "square,pad=0.7", "facecolor": "#d1fae5", "edgecolor": "#059669"},
    )
    handle_axis.add_patch(
        FancyArrowPatch(
            (0.37, 0.64),
            (0.67, 0.64),
            arrowstyle="->",
            mutation_scale=14,
            color="#059669",
            linewidth=2.0,
        )
    )
    handle_axis.text(
        0.76,
        0.66,
        "slot 6\ncurrent generation 3",
        ha="center",
        va="center",
        fontsize=11,
        bbox={"boxstyle": "square,pad=0.7", "facecolor": "#ecfdf5", "edgecolor": "#059669"},
    )
    handle_axis.text(
        0.5,
        0.42,
        "Index directly, then compare the generation",
        ha="center",
        fontsize=10,
        color="#065f46",
    )
    handle_axis.text(
        0.5,
        0.23,
        "Lookup + validation O(1)",
        ha="center",
        fontsize=14,
        fontweight="bold",
        color="#065f46",
    )

    figure.text(
        0.5,
        0.03,
        "After slot 6 is reused with generation 4, the old handle {6, 3} is rejected instead of accessing the new item.",
        ha="center",
        fontsize=10,
        color="#4b5563",
    )
    figure.savefig(
        ASSET_DIR / "handle-design-comparison.png",
        dpi=180,
        facecolor=figure.get_facecolor(),
    )
    plt.close(figure)


def main() -> None:
    ASSET_DIR.mkdir(parents=True, exist_ok=True)
    rows = read_results()
    removal_rows = read_removal_results()
    lookup_rows = read_lookup_results()
    generate_benchmark_chart(rows)
    generate_design_diagram()
    generate_removal_chart(removal_rows)
    generate_removal_diagram()
    generate_lookup_chart(lookup_rows)
    generate_handle_diagram()


if __name__ == "__main__":
    main()
