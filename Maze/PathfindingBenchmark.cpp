#include "pch.h"
#include "PathfindingBenchmark.h"
#include "Board.h"
#include "Player.h"
#include "RandomUtils.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>

namespace
{
	constexpr int32 TRIAL_COUNT = 300;
	constexpr unsigned int BASE_SEED = 20260907;
	constexpr std::array<int32, 3> BOARD_SIZES = { 25, 51, 99 };

	struct AlgorithmSummary
	{
		const char* name;
		int64 totalPathCost = 0;
		int64 totalMoveCount = 0;
		int64 totalExpandedNodes = 0;
		int64 totalEnqueuedNodes = 0;
		int64 totalMaxOpenSize = 0;
		int64 totalElapsedNanoseconds = 0;
	};

	void AddResult(AlgorithmSummary& summary, const PathfindingResult& result)
	{
		summary.totalPathCost += result.pathCost;
		summary.totalMoveCount += result.moveCount;
		summary.totalExpandedNodes += result.expandedNodes;
		summary.totalEnqueuedNodes += result.enqueuedNodes;
		summary.totalMaxOpenSize += result.maxOpenSize;
		summary.totalElapsedNanoseconds += result.elapsedNanoseconds;
	}

	void WriteResult(
		std::ofstream& output,
		int32 boardSize,
		int32 trial,
		unsigned int seed,
		const char* algorithm,
		const PathfindingResult& result)
	{
		output << boardSize << ','
			<< trial << ','
			<< seed << ','
			<< algorithm << ','
			<< result.pathCost << ','
			<< result.moveCount << ','
			<< result.expandedNodes << ','
			<< result.enqueuedNodes << ','
			<< result.maxOpenSize << ','
			<< result.elapsedNanoseconds << '\n';
	}

	void PrintSummary(const AlgorithmSummary& summary)
	{
		const double trialCount = static_cast<double>(TRIAL_COUNT);

		cout << std::left << std::setw(16) << summary.name
			<< std::right << std::fixed << std::setprecision(1)
			<< std::setw(13) << summary.totalPathCost / trialCount
			<< std::setw(13) << summary.totalMoveCount / trialCount
			<< std::setw(13) << summary.totalExpandedNodes / trialCount
			<< std::setw(13) << summary.totalEnqueuedNodes / trialCount
			<< std::setw(13) << summary.totalMaxOpenSize / trialCount
			<< std::setw(13) << summary.totalElapsedNanoseconds / trialCount
			<< '\n';
	}
}

int RunPathfindingBenchmark(const char* outputPath)
{
	const std::filesystem::path csvPath(outputPath);
	if (csvPath.has_parent_path())
		std::filesystem::create_directories(csvPath.parent_path());

	std::ofstream output(csvPath);
	if (output.is_open() == false)
	{
		cout << "벤치마크 결과 파일을 열 수 없습니다: " << outputPath << '\n';
		return 1;
	}

	output << "board_size,trial,seed,algorithm,path_cost,move_count,expanded_nodes,"
		"enqueued_nodes,max_open_size,elapsed_ns\n";

	int32 validationFailureCount = 0;

	for (const int32 boardSize : BOARD_SIZES)
	{
		std::array<AlgorithmSummary, 4> summaries =
		{
			AlgorithmSummary{ "BFS" },
			AlgorithmSummary{ "Dijkstra" },
			AlgorithmSummary{ "AStarClosed" },
			AlgorithmSummary{ "AStarNoClosed" }
		};

		for (int32 trial = 0; trial < TRIAL_COUNT; ++trial)
		{
			const unsigned int seed = BASE_SEED + trial;
			Random::SetSeed(seed);

			Board board;
			Player player;
			board.Init(boardSize, &player);
			player.Init(&board);

			const std::array<PathfindingResult, 4> results =
			{
				player.GetBfsResult(),
				player.GetDijkstraResult(),
				player.GetClosedAStarResult(),
				player.GetNoClosedAStarResult()
			};

			if (results[1].found == false
				|| results[2].found == false
				|| results[3].found == false
				|| results[1].pathCost != results[2].pathCost
				|| results[1].pathCost != results[3].pathCost)
			{
				++validationFailureCount;
			}

			for (int32 algorithm = 0; algorithm < static_cast<int32>(results.size()); ++algorithm)
			{
				AddResult(summaries[algorithm], results[algorithm]);
				WriteResult(
					output,
					boardSize,
					trial,
					seed,
					summaries[algorithm].name,
					results[algorithm]);
			}
		}

		cout << "\nBoard " << boardSize << " x " << boardSize
			<< " / " << TRIAL_COUNT << " trials\n";
		cout << std::left << std::setw(16) << "Algorithm"
			<< std::right << std::setw(13) << "Avg Cost"
			<< std::setw(13) << "Avg Moves"
			<< std::setw(13) << "Avg Expanded"
			<< std::setw(13) << "Avg Enqueued"
			<< std::setw(13) << "Avg MaxOpen"
			<< std::setw(13) << "Avg Time(ns)"
			<< '\n';

		for (const AlgorithmSummary& summary : summaries)
			PrintSummary(summary);
	}

	cout << "\nA* shortest cost validation: "
		<< (validationFailureCount == 0 ? "PASS" : "FAIL")
		<< " (failures: " << validationFailureCount << ")\n";
	cout << "CSV: " << csvPath.string() << '\n';

	return validationFailureCount == 0 ? 0 : 2;
}
