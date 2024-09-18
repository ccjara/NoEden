#ifndef NOEDEN_CONDITION_TYPE_HXX
#define NOEDEN_CONDITION_TYPE_HXX

/**
 * @brief Uniquely identifies a condition
 */
enum class ConditionType {
  AlwaysFalse = 0,
  AlwaysTrue = 1,
  IsAlive = 2,

  MaxValue = 3,
};

#endif
