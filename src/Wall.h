#ifndef __WALL_H__
#define __WALL_H__

class Wall : public Entity<Wall> {
public:
    Wall() {
        LOG(INFO) << "Wall constructed!";
    }
};

#endif
