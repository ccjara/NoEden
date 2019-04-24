#include "stdafx.h"

class EntityAbstract;

template<>
Id StaticId<EntityAbstract>::nextId = 0;
