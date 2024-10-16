#pragma once

/**
 * \brief Uniquely identifies an AI condition
 */
enum class ConditionType {
  Invalid = 0,
  AlwaysFalse = 1,
  AlwaysTrue = 2,
  IsAlive = 3,
};

/**
* \brief Parses a condition type from a string.
*
* \return A condition type != `Invalid` on success, `Invalid` otherwise.
*/
[[nodiscard]] ConditionType parse_condition_type(std::string_view value);
