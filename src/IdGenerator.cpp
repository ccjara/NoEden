#include "stdafx.h"

IdGenerator::IdGenerator() :
    nextId { 0 } {
}

Id IdGenerator::generate() {
    return nextId++;
}
