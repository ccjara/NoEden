#pragma once

enum class TopologicalSortStatus {
    /**
     * @brief The sort operation was successful. Check `sorted_entities` for the sorted entities
     */
    Ok,

    /**
     * @brief A dependency cycle was detected. Check `cycle_entities` for the entities involved in the cycle
     */
    CycleDetected
};

struct TopologicalSortResult {
    /**
     * @brief Status of the sort operation result
     */
    TopologicalSortStatus status = TopologicalSortStatus::Ok;

    /**
     * @brief Entities in topological order
     */
    std::vector<std::string> sorted_entities = {};

    /**
     * @brief Entities belonging to a dependency cycle. Only set when status is CycleDetected
     */
    std::vector<std::string> cycle_entities = {};

    explicit operator bool() const {
        return status == TopologicalSortStatus::Ok;
    }

    static TopologicalSortResult from_sorted_entities(std::vector<std::string>&& sorted_entities) {
        return {TopologicalSortStatus::Ok, std::move(sorted_entities)};
    }

    static TopologicalSortResult from_cycle(std::vector<std::string>&& cycle_entities) {
        return {TopologicalSortStatus::CycleDetected, {}, std::move(cycle_entities)};
    }
};

struct TopologicalSortContext {
    std::unordered_set<std::string> visited = {};
    std::unordered_set<std::string> recursion_stack = {};
    std::vector<std::string> sorted_entities = {};
    std::vector<std::string> cycle_entities = {};

    TopologicalSortContext() = default;
};

static bool topological_sort_internal(
    const std::string& entity_id,
    const std::unordered_map<std::string, std::vector<std::string>>& dependency_graph,
    TopologicalSortContext& context
);

[[nodiscard]] inline TopologicalSortResult topological_sort(
    const std::unordered_map<std::string, std::vector<std::string>>& dependency_graph
) {
    TopologicalSortContext context;

    for (const auto& [entity_id, _] : dependency_graph) {
        if (context.visited.contains(entity_id)) {
            continue;
        }
        if (!topological_sort_internal(entity_id, dependency_graph, context)) {
            return TopologicalSortResult::from_cycle(std::move(context.cycle_entities));
        }
    }

    std::ranges::reverse(context.sorted_entities);

    return TopologicalSortResult::from_sorted_entities(std::move(context.sorted_entities));
}

static bool topological_sort_internal(
    const std::string& entity_id,
    const std::unordered_map<std::string, std::vector<std::string>>& dependency_graph,
    TopologicalSortContext& context
) {
    if (context.recursion_stack.contains(entity_id)) {
        context.cycle_entities.push_back(entity_id);
        return false;
    }
    if (context.visited.contains(entity_id)) {
        return true;
    }

    context.recursion_stack.insert(entity_id);
    context.visited.insert(entity_id);

    if (dependency_graph.contains(entity_id)) {
        for (const auto& parent_id : dependency_graph.at(entity_id)) {
            if (!topological_sort_internal(parent_id, dependency_graph, context)) {
                context.cycle_entities.push_back(entity_id);
                return false;
            }
        }
    }

    context.sorted_entities.push_back(entity_id);
    context.recursion_stack.erase(entity_id);

    return true;
}
