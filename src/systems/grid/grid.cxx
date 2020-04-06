#include "grid.hxx"

uint32_t grid::to_index(position<uint32_t> pos) const noexcept {
    return pos.y * dimensions_.width + pos.x;
}

void grid::resize(size<uint32_t> dimensions) {
    data_.resize(dimensions.area(), null_cell_);
    dimensions_ = dimensions;
}

const cell& grid::at(position<uint32_t> pos) const noexcept {
    if (!in_bounds(pos)) {
        return null_cell_;
    }
    return data_.at(to_index(pos));
}

void grid::set(position<uint32_t> pos, int value) {
    if (!in_bounds(pos)) {
        LOG(ERROR)
            << "Placement at " << pos.x << ", " << pos.y
            << " not within bounds ("
            << dimensions_.width << ", " << dimensions_.height << ")";
        return;
    }
    data_[to_index(pos)].value = value;
}

bool grid::in_bounds(position<uint32_t> pos) const noexcept {
    return pos.x + 1 <= dimensions_.width && pos.y + 1 <= dimensions_.height;
}

void grid::prnt() {
    std::string s;

    for (auto y { 0U }; y < dimensions_.height; y++) {
        for (auto x { 0U }; x < dimensions_.width; x++) {
            s += std::to_string(data_.at(to_index({ x, y })).value);
        }
        s += '\n';
    }

    LOG(DEBUG)
        << "\n-------- PRINTING GRID --------\n\n"
        << s
        << "\n------------------------------";
}