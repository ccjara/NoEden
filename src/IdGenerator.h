#ifndef __ID_GENERATOR_H__
#define __ID_GENERATOR_H__

class IdGenerator
{
private:
    Id nextId;
public:
    IdGenerator();
    Id generate();
};

#endif
