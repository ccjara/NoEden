#ifndef __STATIC_ID_H__
#define __STATIC_ID_H__

// used to auto generate static, class specific Ids during compile time
// differentiates between parent classes so derivatives
// of other classes each start with zero

// Note: You need to initialize the nextId for each PARENT in the implementation file

template <class PARENT>
class StaticId
{
private:
    static Id nextId;
public:
    template <class CHILD> 
    static const Id assign() {
        return nextId++;
    }
    static const Id assign() {
        return nextId;
    }
};

#endif
